//_____C_Xxx.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hXxx -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Xxx.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2022-02-12 Update Xxx::readConfig(...)
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  XXX_SECTION         "xxx"
#define  XXX_DEMO_KEY        "demokey"
#define  XXX_DEMO            "demodefault"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Xxx
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [xxx]
 std::string _demo_;                   // demo value
 public:
 //------constructor & co---------------------------------------
 Xxx();                                // constructor
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
};

// *************************************************************
//       Xxx: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Xxx::Xxx() { setDefaults(); }

//_______set all default properties_____________________________
void Xxx::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = XXX_SECTION;               // prog specifig info
 _demo_   = XXX_DEMO;                  // demo default value
 keys=std::string(XXX_DEMO_KEY);       // all keys in section
}

// *************************************************************
//       Xxx: setter and getter methods
// *************************************************************

// *************************************************************
//       Xxx: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Xxx::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Xxx::readConfig(std::string pfConf) 
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
  if(sKey==XXX_DEMO_KEY) {
   _demo_=sVal;
  }
 
 // ..ToDo..
 
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void Xxx::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<XXX_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 // ..ToDo..
}

//_______act on messages..._____________________________________
bool Xxx::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 // ..ToDo..
 return true;
}

//_______Possibility for cleanup before end of program__________
void Xxx::onExit(struct mosquitto *mosq, int reason)
{
 
}

// *************************************************************
//       Xxx: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Xxx g_xxx=Xxx();
