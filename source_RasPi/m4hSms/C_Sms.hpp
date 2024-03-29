//_____C_Sms.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Sms.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-25 First release
// 2022-01-23 Add pubNum, formatSmsDate
// 2023-02-27 Add devices, topicsub2
// 2023-03-08 Update threads for modem access
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include "./modem/C_Gsm.h"             // modem and serial
#include <algorithm>                   // str.erase

#define  SMS_VERSION         "C_Sms-Version 2023-03-08"
#define  SMS_SECTION         "sms"
#define  SMS_DEVICE_KEY      "device"
#define  SMS_DEVICE          ""
#define  SMS_DEVICE_DEFAULT  "/dev/ttyS0"
#define  SMS_DEVICES_KEY     "devices"
#define  SMS_FROM_KEY        "from"
#define  SMS_TO_KEY          "to"

#define  SMS_TOPICSUB_KEY    "sub"
#define  SMS_TOPICSUB        "m4hSms/send"
#define  SMS_TOPICSUBRET_KEY "subret"
#define  SMS_TOPICSUBRET     "m4hSms/send/ret"

#define  SMS_TOPICPUB_KEY    "pub"
#define  SMS_TOPICPUB        "m4hSms/received"
#define  SMS_PUB_NUM_KEY     "pubnum"
#define  SMS_PUB_NUM         false

#define  SMS_TOPICSUB2_KEY   "sub2"
#define  SMS_TOPICSUB2       "m4hSms/cmd"
#define  SMS_TOPICSUB2RET_KEY "sub2ret"
#define  SMS_TOPICSUB2RET    "m4hSms/cmd/ret"

#define  SMS_CMD_VERSION_KEY "cmdversion"
#define  SMS_CMD_VERSION     "cmd_version"
#define  SMS_CMD_END_KEY     "cmdend"
#define  SMS_CMD_END         ""
#define  SMS_CMD_RELOAD_KEY  "cmdreload"
#define  SMS_CMD_RELOAD      "cmd_reload"
#define  SMS_CMD_CREDIT_KEY  "cmdcredit"
#define  SMS_CMD_CREDIT      "cmd_credit"
#define  SMS_NET_ID_KEY      "netid"
#define  SMS_NET_ID          GSM_NET_ID2   // defined in C_Gsm.h
#define  SMS_NET_TEL_KEY     "netphone"
#define  SMS_NET_TEL         GSM_NET_TEL2  // defined in C_Gsm.h
#define  SMS_NET_TXT_KEY     "nettext"
#define  SMS_NET_TXT         GSM_NET_TXT2  // defined in C_Gsm.h
#define  SMS_SEND_MAX_SEC    60
#define  SMS_SMS_START_KEY   "smsstart"
#define  SMS_SMS_END_KEY     "smsend"

#define  SMS_TXT_SENT        "SMS sent! "
#define  SMS_TXT_NOT_SENT    "SMS NOT sent! "

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
bool g_modemBusy=false;                // true=modem is sending

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Sms
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 bool testDeviceOnStart;               // send msg on error
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [sms]
 std::string testDeviceText2;          // additional text
 std::string device;                   // serial interface
 std::vector<std::string> vDevices;    // possible interfaces
 std::vector<std::string> vFrom;       // authorizised SMS from
 std::vector<std::string> vTo;         // authorizised SMS to
 std::string topicSub;                 // topic base subscribe
 std::string topicSubRet;              // topic subscribe answer
 std::string topicSub2;                // topic base subscribe
 std::string topicSub2Ret;             // topic subscribe answer
 std::string topicPub;                 // topic base publish
 bool retainOut;                       // retain publish
 bool pubNum;                          // publish phone number
 std::string cmdVersion;               // command send version
 std::string cmdEnd;                   // command end of program
 std::string cmdReload;                // cmd reload config file
 std::string cmdCredit;                // cmd send credit sms
 std::string netId;                    // name of provider
 std::string netTel;                   // phone for credit request
 std::string netTxt;                   // text for credit request
 std::string creditAnswerToPhone;      // request from phone #
 int sendTimeout;                      // max waiting to send sms
 std::vector<std::string>vSmsStart;    // send sms on prog start
 std::vector<std::string>vSmsEnd;      // send sms on prog end
 public:
 //------constructor & co---------------------------------------
 Sms();                                // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 std::string getDevice() {return device; } // module interface
 void setTimeout(int sec) { sendTimeout=sec; };
 void setTestDeviceText2(std::string txt) {testDeviceText2=txt;}
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool sendStartSms(Gsm gsm3, struct mosquitto *mosq);
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 int  lookForSmsIn(struct mosquitto *mosq);
 bool isAuthSmsFrom(std::string phoneNumber);
 bool isAuthSmsTo(std::string phoneNumber);
 bool sendCmd(std::string sCmd, std::string& sResult);
 bool publish(struct mosquitto *mosq, std::string topic, std::string payload, bool retain);
 // -----methods that use threads-------------------------------
 bool sendSms(std::string phone, std::string text, struct mosquitto *mosq);
 bool getCredit(struct mosquitto *mosq);
 // ----thread methods------------------------------------------
 static void threadFunctionSendSms(std::string phone, std::string text, std::string device_, struct mosquitto *mosq, std::string sTopic, int iTimeout);
 static void threadFunctionIsModule(std::string device_, std::string topicSub2Ret_, struct mosquitto *mosq_);
 static void threadFunctionIsSim(std::string device_, std::string topicSub2Ret_, struct mosquitto *mosq_);
 static void threadFunctionSendCmd(std::string device_, std::string sCmd, std::string topicSub2Ret_, struct mosquitto *mosq_);
 //-----helper methods------------------------------------------
 std::string getsHotEuro(std::string text); // hot credit
 double getHotEuro(std::string text);  // hot credit
 std::vector<std::string> string2vector(const std::string& data, const std::string& delimiters);
 bool split2phone(std::string sIn, std::vector<std::string>&vOut);
 int  parseSmsIn( std::string smsText, std::string& topicIn, std::string& payloadIn, bool& retainIn);
 std::string formatSmsDate(std::string smsDate, int format_);
 std::string findSmsModule();
 static std::string removeLFCR(std::string s1);
 static std::string replaceChar(std::string s1, char cOld, char cNew);
};

