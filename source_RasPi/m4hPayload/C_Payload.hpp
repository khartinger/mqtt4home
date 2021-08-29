//_____C_Payload.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hPayload -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Payload.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-29 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include <iomanip>                     // setprecision
#include <sstream>                     // stringstream

#define PAYLOAD_SECTION           "payload"
#define PAYLOAD_IN_KEY            "in"
#define PAYLOAD_KEY_KEY           "key"
#define PAYLOAD_IF_KEY            "if"
#define PAYLOAD_ELSE_KEY          "else"
#define PAYLOAD_OUT_KEY           "out"
#define PAYLOAD_RETAIN_OUT_KEY    "retain"
#define PAYLOAD_EXC_TOPIC_IN      "<in>"
#define PAYLOAD_EXC_VALUE         "<value>"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
// class Payload1: Class for a certain payload
// *************************************************************
class Payload1
{
 public:
 //------properties---------------------------------------------
 Message2 m2;
 std::string key;
 int ifCond;                           // 1< 2<= 3= 4!= 5>= 6>
 std::string sCond[7]={"?","<","<=","=","!=",">=",">"};
 std::string ifVal;
 std::string ifText;
 std::string elseText;
 Payload1() {m2=Message2(); key=""; ifCond=0; ifVal=""; ifText=""; elseText="";};
};

// *************************************************************
//    class Payload: Process specific payloads
// *************************************************************
class Payload
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [payload]
 std::vector <Payload1> vPay1;         // payload messages
 public:
 //------constructor & co---------------------------------------
 Payload();                                // constructor
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
 bool editPayload(Payload1 &p1, std::string sValue);
};

// *************************************************************
//       Payload: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Payload::Payload() { setDefaults(); }

//_______set all default properties_____________________________
void Payload::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = PAYLOAD_SECTION;           // prog specifig info
 vPay1.clear();                        // all payloads
 keys=std::string(PAYLOAD_IN_KEY);     // all keys in section
 keys+="|"+std::string(PAYLOAD_KEY_KEY);
 keys+="|"+std::string(PAYLOAD_IF_KEY);
 keys+="|"+std::string(PAYLOAD_ELSE_KEY);
 keys+="|"+std::string(PAYLOAD_OUT_KEY);
 keys+="|"+std::string(PAYLOAD_RETAIN_OUT_KEY);
}

// *************************************************************
//       Payload: setter and getter methods
// *************************************************************

// *************************************************************
//       Payload: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Payload::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Payload::readConfig(std::string pfConf) 
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
 vPay1.clear();                        // new payloads
 if(g_prt) std::cout<<"-----requests for specific payload:-----"<<std::endl;
 //------for all map values in vector---------------------------
 for(int i=0; i<iSec; i++)
 {
  //-----convert one map value to payload1 and add it to vPay1--
  m1=v1.at(i);
  Payload1 p1=Payload1();
  int ok=0;                            // check for in and out
  //-----analyse map parts--------------------------------------
  for(it1=m1.begin(); it1!=m1.end(); it1++)
  {
   //....in message.............................................
   if(it1->first==PAYLOAD_IN_KEY)
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    p1.m2.topicIn=sT;
    p1.m2.payloadOut=sP;
   }
   //....key in payload to search...............................
   if(it1->first==PAYLOAD_KEY_KEY) {
    if((it1->second).length()>0) {
     p1.key=it1->second;
     ok|=1;                            // key is ok
    }
   }
   //....condition..............................................
   if(it1->first==PAYLOAD_IF_KEY) {
    if((it1->second).length()>2) {
     std::string sC="", sT="";
     if(conf.split2String(it1->second, sC, sT, ' ')) {
      if(sC.length()>1)
      {
       if(sC.at(0)=='<') {
        if(sC.at(1)=='=') {p1.ifCond=2; p1.ifVal=sC.substr(2);}
                     else {p1.ifCond=1; p1.ifVal=sC.substr(1);}
       }
       else if(sC.at(0)=='=') {p1.ifCond=3; p1.ifVal=sC.substr(1);}
       else if(sC.at(0)=='!' && sC.at(1)=='=') {p1.ifCond=4;p1.ifVal=sC.substr(2);}
       else if(sC.at(0)=='>') {
        if(sC.at(1)=='=') {p1.ifCond=6; p1.ifVal=sC.substr(2);}
                     else {p1.ifCond=5; p1.ifVal=sC.substr(1);}
       }
       if(p1.ifCond>0) p1.ifText=sT;
      }
     }
    }
   }
   //....else text..............................................
   if(it1->first==PAYLOAD_ELSE_KEY) p1.elseText=it1->second;
   //....out message............................................
   if(it1->first==PAYLOAD_OUT_KEY)
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    p1.m2.topicOut=sT;
    p1.m2.payloadOut=sP;
    ok|=2;                             // out message is ok
   }
   //....retain flag............................................
   if(it1->first==PAYLOAD_RETAIN_OUT_KEY) {
    if(it1->second=="true") p1.m2.retainOut=true;
   }
  } //end for all map values in vector
  if(ok==3) vPay1.push_back(p1);
 }
 if(vPay1.size()<1) return false;
 return true;
}

