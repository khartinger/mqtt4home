//_____C_Coll.hpp________________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hColl -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Coll.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-31 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions

#define COLL_SECTION         "coll"
#define COLL_IN_KEY          "in"
#define COLL_IN_PART_KEY     "in_part"
#define COLL_SKIP_KEY        "skip"
#define COLL_KEY_KEY         "key"
#define COLL_QUERY_KEY       "query"
#define COLL_OUT_KEY         "out"
#define COLL_RETAIN_OUT_KEY  "retain"
#define COLL_QUERY_ALL       "all"
#define COLL_QUERY_ALL2      "?"
#define COLL_QUERY_CLEAR     "clear"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
// class Coll1: Class for a certain collection/key
// *************************************************************
class Coll1
{
 public:
 //------properties---------------------------------------------
 Message2 m2;
 std::string key;                      // key to collect
 std::string in_part;                  // part in topic to find
 std::string query;                    // topic to request data
 std::string queryPayload;             // payload to query data
 std::vector <std::string> vSkip;      // skip messages
 std::map<std::string, std::string>mState; // topic,value of key
 Coll1() { m2=Message2(); key=""; in_part=""; query="";
   queryPayload=""; vSkip.clear(); mState.clear(); };
};

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Coll
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 std::map<std::string, Coll1> mColl;   // all key,collection1
 // for data holding: key,map<topic,keyvalue>
 std::map<std::string,std::map<std::string,std::string>> mVal;
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [coll]
 std::string topicOutLast;             // to avoid endless...
 std::string payloadOutLast;           // ...message sending

 public:
 //------constructor & co---------------------------------------
 Coll();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 bool processQuery(Coll1 &c1, std::string payload);
 //bool savePayload(Coll1 &c1, std::string sValue);
bool savePayload(std::string key, std::string topic, std::string sValue);
 void onExit(struct mosquitto *mosq, int reason);

 //-----helper methods------------------------------------------
};

// *************************************************************
//       Coll: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Coll::Coll() { setDefaults(); }

//_______set all default properties_____________________________
void Coll::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = COLL_SECTION;              // prog specifig info
 mColl.clear();                        // all key,collection
 topicOutLast = "";                    // to avoid endless...
 payloadOutLast = "";                  // ...message sending
 keys=std::string(COLL_IN_KEY);        // 
 keys+="|"+std::string(COLL_IN_PART_KEY);
 keys+="|"+std::string(COLL_SKIP_KEY);
 keys+="|"+std::string(COLL_KEY_KEY);
 keys+="|"+std::string(COLL_QUERY_KEY);
 keys+="|"+std::string(COLL_OUT_KEY);
 keys+="|"+std::string(COLL_RETAIN_OUT_KEY);
}

// *************************************************************
//       Coll: setter and getter methods
// *************************************************************

// *************************************************************
//       Coll: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Coll::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Coll::readConfig(std::string pfConf) 
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
 mColl.clear();                        // new collection
 //------for all map values (=section) in vector----------------
 for(int i=0; i<iSec; i++)
 {
  //--convert one map value (section) to coll1, add it to vColl-
  m1=v1.at(i);
  Coll1 c1=Coll1();
  int ok=0;                            // check for key/query/out
  //-----analyse map parts--------------------------------------
  for(it1=m1.begin(); it1!=m1.end(); it1++)
  {
   //....topic of incomming message to process..................
   if(it1->first==COLL_IN_KEY)
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    c1.m2.topicIn=sT;
    c1.m2.payloadOut=sP;
   }
   //....part of incomming topic to search for..................
   if(it1->first==COLL_IN_PART_KEY) c1.in_part=it1->second;
   //....topics of incomming messages to skip...................
   if(it1->first==COLL_SKIP_KEY)
   {
    c1.vSkip.clear();
    conf.splitString(it1->second,c1.vSkip,',');
   }
   //....key in payload to search...............................
   if(it1->first==COLL_KEY_KEY) {
    if((it1->second).length()>0) {
     c1.key=it1->second;
     ok|=1;                            // key is ok
    }
   }
   //....topic to query data....................................
   if(it1->first==COLL_QUERY_KEY) {
    if((it1->second).length()>0) {
     c1.query=it1->second;
     ok|=4;                            // query topic is ok
    }
   }
   //....out message............................................
   if(it1->first==COLL_OUT_KEY)
   {
    std::string sT="", sP="";
    if(!conf.split2String(it1->second, sT, sP, ' ')) sT=it1->second;
    c1.m2.topicOut=sT;
    c1.m2.payloadOut=sP;
    if(sT.length()>0) ok|=2;           // out message is ok
   }
   //....retain flag............................................
   if(it1->first==COLL_RETAIN_OUT_KEY) {
    if(it1->second=="true") c1.m2.retainOut=true;
   }
  } //end for all map values in vector
  if(ok==0x07) mColl.insert(std::make_pair(c1.key,c1));
  //if(ok==7) vPay1.push_back(p1);
 }
 if(mColl.size()<1) return false;
 return true;
}

