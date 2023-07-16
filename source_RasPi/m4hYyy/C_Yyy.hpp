//____________C_Yyy.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hYyy -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Yyy.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2023-07-16 First release (based on Yyy)
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define YYY_SECTION               "yyy"
#define YYY_TOPICBASE_KEY         "topicbase"
#define YYY_TOPICBASE_VAL         "m4h/yyy"
#define YYY_TOPIC_GET_VAL         "/get"
#define YYY_TOPIC_SET_VAL         "/set/"
#define YYY_TOPIC_RET_VAL         "/ret/"
#define YYY_PAY_HELP1             "?"
#define YYY_PAY_HELP2             "help"
#define YYY_DEMO_KEY              "demotext"
#define YYY_DEMO_VAL              "default_demo_text_:)"

//------------global values-------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Yyy
{
 public:
 //-----------properties----------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //-----------application specific properties-------------------
 std::string keys;                     // keys for [yyy]
 std::string topicBase;                // system MQTT topic base
 std::string demotext_;               // demo text
 public:
 //-----------constructor & co----------------------------------
 Yyy();                                // constructor
 void setDefaults();                   // set all default values
 //-----------setter and getter methods-------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //-----------working methods-----------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 bool periodic(struct mosquitto *mosq, int zsec);

 //----------helper methods-------------------------------------
 std::string getsHelp();               // help text
 std::string getsShow();               // config data as string
};

// *************************************************************
//       Yyy: constructor & co
// *************************************************************

//____________Default constructor_______________________________
Yyy::Yyy() { setDefaults(); }

//____________set all default properties________________________
void Yyy::setDefaults()
{
 pfConfig  = _CONF_PFILE_;             // path&name config file
 section   = YYY_SECTION;              // prog specifig info
 topicBase = YYY_TOPICBASE_VAL;        // topic base
 demotext_=YYY_DEMO_VAL;              // demo text
 keys=YYY_TOPICBASE_KEY;               // all keys in section
 keys+="|"+std::string(YYY_DEMO_KEY);  // demo key
}

// *************************************************************
//       Yyy: setter and getter methods
// *************************************************************

// *************************************************************
//       Yyy: working methods
// *************************************************************

//____________read config data (log path) from file_____________
bool Yyy::readConfig() { return readConfig(pfConfig); }

//____________read config data (log path) from file_____________
// return: true if log path exist, false on error or not found
bool Yyy::readConfig(std::string pfConf) 
{ 
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 //===========try to open config file===========================
 int iTrials=3;
 while((iTrials--)>0) {
  if(conf.isReady()) break;
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
 }
 if(iTrials<=0) return false;
 //===========read all lines of section [...] from config file==
 std::vector<std::string> v1;          // all lines of section
 conf.getSection(section, v1);         // get lines from conf file
 if(v1.size()<1) return false;         // no valid lines read
 //===========for every line in section [...]===================
 for(int i=0; i<v1.size(); i++) {
  //----------split line into key and value---------------------
  std::string sKey="", sVal="";
  std::string s1=v1.at(i);
  if(!g_utils.str2str2(s1, sKey, sVal, ':')) continue;
  g_utils.delExtBlank(sKey);
  g_utils.str2lower(sKey);
  g_utils.delExtBlank(sVal);
  //----------search for topicbase key--------------------------
  if(sKey==YYY_TOPICBASE_KEY) {
   int pos1=sVal.find(" ");            // no blank allowed!
   if(pos1==std::string::npos) {       // blank not found = OK
    topicBase=sVal;
   }
  }
  //----------search demo key-----------------------------------
  if(sKey==YYY_DEMO_KEY) {
   demotext_=sVal;
  }
  // ...ToDo: add your keys here...
 } // END OF for every line in section [..]=====================
 return true;
}

//____________show all properties_______________________________
// must be called by m4hExtension.hpp f2Init(std::string pfConf):
// bR=bR&&g_yyy.readConfig(pfConf);
// if(g_prt) g_yyy.show();
void Yyy::show() { std::cout<<getsShow()<<std::endl; }

