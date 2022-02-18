//_____C_InDelayOut.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hInDelayOut -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_InDelayOut.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-23 First release
// 2021-09-06 Rename from m4hSubPub to m4hInDelayOut
// 2022-02-18 Use of wildcards (+, #) in topic in config-file
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

//------keys in section of config file--------------------------
#define IDO_SECTION          "indelayout"
#define IDO_IN_KEY           "in"
#define IDO_DELAY_KEY        "delay"
#define IDO_ACTION_KEY       "action"
#define IDO_OUT_KEY          "out"
#define IDO_RETAIN_KEY       "retain"
#define IDO_ALLOWSAME_KEY    "allowsame"
//------placeholder---------------------------------------------
#define IDO_PLAHO_TOPIC_IN   "<in>"
#define IDO_PLAHO_TEXT_IN    "<text>"
#define IDO_PLAHO_DELAY      "<delay>"
#define IDO_PLAHO_VALUE      "<value>"
#define IDO_PLAHO_INVERT     "<invert>"
#define IDO_PLAHO_BROKERTIME "<brokertime>"
#define IDO_PLAHO_TIME       "<time>"
//------action values-------------------------------------------
#define IDO_ACT_TEXT         "text"
#define IDO_ACT_DELAY        "delay"
#define IDO_ACT_DELAYTEXT    "delaytext"
#define IDO_ACT_SET          "setvalue"
#define IDO_ACT_INVERT       "invert"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

std::string g_topicOutLast;             // to avoid endless...
std::string g_payloadOutLast;           // ...message sending

// *************************************************************
//    class Message2d: extends class Message by the delay time
// *************************************************************
class Message2d : public Message2
{
 public:
 //------properties---------------------------------------------
 uint32_t    delayms;             // delay time
 std::string action;              // how to interpret payload
 std::string sText;               // content of placeholder text
 std::string sValue;              // content of placeholder value
 std::string sInvert;             // content of placeholder invert
 bool allowsame;                  // false: not 2x same message
// std::string supplement;
 //------constructor & co---------------------------------------
 Message2d() { delayms=0; action=""; sText=""; sValue=""; 
  sInvert=""; allowsame=false; }
};

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class InDelayOut
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [indelayout]
 std::vector <Message2d> vM2d;         // messages for indelayout
 std::string topicOutLast;             // to avoid endless...
 std::string payloadOutLast;           // ...message sending

 public:
 //------constructor & co---------------------------------------
 InDelayOut();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);

 //-----helper methods------------------------------------------
 static void threadFunctionDelay(struct mosquitto *mosq, 
   Message2d m2, std::string currTopic, std::string currPayload);
};

// *************************************************************
//       InDelayOut: constructor & co
// *************************************************************

//_______Default constructor____________________________________
InDelayOut::InDelayOut() { setDefaults(); }

//_______set all default properties_____________________________
void InDelayOut::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = IDO_SECTION;               // prog specifig info
 topicOutLast = "";                    // last topic out
 payloadOutLast = "";                  // last payload out
 keys=std::string(IDO_IN_KEY);         // all keys in section
 keys+="|"+std::string(IDO_DELAY_KEY);
 keys+="|"+std::string(IDO_ACTION_KEY);
 keys+="|"+std::string(IDO_OUT_KEY);
 keys+="|"+std::string(IDO_RETAIN_KEY);
 keys+="|"+std::string(IDO_ALLOWSAME_KEY);
}

// *************************************************************
//       InDelayOut: setter and getter methods
// *************************************************************

