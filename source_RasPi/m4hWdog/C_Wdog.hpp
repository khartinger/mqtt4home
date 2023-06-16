//_____C_Wdog.hpp________________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Wdog.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-02-02 First release
// 2022-02-16 Add keys allin, allout, overin, overout
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define WDOG_SECTION              "wdog"
#define WDOG_OUT_KEY              "out"
#define WDOG_OUT_TOPIC            "m4hWdog/attention"
#define WDOG_OUT_PAYLOAD          "Sensor <in> missing!"
#define WDOG_IN_KEY               "in"
//-------placeholder--------------------------------------------
#define  WDOG_PLAHO_TOPIC_IN      "<in>"
#define  WDOG_PLAHO_LIST          "<list>"
//-------watchdog time limits-----------------------------------
#define  WDOG_SEC_MIN             1
#define  WDOG_SEC_MAX             315576000
//-------query the monitored topics-----------------------------
#define WDOG_DELIMITER1           ","
#define WDOG_GET_ALL_IN_KEY       "allin"
#define WDOG_GET_ALL_IN_T         "m4hWdog/get"
#define WDOG_GET_ALL_IN_P         "all"
#define WDOG_GET_ALL_OUT_KEY      "allout"
#define WDOG_GET_ALL_OUT_T        "m4hWdog/ret/all"
#define WDOG_GET_OVER_IN_KEY      "overin"
#define WDOG_GET_OVER_IN_T        "m4hWdog/get"
#define WDOG_GET_OVER_IN_P        "overdue"
#define WDOG_GET_OVER_OUT_KEY     "overout"
#define WDOG_GET_OVER_OUT_T       "m4hWdog/ret/overdue"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class WdogIn1: class for one sensor in line @ config file
// *************************************************************
class WdogIn1
{
 public:
 //------properties---------------------------------------------
 std::string topicIn;                  // topic to monitor
 time_t secDiff;                       // max. watchdog delay
 time_t secLast;                       // sensor OR overdue msg
 time_t secLastSensor;                 // last time sensor secs
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
 time(&secLastSensor);
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
 std::string wdogInKey;                // topic in key
 std::string wdogOutKey;               // topic out key
 Message  mWdogOut;                    // Out Messages
 std::vector<WdogIn1>vIn;              // topic in values
 std::string wdogGetAllInKey;          // key to get all topics
 std::string wdogGetAllOutKey;         // key to return all topics
 Message2 mGetAll;                     // 
 std::string wdogGetOverInKey;         // 
 std::string wdogGetOverOutKey;        // 
 Message2 mGetOver;                    // 

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
 vIn.clear();                          // topic in values
 wdogInKey         = WDOG_IN_KEY;      // topic in key
 wdogOutKey        = WDOG_OUT_KEY;     // topic out key
 mWdogOut.topic    = WDOG_OUT_TOPIC;   // topic out value
 mWdogOut.payload  = WDOG_OUT_PAYLOAD; // payload out

 wdogGetAllInKey   = WDOG_GET_ALL_IN_KEY;   //key to get all topics
 mGetAll.topicIn   = WDOG_GET_ALL_IN_T;     //topic to get all topics
 mGetAll.payloadIn = WDOG_GET_ALL_IN_P;     //payload to get all topics
 wdogGetAllOutKey  = WDOG_GET_ALL_OUT_KEY;  //key to get all topics
 mGetAll.topicOut  = WDOG_GET_ALL_OUT_T;    //topic return all topics

 wdogGetOverInKey  = WDOG_GET_OVER_IN_KEY;  //key get overdue topics
 mGetOver.topicIn  = WDOG_GET_OVER_IN_T;    //topic get overdue topics
 mGetOver.payloadIn= WDOG_GET_OVER_IN_P;    //payload get overdue topics
 wdogGetOverOutKey = WDOG_GET_OVER_OUT_KEY; //
 mGetOver.topicOut = WDOG_GET_OVER_OUT_T;   //topic return overdue topics

