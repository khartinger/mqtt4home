//_____C_LogM.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hLogM -lmosquitto -lpthread
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_LogM.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-20 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include <algorithm>                   // replace
#include <dirent.h>                    // DIR, dirent, opendir
#include <sys/stat.h>                  // mkdir
#include <unistd.h>                    // stat

#define  LOGM_SECTION        "log"
#define  LOGM_PATH_KEY       "path"
#define  LOGM_PATH1          "./"
#define  LOGM_PATH2          "log/"
#define  LOGM_EXT_LOG_KEY    "ext"
#define  LOGM_EXT_LOG        ".log"
#define  LOGM_PATH           LOGM_PATH1 LOGM_PATH2

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class LogM
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string pathKey;                  // conf: key for log path
 std::string pathLog;                  // path for log files
 std::string _extKey;                  // conf: key for extension
 std::string _extLog;                  // extension for log files
 std::string keys;                     // keys for [logm]
 public:
 //------constructor & co---------------------------------------
 LogM();                               // default constructor
 LogM(std::string pathlog);            // constructor
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 void setPathLog(std::string sPathlog);// set path of log file
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);

 //-----helper methods------------------------------------------
 bool add2log(std::string topic, std::string payload, std::string &sFilename);
 bool isDir(std::string dir_);
 bool makeDir(std::string dir_);
};

// *************************************************************
//       LogM: constructor & co
// *************************************************************

//_______Default constructor____________________________________
LogM::LogM() { setDefaults(); }

//_______constructor with path to log files_____________________
LogM::LogM(std::string pathlog) {
 setDefaults();
 pathLog=pathlog;
}
//_______set all default properties_____________________________
void LogM::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = LOGM_SECTION;              // prog specifig info
 pathKey  = LOGM_PATH_KEY;             // conf: key for log path
 pathLog  = LOGM_PATH;                 // path for log files
 _extKey  = LOGM_EXT_LOG_KEY;          // conf: key for extension
 _extLog  = LOGM_EXT_LOG;              // extension for log files
 keys=std::string(LOGM_PATH_KEY);      // all keys in section
 keys+="|"+std::string(LOGM_EXT_LOG_KEY);
}

// *************************************************************
//       LogM: setter and getter methods
// *************************************************************

//_______set path for log files_________________________________
void LogM::setPathLog(std::string sPathlog) {pathLog=sPathlog;}

// *************************************************************
//       LogM: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool LogM::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool LogM::readConfig(std::string pfConf) 
{ 
 bool bRet=true;
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------(try to) read log path from config file________________
 std::string s1=conf.findValue(section, pathKey);
 if(s1.length()<1) bRet=false;
 else {
  if(s1.at(s1.length()-1)!='/') s1+="/";
  pathLog=s1;
 }
 //------(try to) read extension from config file_______________
 s1=conf.findValue(section, _extKey);
 if(s1.length()<1) bRet=false;
 else {
  if(s1.at(0)!='.') s1="."+s1;         // 1st char is .
  _extLog=s1;
 }
 return bRet;
}

//_______Show all properties____________________________________
void LogM::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<pathKey<<"                 | "<<pathLog<<std::endl;
 std::cout<<_extKey<<"                 | "<<_extLog<<std::endl;
}

//_______act on messages..._____________________________________
bool LogM::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 //------replace blank by _, / by @-----------------------------
 std::replace(topic.begin(), topic.end(), ' ', '_');
 std::replace(topic.begin(), topic.end(), '/', '@');
 //......add message to log file................................
 std::string sFilename;
 if(payload=="") payload="(null)";
 if(!add2log(topic, payload, sFilename)) {
  fprintf(stderr,"Error while appending log-file %s\n",sFilename.c_str());
  return false;
 } else {
  if(g_prt) std::cout<<"Append "<<payload<<" to "<<sFilename<<std::endl;
 }
 return true;
}

//_______Possibility for cleanup before end of program__________
void LogM::onExit(struct mosquitto *mosq, int reason)
{
}

// *************************************************************
//       LogM: helper methods
// *************************************************************

//_______append message to the corresponding log file___________
// return: true=ok, false on Error
bool LogM::add2log(std::string topic, std::string payload,
 std::string &sFilename)
{
 //------general variables--------------------------------------
 int ret;
 //------ calc year + month-------------------------------------
 std::string s1="%y%m";
 std::string _yymm="."+g_base.getDateTime(s1);
 //------build path + filename----------------------------------
 sFilename =pathLog;
 sFilename+=topic;
 sFilename+=_yymm;
 sFilename+=_extLog;
 //------build log-line-----------------------------------------
 s1="%d.%m.%y %H:%M:%S";
 std::string sLine=g_base.getDateTime(s1);
 sLine+=" | ";
 if(topic.length()<1) sLine+="(null)";
 else
 {
  sLine+=topic;
  sLine+=" | ";
  if(payload.length()<1) sLine+="(null)";
  else sLine+=payload;
 }
 //------does directory exist (no: create it)-------------------
 if(!isDir(pathLog)) makeDir(pathLog);
 //------try to open file stream--------------------------------
 int i=3;
 while(i>0) {
  i--;
  std::ofstream ofs(sFilename.c_str(),std::ios::app);
  if(!ofs.good()) 
  { //...could not open file stream.............................
   ofs.close();
   std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  else 
  { //...success: file stream is open, append line..............
   ofs << sLine << std::endl;
   ofs.close();
   return true;
  }
 }
 return false;
}

//_____does given directory exist?______________________________
bool LogM::isDir(std::string dir_)
{
 const char* folder=dir_.c_str();
 DIR* pdir = opendir(folder);
 if (pdir) { closedir(pdir); return true; }
 closedir(pdir);
 return false;
}

//_____make a new directory_____________________________________
bool LogM::makeDir(std::string dir_)
{
 int res=-1;
 struct stat st = {0};
 if(stat(dir_.c_str(), &st) == -1)
 {
  res=mkdir(dir_.c_str(), 0777);       // linux
  //res=mkdir(dir_.c_str());             // windows
 }
//printf("mkdir: %s res=%d\n",dir_.c_str(),res);
 if(res!=0) return false;
 return true;
}

//_______declare a global little helpers object ;)______________
LogM g_logm=LogM();