// *************************************************************
//       InDelayOut: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool InDelayOut::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool InDelayOut::readConfig(std::string pfConf) 
{ 
 std::vector<std::map<std::string, std::string>> v1;
 std::map<std::string, std::string> m1;
 std::map<std::string, std::string>::iterator it1;
 //------check config file--------------------------------------
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------read sections. 1 section = 1 vector with map<key,value>
 int iSec=conf.getSections(section, v1);
 vM2d.clear();                         // new messages
 //------for all map values in vector---------------------------
 for(int i=0; i<iSec; i++)
 {
  //-----convert one map value to Message2 and add it to vBtMsg2
  m1=v1.at(i);
  Message2d m2=Message2d();
  m2.retainOut=false;
  int ok=0;                            // check for in and out
  //-----analyse map parts--------------------------------------
  for(it1=m1.begin(); it1!=m1.end(); it1++)
  {
   if(it1->first==IDO_IN_KEY)
   {//---analyse values for incomming messages------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) 
    {//..delay time is in the incoming message (as payload).....
      sT=it1->second;                  // topic only
    }
    m2.topicIn=sT;                     // topic
    m2.payloadIn=sP;                   // payload, that has to fit
    ok|=1;                             // in ok: set Bit 0
   }
   if(it1->first==IDO_OUT_KEY)
   {//---analyse values for outgoing messages-------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    m2.topicOut=sT;
    m2.payloadOut=sP;
    ok|=2;                             // out ok: set Bit 1
   }
   if(it1->first==IDO_RETAIN_KEY)
   {//---set retain flag for outgoing message?------------------
    if(it1->second=="true") m2.retainOut=true;
   }
   if(it1->first==IDO_DELAY_KEY)
   {//---fixed default in config file for delay time------------
    try{
     uint32_t temp;
     temp=std::stoul(it1->second);
     m2.delayms=temp;
    } catch(...) {}                    // no error handling
   }
   if(it1->first==IDO_ACTION_KEY)
   {//---action to do-------------------------------------------
    std::string s1=it1->second;
    conf.delExtBlank(s1);
    m2.action=s1;
   }
   if(it1->first==IDO_ALLOWSAME_KEY)
   {//---set retain flag for outgoing message?------------------
    if(it1->second=="true") m2.allowsame=true;
   }
  }
  //-----keys for this section finished-------------------------
  if(ok==3) vM2d.push_back(m2);
 }
 if(vM2d.size()<1) return false;
 return true; 
}

//_______Show all properties____________________________________
void InDelayOut::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<".....indelayout messages.............."<<std::endl;
 int iLen=vM2d.size();
 if(iLen<1) std::cout<<"(no messages)"<<std::endl;
 else std::cout<<iLen<<" messages:"<<std::endl;
 for(int i=0; i<iLen; i++)
 {
  Message2d m2d=vM2d.at(i);
  std::cout<<" IN: -t "<<m2d.topicIn<<" -m "<<m2d.payloadIn<<" ===> ";
  std::cout<<m2d.delayms<<"ms";
  std::cout<<" ==> OUT: -t "<<m2d.topicOut<<" -m "<<m2d.payloadOut;
  if(m2d.retainOut) std::cout<<" -r";
  if(m2d.action.length()<1) std::cout<<" | (no action)";
  else std::cout<<" | action="<<m2d.action;
  std::cout<<std::endl;
 }
}

//_______act on messages..._____________________________________
bool InDelayOut::onMessage(struct mosquitto *mosq, 
  std::string topic, std::string payload)
{
 Conf conf=Conf();
 //------check if the topic is included in the config data------
 int iLen=vM2d.size();
 for(int i=0; i<iLen; i++) 
 {//-----compare topic with each topic in config file-----------
  Message2d m2d=Message2d();           // message object for one
  m2d=vM2d.at(i);                      // entry in config file
  //if(m2d.topicIn==topic)               // topics MUST match
  if(conf.fits(topic, m2d.topicIn))
  {//====topic matches==========================================
   if(m2d.payloadIn.length()>0)
   {//---payload in config file given: MUST fit-----------------
    if(m2d.payloadIn!=payload) continue;
   } // end payload in config file given: MUST fit
   //------avoid endless output of the same message---------------
   if(!m2d.allowsame) {
    if(topic==g_topicOutLast && payload==g_payloadOutLast) return false;
   }
   //---start delay thread-------------------------------------
   std::thread mythreadDelay(threadFunctionDelay, mosq, m2d, topic, payload);
   mythreadDelay.detach();
   return true;
  } // end topic matches
 } // end for
 return false;
}

//_______Possibility for cleanup before end of program__________
void InDelayOut::onExit(struct mosquitto *mosq, int reason)
{
}

// *************************************************************
//       InDelayOut: helper methods
// *************************************************************

