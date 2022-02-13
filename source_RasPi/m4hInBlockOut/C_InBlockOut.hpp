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
#define IBO_BLOCK_SEC_DEFAULT 86400         // 86400s = 1 day


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
 //------constructor & co---------------------------------------
 Message2d() { secBlock=IBO_BLOCK_SEC_DEFAULT; }
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
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------read all lines of section from config file_____________
 std::vector<std::string> v1;          // all lines of section
 conf.getSection(section, v1);         // get lines from conf file
 if(v1.size()<1) return false;         // no valid lines read
 //------for every line in section------------------------------
 for(int i=0; i<v1.size(); i++) {
  //-----get key and value--------------------------------------
  std::string sKey="", sVal="";
  std::string s1=v1.at(i);
  if(!conf.split2String(s1, sKey, sVal, ':')) continue;
  conf.delExtBlank(sKey);
  conf.delExtBlank(sVal);
  conf.strToLower(sKey);
  //-----search key---------------------------------------------
  // if(sKey==IBO_DEMO_KEY) {
  //  _demo_=sVal;
  // }
 
 // ..ToDo..
 
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void InBlockOut::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
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
