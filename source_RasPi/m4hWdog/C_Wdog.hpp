//_____C_Wdog.hpp________________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Wdog.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-02-02 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  WDOG_SECTION        "wdog"
#define  WDOG_DEMO_KEY       "demokey"
#define  WDOG_DEMO           "demodefault"
#define  WDOG_OUT_KEY        "out"
#define  WDOG_OUT            "m4hWdog/attention"
#define  WDOG_IN_KEY         "in"
// #define  WDOG_IN             ""


//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class WdogIn1: class for one sensor in line @ config file
// *************************************************************
class WdogIn1
{
 public:
 //------properties---------------------------------------------
 std::string topicIn;
 unsigned long sec;
 public:
 //------constructor & co---------------------------------------
 WdogIn1() { topicIn=""; sec=32000000; }
 WdogIn1(std::string in_, unsigned long sec_) {topicIn=in_; sec=sec_;}
}

// *************************************************************
//    class Wdog: watchdog for sensors
// *************************************************************
class Wdog
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [wdog]
 std::string _demo_;                   // demo value
 std::string wdog_out_key;             // topic out key
 std::string wdog_out;                 // topic out value
 std::string wdog_in_key;              // topic in key
 std::vector<WdogIn1>vIn;              // topic in values

 public:
 //------constructor & co---------------------------------------
 Wdog();                               // constructor
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
//       Wdog: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Wdog::Wdog() { setDefaults(); }

//_______set all default properties_____________________________
void Wdog::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = WDOG_SECTION;              // prog specifig info
 _demo_   = WDOG_DEMO;                 // demo default value
 keys=std::string(WDOG_DEMO_KEY);      // all keys in section
}

// *************************************************************
//       Wdog: setter and getter methods
// *************************************************************

// *************************************************************
//       Wdog: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Wdog::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Wdog::readConfig(std::string pfConf) 
{ 
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------read all lines of section from config file_____________
 std::vector<std::string> v1;          // all lines of section
 conf.getSection(section, v1);         // get lines from conf file
 if(v1.size()<1) return false;         // no valid lines read
 // ..ToDo..
 return true;
}

//_______Show all properties____________________________________
void Wdog::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<WDOG_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 // ..ToDo..
}

//_______act on messages..._____________________________________
bool Wdog::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 // ..ToDo..
 return true;
}

//_______Possibility for cleanup before end of program__________
void Wdog::onExit(struct mosquitto *mosq, int reason)
{
 
}

// *************************************************************
//       Wdog: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Wdog g_wdog=Wdog();
