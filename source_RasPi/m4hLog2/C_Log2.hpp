//_____C_Log2.hpp____________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hLog2 -lmosquitto -lpthread -std=c++17
// This file contains additional code for m4hExtension.hpp, 
// so that the file does not become too confusing.
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Log2.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-24 First release
// 2022-02-12 Add skip log, filetime
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include <algorithm>                   // replace
#include <dirent.h>                    // DIR, dirent, opendir
#include <unistd.h>                    // stat
#include <sys/stat.h>                  // mkdir
#include <sys/types.h>                 // open, O_WRONLY, O_CREAT...
#include <sys/stat.h>                  // open, O_WRONLY, O_CREAT...
#include <fcntl.h>                     // open, O_WRONLY, O_CREAT...

#define  LOG2_PRT            false //true
#define  LOG2_SECTION        "log2"
#define  LOG2_SKIP_LOG_KEY   "skiplog"
#define  LOG2_PATH_LOG_KEY   "pathlog"
#define  LOG2_PATH_LAST_KEY  "pathlast"
#define  LOG2_PATH1          "./"
#define  LOG2_PATH2LOG       "log/"
#define  LOG2_PATH2LAST      "data/"
//#define  LOG2_PATH2LAST      "last/"
#define  LOG2_EXT_LOG        ".log"
#define  LOG2_EXT_LAST       ""
#define  LOG2_PATH_LOG       LOG2_PATH1 LOG2_PATH2LOG
#define  LOG2_PATH_LAST      LOG2_PATH1 LOG2_PATH2LAST
#define  LOG2_TIME_IN_KEY    "timein"
#define  LOG2_TIME_IN_T      "m4hLog2/get/time"
#define  LOG2_TIME_OUT_KEY   "timeout"
#define  LOG2_TIME_OUT_T     "m4hLog2/ret/time"
#define  LOG2_TIMEFORMAT     "%Y%m%d %H%M%S"

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
 std::string skipLogKey;               // conf: key for skip log
 std::string pathLog;                  // path log files
 std::string pathLast;                 // path for last msgs
 std::vector <std::string> vSkipLog;   // skip messages
 std::string timeInTopic;              // topic in time of file changed
 std::string timeOutTopic;             // topic out time of file changed
 std::string timeFormat;               // timeformat of file changed

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
 std::string getFiletime(std::string topic);
 std::string getFiletime(std::string topic, std::string sTimeformat);
 //-----helper methods------------------------------------------
 std::string codeTopic(std::string topic);
 bool add2log(std::string topic, std::string payload);
 bool write2last(std::string topic, std::string payload);
 bool writeFile(std::string dir_, std::string fn_, 
  std::string line_, int flags_, mode_t permissions_);
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
 skipLogKey = LOG2_SKIP_LOG_KEY;       // skip topic @ log
 vSkipLog.clear();                     // clear skip topics
 timeInTopic=LOG2_TIME_IN_T;           // topic in time of file changed
 timeOutTopic=LOG2_TIME_OUT_T;         // topic out time of file changed
 timeFormat=LOG2_TIMEFORMAT;           // timeformat of file changed
 keys=std::string(LOG2_PATH_LOG_KEY);  // all keys in section
 keys+="|"+std::string(LOG2_PATH_LAST_KEY);
 keys+="|"+std::string(LOG2_SKIP_LOG_KEY);
 keys+="|"+std::string(LOG2_TIME_IN_KEY);
 keys+="|"+std::string(LOG2_TIME_OUT_KEY);
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
 Conf conf=Conf(pfConf);               // isReady, replaceAll
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
  //.....path of log directory..................................
  if(sKey==pathLogKey) {
   if(sVal.length()>0) {
    if(sVal.at(sVal.length()-1)!='/') sVal+="/";
    pathLog=sVal;
   }
  }
  //.....path of last payload directory.........................
  if(sKey==pathLastKey) {
   if(sVal.length()>0) {
    if(sVal.at(sVal.length()-1)!='/') sVal+="/";
    pathLast=sVal;
   }
  }
  //.....topic of messages not to be logged.....................
  if(sKey==skipLogKey) {
   conf.replaceAll(sVal, " ", "");
   vSkipLog.clear();
   conf.splitString(sVal, vSkipLog,',');
  }
    //-----search key topic in for get file time------------------
  if(sKey==LOG2_TIME_IN_KEY) {
   if(sVal.length()>0) {
    timeInTopic=sVal;
   }
  }
  //-----search key topic in for get file time------------------
  if(sKey==LOG2_TIME_OUT_KEY) {
   if(sVal.length()>0) {
    std::string sT="", sP="";
    if(conf.split2String(sVal, sT, sP, ' ')) {
     timeOutTopic=sT;
     timeFormat=sP;
    } else timeOutTopic=sVal;
   }
  }
 } // end for every line in section
 return true;
}

//_______Show all properties____________________________________
void Log2::show()
{
 std::cout<<"=====["<<section<<"]=============================="<<std::endl;
 Conf conf=Conf(pfConfig);
 std::cout<<"config file         | "<<pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout<<"all keys            | "<<getKeys()<<std::endl;
 std::cout<<pathLogKey<<"             | "<<pathLog<<std::endl;
 std::cout<<pathLastKey<<"            | "<<pathLast<<std::endl;
 std::cout<<LOG2_TIME_IN_KEY<<"              | "<<timeInTopic<<std::endl;
 std::cout<<LOG2_TIME_OUT_KEY<<"             | "<<timeOutTopic<<std::endl;
 std::cout<<"file time format    | "<<timeFormat<<std::endl;
 int len2=vSkipLog.size();
 std::cout<<".....log: topics to skip.............."<<std::endl;
 if(len2<1) std::cout<<"(no topics)"<<std::endl;
 for(int i=0; i<len2; i++) {
  if(i>0) std::cout<<",";
  std::cout<<vSkipLog.at(i);
 }
 std::cout<<std::endl;
}

