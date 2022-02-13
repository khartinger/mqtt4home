//_____C_InBlockOut.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hInBlockOut -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_InBlockOut.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-02-13 First release
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
//------placeholder---------------------------------------------
#define IBO_PLAHO_TOPIC_IN   "<in>"
#define IBO_PLAHO_TEXT_IN    "<text>"
//------action values-------------------------------------------
#define IBO_ACT_TEXT         "text"
//-------block time limits--------------------------------------
//#define IBO_BLOCK_SEC_DEFAULT 86400         // 86400s = 1 day
#define IBO_BLOCK_SEC_DEFAULT 21600         // 21600s = 6h
#define IBO_BLOCK_SEC_MIN     1             // 1s = 1s
#define IBO_BLOCK_SEC_MAX     2592000       // 2592000 = 30d

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
 //------constructor & co---------------------------------------
 Message2d() { secBlock=IBO_BLOCK_SEC_DEFAULT; secLast=0; }
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
    std::string sHMS="it1->second";
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
      ok|=4;                           // block time ok: set Bit 2
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
  if(ok==7) vM2d.push_back(m2);
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
 //std::cout<<IBO_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 // ..ToDo..
}

//_______act on messages..._____________________________________
bool InBlockOut::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=true;
 // ..ToDo..
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

//_______declare a global little helpers object ;)______________
InBlockOut g_inblockout=InBlockOut();
