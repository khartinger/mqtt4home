//_____m4hExtension.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread
// * This program uses m4hBase to do the following:
//   1. Send and receive SMS and convert to MQTT.
// * All functions of m4hBase are also available.
//   (For more information see file m4hMain.cpp.)
// * m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// * m4hExtension.hpp must have a line "#include "C_Sms.hpp"
// Hardware: (1) Raspberry Pi
//           (2) GSM-Modem e.g. SIM808
//           (3) USB-Serial-Adapter
// Updates:
// 2021-08-25 First release
// Released into the public domain.

#include "mosquitto.h"                 // mosquitto_* functions
#include "m4hBase.h"                   // m4h basic functions
#include "C_Sms.hpp"                   // additional code

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hSms [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4h.conf)\n");
 fprintf(stdout, "Purpose: Send and receive SMS and convert to MQTT.\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2021-08-25");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
void f2Init(std::string pfConf)
{
 g_sms.readConfig(pfConf);            // read conf data
 if(g_prt) g_sms.show();              // show config values Sms
 bool bRet=g_sms.isModule();
 if(g_prt) {
  fprintf(stdout, "GSM module ");
  if(!bRet) fprintf(stdout, "NOT ");
  fprintf(stdout, "found at %s!\n",g_sms.getDevice().c_str());
 }
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //======Sms: respond to messages===============================
 g_sms.onMessage(mosq, topic, payload);

}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
 //======Sms: cleanup before end of program=====================
 g_sms.onExit(mosq, reason);
}


//_______for periodic actions (a parallel thread)_______________
void f5Periodic(struct mosquitto *mosq)
{
 bool bDoPeriodic=true;                // do "endles"
 int  iEnd=4;                          // reason for end
 int zsec=10;                          // check every second
 while(bDoPeriodic) //-----"endless"----------------------------
 {
   //=====check every second for incomming sms===================
  // -4 not allowed|-3 sms read error|-2 modem busy|-1=error
  // 0=no sms, 1=sms received, 2=end sms received
  if((zsec--)<=0) {
   zsec=10;
   if(g_sms.lookForSmsIn(mosq)==2) { bDoPeriodic=false; iEnd=5; }
  }
  //-----end check every second for incomming sms---------------
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
 }; // end "endless"
 terminate_program(iEnd);
}
