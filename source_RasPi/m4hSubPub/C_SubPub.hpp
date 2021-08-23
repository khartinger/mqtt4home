//_____C_SubPub.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hSubPub -lmosquitto -lpthread -std=c++17
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_SubPub.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-23 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  SUBPUB_SECTION         "subpub"
#define  SUBPUB_IN_KEY          "in"
#define  SUBPUB_OUT_KEY         "out"
#define  SUBPUB_DELAY_KEY       "delay"
#define  SUBPUB_RETAIN_KEY      "retain"
#define  SUBPUB_ACTION_KEY      "action"
#define  SUBPUB_DO_01_VALUE     "brokertime"
#define  SUBPUB_DO_02_VALUE     "time"
#define  SUBPUB_DO_04_VALUE     "invert01"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Message2d: extends class Message by the delay time
// *************************************************************
class Message2d : public Message2
{
 public:
 //------properties---------------------------------------------
 uint32_t    delayms;
 std::string action;
 std::string supplement;
 //------constructor & co---------------------------------------
 Message2d() { delayms=0; action=""; supplement=""; }
};

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class SubPub
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [subpub]
 std::vector <Message2d> vM2d;         // messages for subpub
 public:
 //------constructor & co---------------------------------------
 SubPub();                             // constructor
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
 static void threadFunctionDelay(struct mosquitto *mosq, Message2d m2, std::multimap<std::string, std::string> mm1);
 static void evaluatePayload(std::multimap<std::string, std::string> mm1, uint32_t &iDelay, uint32_t &iDo, std::string &sPayOut);
};

// *************************************************************
//       SubPub: constructor & co
// *************************************************************

//_______Default constructor____________________________________
SubPub::SubPub() { setDefaults(); }

//_______set all default properties_____________________________
void SubPub::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = SUBPUB_SECTION;            // prog specifig info
 vM2d.clear();                         // no messages for delay
 keys=std::string(SUBPUB_IN_KEY);      // all keys in section
 keys+="|"+std::string(SUBPUB_OUT_KEY);
 keys+="|"+std::string(SUBPUB_DELAY_KEY);
 keys+="|"+std::string(SUBPUB_RETAIN_KEY);
 keys+="|"+std::string(SUBPUB_ACTION_KEY);
}

// *************************************************************
//       SubPub: setter and getter methods
// *************************************************************

// *************************************************************
//       SubPub: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool SubPub::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool SubPub::readConfig(std::string pfConf) 
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
   if(it1->first==SUBPUB_IN_KEY)
   {//---analyse values for incomming messages------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) 
    {//..delay time is in the incoming message (as payload).....
      sT=it1->second;                 // topic only
    }
    m2.topicIn=sT;                    //
    m2.payloadIn=sP;                  //
    ok|=1;                            // in ok: set Bit 0
   }
   if(it1->first==SUBPUB_OUT_KEY)
   {//---analyse values for outgoing messages-------------------
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    m2.topicOut=sT;
    m2.payloadOut=sP;
    ok|=2;                             // out ok: set Bit 1
   }
   if(it1->first==SUBPUB_RETAIN_KEY)
   {//---set retain flag for outgoing message?------------------
    if(it1->second=="true") m2.retainOut=true;
   }
   if(it1->first==SUBPUB_DELAY_KEY)
   {//---fixed default in config file for delay time------------
    try{
     uint32_t temp;
     temp=std::stoul(it1->second);
     m2.delayms=temp;
    } catch(std::string& error) {}     // no error handling
   }
   if(it1->first==SUBPUB_ACTION_KEY)
   {//---json action string-------------------------------------
    std::string s1=it1->second;
    conf.delExtBlank(s1);
    if(s1.length()>0) {
     if(s1.at(0)=='{' && s1.at(s1.length()-1)=='}') m2.action=s1;
    }
   }
  }
  //-----keys for this section finished-------------------------
  if(ok==3) vM2d.push_back(m2);
 }
 if(vM2d.size()<1) return false;
 return true; 
}

//_______Show all properties____________________________________
void SubPub::show()
{
 std::cout<<"-----["<<section<<"]-------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<".....subpub messages.................."<<std::endl;
 int iLen=vM2d.size();
 if(iLen<1) std::cout<<"(no messages)"<<std::endl;
 for(int i=0; i<iLen; i++)
 {
  Message2d m2d=vM2d.at(i);
  std::cout<<" IN: -t "<<m2d.topicIn<<" -m "<<m2d.payloadIn<<" ===> ";
  std::cout<<m2d.delayms<<"ms";
  std::cout<<" ==> OUT: -t "<<m2d.topicOut<<" -m "<<m2d.payloadOut;
  if(m2d.retainOut) std::cout<<" -r";
  std::cout<<std::endl;
 }
}