// *************************************************************
//       Sms: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Sms::Sms() { setDefaults(); }

//_______set all default properties_____________________________
void Sms::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = SMS_SECTION;               // prog specifig info
 device     = SMS_DEVICE;              // serial interface
 testDeviceOnStart = true;             // test device
 testDeviceText2="";                   // additional text
 vDevices.clear();                     // possible interfaces
 vFrom.clear();                        // authorizised SMS from
 vTo.clear();                          // authorizised SMS to
 topicSub   = SMS_TOPICSUB;            // topic base subscribe
 topicSubRet= SMS_TOPICSUBRET;         // topic subscribe answer
 topicSub2  = SMS_TOPICSUB2;           // topic base subscribe
 topicSub2Ret= SMS_TOPICSUB2RET;       // topic subscribe answer
 topicPub   = SMS_TOPICPUB;            // topic base publish
 pubNum     = SMS_PUB_NUM;             // publish phone number
 retainOut  = false;                   // retain publish msg
 cmdEnd     = SMS_CMD_END;             // command end of program
 cmdVersion = SMS_CMD_VERSION;         // command send version
 cmdReload  = SMS_CMD_RELOAD;          // cmd reload config file
 cmdCredit  = SMS_CMD_CREDIT;          // cmd send credit sms
 netId      = SMS_NET_ID;              // name of provider
 netTel     = SMS_NET_TEL;             // phone for credit request
 netTxt     = SMS_NET_TXT;             // text for credit request
 creditAnswerToPhone="";               // send credit answer to
 sendTimeout= SMS_SEND_MAX_SEC;        // max waiting to send sms
 vSmsStart.clear();                    // send sms on prog start
 vSmsEnd.clear();                      // send sms on prog end
 keys=std::string(SMS_DEVICE_KEY);
 keys+="|"+std::string(SMS_FROM_KEY)+"|"+std::string(SMS_TO_KEY);
 keys+="|"+std::string(SMS_TOPICSUB_KEY)+"|"+std::string(SMS_TOPICSUBRET_KEY);
 keys+="|"+std::string(SMS_TOPICSUB2_KEY)+"|"+std::string(SMS_TOPICSUB2RET_KEY);
 keys+="|"+std::string(SMS_TOPICPUB_KEY)+"|"+std::string(SMS_PUB_NUM_KEY);
 keys+="|"+std::string(SMS_SMS_START_KEY)+"|"+std::string(SMS_SMS_END_KEY);
 keys+="|"+std::string(SMS_CMD_VERSION_KEY)+"|"+std::string(SMS_CMD_END_KEY);
 keys+="|"+std::string(SMS_CMD_RELOAD_KEY)+"|"+std::string(SMS_CMD_CREDIT_KEY);
 keys+="|"+std::string(SMS_NET_ID_KEY)+"|"+std::string(SMS_NET_TEL_KEY);
 keys+="|"+std::string(SMS_NET_TXT_KEY);
}

// *************************************************************
//       Sms: setter and getter methods
// *************************************************************

// *************************************************************
//       Sms: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Sms::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Sms::readConfig(std::string pfConf) 
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
  //.....device.................................................
  if(sKey==SMS_DEVICE_KEY) {
   if(sVal.length()>0) {
    if(sVal.at(0)!='/') sVal="/"+sVal;
    device=sVal;
   }
  }
  //.....from - authorisized phone numbers......................
  if(sKey==SMS_DEVICES_KEY) vDevices=string2vector(sVal, ", ");
  //.....from - authorisized phone numbers......................
  if(sKey==SMS_FROM_KEY) split2phone(sVal, vFrom);
  //.....to - authorisized phone numbers........................
  if(sKey==SMS_TO_KEY) split2phone(sVal, vTo);
  //.....subscribe topic -> to send sms.........................
  if(sKey==SMS_TOPICSUB_KEY) topicSub=sVal;
  //.....answer topic after sending sms.........................
  if(sKey==SMS_TOPICSUBRET_KEY) topicSubRet=sVal;
  //.....subscribe topic -> to send commands to modem...........
  if(sKey==SMS_TOPICSUB2_KEY) topicSub2=sVal;
  //.....answer topic after receiving a command.................
  if(sKey==SMS_TOPICSUB2RET_KEY) topicSub2Ret=sVal;
  //.....publish topic, if a sms was received...................
  if(sKey==SMS_TOPICPUB_KEY) topicPub=sVal;
  //.....publish topic, if a sms was received...................
  if(sKey==SMS_PUB_NUM_KEY) pubNum=(sVal=="true");
  //.....sms command to request program version by sms..........
  if(sKey==SMS_CMD_VERSION_KEY) cmdVersion=sVal;
  //.....sms command to exit program............................
  if(sKey==SMS_CMD_END_KEY) cmdEnd=sVal;
  //.....sms command to reload sms config data..................
  if(sKey==SMS_CMD_RELOAD_KEY) cmdReload=sVal;
  //.....sms command send credit sms............................
  if(sKey==SMS_CMD_CREDIT_KEY) cmdCredit=sVal;
  //.....provider...............................................
  if(sKey==SMS_NET_ID_KEY) netId=sVal;
  //.....provider phone number for credit request...............
  if(sKey==SMS_NET_TEL_KEY) netTel=sVal;
  //.....provider text for credit request.......................
  if(sKey==SMS_NET_TXT_KEY) netTxt=sVal;
  //.....send sms on program start..............................
  if(sKey==SMS_SMS_START_KEY) { vSmsStart.push_back(sVal); }
  //.....send sms on program end................................
  if(sKey==SMS_SMS_END_KEY) { vSmsEnd.push_back(sVal); }
 } // end for every line in section
 //------if device not defined: search interface----------------
 if(device.length()==0) {
  std::cout<<"try to find SMS module..."<<std::endl;
  device = findSmsModule();
  if(device.length()==0) device=SMS_DEVICE_DEFAULT;
 }
 return true;
}