//_______Show all properties____________________________________
void Payload::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 int len2=vPay1.size();
 std::cout<<".....Payloads to check..........................."<<std::endl;
 if(len2<1) std::cout<<"(no payloads)"<<std::endl;
 for(int i=0; i<len2; i++) {
  Payload1 p1=vPay1.at(i);
  std::cout<<"topic-"<<PAYLOAD_IN_KEY<<": "<<p1.m2.topicIn<<" | ";
  std::cout<<PAYLOAD_KEY_KEY<<": "<<p1.key;
  if(p1.ifCond>0)
  {
   std::cout<<" | "<<PAYLOAD_IF_KEY<<" ";
  std::cout<<p1.sCond[p1.ifCond]<<p1.ifVal;
  std::cout<<" "<<p1.ifText<<" "<<PAYLOAD_ELSE_KEY<<" "<<p1.elseText;
  }
  std::cout<<" | OUT: -t "<<p1.m2.topicOut<<" -m "<<p1.m2.payloadOut;
  if(p1.m2.retainOut) std::cout<<" -r";
  std::cout<<std::endl;
 }
}

//_______act on messages..._____________________________________
bool Payload::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=false;
 //-----------check if payload is JSON format-------------------
 if(payload.length()<5) return false;
 if(payload.at(0)!='{') return false;
 if(payload.at(payload.length()-1)!='}') return false;
 //-----------check all payload entries-------------------------
 int iLen=vPay1.size();
 for(int i=0; i<iLen; i++) 
 {//----------for: process one config payload entry-------------
  Payload1 p1=Payload1();
  p1=vPay1.at(i);
  //----------search for payload key----------------------------
  int pos1=payload.find(p1.key);
  if(pos1==std::string::npos) continue;     // key not found
  //----------payload key found---------------------------------
  //----------find value of payload key-------------------------
  std::multimap<std::string, std::string>mm1;
  std::multimap<std::string, std::string>::iterator it1;
  std::string sValue="";
  Conf conf=Conf();
  conf.split2pairs(payload, mm1);
  if(mm1.size()>0) 
  {//---------split payload to pairs ok-------------------------
   it1=mm1.find(p1.key);
   if(it1!=mm1.end()) 
   {//--------payload ok, special topic given?------------------
    if(p1.m2.topicIn.length()>0) {
     if(p1.m2.topicIn!=topic) continue;
    }
    p1.m2.topicIn=topic;
    //--------process payload-----------------------------------
    if(editPayload(p1, it1->second))
    {//-------send answer message-------------------------------
     int iRet=mosquitto_publish(mosq, NULL,p1.m2.topicOut.c_str(),
     p1.m2.payloadOut.length(), p1.m2.payloadOut.c_str(), 0, p1.m2.retainOut);
     if(iRet==0) bRet=true;
     else {
      if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",p1.m2.topicOut.c_str(),iRet);
      bRet=false;
     } //end else iRet==0
    } //end send answer message
   } //end payload ok
  } //end split payload to pairs ok
 } //end for: process one config payload entry
 return bRet;
}