//_______act on messages..._____________________________________
bool SubPub::onMessage(struct mosquitto *mosq, std::string topic,
 std::string payload_)
{
 //------check if the topic is included in the config data------
 int iLen=vM2d.size();
 for(int i=0; i<iLen; i++) 
 {//-----compare topic with each topic in config file-----------
  Message2d m2d=Message2d();           // message object for one
  m2d=vM2d.at(i);                      // entry in config file
  if(m2d.topicIn==topic)               // topics MUST match
  {//====topic matches==========================================
   std::multimap<std::string, std::string> mm1;  // if payload
   mm1.clear();                                  // out is JSON
   int iLen_=payload_.length();        // length current payload
   int iLen1=m2d.payloadIn.length();   // length config payload
   if(iLen1>0)
   {//---expect certain payload---------------------------------
    if(payload_!=m2d.payloadIn) continue; //wrong payload
   }
   else
   {//---no specific conf payload:------------------------------
    //current payload must be empty or json OR action is defined
    if(iLen_>0) { // payload must be json-fomatted or action
     if((payload_.at(0)!='{' || payload_.at(iLen_-1)!='}')
      && m2d.action.length()==0) continue;
     //--payload is json----------------------------------------
     Conf conf=Conf();
     if(!conf.split2pairs(payload_, mm1)) mm1.clear();
    }
   }// end no specific conf payload
   //----action key given?--------------------------------------
   if(m2d.action.length()>0) {
    std::multimap<std::string, std::string> mm2;  // help value
    Conf conf=Conf();
    if(conf.split2pairs(m2d.action, mm2)) mm1.merge(mm2);
   }
   //----start delay thread-------------------------------------
   m2d.supplement=payload_;
   std::thread mythreadDelay(threadFunctionDelay, mosq, m2d, mm1);
   mythreadDelay.detach();
   return true;
  }//====end topic matches======================================
 }// end compare topic with each topic in config file
 return false;
}

//_______Possibility for cleanup before end of program__________
void SubPub::onExit(struct mosquitto *mosq, int reason)
{
}

// *************************************************************
//       SubPub: helper methods
// *************************************************************

//_______thread delay and send answer message___________________
// m2d: message data from config file (topic in, out, ...)
// mm1: multimap with json pairs of incomming payload
void SubPub::threadFunctionDelay(struct mosquitto *mosq, 
  Message2d m2d, std::multimap<std::string, std::string> mm1) 
{
 int ret;
 //------prepare payload----------------------------------------
 std::string sPayOut=m2d.payloadOut;   // default payload out text
 uint32_t iDelay=m2d.delayms;          // default delay time
 uint32_t iDo=0;                       // nothing to do
 std::string s1;                       // help value
 if(mm1.size()>0) 
 {//.....special json data available............................
  evaluatePayload(mm1, iDelay, iDo, sPayOut);
 }
 //------do something special BEFORE delay----------------------
 // add code here... e.g. if((iDo&8)>0) {...}
 //------wait until the response is to be sent------------------
 std::this_thread::sleep_for(std::chrono::milliseconds(iDelay));
 //------do something special AFTER delay-----------------------
 if((iDo&1)>0) { //.....return date in brokertime format........
  sPayOut+=g_base.getDateTime("%Y%m%d %H%M%S");
 }
 if((iDo&2)>0) { //.....return date in german format............
  sPayOut+=g_base.getDateTime("%d.%m.%Y %H:%M:%S");
 }
 if((iDo&4)>0) { //.....invert payload in: 0->1, 1->0...........
  if(m2d.supplement=="0") sPayOut="1";
  if(m2d.supplement=="1") sPayOut="0";
 }
 // ad code here... e.g. if((iDo&8)>0) {...}
 //------publish message----------------------------------------
 ret=mosquitto_publish(mosq, NULL,m2d.topicOut.c_str(),
     sPayOut.length(), sPayOut.c_str(), 0, m2d.retainOut);
 if(ret!=0) {
  if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",m2d.topicOut.c_str(),ret);
 } else {
  if(g_prt) std::cout<<" IN: -t "<<m2d.topicIn<<" -m "<<m2d.payloadIn<<" ===> delay "<<iDelay<<"ms ==> OUT: -t "<<m2d.topicOut<<" -m "<<sPayOut;
  if(g_prt && m2d.retainOut) std::cout<<" -r";
  if(g_prt) std::cout<<std::endl;
 }
}

//_______evaluate payload_______________________________________
// called by threadFunctionDelay
// mm1: <string,string> pairs if payload out was a JSON string
// iDelay: delay in ms, if JSON key was "delay"
// iDo: 0=do nothing, 1=send brokertime, 2=send time, 
//      4=invert 0<->1...
// return new payload or "" on error
void SubPub::evaluatePayload(std::multimap<std::string, std::string> mm1,
 uint32_t &iDelay, uint32_t &iDo, std::string &sPayOut)
{
 std::string sPayNew="";
 bool newText=false;
 std::multimap<std::string, std::string>::iterator it;
 //------for every JSON pair in payload out---------------------
 for (it = mm1.begin(); it != mm1.end(); it++)
 {
  //-----delay time given?--------------------------------------
  if(it->first=="delay") {
   try {
    int temp=std::stoul(it->second);
    iDelay=temp;
   } catch(std::invalid_argument) {};
  }
  //-----payload answer text given?-----------------------------
  if(it->first=="text") { sPayNew+=it->second; newText=true; }
  //-----do something special found-----------------------------
  if(it->first=="do") {
   if(it->second==SUBPUB_DO_01_VALUE) iDo|=1;
   if(it->second==SUBPUB_DO_02_VALUE) iDo|=2;
   if(it->second==SUBPUB_DO_04_VALUE) iDo|=4;
  }
 } //end for
 if(newText) sPayOut=sPayNew;
}

//_______declare a global little helpers object ;)______________
SubPub g_subpub=SubPub();