//_______Show all properties____________________________________
void Coll::show()
{
 std::cout<<"=====["<<section<<"]==========================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 //std::cout<<COLL_DEMO_KEY<<"             | "<<_demo_<<std::endl;
 //------for all maps (sections)--------------------------------
 std::map<std::string,Coll1>::iterator it;
 int len2=mColl.size();
 std::cout<<"-----keys in payload to check---------"<<std::endl;
 if(len2<1) std::cout<<"(no keys)"<<std::endl;
 for (it = mColl.begin(); it != mColl.end(); it++)
 {
  Coll1 c1=it->second;
  std::cout<<"....."<<it->first<<".........................."<<std::endl;
  if(it->first!=c1.key) std::cout<<"ERROR! Unequal keys "<<it->first<<" and "<<c1.key<<std::endl;
  //-----topic in-----------------------------------------------
  std::cout<<"topic-"<<COLL_IN_KEY<<": ";
  if(c1.m2.topicIn.length()>0)   std::cout<<c1.m2.topicIn<<" | ";
  else std::cout<<"(all) | ";
  std::cout<<"topic-"<<COLL_IN_PART_KEY<<": ";
  if(c1.in_part.length()>0)   std::cout<<c1.in_part<<" | ";
  else std::cout<<"(all) | ";
  //-----topics to skip-----------------------------------------
  std::cout<<COLL_SKIP_KEY<<": ";
  if(c1.vSkip.size()>0) {
   for(int j=0; j<c1.vSkip.size(); j++) {
    if(j>0) std::cout<<",";
    std::cout<<c1.vSkip.at(j);
   }
  }
  else std::cout<<"(none)";
  std::cout<<std::endl;
  //-----query topic, out topic---------------------------------
  std::cout<<"     "<<COLL_QUERY_KEY<<": "<<c1.query<<" | ";
  std::cout<<COLL_OUT_KEY<<": -t "<<c1.m2.topicOut<<" -m "<<c1.m2.payloadOut;
  if(c1.m2.retainOut) std::cout<<" -r";
  std::cout<<std::endl;
 }
}

//_______act on messages..._____________________________________
bool Coll::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=false;
 //-----------plausibitity check (if payload is JSON format, ..)
 //if(payload.length()<5) return false;
 //if(payload.at(0)!='{') return false;
 //if(payload.at(payload.length()-1)!='}') return false;
 if(topic==topicOutLast && payload==payloadOutLast) return false;
 if(mColl.size()<1) return false;
 //-----------check all sections--------------------------------
 std::map<std::string,Coll1>::iterator it0;
 for (it0 = mColl.begin(); it0 != mColl.end(); it0++)
 {//----------for: process one config section entry-------------
  Coll1 c1=Coll1();                    // help value
  c1=it0->second;                      // this collection
//std::cout<<"onMessage(): topic="<<topic<<", c1.query="<<c1.query<<std::endl;
  if(c1.query==topic)
  {//=-=-=-=-=query topic found-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   c1.queryPayload=payload;
   if(processQuery(c1, payload))
   {//--------send answer message-------------------------------
    topicOutLast=c1.m2.topicOut;
    payloadOutLast=c1.m2.payloadOut;
    int iRet=mosquitto_publish(mosq, NULL,topicOutLast.c_str(),
     payloadOutLast.length(), payloadOutLast.c_str(), 0, c1.m2.retainOut);
    if(iRet==0) bRet=true;
    else {
     if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",c1.m2.topicOut.c_str(),iRet);
     bRet=false;
    } //end else iRet==0
   } //end send answer message
   continue;
  } // end query topic found
  //=-=-=-=-=NOT query topic-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //----------check topic---------------------------------------
  if(c1.m2.topicIn.length()>0) {       // special topic given?
   if(c1.m2.topicIn!=topic) continue;  // topics unequal
  }
  if(c1.in_part.length()>0) {          // part of topic given?
   int iPos=topic.find(c1.in_part);
   if(iPos==std::string::npos) continue;// part not found
  }
  c1.m2.topicIn=topic;
  //----------search for payload key----------------------------
  int pos1=payload.find(c1.key);
  if(pos1==std::string::npos) continue;// key not found
  //----------payload key found: find value of payload key------
  std::multimap<std::string, std::string>mm1;
  std::multimap<std::string, std::string>::iterator it1;
  Conf conf=Conf();                    // for help functions
  conf.split2pairs(payload, mm1);      // payload -> json
  if(mm1.size()<1) continue;           // no pairs
  //----------split payload to pairs ok-------------------------
  it1=mm1.find(c1.key);
  if(it1==mm1.end()) continue;         // key not found
  //--------process payload-----------------------------------
  if(savePayload(c1.key, topic, it1->second)) {

  }
 } //end for: process one config payload entry
 return bRet;
}