//_______delay thread and send answer message___________________
// m2d: message data from config file (topic in, out, ...)
// cpay: current payload
void InDelayOut::threadFunctionDelay(struct mosquitto *mosq, 
   Message2d m2d, std::string currTop, std::string currPay)
{
 Conf conf=Conf();                     // useful methods
 std::string s1;                       // help value
 //===========check if action is given==========================
 if(m2d.action.length()>0)
 {//----------action given: says how to interpret the payload---
  //----------(try to) split action parameter-------------------
  std::string actionKey="", actionVal="";
  if(!conf.split2String(m2d.action, actionKey, actionVal, ' '))
   actionKey=m2d.action;
  //----------payloadIn is text---------------------------------
  if(actionKey==IDO_ACT_TEXT) m2d.sText=currPay;
  
  if(actionKey==IDO_ACT_DELAY)
  {//----------convert delay value------------------------------
   try{
    uint32_t temp=std::stoul(currPay); // help value
    m2d.delayms=temp;                  // conversion ok
   } catch(std::string& error) { return; }
  } // end convert delay value
  
  if(actionKey==IDO_ACT_DELAYTEXT) 
  {//---------cpay=delay text-----------------------------------
   std::string sDelay="", sText="";
   if(!conf.split2String(currPay, sDelay, sText,' ')) return;
   m2d.sText=sText;
   try{
    uint32_t temp=std::stoul(sDelay);  // help value
    m2d.delayms=temp;                  // conversion ok
   } catch(std::string& error) { return; }
  } // end cpay=delay text

  ///---------set value-----------------------------------------
  if(actionKey==IDO_ACT_SET) m2d.sValue=actionVal;
  
  if(actionKey==IDO_ACT_INVERT) 
  {//---------invert payloadIn----------------------------------
   std::string sInv1="", sInv2="";
   if(!conf.split2String(actionVal, sInv1, sInv2,' ')) return;
   if(currPay==sInv1) m2d.sInvert=sInv2;
   if(currPay==sInv2) m2d.sInvert=sInv1;
   if(m2d.sInvert.length()<1) return;
  }
 } // end action given

 //===========replace placeholder in topic out==================
 //conf.replaceAll(m2d.topicOut,IDO_PLAHO_TOPIC_IN,m2d.topicIn);
 conf.replaceAll(m2d.topicOut,IDO_PLAHO_TOPIC_IN,currTop);
 if(m2d.topicOut.length()<1) return;

 //===========wait until the response is to be sent=============
 std::this_thread::sleep_for(std::chrono::milliseconds(m2d.delayms));

 //===========replace placeholder in payload out================
 //conf.replaceAll(m2d.payloadOut,IDO_PLAHO_TOPIC_IN,m2d.topicIn);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_TOPIC_IN,currTop);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_TEXT_IN,m2d.sText);
 s1=std::to_string(m2d.delayms);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_DELAY,s1);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_VALUE,m2d.sValue);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_INVERT,m2d.sInvert);
 s1="%Y%m%d %H%M%S";
 s1=g_base.getDateTime(s1);
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_BROKERTIME,s1);
 s1=g_base.getDateTime();
 conf.replaceAll(m2d.payloadOut,IDO_PLAHO_TIME,s1);
 if(m2d.payloadOut.length()<1) return;

 //===========publish answer message============================
 g_topicOutLast=m2d.topicOut;          // remember last topic
 g_payloadOutLast=m2d.payloadOut;      // remember last payload
 int ret=mosquitto_publish(mosq, NULL,m2d.topicOut.c_str(),
     m2d.payloadOut.length(), m2d.payloadOut.c_str(), 0, m2d.retainOut);
 if(ret!=0) {
  if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",m2d.topicOut.c_str(),ret);
 } else {
  if(g_prt) std::cout<<" IN: -t "<<currTop<<" -m "<<currPay<<" ===> delay "<<m2d.delayms<<"ms ==> OUT: -t "<<m2d.topicOut<<" -m "<<m2d.payloadOut;
  if(g_prt && m2d.retainOut) std::cout<<" -r";
  if(g_prt) std::cout<<std::endl;
 }
}

//_______declare a global little helpers object ;)______________
InDelayOut g_indelayout=InDelayOut();
