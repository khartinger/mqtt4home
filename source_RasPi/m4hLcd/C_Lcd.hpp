//_____C_Lcd.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hLcd -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Lcd.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2022-09-19 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include <fcntl.h>                     // open, O_WRONLY, O_CREAT...

#define LCD_SECTION               "lcd"
#define LCD_FILE                  "/sys/class/backlight/rpi_backlight/brightness"
// Raspi: sudo chmod 777 /sys/class/backlight/rpi_backlight/brightness
//-------query brightness value---------------------------------
#define LCD_GET_IN_KEY            "getin"
#define LCD_GET_IN_T              "lcd/get"
#define LCD_GET_IN_P              "?,help,brightness"
#define LCD_GET_OUT_KEY           "getout"
#define LCD_GET_OUT_T             "lcd/ret/"
//-------set brightness value-----------------------------------
#define LCD_SET_IN_KEY            "setin"
#define LCD_SET_IN_T_BASE         "lcd/set"
#define LCD_SET_IN_T_EXT          "brightness"
#define LCD_SET_OUT_KEY           "setout"
#define LCD_SET_OUT_T             "lcd/ret/"

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Lcd
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [lcd]
 //------key words in config file-------------------------------
 std::string sGetinKey;                // 
 std::string sGetoutKey;               // 
 std::string sSetinKey;                // 
 std::string sSetoutKey;               // 
 std::string sGetinTopic;              // get topic in
 std::string sGetoutTopic;             // get topic return
 std::string sSetinTopicBase;          // set topic in base
 std::string sSetoutTopic;             // set topic return
 std::vector<std::string> vGetinPayload; // all get payloads
 std::vector<std::string> vSetinTopicExt;// all set topics 
 
 public:
 //------constructor & co---------------------------------------
 Lcd();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 bool periodic(struct mosquitto *mosq);

 //-----helper methods------------------------------------------
 bool doGetinAction(struct mosquitto *mosq, std::string sGetWhat);
 bool doSetinAction(struct mosquitto *mosq, std::string sSetWhat, std::string sSetValue);
 bool writeBrightness(std::string sBrightness);
 std::string readBrightness();
};

// *************************************************************
//       Lcd: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Lcd::Lcd() { setDefaults(); }

//_______set all default properties_____________________________
void Lcd::setDefaults()
{
 pfConfig  = _CONF_PFILE_;             // path&name config file
 section   = LCD_SECTION;              // prog specifig info
 Conf conf=Conf();                     // config object
 //------key words in config file-------------------------------
 sGetinKey  = LCD_GET_IN_KEY;          // 
 sGetoutKey = LCD_GET_OUT_KEY;         // 
 sSetinKey  = LCD_SET_IN_KEY;          // 
 sSetoutKey = LCD_SET_OUT_KEY;         // 
 //------parameter for get message------------------------------
 sGetinTopic = LCD_GET_IN_T;           // get topic (in)
 vGetinPayload.clear();                // all get payloads
 conf.splitString(LCD_GET_IN_P, vGetinPayload, ',');
 //......remove remove blank(s) from begin and end of string....
 int iSize = vGetinPayload.size();
 for(int i=0; i<iSize; i++)
 {
   conf.delExtBlank(vGetinPayload.at(i));
 }
 sGetoutTopic = LCD_GET_OUT_T;         // get topic (answer)
 //------parameter for set message------------------------------
 sSetinTopicBase = LCD_SET_IN_T_BASE;  // set topic base
 vSetinTopicExt.clear();               // all set topics extensions
 conf.splitString(LCD_SET_IN_T_EXT, vSetinTopicExt, ',');
 //......remove remove blank(s) from begin and end of string....
 iSize = vSetinTopicExt.size();
 for(int i=0; i<iSize; i++)
 {
   conf.delExtBlank(vSetinTopicExt.at(i));
 }
 sSetoutTopic = LCD_SET_OUT_T;         // set topic (answer)
 //------key words as one string--------------------------------
 keys=sGetinKey;                       // all keys in section
 keys+="|"+sGetoutKey;
 keys+="|"+sSetinKey;
 keys+="|"+sSetoutKey;
}

// *************************************************************
//       Lcd: setter and getter methods
// *************************************************************

