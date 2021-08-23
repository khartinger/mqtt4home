//_____m4hExtension.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hSubPub -lmosquitto -lpthread -std=c++17
// * This program uses m4hBase to do the following:
//   1. After receiving a message, a specified time is waited
//      and then a reply message is sent.
//   2. topic in, topic out and delay time must be specified
//      in config file.
//   3. The program can be easily extended to process runtime
//      data (JSON formatted incoming payload or action key).
//      Built-in examples are:
//    + Send along the wait time in the payload {"delay": 4000}
//    + Send along the outgoing payload 
//      {"text": "new payload out :)"}
//    + Send the current time {"do": "brokertime"} or
//      {"do": "time"}
//    + Inverted forwarding of the incoming payload 0 or 1 
//      as 1 or 0 respectively {"do": "invert01"}
// * All functions of m4hBase are also available.
//   (For more information see file m4hMain.cpp.)
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_SubPub.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-23 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include "C_SubPub.hpp"                   // additional code

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hSubPub [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4h.conf)\n");
 fprintf(stdout, "Purpose: Reply to a received message after a waiting time (specified in config file).\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2021-08-23");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
void f2Init(std::string pfConf)
{
 g_subpub.readConfig(pfConf);            // read conf data
 if(g_prt) g_subpub.show();              // show config values SubPub
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //======SubPub: respond to messages===============================
 g_subpub.onMessage(mosq, topic, payload);

}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
 //======SubPub: cleanup before end of program=====================
 g_subpub.onExit(mosq, reason);
}


//_______for periodic actions (a parallel thread)_______________
void f5Periodic(struct mosquitto *mosq)
{
 bool bDoPeriodic=true;                // do "endles"
 int  iEnd=4;                          // reason for end
 while(bDoPeriodic) //-----"endless"----------------------------
 { //...Do something...
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 };
 terminate_program(iEnd);
}