//_______act on messages..._____________________________________
// must be called by m4hExtension.hpp f3OnMessage():
// g_yyy1.onMessage(mosq, topic, payload);
bool Yyy::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=true;
 //===========look for system topic(s)==========================
 std::string s1= topicBase+std::string(YYY_TOPIC_GET_VAL);
 if(topic==s1)
 { //=-=-=-=- its a /get topic! =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  for(auto& c : payload) { c = tolower(c); }; // string to lower
  std::string sPay="";                 // get answer payload
  //----------process the get request---------------------------
  if(payload==YYY_PAY_HELP1 || payload==YYY_PAY_HELP2) sPay=getsHelp();
  if(payload==YYY_DEMO_KEY) sPay=demotext_;
  //----------send an answer message---------------------------
  if(sPay!="") {
   std::string sTop=topicBase+std::string(YYY_TOPIC_RET_VAL)+payload;// answer topic
   int ret=mosquitto_publish(mosq, NULL,sTop.c_str(), sPay.length(), sPay.c_str(), 0, false);
   if(g_prt) {
    if(ret!=0) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
   }
  }
 } // END OF "its a /get topic!"" =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 s1=topicBase+std::string(YYY_TOPIC_SET_VAL);
 if(topic.find(s1)!=std::string::npos)
 { //=-=-=-=- its a /set topic! -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  std::string topicpart=topic.substr(s1.length());
  std::string sPay="";                 // get answer payload
  //----------process the set request---------------------------
  if(topicpart==YYY_DEMO_KEY) {
   demotext_=payload;
   sPay=payload;
  }
  //----------send an answer message---------------------------
  if(sPay!="") {
   std::string sTop=topicBase+std::string(YYY_TOPIC_RET_VAL)+topicpart;// answer topic
   int ret=mosquitto_publish(mosq, NULL,sTop.c_str(), sPay.length(), sPay.c_str(), 0, false);
   if(g_prt) {
    if(ret!=0) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
   }
  }
 } // END OF "its a /set topic!" -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

 // ...ToDo: Add additional topics here...

 return bRet;
}

//____________possibility for cleanup before end of program_____
// If the function is used, it must be called by 
// m4hExtension.hpp f4OnExit():
// g_yyy.onExit(mosq, reason);
void Yyy::onExit(struct mosquitto *mosq, int reason)
{
 std::cout<<std::endl<<"§§**Goodbye from Yyy::onExit()**§§"<<std::endl;
}

//____________periodic action___________________________________
// If the function is used, it must be called by 
// m4hExtension.hpp f5periodic():
// g_yyy.periodic(mosq, zsec);
bool Yyy::periodic(struct mosquitto *mosq, int zsec)
{
 if(zsec%100==0) std::cout<<"§§**Hello from Yyy::periodic() every 10sec!**§§"<<std::endl;
 return true;
}

// *************************************************************
//       Yyy: helper methods
// *************************************************************

//____________return a help string______________________________
std::string Yyy::getsHelp()
{
 std::string s0="***Help text for the module Yyy.";
 s0+=" ./get "+std::string(YYY_DEMO_KEY);
 return s0;
}

//____________return config data as string______________________
// called by show()
std::string Yyy::getsShow()
{
 std::string s0="";                    // result string
 std::string s1="=====["+section+"]============================================================";
 s0=s0+s1.substr(0,60)+END_OF_LINE;
 Conf conf=Conf(pfConfig);
 s0=s0+"config file         | "+pfConfig;
 if(!conf.isReady()) s0+=" (file not found)";
 s0+=END_OF_LINE;
 s0=s0+"all keys            | "+keys+END_OF_LINE;
 s0=s0+"---key--------------|---value-------------------------------"+END_OF_LINE;
 s0=s0+"topic base          | "+topicBase+END_OF_LINE;
 s1="demo key ("+std::string(YYY_DEMO_KEY)+")         ";
 s0=s0+s1.substr(0,20)+"| "+demotext_+END_OF_LINE;
 // ...ToDo: Add additional key/values here...
 return s0;
}

//____________declare a global little helpers object ;)_________
Yyy g_yyy=Yyy();