// *************************************************************
//       Lcd: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Lcd::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Lcd::readConfig(std::string pfConf) 
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
  //.....topic for get messages.................................
  if(sKey==sGetinKey) {
   if(sVal.length()>0) {
    std::string sVal1=sVal, sVal2;
    if(conf.split2String(sVal, sVal1, sVal2, ' ')) {
     vGetinPayload.clear();
     conf.splitString(sVal2, vGetinPayload, ',');
    }
    sGetinTopic=sVal1;
   }
  }
  //.....topic for get answer (return) message..................
  if(sKey==sGetoutKey) {
   if(sVal.length()>0) {
    if(sVal.find_first_of(' ')==std::string::npos) {
     if(sVal.at(sVal.length()-1)!='/') sVal+="/";
     sGetoutTopic=sVal;
    }
   }
  }
  //.....topic for set messages.................................
  if(sKey==sSetinKey) {
   if(sVal.length()>0) {
    std::string sVal1=sVal, sVal2;
    if(conf.split2String(sVal, sVal1, sVal2, ' ')) {
     vSetinTopicExt.clear();
     conf.splitString(sVal2, vSetinTopicExt, ',');
    }
    sSetinTopicBase=sVal1;
   }
  }
  //.....topic for get answer (return) message..................
  if(sKey==sSetoutKey) {
   if(sVal.length()>0) {
    if(sVal.find_first_of(' ')==std::string::npos) {
     if(sVal.at(sVal.length()-1)!='/') sVal+="/";
     sSetoutTopic=sVal;
    }
   }
  }
 } // end for every line in section
 //------key words as one string--------------------------------
 keys=sGetinKey;                       // all keys in section
 keys+="|"+sGetoutKey;
 keys+="|"+sSetinKey;
 keys+="|"+sSetoutKey;
 return true;
}

//_______Show all properties____________________________________
void Lcd::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 //------show get topic and payload-----------------------------
 std::cout<<".....lcd: get message(s).............."<<std::endl;
 std::string s2="";
 for(int i=0; i<vGetinPayload.size(); i++)
 {
  if(i>0) s2+="|";
  s2+=vGetinPayload.at(i);
 }
 if(s2=="") s2="(no get payload!)";
 std::cout<<"get topic (in)      | "<<sGetinTopic<<std::endl;
 std::cout<<"get possible payload| "<<s2<<std::endl;
 std::cout<<"get return topic    | "<<sGetoutTopic<<std::endl;
 //------show get answer topic----------------------------------
 std::cout<<".....lcd: set message(s).............."<<std::endl;
 s2="";
 for(int i=0; i<vSetinTopicExt.size(); i++)
 {
  if(i>0) s2+="|";
  s2+=vSetinTopicExt.at(i);
 }
 if(s2=="") s2="(no set topic!)";
 std::cout<<"set topic base      | "<<sSetinTopicBase<<std::endl;
 std::cout<<"set possible values | "<<s2<<std::endl;
 std::cout<<"set return topic    | "<<sSetoutTopic<<std::endl;
}

//_______act on messages..._____________________________________
bool Lcd::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=true;
 if(sGetinTopic.length()<1 || sSetinKey.length()<1) return false;
 //======test for get message===================================
 if(topic==sGetinTopic) return doGetinAction(mosq, payload);
 //======test for set message===================================
 int len1=sSetinTopicBase.length();
 if(topic.substr(0,len1)==sSetinTopicBase)
  return doSetinAction(mosq, topic.substr(len1+1), payload);
 return bRet;
}

//_______Possibility for cleanup before end of program__________
void Lcd::onExit(struct mosquitto *mosq, int reason)
{
 
}

//_______periodic action________________________________________
bool Lcd::periodic(struct mosquitto *mosq)
{
 return true;
}

