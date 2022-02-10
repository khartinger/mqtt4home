//_____m4hExtension.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hBase -lmosquitto -lpthread
// *  This program demonstrates a basic implementation for the
//    m4hBase template.
// 1. show which five functions must be provided by the user.
// 2. print all incoming messages (so you can see that the 
//    program is working ;))
// *  All functions of m4hBase are also available.
//    (For more information see file m4hMain.cpp.)
// *  m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2022-02-10 add comments, zsec
// Released into the public domain.

#include "mosquitto.h"            // mosquitto_* functions
#include "m4hBase.h"              // m4h basic functions

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);    // myMqttPeriodicExec

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hBase [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4hLocal.conf)\n");
 fprintf(stdout, "Purpose: (1) Demo for m4hBase\n");
 fprintf(stdout, "         (2) Program shows all incomming messages.\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2022-02-10");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
bool f2Init(std::string pfConf)
{
 bool bRet=true;
 //...Add "g_xxx.readConfig(pfConf);" lines here ;) ...
 return bRet;
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //------print all received messages----------------------------
 std::cout<<topic<<" | "<<payload<<std::endl;
 //...Add "g_xxx.onMessage(mosq, topic, payload);" lines here...
}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
 //...Add "g_xxx.onExit(mosq, reason);" lines here...
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
  //...Add "g_xxx.periodic(mosq);" lines here...
  //
  //=====wait for 0.1sec========================================
  if((--zsec)<=0) zsec=600;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
 }; // end of "endless loop"
 terminate_program(iEnd);
}