 keys=std::string(WDOG_IN_KEY);
 keys+="|"+std::string(WDOG_OUT_KEY);
 keys+="|"+std::string(WDOG_GET_ALL_IN_KEY);
 keys+="|"+std::string(WDOG_GET_ALL_OUT_KEY);
 keys+="|"+std::string(WDOG_GET_OVER_IN_KEY);
 keys+="|"+std::string(WDOG_GET_OVER_OUT_KEY);
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
  if(!g_utils.str2str2(s1, sKey, sVal, ':')) continue;
  g_utils.delExtBlank(sKey);
  g_utils.delExtBlank(sVal);
  g_utils.str2lower(sKey);
  //-----search key---------------------------------------------
  if(sKey==wdogOutKey)
  {//....message out (topic payload)............................
   std::string sT="", sP="";
   if(g_utils.str2str2(sVal, sT, sP, ' ')) {
    mWdogOut.topic=sT;
    mWdogOut.payload=sP;
   } else {
    mWdogOut.topic=sT;
    mWdogOut.payload=WDOG_OUT_PAYLOAD;
   }
  } // end message out
  //-----message in (topic watchdog-time)-----------------------
  if(sKey==wdogInKey)
  {//....decompose the value into topic and payload.............
   std::string sT="", sHMS="";
   if(g_utils.str2str2(sVal, sT, sHMS, ' '))
   {
    std::vector<std::string> vt;
    vt.clear();
    g_utils.str2vector1(sHMS, vt, ':');
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
  //-----message in to get all registered topics----------------
  if(sKey==wdogGetAllInKey)            // allin
  {//....decompose the value into topic and payload.............
   std::string sT="", sP="";
   if(g_utils.str2str2(sVal, sT, sP, ' ')) {
    mGetAll.topicIn=sT;
    mGetAll.payloadIn=sP;
   } else {
    mGetAll.topicIn=sVal;
    mGetAll.payloadIn="";
   }
  }
  //-----message out for all registered topics------------------
  if(sKey==wdogGetAllOutKey)           // allout
  {//....decompose the value into topic and payload.............
   std::string sT="", sP="";
   if(g_utils.str2str2(sVal, sT, sP, ' ')) {
    mGetAll.topicOut=sT;
    mGetAll.payloadOut=sP;
   } else {
    mGetAll.topicOut=sVal;
    mGetAll.payloadOut="";
   }
  }
  //-----message in to get overdue topics-----------------------
  if(sKey==wdogGetOverInKey)           // overin
  {//....decompose the value into topic and payload.............
   std::string sT="", sP="";
   if(g_utils.str2str2(sVal, sT, sP, ' ')) {
    mGetOver.topicIn=sT;
    mGetOver.payloadIn=sP;
   } else {
    mGetOver.topicIn=sVal;
    mGetOver.payloadIn="";
   }
  }
  //-----message out for all overdue topics---------------------
  if(sKey==wdogGetOverOutKey)          // overout
  {//....decompose the value into topic and payload.............
   std::string sT="", sP="";
   if(g_utils.str2str2(sVal, sT, sP, ' ')) {
    mGetOver.topicOut=sT;
    mGetOver.payloadOut=sP;
   } else {
    mGetOver.topicOut=sVal;
    mGetOver.payloadOut="";
   }
  }
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void Wdog::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<wdogOutKey<<"                 | -t "<<mWdogOut.topic<<" -m "<<mWdogOut.payload<<std::endl;
 std::cout<<wdogGetAllInKey<<"               | -t "<<mGetAll.topicIn<<" -m "<<mGetAll.payloadIn<<std::endl;
 std::cout<<wdogGetAllOutKey<<"              | -t "<<mGetAll.topicOut<<" -m "<<mGetAll.payloadOut<<std::endl;
 std::cout<<wdogGetOverInKey<<"              | -t "<<mGetOver.topicIn<<" -m "<<mGetOver.payloadIn<<std::endl;
 std::cout<<wdogGetOverOutKey<<"             | -t "<<mGetOver.topicOut<<" -m "<<mGetOver.payloadOut<<std::endl;
 
 std::cout<<"-----in----------------------------------"<<std::endl;
 for(int i=0; i<vIn.size(); i++) {
  std::cout<<vIn.at(i).topicIn<<": "<<vIn.at(i).secDiff<<"sec | ";
 }
 std::cout<<std::endl;
}

//_______act on messages..._____________________________________
bool Wdog::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=true;
 std::string sPay;                     // help value
 Conf conf=Conf();                     // useful methods
 int num=vIn.size();                   // number of topics to watch
 time_t now;
 time(&now);                           // get sec since 1.1.1970
 
 //======is the message a query of all registered topics?=======
 if(topic==mGetAll.topicIn && payload==mGetAll.payloadIn)
 {//.....get all topics.........................................
  std::string s1="";
  for(int i=0; i<num; i++){
   if(i>0) s1+=WDOG_DELIMITER1;
   s1+=vIn.at(i).topicIn;
  }
  //.....prepare output.........................................
  if(s1.length()<1) s1="(NO topics to be monitored)";
  sPay=mGetAll.payloadOut;
  if(sPay.find(WDOG_PLAHO_LIST)!=std::string::npos)
   g_utils.replaceAll(sPay, WDOG_PLAHO_LIST, s1);
  else {
   if(sPay.length()<1) sPay=s1; else sPay+=" "+s1;
  }
  //....topics available: send message.........................
  int iRet=mosquitto_publish(mosq,NULL,mGetAll.topicOut.c_str(),
  sPay.length(), sPay.c_str(), 0, mGetAll.retainOut);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",mGetAll.topicOut.c_str(),iRet);
   bRet=false;
  }
 }
 