// *************************************************************
//       Lcd: helper methods
// *************************************************************
//_______getin action___________________________________________
// send a return message with the requested value, e.g.
// ?, help   : all keys (e.g. getin|getout|setin|setout)
// brightness: brightness value (12...255)
// value from file /sys/class/backlight/rpi_backlight/brightness
// return: true on success, false on error
bool Lcd::doGetinAction(struct mosquitto *mosq, std::string sGetWhat)
{
 bool bRet=false;
 std::string sTop="";
 std::string sPay="";
 // if(g_prt) std::cout<<"Lcd::onMessage(): get "<<payload<<std::endl;
 if(sGetWhat==vGetinPayload.at(0) || sGetWhat==vGetinPayload.at(1))
 { //---(? or) help--------------------------------------------
  sTop=sGetoutTopic+vGetinPayload.at(1);
  for(int i=0; i<vGetinPayload.size(); i++) {
   if(i>0) sPay+="|";
   sPay+=vGetinPayload.at(i);
  }
  sPay+=" keys:"+keys;
 }
 if(sGetWhat==vGetinPayload.at(2))
 { //---brightness---------------------------------------------
  sTop=sGetoutTopic+vGetinPayload.at(2);
  sPay=readBrightness();
 }
 //-----send get answer message--------------------------------
 if(sTop!="") {
  int ret=mosquitto_publish(mosq, NULL,sTop.c_str(),
    sPay.length(), sPay.c_str(), 0, false);
  if(ret!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
  } else {
   if(g_prt) std::cout<<"Sent -t "<<sTop<<" -m "<<sPay<<std::endl;
   bRet=true;
  }
 }
 else
 {
  if(g_prt) fprintf(stderr, "MQTT response for \"%s\" not sent: Wrong payload \"%s\"\n", sGetinTopic.c_str(), sGetWhat.c_str());
 }
 return bRet;
}

//_______setin action___________________________________________
// set brightness value to file
// /sys/class/backlight/rpi_backlight/brightness
// and send a return message (brightness or error message)
// return: true on success, false on error
bool Lcd::doSetinAction(struct mosquitto *mosq, std::string sSetWhat, std::string sSetValue)
{
 // if(g_prt) std::cout<<"Lcd::onMessage(): try to set "<<sSetWhat<<" to "<<sSetValue<<std::endl;
 bool bRet=false;
 std::string sTop=sSetoutTopic+sSetWhat;
 std::string sPay=sSetValue+"_not_set!_(12...255)";
 if(writeBrightness(sSetValue)) sPay=sSetValue;
 //-----send set answer message--------------------------------
 if(sTop!="") {
  int ret=mosquitto_publish(mosq, NULL,sTop.c_str(),
    sPay.length(), sPay.c_str(), 0, false);
  if(ret!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
  } else {
   bRet=true;
   if(g_prt) std::cout<<"Sent -t "<<sTop<<" -m "<<sPay<<std::endl;
  }
 }
 else
 {
  std::string sSetinTopic = sSetinTopicBase + sSetWhat;
  if(g_prt) fprintf(stderr, "MQTT response for \"%s\" not sent: Wrong payload \"%s\"\n", sSetinTopic.c_str(), sSetValue.c_str());
 }
 return bRet;
}

//_______write brightness value (12..255) to file_______________
// /sys/class/backlight/rpi_backlight/brightness
// return: true if file written, false on error
bool Lcd::writeBrightness(std::string sBrightness)
{
 //------check brightness value---------------------------------
 int i=atoi(sBrightness.c_str());
 if(i<12 || i>255) return false;
 //------try to open file stream--------------------------------
 int fh;                               // file handle
 std::string fname=LCD_FILE;
 int flags_ = O_WRONLY | O_CREAT;
 mode_t permissions_ = S_IRWXU | S_IRWXG | S_IRWXO; // = 0777
 int iTrials=3;
 while(iTrials>0) {
  iTrials--;
  fh=open(fname.c_str(), flags_, permissions_);
  if(fh != -1)
  {
   //----write or append line to file---------------------------
   int len=sBrightness.length();
   int iret = write(fh, sBrightness.c_str(), len);
   if( iret == len) { 
    close(fh); 
    return true;                       // success!
   }
   close(fh);
  }
  //-----wait and try again-------------------------------------
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
 }
 return false;
}

//_______read brightness value (12..255) from file as string____
// /sys/class/backlight/rpi_backlight/brightness
// return: value as string or "" on error
std::string Lcd::readBrightness()
{
 std::string sRet="";
 std::string fname=LCD_FILE;
 //------read brightness from file------------------------------
 std::ifstream fstream(fname.c_str());
 if(!fstream.good()) return sRet;
 std::getline(fstream, sRet);
 fstream.close();
 return sRet;
}

//_______declare a global little helpers object ;)______________
Lcd g_lcd=Lcd();
