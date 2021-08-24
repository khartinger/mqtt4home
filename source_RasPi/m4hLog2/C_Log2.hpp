//_____C_Log2.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hLog2 -lmosquitto -lpthread -std=c++17
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Log2.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-24 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include <algorithm>                   // replace
#include <dirent.h>                    // DIR, dirent, opendir
#include <sys/stat.h>                  // mkdir
#include <unistd.h>                    // stat

#define  LOG2_SECTION         "log2"
#define  LOG2_PATH_LOG_KEY   "pathlog"
#define  LOG2_PATH_LAST_KEY  "pathlast"
#define  LOG2_PATH1          "./"
#define  LOG2_PATH2LOG       "log/"
#define  LOG2_PATH2LAST      "last/"
#define  LOG2_EXT_LOG        ".log"
#define  LOG2_EXT_LAST       ""
#define  LOG2_PATH_LOG       LOG2_PATH1 LOG2_PATH2LOG
#define  LOG2_PATH_LAST      LOG2_PATH1 LOG2_PATH2LAST

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet

// *************************************************************
//    class Log: add incomming messages to files
// *************************************************************
class Log2
{
 public:
 //------properties---------------------------------------------
 std::string pfConfig;                 // path&name config file
 std::string section;                  // one session msg data
 protected:
 //------application specific properties------------------------
 std::string keys;                     // keys for [log2]
 std::string pathLogKey;               // conf: key for log path
 std::string pathLastKey;              // conf: key for log path
 std::string pathLog;                  // path log files
 std::string pathLast;                 // path for last msgs
 public:
 //------constructor & co---------------------------------------
 Log2();                                // constructor
 Log2(std::string logpath, std::string lastpath);
 void setDefaults();                   // set all default values
 //------setter and getter methods------------------------------
 std::string getKeys() {return keys;}  // keys in config file
 void setPaths(std::string sLogpath, std::string sLastpath);
 //------working methods----------------------------------------
 bool readConfig();                    // read config file
 bool readConfig(std::string pfConf);  // read config file
 void show();                          // show config values
 bool onMessage(struct mosquitto *mosq, std::string topic, std::string payload);
 void onExit(struct mosquitto *mosq, int reason);
 //-----helper methods------------------------------------------
 bool add2log(std::string topic, std::string payload);
 bool write2last(std::string topic, std::string payload);
 bool isDir(std::string dir_);
 bool makeDir(std::string dir_);
};

// *************************************************************
//       Log2: constructor & co
// *************************************************************

//_______Default constructor____________________________________
Log2::Log2() { setDefaults(); }

//_______constructor with path to log files_____________________
Log2::Log2(std::string logpath, std::string lastpath) {
 setDefaults();
 if(logpath.length()<1) logpath=LOG2_PATH_LOG;
 if(logpath.at(logpath.length()-1)!='/') logpath+="/";
 if(lastpath.length()<1) lastpath=LOG2_PATH_LOG;
 if(lastpath.at(lastpath.length()-1)!='/') lastpath+="/";
 pathLog=logpath;
 pathLast=lastpath;
}

//_______set all default properties_____________________________
void Log2::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = LOG2_SECTION;               // prog specifig info
 pathLog  = LOG2_PATH_LOG;             // path for log files
 pathLast = LOG2_PATH_LAST;            // path for last msgs
 pathLogKey  = LOG2_PATH_LOG_KEY;      // conf: key for log path
 pathLastKey = LOG2_PATH_LAST_KEY;     // conf: key for log path
 keys=std::string(LOG2_PATH_LOG_KEY);  // all keys in section
 keys+="|"+std::string(LOG2_PATH_LAST_KEY);
}

// *************************************************************
//       Log2: setter and getter methods
// *************************************************************

//_______set path for log and last msg files____________________
void Log2::setPaths(std::string sLogpath, std::string sLastpath)
{
 pathLog=sLogpath;
 pathLast=sLastpath;
}

// *************************************************************
//       Log2: working methods
// *************************************************************

//_______read config data (log path) from file__________________
bool Log2::readConfig() { return readConfig(pfConfig); }