 //======is the message a query of all overdue topics?==========
 if(topic==mGetOver.topicIn && payload==mGetOver.payloadIn)
 {//.....get overdue topics.....................................
  std::string s1="";
  for(int i=0; i<num; i++){
   WdogIn1 vIn1_=vIn.at(i);             // current sensor
   time_t tdiff=now-vIn1_.secLastSensor;
   if(tdiff>vIn1_.secDiff)
   {//---watchdog timeout---------------------------------------
    if(s1.length()>0) s1+=WDOG_DELIMITER1;
    s1+=vIn.at(i).topicIn;
    s1+=" ";
    s1+=g_utils.sec2HMS(tdiff);
   }
  }
  //.....prepare output.........................................
  if(s1.length()<1) s1="(No overdue topics)";
  sPay=mGetOver.payloadOut;
  if(sPay.find(WDOG_PLAHO_LIST)!=std::string::npos)
   g_utils.replaceAll(sPay, WDOG_PLAHO_LIST, s1);
  else {
   if(sPay.length()<1) sPay=s1; else sPay+=" "+s1;
  }

  /*
  if(s1.length()>0)
  {//....topics available: replace <list>.......................
   sPay=mGetOver.payloadOut;
   if(sPay.length()<1) sPay=s1;
   else g_utils.replaceAll(sPay, WDOG_PLAHO_LIST, s1);
  }
  else sPay="(No overdue topics)";
  */
  //....topics available: send message.........................
  int iRet=mosquitto_publish(mosq,NULL,mGetOver.topicOut.c_str(),
  sPay.length(), sPay.c_str(), 0, mGetOver.retainOut);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",mGetOver.topicOut.c_str(),iRet);
   bRet=false;
  }
 }
 //======for all sensors========================================
 for(int i=0; i<num; i++){
  if(topic==vIn.at(i).topicIn) 
  {//----topic for watchdog found-------------------------------
   vIn.at(i).secLast=now;              // sensor OR overdue msg
   vIn.at(i).secLastSensor=now;        // last time sensor secs
   if(g_prt) std::cout<<"Update secLast: "<<vIn.at(i).topicIn<<std::endl;
   return true;
  }
 }
 return bRet;
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
  if((now-vIn1_.secLast)>vIn1_.secDiff)
  {//----watchdog timeout---------------------------------------
   vIn.at(i).secLast=now;              // time sensor OR overdue msg
   //....replace <in> by topicIn................................
   std::string sPay=mWdogOut.payload;
   std::string sOld=WDOG_PLAHO_TOPIC_IN;
   std::string sNew=vIn1_.topicIn;
   g_utils.replaceAll(sPay, sOld, sNew);  // replace <in>
   //....publish attention message..............................
   int iRet=mosquitto_publish(mosq, NULL, mWdogOut.topic.c_str(),
    sPay.length(), sPay.c_str(), 0, true);
   if(iRet!=0) {
    if(g_prt) fprintf(stderr, "Error %d: NOT published -t %s -m %s.\n",
      iRet, mWdogOut.topic.c_str(), sPay.c_str());
   } else {
    if(g_prt) std::cout<<"ATTENTION! Watchdog timeout "<<vIn1_.topicIn<<std::endl;
   }
  }
 }
}

// *************************************************************
//       Wdog: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Wdog g_wdog=Wdog();