//_______edit special payload___________________________________
bool Payload::editPayload(Payload1 &p1, std::string sValue)
{
 bool bRet=false;
 Conf conf=Conf();
 std::string sPay="";
 //std::cout<<"editPayload(): key="<<p1.key<<", topic="<<p1.m2.topicIn<<", ifVal="<<p1.ifVal<<", sValue="<<sValue;
 //std::cout<<", ifText="<<p1.ifText<<", elseText="<<p1.elseText<<std::endl;
 //===========battery===========================================
 if(p1.key=="battery")
 {
  if(p1.ifCond>0 && p1.ifCond<7)
  {//-=-=-=-=-if condition given =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   bool ok=false;
   double dValConf=0, dValIn=0;
   try { //----string to double----------------------------------
    double temp=std::stod(p1.ifVal); dValConf=temp;
    temp=std::stod(sValue); dValIn=temp;
    ok=true;
   } catch(std::invalid_argument) {};
   if(ok) {
    sPay+=" ";
    //---------process condition---------------------------------
    switch(p1.ifCond) {
     case 1: // <
      if(dValIn<dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     case 2: // <=
      if(dValIn<=dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     case 3: // =
      if(dValIn==dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     case 4: // !=
      if(dValIn!=dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     case 5: // >=
      if(dValIn>=dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     case 6: // >
      if(dValIn>dValConf) sPay+=p1.ifText; else sPay+=p1.elseText;
      bRet=true;
      break;
     default: bRet=false; break;       // should never been done
    } // end switch
   } //end if(ok)
   else bRet=false;
   if(sPay.length()<2) bRet=false;
  } // end if condition given
  else
  {//-=-=-=-=-NO if condition given =-=-=-=-=-=-=-=-=-=-=-=-=-=-
   bRet=true;
  }
 } // end "battery"

 //===========double incomming payload==========================
 if(p1.key=="double")
 {
  bool ok=false;
  double d1=0;
  try { //----string to double----------------------------------
   double temp=std::stod(sValue); d1=temp;
   ok=true;
  } catch(std::invalid_argument) {};
  if(ok) {
   //----------build string with double value-------------------
   std::stringstream stream;
   stream << std::fixed << std::setprecision(2) << (2*d1);
   sPay+=" "+stream.str();
   bRet=true;
  }
  else bRet=false;
 } // end double incoming payload

 //===========contact===========================================
 if(p1.key=="contact")
 {//----------process condition---------------------------------
  switch(p1.ifCond) {
   case 3: // =
   case 4: // !=
    if(p1.ifVal=="true") {
     if(sValue=="true") sPay+=p1.ifText; else sPay+=p1.elseText;
     bRet=true;
     break;
    }
    if(p1.ifVal=="false") {
     if(sValue=="false") sPay+=p1.ifText; else sPay+=p1.elseText;
     bRet=true;
     break;
    }
    break;
   default: bRet=false; break;
  }
 } // end contact
 
 //=========== add your code here... ===========================
 // ...

 //===========finish answer=====================================
 if(bRet) {
  //----------build topic out-----------------------------------
  conf.replaceAll(p1.m2.topicOut,PAYLOAD_EXC_TOPIC_IN,p1.m2.topicIn);
  //----------build payload out---------------------------------
  //conf.replaceAll(p1.m2.payloadOut,PAYLOAD_EXC_TOPIC_IN,p1.m2.topicIn);
  //conf.replaceAll(p1.m2.payloadOut,PAYLOAD_EXC_VALUE,sValue);
  p1.m2.payloadOut+=sPay;
  conf.replaceAll(p1.m2.payloadOut,PAYLOAD_EXC_TOPIC_IN,p1.m2.topicIn);
  conf.replaceAll(p1.m2.payloadOut,PAYLOAD_EXC_VALUE,sValue);
  if(p1.m2.payloadOut.length()<1) bRet=false;    // no payload
 }
 return bRet;
}


//_______Possibility for cleanup before end of program__________
void Payload::onExit(struct mosquitto *mosq, int reason)
{
 
}

// *************************************************************
//       Payload: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Payload g_payload=Payload();
