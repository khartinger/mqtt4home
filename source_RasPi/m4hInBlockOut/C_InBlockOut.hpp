//_____C_InBlockOut.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hInBlockOut -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_InBlockOut.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-02-14 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

//------keys in section of config file--------------------------
#define IBO_SECTION          "inblockout"
#define IBO_IN_KEY           "in"
#define IBO_BLOCK_KEY        "block"
#define IBO_ACTION_KEY       "action"
#define IBO_OUT_KEY          "out"
#define IBO_RETAIN_KEY       "retain"
//------predefined placeholderser-------------------------------
#define IBO_PLAHO_TOPIC_IN   "<in>"         // current topic in
#define IBO_PLAHO_TEXT_IN    "<text>"       // current payload in
#define IBO_PLAHO_BLOCK      "<block>"      // secs to block
//------action values-------------------------------------------
//#define IBO_ACT_TEXT         "text"
//-------block time limits--------------------------------------
#define IBO_BLOCK_SEC_DEFAULT 21600         // 21600s = 6h
#define IBO_BLOCK_SEC_MIN     1             // 1s = 1s
#define IBO_BLOCK_SEC_MAX     2592000       // 2592000 = 30d
//------action values-------------------------------------------
//#define IBO_ACT_TEXT         "text"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Message2b: extends class Message by the block time
// *************************************************************
class Message2b : public Message2
{
 public:
 //------properties---------------------------------------------
 time_t      secBlock;            // time to block outgoing msgs
 time_t      secLast;             // last unix secs of sending
 std::string action;              // how to interpret payload
 //------constructor & co---------------------------------------
 Message2b() { secBlock=IBO_BLOCK_SEC_DEFAULT; secLast=0; 
  action=""; }
};

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class InBlockOut
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [inblockout]
 std::vector <Message2b> vM2b;         // messages for inblockout
 public:
 //------constructor & co---------------------------------------
 InBlockOut();                         // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 bool periodic(struct mosquitto *mosq);

 //-----helper methods------------------------------------------
 bool sendMessage(struct mosquitto *mosq, Message2b m2, std::string payload);
};

// *************************************************************
//       InBlockOut: constructor & co
// *************************************************************

//_______Default constructor____________________________________
InBlockOut::InBlockOut() { setDefaults(); }

//_______set all default properties_____________________________
void InBlockOut::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = IBO_SECTION;               // prog specifig info

 keys=std::string(IBO_IN_KEY);         // all keys in section
 keys+="|"+std::string(IBO_BLOCK_KEY);
 keys+="|"+std::string(IBO_ACTION_KEY);
 keys+="|"+std::string(IBO_OUT_KEY);
 keys+="|"+std::string(IBO_RETAIN_KEY);
}

// *************************************************************
//       InBlockOut: setter and getter methods
// *************************************************************

// *************************************************************
//       InBlockOut: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool InBlockOut::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool InBlockOut::readConfig(std::string pfConf) 
{
 //------check config file--------------------------------------
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------read sections. 1 section = 1 vector with map<key,value>
 std::vector<std::map<std::string, std::string>> v1;
 std::map<std::string, std::string> m1;
 std::map<std::string, std::string>::iterator it1;

 int iSec=conf.getSections(section, v1);
 vM2b.clear();                         // new messages
 //------for all map values in vector---------------------------
 for(int i=0; i<iSec; i++)
 {
  //-----convert one map value to Message2 and add it to vBtMsg2
  m1=v1.at(i);
  Message2b m2=Message2b();
  m2.retainOut=false;
  int ok=0;                            // check for in and out
  //-----analyse map parts--------------------------------------
  for(it1=m1.begin(); it1!=m1.end(); it1++)
  {
   if(it1->first==IBO_IN_KEY)
   {//---analyse values for incomming messages------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) 
    {//..delay time is in the incoming message (as payload).....
      sT=it1->second;                 // topic only
    }
    m2.topicIn=sT;                    // topic
    m2.payloadIn=sP;                  // payload, that has to fit
    ok|=1;                            // in ok: set Bit 0
   }
   if(it1->first==IBO_OUT_KEY)
   {//---analyse values for outgoing messages-------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    m2.topicOut=sT;
    m2.payloadOut=sP;
    ok|=2;                             // out ok: set Bit 1
   }
   if(it1->first==IBO_RETAIN_KEY)
   {//---set retain flag for outgoing message?------------------
    if(it1->second=="true") m2.retainOut=true;
   }
   if(it1->first==IBO_BLOCK_KEY)
   {//---fixed default in config file for delay time------------
    std::string sHMS=it1->second;
    std::vector<std::string> vt;
    vt.clear();
    conf.splitString(sHMS, vt, ':');
    if(vt.size()==3)
    {
     try {
      time_t tH, tM, tS, tsec;
      tH=std::stoul(vt.at(0));
      tM=std::stoul(vt.at(1));
      tS=std::stoul(vt.at(2));
      tsec=tH*3600 + tM*60 + tS;
      if(tsec<IBO_BLOCK_SEC_MIN) tsec=IBO_BLOCK_SEC_MIN;
      if(tsec>IBO_BLOCK_SEC_MAX) tsec=IBO_BLOCK_SEC_MAX;
      m2.secBlock=tsec;
      //ok|=4;                           // block time ok: set Bit 2
     } catch(...) {}
    }
   }
   if(it1->first==IBO_ACTION_KEY)
   {//---action to do-------------------------------------------
    std::string s1=it1->second;
    conf.delExtBlank(s1);
    m2.action=s1;
   }
  }
  //-----keys for this section finished-------------------------
  if(ok==3) vM2b.push_back(m2);
 }
 if(vM2b.size()<1) return false;
 return true;
}

