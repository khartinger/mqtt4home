//_____h6Base.h_________________________________khartinger_____
// Definitions and many basic functions for mqtt4home, divided 
//into five classes:
// * Message : Class for one MQTT message with topic, payload
//             and retain-flag.
// * Message2: Class for two related MQTT messages (in, out)
//             with 2x topic, 2x payload and retain-flag (out).
// * Conf    : Class for reading the configuration file.
// * H6Base  : Properties and methods for the basic functions
//             of mqtt4home.
// * M4hUtils : Class with special auxiliary methods
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2022-02-10 add reload conf file by mqtt command
// 2022-02-15 add class Conf: DHMS2sec(), sec2DHMS(), sec2HMS()
// 2022-02-17 add class Conf: fits()
// 2023-06-16 move helper methods to class M4hUtils, rename str...
// Released into the public domain.

#ifndef C_M4HBASE_H
#define C_M4HBASE_H true
#include <stdio.h>                     // fprintf, stdout
#include <stdlib.h>                    // exit
#include <string.h>                    // sprintf
#include <signal.h>                    // SIGINT
#include <time.h>                      // struct tm, time_t
#include <unistd.h>                    // getpid, sleep

#include <thread>                      // thread, std::this_thread::sleep_for
#include <chrono>                      // std::chrono::milliseconds
#include <iostream>                    // cout
#include <string>                      // string
#include <vector>                      // vector
#include <map>                         // (multi)map
#include <fstream>                     // ifstream, ofstream
#include <sstream>                     // ostringstream, istringstream

//-------broker values------------------------------------------
#define  M4H_HOST            "127.0.0.1"
#define  M4H_PORT            1883
#define  M4H_KEEPALIVE       60
#define  M4H_SUBSCRIBE       "#"
//-------values for config data---------------------------------
#define  _CONF_PATH_         "./"
//#define  _CONF_PATH_         "/usr/local/bin/"
#define  _CONF_FILE_         "m4h.conf"
#define  _CONF_PFILE_        _CONF_PATH_ _CONF_FILE_
#define  M4H_SECTION         "base"
//-------default values for config data-------------------------
// These values can be overwritten by the config file.
#define  M4H_VERSION_KEY_IN  "versionin"
#define  M4H_VERSION_T_IN    "m4hbase/get"
#define  M4H_VERSION_P_IN    "version"
#define  M4H_VERSION_KEY_OUT "versionout"
#define  M4H_VERSION_T_OUT   "m4hbase/ret/version"
#define  M4H_VERSION_P_OUT   "2022-02-12"
#define  M4H_MQTTSTART_KEY   "mqttstart"
#define  M4H_MQTTEND_KEY     "mqttend"
#define  M4H_PROGEND_KEY     "progend"
#define  M4H_CONF_IN_KEY     "readconfin"
#define  M4H_CONF_IN_T       "m4hbase/set/conf"
#define  M4H_CONF_IN_P       _CONF_PFILE_
#define  M4H_CONF_OUT_KEY    "readconfout"
#define  M4H_CONF_OUT_T      "m4hbase/ret/conf"
#define  M4H_CONF_OUT_P      "Read config: "
#define  M4H_ADDTIME_KEY     "addtime"

// *************************************************************
//       class Message
// *************************************************************
//_______Class for (MQTT-) Message______________________________
class Message
{
 public:
  std::string topic;
  std::string payload;
  bool retain;
  Message() { topic=""; payload=""; retain=false; };
  Message(std::string topic, std::string payload) { this->topic=topic; this->payload=payload; };
  Message(std::string topic, std::string payload, bool retain) 
  { this->topic=topic; this->payload=payload; this->retain=retain; };
};

// *************************************************************
//       class Message2
// *************************************************************
//_______Class for zwo (MQTT-) Messages (receive-> send)________
class Message2
{
 public:
  std::string topicIn;
  std::string payloadIn;
  std::string topicOut;
  std::string payloadOut;
  bool retainOut;
  Message2() { topicIn=""; payloadIn=""; topicOut=""; payloadOut=""; retainOut=false; };
  Message2(std::string inTopic, std::string inPayload, std::string outTopic, std::string outPayload) 
  { topicIn=inTopic; payloadIn=inPayload; topicOut=outTopic; payloadOut=outPayload; retainOut=false; };
  Message2(std::string inTopic, std::string inPayload, std::string outTopic, std::string outPayload, bool retain) 
  { topicIn=inTopic; payloadIn=inPayload; topicOut=outTopic; payloadOut=outPayload; retainOut=retain; };
};

// *************************************************************
//     class Conf: methods for reading config file
// *************************************************************