//_______act on messages..._____________________________________
bool Log2::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 bool bRet=true;
 //------request file date and time?----------------------------
 if(topic==timeInTopic)
 {
  std::string s1=getFiletime(payload);
  if(s1.length()<1) { s1="==NO TIME=="; bRet=false; }
  s1+=" | "+payload;
  int iRet=mosquitto_publish(mosq, NULL,timeOutTopic.c_str(),
   s1.length(), s1.c_str(), 0, false);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",timeOutTopic.c_str(),iRet);
   bRet=false;
  } //end iRet!=0
 }
 //------check for topics to skip-------------------------------
 int len2=vSkipLog.size();
 for(int i=0; i<len2; i++) {
  if(topic==vSkipLog.at(i)) return bRet;
 }
 //------replace blank by _, / by @-----------------------------
 topic=codeTopic(topic);
 //------add message to log file--------------------------------
 if(!add2log(topic, payload)) {
  // fprintf(stderr,"Error while appending log-file %s\n",topic.c_str());
  bRet=false;
 }
 //------write payload to last message file---------------------
 if(!write2last(topic, payload)) {
  // fprintf(stderr,"Error while writing last-file %s\n",topic.c_str());
  bRet=false;
 }
 return bRet;
}

//_______Possibility for cleanup before end of program__________
void Log2::onExit(struct mosquitto *mosq, int reason)
{
}

//_______read most recent time the file contents were changed___
// return date and time or "" on error
std::string Log2::getFiletime(std::string topic)
{
 return getFiletime(topic, timeFormat);
}

//_______read most recent time the file contents were changed___
// return date and time or "" on error
std::string Log2::getFiletime(std::string topic, std::string sTimeformat)
{
 std::string sTimeformat_="%Y%m%d %H%M%S";
 if(sTimeformat.length()>0) sTimeformat_=sTimeformat;
 std::string sTime="";
 int len=20+sTimeformat_.length();
 char caTime[len];
 time_t tChange;
 struct stat status;
 std::string topic_=codeTopic(topic);  // replace blank, slash
 //------build path + filename----------------------------------
 std::string sFilename;
 sFilename =pathLast;
 sFilename+=topic_;
 sFilename+=LOG2_EXT_LAST;
 //------get file status----------------------------------------
 if(stat(sFilename.c_str(), &status)!=0) return sTime;
 //------build change time--------------------------------------
 tChange=status.st_mtime;
 //tChange=status.st_atime;
 struct tm* time_=localtime(&tChange);
 strftime(caTime,len,sTimeformat_.c_str(),time_);
 sTime=caTime;
 return sTime;
}

// *************************************************************
//       Log2: helper methods
// *************************************************************

//_______code topic: replace blank by _, / by @_________________
std::string Log2::codeTopic(std::string topic)
{
 std::string topic_=topic;
 std::replace(topic_.begin(), topic_.end(), ' ', '_');
 std::replace(topic_.begin(), topic_.end(), '/', '@');
 return topic_;
}

//_______append message to the corresponding log file___________
// return: true=ok, false on Error
bool Log2::add2log(std::string topic, std::string payload)
{
 //------general variables--------------------------------------
 int ret;
 int fd;
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
 sLine+=std::string("\n");
 int flags = O_WRONLY | O_CREAT | O_APPEND;
 mode_t permissions = S_IRWXU | S_IRWXG | S_IRWXO; // = 0777
 if(writeFile(pathLog, sFilename, sLine, flags, permissions)) {
  if(g_prt && LOG2_PRT) std::cout<<"*** Append "<<sLine<<"    to "<<sFilename<<std::endl;
  return true;
 }
  if(g_prt) std::cout<<"*** ERROR while appending "<<sLine<<" to "<<sFilename<<std::endl;
 return false;
}
 
//_______write payload to last message file_____________________
// return: true=ok, false on Error
bool Log2::write2last(std::string topic, std::string payload)
{
 //------general variables--------------------------------------
 int ret;
 int fd;
 //------build path + filename----------------------------------
 std::string sFilename;
 sFilename =pathLast;
 sFilename+=topic;
 sFilename+=LOG2_EXT_LAST;
 //------build text---------------------------------------------
 std::string sLine;
 sLine=payload;
 sLine+=std::string("\n");
 int flags = O_WRONLY | O_CREAT;
 mode_t permissions = S_IRWXU | S_IRWXG | S_IRWXO; // = 0777
 if(writeFile(pathLog, sFilename, sLine, flags, permissions)) {
  if(g_prt && LOG2_PRT) std::cout<<"___ Write "<<payload<<" to "<<sFilename<<std::endl;
  return true;
 }
  if(g_prt) std::cout<<"___ ERROR while writing "<<payload<<" to "<<sFilename<<std::endl;
 return false;
}

//_______write or append to file (depending on flags)___________
// function tries 3x to write data
// return true = OK
bool Log2::writeFile(std::string dir_, std::string fn_, 
  std::string line_, int flags_, mode_t permissions_)
{
 int fh;                     // file handle
 //------does directory exist (no: create it)-------------------
 if(!isDir(dir_)) { 
  if(!makeDir(dir_)) return false;
 }
 //------try to open file stream--------------------------------
 int i=3;
 while(i>0) {
  i--;
  fh=open(fn_.c_str(), flags_, permissions_);
  if(fh != -1)
  {
   //----write or append line to file---------------------------
   int len=line_.length();
   int iret = write(fh, line_.c_str(), len);
   if( iret == len) { 
    close(fh); 
    return true;             // success!
   }
   close(fh);
  }
  //-----wait and try again-------------------------------------
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
