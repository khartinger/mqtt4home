//_____C_Demo1.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hDemo1 -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Demo1.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2022-02-12 Update Demo1::readConfig(...)
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  DEMO1_SECTION         "demo1"
//#define  DEMO1_DEMO_KEY        "demokey"
//#define  DEMO1_DEMO            "demodefault"
#define DEMO1_STARTVALUE_KEY "startvalue"   
#define DEMO1_STARTVALUE     30

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Demo1
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [demo1]
 int startvalue;
 //std::string _demo_;                   // demo value
 public:
 //------constructor & co---------------------------------------
 Demo1();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 int getStartvalue() { return startvalue; }
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
//       Demo1: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Demo1::Demo1() { setDefaults(); }

//_______set all default properties_____________________________
void Demo1::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = DEMO1_SECTION;               // prog specifig info
 startvalue=DEMO1_STARTVALUE;
 keys=std::string(DEMO1_STARTVALUE_KEY);
 //_demo_   = DEMO1_DEMO;                  // demo default value
 //keys=std::string(DEMO1_DEMO_KEY);       // all keys in section
}

// *************************************************************
//       Demo1: setter and getter methods
// *************************************************************

// *************************************************************
//       Demo1: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Demo1::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Demo1::readConfig(std::string pfConf) 
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
  if(!g_utils.str2str2(s1, sKey, sVal, ':')) continue;
  g_utils.delExtBlank(sKey);
  g_utils.delExtBlank(sVal);
  g_utils.str2lower(sKey);
  //-----search key---------------------------------------------
  //if(sKey==DEMO1_DEMO_KEY) {
  // _demo_=sVal;
  //}
  if(sKey==DEMO1_STARTVALUE_KEY) {
   try{ // string to int
    int temp=std::stoi(sVal);
    startvalue=temp;
   } catch(std::invalid_argument) {};
  }
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void Demo1::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<DEMO1_STARTVALUE_KEY<<"          | "<<startvalue<<std::endl;
 //std::cout<<DEMO1_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 // ..ToDo..
}

//_______act on messages..._____________________________________
bool Demo1::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;
 bool bRet=true;
 if(topic=="m4hDemo1/get" && payload=="keys") {
  std::string sTopic="m4hDemo1/ret/keys";
  std::string sPayload=getKeys();
  int iRet=mosquitto_publish(mosq, NULL,sTopic.c_str(),
   sPayload.length(), sPayload.c_str(), 0, false);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTopic.c_str(),iRet);
   bRet=false;
  }
 }
 return bRet;
}

//_______Possibility for cleanup before end of program__________
void Demo1::onExit(struct mosquitto *mosq, int reason)
{
 std::cout<<"Thank you for using m4hDemo1!"<<std::endl;
}

//_______periodic action________________________________________
bool Demo1::periodic(struct mosquitto *mosq)
{
 char cSec[16];
 static int iSec=getStartvalue();
 sprintf(cSec," %d ",iSec);
 fputs(cSec, stdout); fflush(stdout);
 iSec--; if(iSec<0) return false;
 return true;
}

// *************************************************************
//       Demo1: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Demo1 g_demo1=Demo1();