//_______class for config file__________________________________
class Conf {
 //------convert to and from string-----------------------------
 public:
 template <typename Typ>
 std::string toString(Typ val) {
  std::ostringstream strout;           // helper output stream
  std::string str;                     // string object
  strout << val;                       // value to stream
  str = strout.str();                  // stream to string
  return str;                          // return string
 }

 template <typename Typ>
 void stringTo(std::string str, Typ &val) {
  std::istringstream strin;            // helper input stream
  strin.str(str);                      // string to stream
  strin >> val;                        // stream to value
 }

 protected:
 //------properties---------------------------------------------
  std::string fname;
 public:
  //-----constructor & co---------------------------------------
  Conf();
  Conf(std::string filename);
  void   initConf();                   // initialize properties
  //-----setter and getter methods------------------------------
  void   setFilename(std::string fname_);
  std::string getFilename();
  //-----working methods----------------------------------------
  bool   isReady();                    // does config file exist
  bool   isSection(std::string text);
  bool   getSection(std::string section, std::vector<std::string>& v1);
  int    getSectionsJson(std::string section, std::vector<std::string>& v1);
  int    getSections(std::string section, std::vector<std::map<std::string, std::string>>& v1);
  bool   isLine(std::string section, std::string line_);
  bool   isLineCI(std::string section, std::string line_);
  std::string findKey(std::string section, std::string value_);
  std::string findValue(std::string section, std::string key_);
  int    readAllSections(std::multimap<std::string,std::string>& m1);
  //-----helper methods-----------------------------------------
  bool   fits(std::string topic, std::string pattern);
};

// *************************************************************
//     class M4hBase: properties and little helper for mqtt4home
// *************************************************************
//_______Class for properties and little helper for mqtt4home___
class M4hBase {
 public:
  //-----properties----------------------------------------------
  std::string pfConfig;                // path&name config file
  std::string section;                 // section name in config
  Message2 msgVersion;                 // request prog version
  Message  msgMqttStart;               // MQTT message at start
  Message  msgMqttEnd;                 // MQTT message at end
  Message  msgProgEnd;                 // prog end via MQTT
  Message2 msgReadConf;                // reload config
  bool     timeShouldBeAdded;          // @ msg start, end, ....
 protected:
  std::string keys;                    // keys for [sms]
 public:
  //-----constructor & co---------------------------------------
  M4hBase();
  void setDefaults();                  // set all default values
  //-----setter and getter methods------------------------------
  std::string getKeys() {return keys;}  // keys in config file
  //-----working methods----------------------------------------
  //void printHelptext();                // print help text
  bool readConfig();                   // read config file
  bool readConfig(std::string pfile_); // read config file
  //-----helper methods-----------------------------------------
  void show();                         // show all properties
};

// *************************************************************
//     class M4hUtils: little helpers for mqtt4home
// *************************************************************

class M4hUtils {
 public:
  //-----constructor & co---------------------------------------
  M4hUtils() {};
  //-----working methods----------------------------------------
  //_____remove line feed and carriage return from end of line__
  void   delTrailLFCR(std::string& s1);

  //_____remove blank(s) from end of line_______________________
  void   delTrailBlank(std::string& s1);

  //_____remove blank(s) from begin of line_____________________
  void   delLeadBlank(std::string& s1);

  //_____remove blank(s) from begin and end of line_____________
  void   delExtBlank(std::string &s1);

  //_____replace a part of the string to another________________
  void   replaceAll(std::string &str, const std::string& old_, const std::string& new_);

  //_____convert string to lower string_________________________
  void   str2lower(std::string &s1);

  // ___________split string to vector of strings (more delim)__
  std::vector<std::string> str2vector(const std::string& data, const std::string& delimiters);

  //_____convert json-string to pairs___________________________
  bool   split2pairs(std::string s1, std::multimap<std::string, std::string>& mm1);

  //_____split string to vector of strings (1 char delimiter)___
  void   str2vector1(std::string sIn, std::vector<std::string>&vOut, char delimiter);

  //_____split string to 2 strings______________________________
  bool   str2str2(std::string sIn,std::string& sPart1,std::string& sPart2,char delimiter);

  //_____convert d HMS string to sec____________________________
  time_t DHMS2sec(std::string sDHMS);

  //_____convert time to d HMS string___________________________
  std::string sec2DHMS(time_t tsec);

  //_____convert time to HMS string_____________________________
  std::string sec2HMS(time_t tsec);

  //_____return actual system date and time_____________________
  std::string getDateTime();           // actual date+time

  //_____return actual system date and time_____________________
  std::string getDateTime(std::string sTimeformat);
};

//_______declare a global little helpers object ;)______________
extern M4hBase g_base;
extern M4hUtils g_utils;

#endif // C_M4HBASE_H