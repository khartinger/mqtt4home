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
// #define  WDOG_DEMO_KEY       "demokey"
// #define  WDOG_DEMO           "demodefault"
#define  WDOG_OUT_KEY        "out"
#define  WDOG_OUT_TOPIC      "m4hWdog/attention"
#define  WDOG_OUT_PAYLOAD    "Sensor <in> missing!"
#define  WDOG_IN_KEY         "in"
//-------placeholder--------------------------------------------
#define  WDOG_PLAHO_TOPIC_IN "<in>"
//-------watchdog time limits-----------------------------------
#define  WDOG_SEC_MIN        1
#define  WDOG_SEC_MAX        315576000

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
 time_t secDiff;
 time_t secLast;
 public:
 //------constructor & co---------------------------------------
 WdogIn1() { init("", WDOG_SEC_MAX); }
 WdogIn1(std::string in_, unsigned long sec_) { init(in_, sec_); }
 void init(std::string in_, unsigned long secDiff_);
};

//_____init wdogIn1 object______________________________________
void WdogIn1::init(std::string in_, unsigned long secDiff_)
{
 topicIn=in_;
 if(secDiff_ < WDOG_SEC_MIN) secDiff_ = WDOG_SEC_MIN;
 if(secDiff_ > WDOG_SEC_MAX) secDiff_ = WDOG_SEC_MAX;
 secDiff = secDiff_;
 time(&secLast);
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
 // std::string _demo_;                // demo value
 std::string wdog_out_key;             // topic out key
 std::string wdog_out_topic;           // topic out
 std::string wdog_out_payload;         // value out
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
 void periodic(struct mosquitto *mosq);

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
 // _demo_   = WDOG_DEMO;              // demo default value
 wdog_out_key     = WDOG_OUT_KEY;      // topic out key
 wdog_out_topic   = WDOG_OUT_TOPIC;    // topic out value
 wdog_out_payload = WDOG_OUT_PAYLOAD;  // payload out
 wdog_in_key      = WDOG_IN_KEY;       // topic out
 vIn.clear();                          // topic in values
 // keys=std::string(WDOG_DEMO_KEY);   // all keys in section
 keys=std::string(WDOG_OUT_KEY);
 keys+="|"+std::string(WDOG_IN_KEY);
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
  if(sKey==wdog_out_key)
  {//....message out (topic payload)............................
   std::string sT="", sP="";
   if(conf.split2String(sVal, sT, sP, ' ')) {
    wdog_out_topic=sT;
    wdog_out_payload=sP;
   } else {
    wdog_out_topic=sT;
    wdog_out_payload=WDOG_OUT_PAYLOAD;
   }
  } // end message out
  if(sKey==wdog_in_key)
  {//....message in (topic watchdog-time).......................
   std::string sT="", sHMS="";
   if(conf.split2String(sVal, sT, sHMS, ' '))
   {
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
      if(tsec<WDOG_SEC_MIN) tsec=WDOG_SEC_MIN;
      if(tsec>WDOG_SEC_MAX) tsec=WDOG_SEC_MAX;
      WdogIn1 w1=WdogIn1(sT, tsec);
      vIn.push_back(w1);
     } catch(...) {}
    }
   }
  } // end message in
 } // end for every line in section
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
 //std::cout<<WDOG_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 std::cout<<wdog_out_key<<"                 | -t "<<wdog_out_topic<<" -m "<<wdog_out_payload<<std::endl;
 std::cout<<"-----in----------------------------------"<<std::endl;
 for(int i=0; i<vIn.size(); i++) {
  std::cout<<vIn.at(i).topicIn<<": "<<vIn.at(i).secDiff<<"sec | ";
 }
 std::cout<<std::endl;
}

//_______act on messages..._____________________________________
bool Wdog::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 int num=vIn.size();
 time_t now;
 time(&now);                           // get sec since 1.1.1970
 //------for all sensors----------------------------------------
 for(int i=0; i<num; i++){
  if(topic==vIn.at(i).topicIn) 
  {//----topic for watchdog found-------------------------------
   vIn.at(i).secLast=now;              // update sensor
   if(g_prt) std::cout<<"Update secLast: "<<vIn.at(i).topicIn<<std::endl;
   return true;
  }
 }
 return false;
}

//_______Possibility for cleanup before end of program__________
void Wdog::onExit(struct mosquitto *mosq, int reason)
{
 
}

//_______watchdog: check topics_________________________________
void Wdog::periodic(struct mosquitto *mosq)
{
 time_t now;                           // secs since 1.1.1970
 time(&now);                           // get secs since 1.1.1970
 Conf conf=Conf();                     // for replaceAll
 int num=vIn.size();                   // number of sensors
 //------for all sensors----------------------------------------
 for(int i=0; i<num; i++){
  WdogIn1 vIn1_=vIn.at(i);             // current sensor
  if((now-vIn1_.secLast)>=vIn1_.secDiff)
  {//----watchdog timeout---------------------------------------
   vIn.at(i).secLast=now;              // update time last message
   //....replace <in> by topicIn................................
   std::string sPay=wdog_out_payload;
   std::string sOld=WDOG_PLAHO_TOPIC_IN;
   std::string sNew=vIn1_.topicIn;
   conf.replaceAll(sPay, sOld, sNew);  // replace <in>
   //....publish attention message..............................
   int iRet=mosquitto_publish(mosq, NULL, wdog_out_topic.c_str(),
    sPay.length(), sPay.c_str(), 0, true);
   if(iRet!=0) {
    if(g_prt) fprintf(stderr, "Error %d: NOT published -t %s -m %s.\n",
      iRet, wdog_out_topic.c_str(), sPay.c_str());
   } else {
    if(g_prt) std::cout<<"ERROR! Watchdog timeout "<<vIn1_.topicIn<<std::endl;
   }
  }
 }
}

// *************************************************************
//       Wdog: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Wdog g_wdog=Wdog();