//_______Show all properties____________________________________
void Sms::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"C_Sms-Version       | "<<SMS_VERSION<<std::endl;
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<"device              | "<<device<<std::endl;
 std::cout<<"-----possible interfaces---------------"<<std::endl;
 for(int i=0; i<vDevices.size(); i++) std::cout<<vDevices.at(i)<<" | ";
 std::cout<<std::endl;
 std::cout<<"-----authorizised SMS from-------------"<<std::endl;
 for(int i=0; i<vFrom.size(); i++) std::cout<<vFrom.at(i)<<" | ";
 std::cout<<std::endl;
 std::cout<<"-----authorizised SMS to---------------"<<std::endl;
 for(int i=0; i<vTo.size(); i++) std::cout<<vTo.at(i)<<" | ";
 std::cout<<std::endl;
 std::cout<<"topicSub (send SMS) | "<<topicSub<<std::endl;
 std::cout<<"topicSubRet (answer)| "<<topicSubRet<<std::endl;
 std::cout<<"topicSub2 (send SMS)| "<<topicSub2<<std::endl;
 std::cout<<"topicSub2Ret (answer)| "<<topicSub2Ret<<std::endl;
 std::cout<<"topicPub (SMS in)   | "<<topicPub<<std::endl;
 std::cout<<"publish phone number| ";
 if(pubNum) std::cout<<"true"<<std::endl; else std::cout<<"false"<<std::endl;
 std::cout<<"-----send sms on prog start to---------"<<std::endl;
 for(int i=0; i<vSmsStart.size(); i++) std::cout<<vSmsStart.at(i)<<std::endl;
 std::cout<<"-----send sms on prog end to-----------"<<std::endl;
 for(int i=0; i<vSmsEnd.size(); i++) std::cout<<vSmsEnd.at(i)<<std::endl;
 std::cout<<"cmdversion          | "<<cmdVersion<<std::endl;
 std::cout<<"cmdend              | "<<cmdEnd<<std::endl;
 std::cout<<"cmdreload           | "<<cmdReload<<std::endl;
 std::cout<<"cmdcredit           | "<<cmdCredit<<std::endl;
 std::cout<<"netid               | "<<netId<<std::endl;
 std::cout<<"netTel              | "<<netTel<<std::endl;
 std::cout<<"netTxt              | "<<netTxt<<std::endl;
 std::cout<<"sendTimeout (sec)   | "<<sendTimeout<<std::endl;
}

//_______test device, send MQTT message on error________________
// return true: device OK
bool Sms::sendStartSms(Gsm gsm3, struct mosquitto *mosq)
{
 Conf conf=Conf();                     // helper: split2String
 g_modemBusy=true;                     // modem busy
 //===========test if device is ready, send mqtt on error=======
 if(!gsm3.isModule())
 {//----------no device found-----------------------------------
  g_modemBusy=false;                   // modem not busy
  std::string sPay="Modem "+device+" not found.";
  publish(mosq, topicPub, sPay, retainOut);
  return false;
 } // end no device found
 //-----------device found--------------------------------------
 if(!gsm3.begin()) 
 {//---------could not begin modem-----------------------------
  g_modemBusy=false;                   // modem not busy
  std::string sPay="Modem "+device+": begin() error";
  publish(mosq, topicPub, sPay, retainOut);
  return false;
 } // end could not begin modem
 //-----------begin modem OK------------------------------------
 g_modemBusy=false;                    // modem not busy
 std::string sPay="Modem "+device+" found :)";
 int iRet=-1;                          // publish error
 if(publish(mosq, topicPub, sPay, retainOut)) {
  iRet=0;                             // publish ok
  if(g_prt) std::cout<<sPay<<std::endl;
 }
 //-----------send start sms to all listed user-----------------
 bool ok=true;
 for(int i=0; i<vSmsStart.size(); i++) {
  std::string phone, text;
  std::string s2=vSmsStart.at(i);
  g_utils.str2str2(s2, phone, text, ' ');
  ok|=sendSms(phone,text+testDeviceText2,mosq);
 }
 if(ok && iRet==0) return true;
 return false;
}

