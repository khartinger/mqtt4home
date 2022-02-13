//_____C_AdZigbee2mqtt.hpp_______________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hZ2m -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_AdZigbee2mqtt.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-01-31 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  Z2M_SECTION         "z2m"
#define  Z2M_HEALTH_OUT_KEY  "healthout"
#define  Z2M_HEALTH_IN_KEY   "healthin"
#define  Z2M_GET_HEALTH_KEY  "gethealth"
#define  Z2M_RET_HEALTH_KEY  "rethealth"
#define  Z2M_HEALTH_OUT      "z2m/bridge/request/health_check"
#define  Z2M_HEALTH_IN       "z2m/bridge/response/health_check"
#define  Z2M_GET_HEALTH_PAY  "z2m/get"
#define  Z2M_GET_HEALTH_TOP  "health"
#define  Z2M_RET_HEALTH_TOP  "z2m/ret/health"
#define  Z2M_HEALTH_UNKNOWN  -1

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Z2m
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 int health;                           // 
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [z2m]
 std::string healthoutKey;             // 
 std::string healthinKey;              // 
 std::string getHealthKey;             // 
 std::string retHealthKey;             // 
 std::string healthout;                // 
 std::string healthin;                 // 
 std::string getHealthTop;             // 
 std::string getHealthPay;             // 
 std::string retHealthTop;             // 
 
 public:
 //------constructor & co---------------------------------------
 Z2m();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 int  testHealth(struct mosquitto *mosq);
 std::string getsHealth();
 //-----helper methods------------------------------------------
};

// *************************************************************
//       Z2m: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Z2m::Z2m() { setDefaults(); }

//_______set all default properties_____________________________
void Z2m::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = Z2M_SECTION;               // prog specifig info
 healthoutKey = Z2M_HEALTH_OUT_KEY;    // 
 healthinKey  = Z2M_HEALTH_IN_KEY;     // 
 getHealthKey = Z2M_GET_HEALTH_KEY;    // 
 retHealthKey = Z2M_RET_HEALTH_KEY;    // 
 healthout    = Z2M_HEALTH_OUT;        // 
 healthin     = Z2M_HEALTH_IN;         // 
 health       = Z2M_HEALTH_UNKNOWN;    // 
 getHealthTop = Z2M_GET_HEALTH_TOP;    //
 getHealthPay = Z2M_GET_HEALTH_PAY;    //
 retHealthTop = Z2M_RET_HEALTH_TOP;    //
 keys=std::string(Z2M_HEALTH_OUT_KEY); // all keys in section
 keys+="|"+std::string(Z2M_HEALTH_IN_KEY);
 keys+="|"+std::string(Z2M_GET_HEALTH_KEY);
 keys+="|"+std::string(Z2M_RET_HEALTH_KEY);
}

// *************************************************************
//       Z2m: setter and getter methods
// *************************************************************

// *************************************************************
//       Z2m: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Z2m::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Z2m::readConfig(std::string pfConf) 
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
  //.....topic of request for check_health......................
  if(sKey==healthoutKey) healthout=sVal;
  //.....topic of response check_health.........................
  if(sKey==healthinKey)  healthin=sVal;
  //.....topic (and payload) for z2m health request.............
  if(sKey==getHealthKey) {
   // sVal may be topic or topic payload
   std::string sT, sP;
    if(!conf.split2String(sVal, sT, sP, ' ')) sT=sVal;
    getHealthTop=sT;
    getHealthPay=sP;
  }
  //.....topic for z2m health response..........................
  if(sKey==retHealthKey) retHealthTop=sVal;
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void Z2m::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<healthoutKey<<"           | "<<healthout<<std::endl;
 std::cout<<healthinKey<<"            | "<<healthin<<std::endl;
 std::cout<<getHealthKey<<"           | -t "<<getHealthTop<<" -m "<<getHealthPay<<std::endl;
 std::cout<<retHealthKey<<"           | "<<retHealthTop<<std::endl;
}

//_______act on messages..._____________________________________
bool Z2m::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 if(topic==healthin)
 { //---------topic "health responce" received------------------
  health=0;
  if(payload.find("{\"healthy\":true}")>0) health|=1;
  if(payload.find("{\"status\":\"ok\"")>0) health|=2;
  if(g_prt) std::cout<<"topic: "<<topic<<" ==> "<<getsHealth()<<std::endl;
  return true;
 }
 if(topic==getHealthTop)
 { //---------send zigbee2mqtt health state---------------------
  if(payload==getHealthPay) {
   testHealth(mosq);
   std::string s1 = getsHealth();
   int ret=mosquitto_publish(mosq, NULL,retHealthTop.c_str(), s1.length(), s1.c_str(), 0, false);
   if(g_prt) {
    if(ret!=0) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",retHealthTop.c_str(),ret);
    else std::cout<<s1<<std::endl;
   }
  }
 }
 //-----------Add additional code here...-----------------------
 return true;
}

//_______Possibility for cleanup before end of program__________
void Z2m::onExit(struct mosquitto *mosq, int reason)
{
 
}

//_______send health request to adzigbee2mqtt___________________
// send to z2m: -t z2m/bridge/request/health_check -n
// receive: -t z2m/bridge/response/health_check
//          -m {"data":{"healthy":true},"status":"ok"}
// return: 1 = health OK, 2 = status OK, 3 = both ok
//         0 = not OK, -1 result unknown, -2 publish error
int Z2m::testHealth(struct mosquitto *mosq)
{
 int ret=mosquitto_publish(mosq, NULL,healthout.c_str(), 0, "", 0, false);
 if(ret!=0) return -2;
 //-----wait for answer message (processed in onMessage)--------
 std::this_thread::sleep_for(std::chrono::milliseconds(10));
 return health;
}

//_______value of "health" of adzigbee2mqtt bridge as string______
std::string Z2m::getsHealth()
{
 std::string s1="zigbee2mqtt-health is ";
 switch(health)
 {
  case 3: s1+="perfect"; break;
  case 2: s1+="ok-status"; break;
  case 1: s1+="ok-healthy"; break;
  case 0: s1+="bad"; break;
  default: s1+="unknown"; break;
 }
 return s1;
}

// *************************************************************
//       Z2m: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Z2m g_z2m=Z2m();