//_______process a query________________________________________
bool Coll::processQuery(Coll1 &c1, std::string payload)
{
//std::cout<<"processQuery(): key="<<c1.key<<", pay="<<payload;
//std::cout<<", out="<<c1.m2.topicOut<<std::endl;
 Conf conf=Conf();
 std::string sPay="";
 std::map<std::string,std::string> m1;
 std::map<std::string,std::string>::iterator it1;
 std::map<std::string,std::map<std::string,std::string>>::iterator it2;
 //------is there an entry for the key?-------------------------
 it2=mVal.find(c1.key);
 //if(it2==mVal.end()) { c1.m2.payloadOut=""; return false; }
 if(it2==mVal.end()) { c1.m2.payloadOut="(null)"; return true; }
 //------entry found for the key--------------------------------
 m1=it2->second;  // values to spec key
 //if(m1.size()<1) { c1.m2.payloadOut=""; return false; }
 if(m1.size()<1) { c1.m2.payloadOut="(null)"; return true; }
 if(c1.queryPayload==COLL_QUERY_ALL || c1.queryPayload==COLL_QUERY_ALL2)
 {//-----return all pairs---------------------------------------
  sPay="{\"";
  for(it1 = m1.begin(); it1 != m1.end(); it1++) 
  {
   if(it1!=m1.begin()) sPay+="\",\"";
   sPay+=it1->first;
   sPay+="\":\"";
   sPay+=it1->second;
  } 
  if(sPay.length()>2) sPay+="\"}";
  else sPay="(null)";
 }
 else if(c1.queryPayload==COLL_QUERY_CLEAR)
 {//-----add your code here...----------------------------------
  mVal.clear();
 }
 else if(false)
 {//-----add your code here...----------------------------------
  
 }
 else
 {//-----compare value with c1.queryPayload---------------------
  sPay="{\"";
  for(it1 = m1.begin(); it1 != m1.end(); it1++) 
  {
   if(it1->second==c1.queryPayload)
   {
    if(sPay.length()>2) sPay+="\",\"";
    sPay+=it1->first;
    sPay+="\":\"";
    sPay+=it1->second;
   }
  } 
  if(sPay.length()>2) sPay+="\"}";
  else sPay="(null)";
 }
 c1.m2.payloadOut=sPay;
 return true;
}

//_______edit special payload___________________________________
bool Coll::savePayload(std::string key, std::string topic, std::string sValue)
{
 //std::cout<<"savePayload(): key="<<key<<", topic="<<topic<<", val="<<sValue<<std::endl;
 if(key.length()<1) return false;
 if(topic.length()<1) return false;
 std::map<std::string,std::string> m1;
 std::map<std::string,std::string>::iterator it1;
 std::map<std::string,std::map<std::string,std::string>>::iterator it2;
 //------is there an entry for the key?-------------------------
 it2=mVal.find(key);
 if(it2!=mVal.end()) m1=it2->second;   // values to spec key
 //------insert new value---------------------------------------
 mVal.insert(std::make_pair(key, std::map<std::string,std::string>()));
 mVal[key].erase(topic);
 mVal[key].insert(std::make_pair(topic,sValue));
 return true;
}

//_______Possibility for cleanup before end of program__________
void Coll::onExit(struct mosquitto *mosq, int reason)
{
 
}

// *************************************************************
//       Coll: helper methods
// *************************************************************

//_______declare a global little helpers object ;)______________
Coll g_coll=Coll();