//_______act on messages..._____________________________________
bool Sms::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 if(topic==topicSub)
 { //====incoming message matches stored topic 1================
  std::string phone, text;
  Conf conf=Conf();
  g_utils.str2str2(payload, phone, text, ' ');
  if(!sendSms(phone, text, mosq)) return false;
  return true;
 } // END OF incoming message matches stored topic 1============
 int pos1 = topic.find(topicSub2);
 if(pos1==0)
 { //=====incoming message matches first part of stored topic 2=
  std::string topicpart=topic.substr(topicSub2.length());
  if(topicpart=="/get") 
  { //=-=its a /get topic! =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   for(auto& c : payload) { c = tolower(c); }; // string to lower
   if(payload=="?" || payload=="help")
   { //--payload 1----------------------------------------------
    std::string sTop=topicSub2Ret+"/"+payload;
    std::string sPay="{\"get\":\"?|help|version|module|sim|credit\",\"set\":\"at\"}";
    int ret=mosquitto_publish(mosq, NULL,sTop.c_str(), sPay.length(), sPay.c_str(), 0, false);
    if(g_prt) {
     if(ret!=0) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
    }
   }
   if(payload=="version")
   { //--payload 2----------------------------------------------
    std::string sTop=topicSub2Ret+"/"+payload;
    std::string sPay=SMS_VERSION;
    int ret=mosquitto_publish(mosq, NULL,sTop.c_str(), sPay.length(), sPay.c_str(), 0, false);
    if(g_prt) {
     if(ret!=0) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),ret);
    }
   }
   if(payload=="module")
   { //--payload 3----------------------------------------------
    std::thread mythreadIsModule(threadFunctionIsModule, device, topicSub2Ret, mosq);
    mythreadIsModule.detach();
    return true;
   }
   if(payload=="sim")
   { //--payload 4----------------------------------------------
    std::thread mythreadIsSim(threadFunctionIsSim, device, topicSub2Ret, mosq);
    mythreadIsSim.detach();
    return true;
   }
   if(payload=="credit")
   { //--payload 5----------------------------------------------
    return getCredit(mosq);
   }
   if(payload=="...")
   { //--payload X----------------------------------------------
    // ...add your code here...
   }
  } // END OF ist a /get topic! =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  int pos1 = topicpart.find("/set");
  if(pos1==0)
  { // =-its a /set topic !=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   std::string topicpart2 = topicpart.substr(4);
   if(topicpart2 == "/at")
   { // -set 1: send a (AT) command to GSM modem----------------
    std::thread mythreadSendCmd(threadFunctionSendCmd, device, payload, topicSub2Ret, mosq);
    mythreadSendCmd.detach();
   }
   if(topicpart2 == "/...")
   { //--set 2--------------------------------------------------
    // ...add your code here...
   }
  } // =-END OF its a /set topic !=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

 } // END OF incoming message matches first part of topic 2=====
 return false;
}

//_______Possibility for cleanup before end of program__________
void Sms::onExit(struct mosquitto *mosq, int reason)
{
 Conf conf=Conf();
 if(g_prt) std::cout<<"send end sms..."<<std::endl;
 for(int i=0; i<vSmsEnd.size(); i++)
 {
  std::string phone, text;
  std::string s1=vSmsEnd.at(i);
  g_utils.str2str2(s1, phone, text, ' ');
  if(!isAuthSmsTo(phone)) continue;
  std::thread mythreadSendSms(threadFunctionSendSms, phone, text, device, mosq, topicPub, 30);
  mythreadSendSms.join();
 }
}