//_______read config data (log path) from file__________________
// return: true if log path exist, false on error or not found
bool Log2::readConfig(std::string pfConf) 
{ 
 if(pfConf.length()<1) return false;   // wrong file name
 pfConfig=pfConf;                      // remember file name
 Conf conf=Conf(pfConf);               // config object
 if(!conf.isReady()) return false;     // config file ready?
 //------(try to) read paths from config file___________________
 std::string s1=conf.findValue(section, pathLogKey);
 std::string s2=conf.findValue(section, pathLastKey);
 if(s1.length()>0) {
  if(s1.at(s1.length()-1)!='/') s1+="/";
  pathLog=s1;
 }
 if(s2.length()>0) {
  if(s2.at(s2.length()-1)!='/') s2+="/";
 pathLast=s2;
 }
 return true;
}

//_______Show all properties____________________________________
void Log2::show()
{
 std::cout<<"-----["<<section<<"]------------------------------"<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<pathLogKey<<"          | "<<pathLog<<std::endl;
 std::cout<<pathLastKey<<"          | "<<pathLast<<std::endl;
}

//_______act on messages..._____________________________________
bool Log2::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 //------replace blank by _, / by @-----------------------------
 std::replace(topic.begin(), topic.end(), ' ', '_');
 std::replace(topic.begin(), topic.end(), '/', '@');
 //------add message to log file--------------------------------
 if(!add2log(topic, payload)) {
  fprintf(stderr,"Error while appending log-file %s\n",topic.c_str());
  return false;
 }
 //------write payload to last message file---------------------
 if(!write2last(topic, payload)) {
  fprintf(stderr,"Error while writing last-file %s\n",topic.c_str());
  return false;
 }
 return true;
}

//_______Possibility for cleanup before end of program__________
void Log2::onExit(struct mosquitto *mosq, int reason)
{
}

// *************************************************************
//       Log2: helper methods
// *************************************************************

//_______append message to the corresponding log file___________
// return: true=ok, false on Error
bool Log2::add2log(std::string topic, std::string payload)
{
 //------general variables--------------------------------------
 int ret;
 //------build yymm (year month) string-------------------------
 char caNow[18];
 time_t now_t;
 time(&now_t);
 struct tm* time_=localtime(&now_t);
 ret=strftime(caNow,5,"%y%m\0",time_);
 //------build path + filename----------------------------------
 std::string sFilename;
 sFilename =pathLog;
 sFilename+=topic;
 sFilename+=".";
 sFilename+=caNow;
 sFilename+=LOG2_EXT_LOG;
 //------build log-line-----------------------------------------
 std::string s1="%d.%m.%y %H:%M:%S";
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
   if(g_prt) std::cout<<"Write "<<sLine<<" to "<<sFilename<<std::endl;
   return true;
  }
 }
 return false;
}

//_______write payload to last message file_____________________
// return: true=ok, false on Error
bool Log2::write2last(std::string topic, std::string payload)
{
 //------general variables--------------------------------------
 int ret;
 //------build path + filename----------------------------------
 std::string sFilename;
 sFilename =pathLast;
 sFilename+=topic;
 sFilename+=LOG2_EXT_LAST;
 //------does directory exist (no: create it)-------------------
 if(!isDir(pathLast)) makeDir(pathLast);
 //------try to open file stream--------------------------------
 int i=3;
 while(i>0) {
  i--;
  std::ofstream ofs(sFilename.c_str(),std::ios::out);
  if(!ofs.good()) 
  { //...could not open file stream.............................
   ofs.close();
   std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  else 
  { //...success: file stream is open, append line..............
   ofs << payload << std::endl;
   ofs.close();
   if(g_prt) std::cout<<"Append "<<payload<<" to "<<sFilename<<std::endl;
   return true;
  }
 }
 return false;
}

//_____does given directory exist?______________________________
bool Log2::isDir(std::string dir_)
{
 const char* folder=dir_.c_str();
 DIR* pdir = opendir(folder);
 if (pdir) { closedir(pdir); return true; }
 closedir(pdir);
 return false;
}

//_____make a new directory_____________________________________
bool Log2::makeDir(std::string dir_)
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
Log2 g_log2=Log2();