//_______Show all properties____________________________________
void InBlockOut::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;

 std::cout<<".....inblockout messages.............."<<std::endl;
 int iLen=vM2b.size();
 if(iLen<1) std::cout<<"(no messages)"<<std::endl;
 else std::cout<<iLen<<" messages:"<<std::endl;
 for(int i=0; i<iLen; i++)
 {
  Message2b m2b=vM2b.at(i);
  std::cout<<" IN: -t "<<m2b.topicIn<<" -m "<<m2b.payloadIn<<" | block ";
  std::cout<<m2b.secBlock<<"s";
  std::cout<<" | OUT: -t "<<m2b.topicOut<<" -m "<<m2b.payloadOut;
  if(m2b.retainOut) std::cout<<" -r";
  if(m2b.action.length()<1) std::cout<<" | (no action)";
  else std::cout<<" | action="<<m2b.action;
  std::cout<<std::endl;
 }
}

//_______act on messages..._____________________________________
bool InBlockOut::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=false;
 //------check if the topic is included in the config data------
 int iLen=vM2b.size();
 for(int i=0; i<iLen; i++) 
 {//-----compare topic with each topic in config file-----------
  Message2b m2b=Message2b();           // message object for one
  m2b=vM2b.at(i);                      // entry in config file
  if(m2b.topicIn==topic)               // topics MUST match
  {//====topic matches==========================================
   //----check if a payload is given in config file-------------
   if(m2b.payloadIn.length()>0)
   {//...yes: payload in config file given: MUST fit............
    if(m2b.payloadIn!=payload) continue;
   } //..end payload in config file given: MUST fit
   //----check if the message should be blocked-----------------
   time_t now;                         // secs since 1.1.1970
   time(&now);                         // get secs since 1.1.1970
   if((now-m2b.secLast)<m2b.secBlock) {// check blocking
    if(g_prt) std::cout<<"Incoming message \""<<topic<<"\" blocked!"<<std::endl;
    return false; // dont send message
   } //..check blocking
   vM2b.at(i).secLast=now;             // send message
   bRet|=sendMessage(mosq, m2b, payload);
  } // end topic matches
 } // end for
 return bRet;
}

//_______Possibility for cleanup before end of program__________
void InBlockOut::onExit(struct mosquitto *mosq, int reason)
{
}

//_______periodic action________________________________________
bool InBlockOut::periodic(struct mosquitto *mosq)
{
 return true;
}

// *************************************************************
//       InBlockOut: helper methods
// *************************************************************

//______forward message_________________________________________
// m2b: message data from config file (topic in, out, ...)
// cpay: current payload
bool InBlockOut::sendMessage(struct mosquitto *mosq, 
  Message2b m2b, std::string cpay)
{
 Conf conf=Conf();                     // useful methods
 std::string s1;                       // help value
 std::string sText=cpay;               // helper for cpay
 //===========check if action is given==========================
 if(m2b.action.length()>0)
 {//----------action given: says how to interpret the payload---
  //----------(try to) split action parameter-------------------
  std::string actionKey="", actionVal="";
  if(!conf.split2String(m2b.action, actionKey, actionVal, ' '))
   actionKey=m2b.action;
  //----------payloadIn is text---------------------------------
  //if(actionKey==IBO_ACT_TEXT) sText=cpay;

  //...ToDo: other actions...

 } // end action given

 //===========replace placeholder in topic out==================
 conf.replaceAll(m2b.topicOut,IBO_PLAHO_TOPIC_IN,m2b.topicIn);
 if(m2b.topicOut.length()<1) return false;
 //===========replace placeholder in payload out================
 conf.replaceAll(m2b.payloadOut,IBO_PLAHO_TOPIC_IN,m2b.topicIn);
 conf.replaceAll(m2b.payloadOut,IBO_PLAHO_TEXT_IN,sText);
 s1=std::to_string(m2b.secBlock);
 conf.replaceAll(m2b.payloadOut,IBO_PLAHO_BLOCK,s1);
 //===========publish answer message============================
 int ret=mosquitto_publish(mosq, NULL,m2b.topicOut.c_str(),
     m2b.payloadOut.length(), m2b.payloadOut.c_str(), 0, m2b.retainOut);
 if(ret!=0) {
  if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",m2b.topicOut.c_str(),ret);
  return false;
 } else {
  if(g_prt) std::cout<<" IN: -t "<<m2b.topicIn<<" -m "<<cpay<<" ==> NOT blocked => OUT: -t "<<m2b.topicOut<<" -m "<<m2b.payloadOut;
  if(g_prt && m2b.retainOut) std::cout<<" -r";
  if(g_prt) std::cout<<std::endl;
 }
 return true;
}

//_______declare a global little helpers object ;)______________
InBlockOut g_inblockout=InBlockOut();