//_______check for an incomming sms_____________________________
// called by myMqttPeriodicExec(struct mosquitto *mosq)
// uses g_modemBusy: true if modem is used to send/receive
// return: -4 sender not allowed, -3 sms read error
//         -2 modem busy, -1=error, 
//         0=no sms, 1=sms received, 2=end sms received
int Sms::lookForSmsIn(struct mosquitto *mosq)
{
 bool bRet=true;                            // bool return
 int  iRet;                            // int return
 bool bRetain;                         // true=retain message
 SSMS smsIn;                           // incomming SMS
 if(g_modemBusy) return -2;            // modem busy
 Conf conf=Conf();                     // helper: 
 g_modemBusy=true;                     // modem busy
 Gsm gsm2=Gsm(device);                 // gsm object
 //-----------test if device is ready, send mqtt on error-------
 if(testDeviceOnStart)
 {
  sendStartSms(gsm2, mosq);
  testDeviceOnStart = false;
 }
 //===========check for incomming sms===========================
 // uses g_modemBusy: true if modem is used to send/receive
 g_modemBusy=true;                     // use modem to read sms
 //if(!gsm2.begin())  { g_modemBusy=false; return -1; }
 if(!gsm2.isSms1()) { g_modemBusy=false; return 0; }
 //-----------yes: a sms arrived--------------------------------
 bRet=gsm2.readSms1(smsIn);            // read sms #1
 gsm2.deleteSms1();                    // delete sms #1
 g_modemBusy=false;                    // free modem
 if(!bRet) return -3;                  // sms read error
 //-----------read sms 1: success-------------------------------
 if(g_prt) std::cout<<"SMS in: "<<smsIn.text<<std::endl;
 if(!isAuthSmsFrom(smsIn.phone)) return -4; // wrong sender
 //===========SMS comes from an authorized sender===============
 //-----------credit request answer?----------------------------
 if(smsIn.phone==netTel) {
  // yes: credit request answer
  std::string sEuro=getsHotEuro(smsIn.text);
  if(creditAnswerToPhone.length()>0) 
  { // -------send credit info to phone-------------------------
   sEuro=sEuro+" Euro";
   bRet=sendSms(creditAnswerToPhone, sEuro, mosq);
   creditAnswerToPhone="";
   if(!bRet) return -1;
   return 1;
  } else 
  { // -------send credit info as MQTT message------------------
   std::string sTopic=topicSub2+"/ret/credit";
   int pos2=sEuro.find(",");
   if(pos2!=String::npos) sEuro.replace(pos2,1,".");
   std::string sPayload="{\"euro\":"+sEuro+"}";
   bool sRetain=false;
   if(!publish(mosq, sTopic, sPayload, sRetain)) return -1;
   return 1;
  }
 }
 //===========analyse incoming sms test=========================
 // iret <0: error (IN values unchanged), 1: only plain text
 //      2: correct MQTT message (-t -m [-r]) IN values changed
 std::string smsTopic, smsPayload;
 bool smsRetain;
 iRet=parseSmsIn(smsIn.text, smsTopic, smsPayload, smsRetain);
 if(iRet==1)
 {//----------SMS is plain text (command or credit answer)------
  if(smsIn.text==cmdEnd) return 2;     // sms cmd end
  if(smsIn.text==cmdVersion) {
  // if(!sendSms(smsIn.phone,g_base.msgVersion.payloadOut,mosq)) return -1;
  if(!sendSms(smsIn.phone,g_base.msgVersion.payloadOut + "\r\n"+ SMS_VERSION,mosq)) return -1;
   return 1;
  }
  if(smsIn.text==cmdReload) { readConfig(); return 1; }
  if(smsIn.text==cmdCredit) { 
   creditAnswerToPhone=smsIn.phone;    // remember for answer sms
   bRet=sendSms(netTel, netTxt, mosq); // request credit sms
   if(!bRet) return -1;
   return 1;
  }
  //----------other plain text (not a command!): publish it-----
  std::string s1= smsIn.text;
  // .........publish all? (phone number, date and sms text)....
  if(pubNum) s1=smsIn.phone+"\\n"+formatSmsDate(smsIn.date,2)+"\\n"+smsIn.text;
std::cout << "try to publish -t " << topicPub << " -m " << s1 << std::endl;
  if(!publish(mosq, topicPub, s1, false)) return -1;
  return 1;
 } // end SMS is plain text
 if(iRet==2)
 {//----------SMS is a correct MQTT message: publish it---------
  if(!publish(mosq, smsTopic, smsPayload, smsRetain)) return -1;
  return 1;
 } // end SMS is a correct MQTT message
 return -1;
}

//_______is given phone number included in "from list"?_________
bool Sms::isAuthSmsFrom(std::string phoneNumber)
{
 for(int i=0; i<vFrom.size(); i++) {
  if(vFrom.at(i)==phoneNumber) return true;
 }
 return false;
}

//_______is given phone number included in "to list"?___________
bool Sms::isAuthSmsTo(std::string phoneNumber)
{
 for(int i=0; i<vTo.size(); i++) {
  if(vTo.at(i)==phoneNumber) return true;
 }
 return false;
}

//_______send an AT commant to module___________________________
bool Sms::sendCmd(std::string sCmd, std::string& sResult) {
 bool bRet=true;
 g_modemBusy=true;                     // modem busy
 Gsm gsm2=Gsm(device);                 // gsm object
 bRet=gsm2.begin();                    // test module
 if(!bRet) {
  sResult = gsm2.getsResult(false);    // text with \r=^ \n=~
  g_modemBusy=false;                   // modem busy
  return bRet;                         // error
 }
 bRet=gsm2.sendCmd(sCmd);
 sResult = gsm2.getsResult(false);     // text with \r=^ \n=~
 g_modemBusy=false;                    // modem not busy
 return bRet;
}

//_______publish a message, print error (if allowed)____________
bool Sms::publish(struct mosquitto *mosq_, 
 std::string topic_, std::string payload_, bool retain_)
{
 int iRet=mosquitto_publish(mosq_, NULL, topic_.c_str(),
    payload_.length(), payload_.c_str(), 0, retain_);
 if(iRet!=0) {
  if(g_prt) fprintf(stderr, "Error %d: NOT published -t %s -m %s.\n",
    iRet, topic_.c_str(), payload_.c_str());
  return false; 
 }
 return true;
}

// *************************************************************
//       methods that use threads
// *************************************************************

//_______send SMS_______________________________________________
// uses threadFunctionSendSms()
// return: true=sms sending started, false: not started
bool Sms::sendSms(std::string phone, std::string text, struct mosquitto *mosq)
{
 if(!isAuthSmsTo(phone)) {
   std::string payload_ = SMS_TXT_NOT_SENT;
   payload_ += "Error #6: Receiver forbidden ("+phone+": "+text+")";
   bool retain_ = false;
   int iRet=mosquitto_publish(mosq, NULL, topicSubRet.c_str(),
    payload_.length(), payload_.c_str(), 0, retain_);
   if(iRet!=0) {
    if(g_prt) fprintf(stderr, "Error %d: NOT published -t %s -m %s.\n",
     iRet, topicSubRet.c_str(), payload_.c_str());
    return false; 
   }
   return false;
 }
 std::thread mythreadSendSms(threadFunctionSendSms, phone, text, device, mosq, topicSubRet, sendTimeout);
 mythreadSendSms.detach();
 return true;
}

