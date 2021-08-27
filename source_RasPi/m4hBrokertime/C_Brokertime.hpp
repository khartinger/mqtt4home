//_____C_Brokertime.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hBrokertime -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Brokertime.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define  BROKERTIME_SECTION       "brokertime"
#define  BROKERTIME_IN_KEY        "in"
#define  BROKERTIME_OUT_KEY       "out"
#define  BROKERTIME_RETAIN_KEY    "retain"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Brokertime: messages that request brokertime
// *************************************************************
class Brokertime
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [brokertime]
 std::vector <Message2> vBtMsg2;       // brokertime messages
 public:
 //------constructor & co---------------------------------------
 Brokertime();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 std::vector <Message2> getMessages() { return vBtMsg2; }
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);

 //-----helper methods------------------------------------------
};

// *************************************************************
//       Brokertime: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Brokertime::Brokertime() { setDefaults(); }

//_______set all default properties_____________________________
void Brokertime::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = BROKERTIME_SECTION;        // prog specifig info
 vBtMsg2.clear();                      // no brokertime messages
 keys=std::string(BROKERTIME_IN_KEY);  // all keys in section
 keys+="|"+std::string(BROKERTIME_OUT_KEY);
 keys+="|"+std::string(BROKERTIME_RETAIN_KEY);
}

// *************************************************************
//       Brokertime: setter and getter methods
// *************************************************************

// *************************************************************
//       Brokertime: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Brokertime::readConfig() { return readConfig(pfConfig); }

//_______read config data (messages) for brokertime_____________
// convert sections to Message2 objects stored in vector vBtMsg2
// return: true if messages exist, false on error
bool Brokertime::readConfig(std::string pfConf) 
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
 vBtMsg2.clear();                      // new messages
 if(g_prt) std::cout<<"-----requests for brokertime:-----"<<std::endl;
 //------for all map values in vector---------------------------
 for(int i=0; i<iSec; i++)
 {
  //-----convert one map value to Message2 and add it to vBtMsg2
  m1=v1.at(i);
  Message2 m2=Message2();
  m2.retainOut=false;
  int ok=0;                            // check for in and out
  //-----analyse map parts--------------------------------------
  for(it1=m1.begin(); it1!=m1.end(); it1++)
  {
   if(it1->first=="in")
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    m2.topicIn=sT;
    m2.payloadIn=sP;
    ok|=1;                             // in ok: set Bit 0
   }
   if(it1->first=="out")
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    m2.topicOut=sT;
    m2.payloadOut=sP;
    ok|=2;                             // out ok: set Bit 1
   }
   if(it1->first=="retain")
   {
    if(it1->second=="true") m2.retainOut=true;
   }
  }
  if(ok==3) vBtMsg2.push_back(m2);
  if(g_prt) std::cout<<m2.topicIn<<"|"<<m2.payloadIn<<" ==> "<<m2.topicOut<<"|"<<m2.payloadOut<<"|"<<m2.retainOut<<std::endl;
 }
 if(vBtMsg2.size()<1) return false;
 return true; 
}

//_______Show all properties____________________________________
void Brokertime::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 int len2=vBtMsg2.size();
 std::cout<<".....Answer messages..........................."<<std::endl;
 if(len2<1) std::cout<<"(no messages)"<<std::endl;
 for(int i=0; i<len2; i++) {
  Message2 m2=vBtMsg2.at(i);
  std::cout<<"IN: -t "<<m2.topicIn<<" -m "<<m2.payloadIn<<" ==> ";
  std::cout<<"OUT: -t "<<m2.topicOut<<" -m "<<m2.payloadOut;
  if(m2.retainOut) std::cout<<" -r";
  std::cout<<std::endl;
 }
}

//_______send date+time answer message__________________________
// return: true=message sent
bool Brokertime::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 //------check all brokertime requests--------------------------
 int iLen=vBtMsg2.size();
 for(int i=0; i<iLen; i++) {
  Message2 m2=Message2();
  m2=vBtMsg2.at(i);
  if(m2.topicIn==topic) {
   if(payload==m2.payloadIn || m2.payloadIn.length()==0)
   {//---incoming message matches a stored message--------------
    std::string s1=g_base.getDateTime(m2.payloadOut);
    int ret=mosquitto_publish(mosq, NULL,m2.topicOut.c_str(),
     s1.length(), s1.c_str(), 0, m2.retainOut);
    if(ret!=0) {
     if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",ret,m2.topicOut.c_str());
     return false;
    } else {
     if(g_prt) std::cout<<" IN: -t "<<m2.topicIn<<" -m "<<m2.payloadIn<<" ==> OUT: -t "<<m2.topicOut<<" -m "<<s1;
     if(g_prt && m2.retainOut) std::cout<<" -r";
     if(g_prt) std::cout<<std::endl;
     return true;
    }
   }
  }
 }
 return false;
}

//_______Possibility for cleanup before end of program__________
void Brokertime::onExit(struct mosquitto *mosq, int reason)
{
}

// *************************************************************
//       Brokertime: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Brokertime g_brokertime=Brokertime();
