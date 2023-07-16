//_____m4hExtension.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hYyy -lmosquitto -lpthread
// *  This program uses m4hBase to do the following:
//    ..ToDo..
// *  All functions of m4hBase are also available.
//    (For more information see file m4hMain.cpp.)
// *  m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// *  m4hExtension.hpp must have a line "#include "C_Yyy.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2022-02-10 add comments, zsec
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include "C_Yyy.hpp"                   // additional code

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);    // myMqttPeriodicExec

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hYyy [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4h.conf)\n");
 fprintf(stdout, "Purpose: Template to create a module for m4h.\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2023-07-16");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
bool f2Init(std::string pfConf)
{
 bool bRet=true;
 //...Add "g_yyy.readConfig(pfConf);" lines here ;) ...
 bRet=bRet && g_yyy.readConfig(pfConf);// read conf data
 if(g_prt) {
  if(bRet) std::cout<<"All config file data read :)"<<std::endl;
  else std::cout<<"NOT all config file data read :("<<std::endl;
 }
 if(g_prt) g_yyy.show();              // show config values Yyy
 return bRet;
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //======respond to messages====================================
 g_yyy.onMessage(mosq, topic, payload);
}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
 g_yyy.onExit(mosq, reason);
}

//_______for periodic actions (a parallel thread)_______________
void f5Periodic(struct mosquitto *mosq)
{
 bool bDoPeriodic=true;                // do "endless"
 int  iEnd=4;                          // reason for end
 int  zsec=600;                        // 60 secs
 //======"endless loop"=========================================
 while(bDoPeriodic)
 {
  //...Add here the code that should be executed periodically...
  //...Add "g_yyy.periodic(mosq);" lines here...
  g_yyy.periodic(mosq, zsec);
  //=====wait for 0.1sec========================================
  if((--zsec)<=0) zsec=600;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
 }; // end of "endless loop"
 terminate_program(iEnd);
}