// ______get credit from provider, send message_________________
bool Sms::getCredit(struct mosquitto *mosq)
{
 creditAnswerToPhone="";
 return sendSms(netTel, netTxt, mosq); // request credit sms
}

// *************************************************************
//       thread methods
// *************************************************************

//_______thread send sms________________________________________
// uses g_modemBusy: true if modem is used to send/receive
void Sms::threadFunctionSendSms(std::string sPhone,
  std::string sText, std::string sDevice, 
  struct mosquitto *sMosq, std::string sTopic, int iTimeout)
{
 std::string s1;
 bool bRetain=false;
 bool bRet=true;
 int  watchdog1=iTimeout;
 // ----------wait for modem to be ready or 120secs-------------
 while(g_modemBusy && watchdog1>0)
 {
  watchdog1--;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 // ----------start sms sending procedure-----------------------
 if(watchdog1<1) {
  s1 = SMS_TXT_NOT_SENT;
  s1 += " #1 timeout ("+sPhone+": "+sText+")";
 } else {
  g_modemBusy=true;                    // start sending
  Gsm gsm_=Gsm(sDevice);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if(!gsm_.isModule()) {
    s1 = SMS_TXT_NOT_SENT;
    s1 += " #3 "+gsm_.getsStatus()+" ("+sPhone+": "+sText+")";
  } else {
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   if(!gsm_.begin()) {
    s1 = SMS_TXT_NOT_SENT;
    s1 += " #4 "+gsm_.getsStatus()+" ("+sPhone+": "+sText+")";
   } else {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(gsm_.sendSms(sPhone, sText)) {
     s1 = SMS_TXT_SENT;
     s1 += " ("+sPhone+": "+sText+")";
    } else {
     s1 = SMS_TXT_NOT_SENT;
     s1 += " #5 "+gsm_.getsStatus()+" ("+sPhone+": "+sText+")";
    }
   }
  }
 }
 g_modemBusy=false;                    // enable next send
 try {
  int iRet=mosquitto_publish(sMosq, NULL, sTopic.c_str(),
      s1.length(), s1.c_str(), 0, bRetain);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTopic.c_str(),iRet);
  }
 } catch(std::string& error) {
  fprintf(stderr,"Error \"%s\" while publishing SMS (%s)\n",error, s1);
 }
}

//_______thread looking for a GSM module (AT command)___________
// uses g_modemBusy: true if modem is used to send/receive
void Sms::threadFunctionIsModule(std::string device_, std::string topicSub2Ret_, struct mosquitto *mosq_)
{
 std::string sRes="?", sRet, sTop, sPay;
 bool bRetain = false;
 bool bRet=true;
 int  watchdog1=120;
 // ----------modem ready or wait for modem ready (120sec)------
 while(g_modemBusy && watchdog1>0)
 {
  watchdog1--;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 // ----------start sms sending procedure-----------------------
 if(watchdog1<1) 
 { // --------modem is still busy-------------------------------
  sPay = "{\"error\":\"Timeout while waiting for modem access\"}";
 } else
 { // --------modem access--------------------------------------
  g_modemBusy=true;                    // modem busy
  Gsm gsm2=Gsm(device_);               // gsm object
  bRet=gsm2.isModule();                // test module
  if(bRet) {
   gsm2.sendCmd("ATI");
   sRes = gsm2.getsResult(true);        // text with \r \n
  }
  g_modemBusy=false;                   // modem not busy
  // ---------END OF modem access-------------------------------
  sRes = Sms::removeLFCR(sRes);
  sRet=bRet?"true":"false";
  sTop=topicSub2Ret_+"/module";
  sPay="{\"device\":\"" + device_ +"\",\"module\":"+sRet+",\"product\":\""+sRes+"\"}";
 }
 // -----publish result-----------------------------------------
 try {
  int iRet=mosquitto_publish(mosq_, NULL, sTop.c_str(),
   sPay.length(), sPay.c_str(), 0, bRetain);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),iRet);
  }
 } catch(std::string& error) {
  fprintf(stderr,"Error \"%s\" while publishing MQTT message %s\n",error, sPay);
 }
}

//_______thread looking for a SIM card in a GSM module__________
// uses g_modemBusy: true if modem is used to send/receive
void Sms::threadFunctionIsSim(std::string device_, std::string topicSub2Ret_, struct mosquitto *mosq_)
{
 std::string sRes, sRet, sTop, sPay, sProvider;
 bool bRetain = false;
 bool bRet=true;
 int  watchdog1=120;
 // ----------modem ready or wait for modem ready (120sec)------
 while(g_modemBusy && watchdog1>0)
 {
  watchdog1--;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 // ----------start sms sending procedure-----------------------
 if(watchdog1<1) 
 { // --------modem is still busy-------------------------------
  sPay = "{\"error\":\"Timeout while waiting for modem/sim access\"}";
 } else
 { // --------modem access--------------------------------------
  g_modemBusy=true;                    // modem busy
  Gsm gsm2=Gsm(device_);               // gsm object
  bRet=gsm2.begin();                   // test module
  if(!bRet) {
   sRes = gsm2.getsResult(false);      // text with \r=^ \n=~
  } else {
  sProvider = gsm2.readNetwork();      // provider name
  sRes = gsm2.getsResult(false);       // text with \r=^ \n=~
  }
  g_modemBusy=false;                   // modem not busy
  // ---------END OF modem access-------------------------------
  sRes = removeLFCR(sRes);
  sRes = replaceChar(sRes, '\"', '\'');
  sRet=bRet?"true":"false";
  sTop=topicSub2Ret_+"/sim";
  sPay="{\"provider\":\"" + sProvider +"\",\"sim\":"+sRet+",\"result\":\""+sRes+"\"}";
 }
 // -----publish result-----------------------------------------
 try {
  int iRet=mosquitto_publish(mosq_, NULL, sTop.c_str(),
   sPay.length(), sPay.c_str(), 0, bRetain);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),iRet);
  }
 } catch(std::string& error) {
  fprintf(stderr,"Error \"%s\" while publishing MQTT message %s\n",error, sPay);
 }
}

//_______thread looking for a SIM card in a GSM module__________
// uses g_modemBusy: true if modem is used to send/receive
void Sms::threadFunctionSendCmd(std::string device_, std::string sCmd, std::string topicSub2Ret_, struct mosquitto *mosq_)
{
 std::string sRes, sRet, sTop, sPay;
 bool bRetain = false;
 bool bRet=true;
 int  watchdog1=120;
 // ----------modem ready or wait for modem ready (120sec)------
 while(g_modemBusy && watchdog1>0)
 {
  watchdog1--;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 // ----------start sms sending procedure-----------------------
 if(watchdog1<1) 
 { // --------modem is still busy-------------------------------
  sPay = "{\"error\":\"Timeout while waiting for modem access\"}";
 } else
 { // --------modem access--------------------------------------
  g_modemBusy=true;                    // modem busy
  Gsm gsm2=Gsm(device_);                // gsm object
  bRet=gsm2.sendCmd(sCmd);
  sRes=gsm2.getsResult(false);         // text with \r=^ \n=~
  g_modemBusy=false;                   // modem not busy
  // ---------END OF modem access-------------------------------
  // sRet=bRet?"true":"false";
  sTop=topicSub2Ret_+"/at";
  sPay=sRes;
 }
 // -----publish result-----------------------------------------
 try {
  int iRet=mosquitto_publish(mosq_, NULL, sTop.c_str(),
   sPay.length(), sPay.c_str(), 0, bRetain);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTop.c_str(),iRet);
  }
 } catch(std::string& error) {
  fprintf(stderr,"Error \"%s\" while publishing MQTT message %s\n",error, sPay);
 }
}

// *************************************************************
//       Sms: helper methods
// *************************************************************

//_______isolate euro amount from hot sms answer string_________
//  Lieber Kunde, Ihr aktuelles Guthaben betr▒gt 16,95 Euro. Ihr HoT Service Team
// return: credit or -1 on error
std::string Sms::getsHotEuro(std::string text)
{
 int pos1=text.find("Guthaben betr");
 if(pos1==String::npos) return "-1";
 std::string s1=text.substr(pos1+15);
 pos1=text.find("gt");
 if(pos1==String::npos) return "-1";
 int pos2=text.find("Euro.");
 if(pos2==String::npos) return "-1";
 pos1+=3;
 int len1=pos2-pos1-1;
 if(len1<1) return "-1";
 s1=text.substr(pos1, len1);
 return s1;
}

//_______isolate euro amount from hot sms answer string_________
//  Lieber Kunde, Ihr aktuelles Guthaben betr▒gt 16,95 Euro. Ihr HoT Service Team
// return: credit or -1 on error
double Sms::getHotEuro(std::string text)
{
 std::string s1=getsHotEuro(text);
 if(s1.length()<1) return -1.0;
 if(s1=="-1") return -1.0;
 int pos2=s1.find(",");
 if(pos2!=String::npos) s1.replace(pos2,1,".");
 double euro=std::stod(s1);
 return euro;
}

// ___________split string to vector of strings_________________
std::vector<std::string> Sms::string2vector(const std::string& data, const std::string& delimiters) {
    auto is_delim = [&](auto & c) { return delimiters.find(c) != std::string::npos; };
    std::vector< std::string > result;
    for (std::string::size_type i(0), len(data.length()), pos(0); i <= len; i++) {
        if (is_delim(data[i]) || i == len) {
            auto tok = data.substr(pos, i - pos);
            if ( !tok.empty() )
                result.push_back( tok );
            pos = i + 1;
        }
    } return result;
}

//_______split string to phone numbers__________________________
// aaa,bbb,ccc -> vector
// return true=phone numbers ok
bool Sms::split2phone(std::string sIn, std::vector<std::string>&vOut)
{
 char delimiter=',';
 char c1;
 bool bPhoneNum=true;
 std::string temp="";
 vOut.clear();
 for(int i=0; i<sIn.size(); i++)
 {//----------for each char-------------------------------------
  c1=sIn.at(i);
  if(c1!=' ')
  {//---------char is not blank---------------------------------
   if(sIn[i] == delimiter)
   {//--------char is delimiter---------------------------------
    if(bPhoneNum) {
     if(temp.length()>2) vOut.push_back(temp);
    }
    temp="";
    bPhoneNum=true;
   }// end char is delimiter
   else
   {//--------char is not the delimiter-------------------------
    if((c1>='0' && c1<='9') || (temp=="" && c1=='+'))
     temp+=c1;
    else
     bPhoneNum=false;
   } // end char is not the delimiter
  } // end char is not blank
 } // end for each char
 if(bPhoneNum) { // last phone number
  if(temp.length()>2) vOut.push_back(temp);
 }
 if(vOut.size()<1) return false;
 return true;
}

//_______split SMS to MQTT message parts________________________
// return <0: error, ().....In values unchanged)
// return  1: only plain text (.....In values changed)
// return  2: correct MQTT message (.....In values changed)
int Sms::parseSmsIn(std::string smsText, 
  std::string& topicIn, std::string& payloadIn, bool& retainIn)
{
 bool bRetain=false;
 int ixt, ixm, ixr;
 int lent, lenm, lenx;
 int lenText=smsText.length();
 std::string key1, val1;
 Conf conf=Conf();
 //-----------Parse SMS (-t topic -m message)-------------------
 ixt=smsText.find("-t");                    // search for -t
 if(ixt==std::string::npos)
 {//----------NOT found -t--------------------------------------
  ixm=smsText.find("-m");                   // search for -m
  ixr=smsText.find("-r");                   // search for -r
  if(ixm==std::string::npos && ixr==std::string::npos)
  {//---------neither -t nor -m nor -r--------------------------
   g_utils.delExtBlank(smsText);               // 
   if(smsText.length()>0)
   {//--------plain text without -X is ok-----------------------
    topicIn=SMS_TOPICPUB;
    payloadIn=smsText;
    retainIn=true;
    return 1;                               // only plain text
   } // end plain text without -X is ok
  } // end neither -t nor -m nor -r
  return -1;                                // wrong -X
 } // end NOT found -t
 //-----------found -t------------------------------------------
 ixm=smsText.find("-m");                    // search for -m
 if(ixm==std::string::npos) return -2;      // wrong -t -m
 ixr=smsText.find("-r");                    // search for -r
 if(ixr==std::string::npos) ixr=lenText;    // string end
 else bRetain=true;                         // -r found
 //-----------find topic length---------------------------------
 lent=lenText-ixt-2;
 if(ixr>ixt) {
  lenx=ixr-ixt-2; 
  if(lenx<lent) lent=lenx;
 }
 if(ixm>ixt) {
  lenx=ixm-ixt-2;
  if(lenx<lent) lent=lenx;
 }
 key1=smsText.substr(ixt+2, lent);
 g_utils.delExtBlank(key1);                    // 
 if(key1.length()<1) return -3;             // wrong topic
 //-----------find payload length-------------------------------
 lenm=lenText-ixm-2;
 if(ixr>ixm) {
  lenx=ixr-ixm-2; 
  if(lenx<lenm) lenm=lenx;
 }
 if(ixt>ixm) {
  lenx=ixt-ixm-2;
  if(lenx<lenm) lenm=lenx;
 }
 val1=smsText.substr(ixm+2, lenm);
 g_utils.delExtBlank(val1);                    // 
 if(val1.length()<1) return -4;             // wrong payload
 //-----------topic and payload ok------------------------------
 topicIn=key1;
 payloadIn=val1;
 retainIn=bRetain;
 return 2;                                  // correct message
}

//_______format sms date+time___________________________________
// format_ 0: unchanged, 1: yyyy-mm-dd HH:MM:SS
//         2: dd.mm.yyyy HH:MM:SS
std::string Sms::formatSmsDate(std::string smsDate, int format_)
{
  std::string s1="";
  if(smsDate.length()<17) return smsDate;
  switch(format_)
  {
    case 1: s1="20"+smsDate.substr(0,2)+"-"+smsDate.substr(3,2)+"-"+smsDate.substr(6,2)+" "+smsDate.substr(9,8);
     break;
    case 2: s1=smsDate.substr(6,2)+"."+smsDate.substr(3,2)+"."+"20"+smsDate.substr(0,2)+" "+smsDate.substr(9,8);
     break;
    default: s1=smsDate; break;
  }
  return s1;
}

//_______search for interface of sms module_____________________
// return: device name or "", if GSM modem not found
// does not change value of 'device'
// uses no thread: -> method may take some time...
std::string Sms::findSmsModule()
{
 bool bRet=true;                       // return value
 std::string sDeviceTest="";           //
 int  watchdog1=120;
 // ----------wait for modem to be ready or 120secs-------------
 while(g_modemBusy && watchdog1>0)
 {
  watchdog1--;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 if(watchdog1 < 1) return sDeviceTest;
 // ----------modem ready---------------------------------------
 g_modemBusy=true;                     // start sending
 for(int i=0; i<vDevices.size(); i++) {
  sDeviceTest = vDevices.at(i);
  if(g_prt) std::cout<<"Search for modem at \""<<device<<"\": ";
  Gsm gsm2=Gsm(sDeviceTest);           // gsm object
  bRet=gsm2.isModule();                // test module
  if(bRet) {
   if(g_prt) std::cout<<"OK - modem found!"<<std::endl;
   break;
  }
  if(g_prt) std::cout<<"ERROR - modem NOT found!"<<std::endl;
 }
 g_modemBusy=false;                    // enable next modem access
 return sDeviceTest;
}

//_______remove \r\n from string________________________________
std::string Sms::removeLFCR(std::string s1)
{
  int len_=s1.length();
  if (len_<1) return "";
  char ca[len_+1];
  int j=0;
  for(int i=0; i<len_; i++) {
    if(s1[i]!='\r' && s1[i]!='\n') ca[j++] = s1[i];
  }
  ca[j]=0;
  std::string sRes = ca;
  return sRes;
}

//_______remove \r\n from string________________________________
std::string Sms::replaceChar(std::string s1, char cOld, char cNew)
{
  int len_=s1.length();
  for(int i=0; i<len_; i++) {
    if(s1[i]==cOld) s1[i]=cNew;
  }
  return s1;
}

//_______declare a global little helpers object ;)______________
Sms g_sms=Sms();
