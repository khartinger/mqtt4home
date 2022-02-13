//_____m4hMain.cpp_______________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hBase -lmosquitto -lpthread
// This program serves as a basis for own C++ programs and 
// realizes the following basic functionalities:
// 1. Reading settings from the config file m4h.conf.
// 2. Possibility to specify a different config file 
//    (when starting the program on the command line).
// 3. Answering a request for the program version.
// 4. Sending (or not sending) an MQTT message at 
//    program start and/or program end.
// 5. Use of the global objects g_base, g_prt, g_mosq
// 6. Ability to exit program by a MQTT message specified
//    in the config file.
// 7. Exit program by <Ctrl>c.
//
// Other functionalities requested by the user must be 
// implemented in the file m4hExtension.hpp using the functions
// * void f1PrintHelptext();
// * bool f2Init(std::string pfConf);
// * void f3OnMessage(struct mosquitto *mosq, std::string topic, std::string payload);
// * void f4OnExit(struct mosquitto *mosq, int reason)
// * void f5Periodic(struct mosquitto *mosq);
//
// (Needs sudo apt-get install libmosquitto-dev !)
// LIB-Info: https://mosquitto.org/api/files/mosquitto-h.html
//
// Hardware: (1) Raspberry Pi
// Updates: 
// 2021-08-19 First release
// 2021-08-20 m4h_msg_callback(): add if(message->payload!=NULL)...
// 2022-02-10 add reload conf file by mqtt command
// Released into the public domain.

#include "mosquitto.h"            // mosquitto_* functions 
#include "m4hBase.h"              // m4h basic functions
#include "m4hExtension.hpp"       // my other functionalities

//-------global values------------------------------------------
bool  g_prt=true;                      //true=printf,false=quiet
pid_t g_pid_main=-1;                   //main process id to kill
int   g_endBy___=-1;                   //help value
struct mosquitto *g_mosq = NULL;       //mosquitto "object"

//_______function delaration____________________________________
void terminate_program2(int endBy___);
void terminate_program(int endBy___);
void my_signal_handler(int signum);

//_______terminate programm (part 2)____________________________
// This function prints the reason for the program end, stops
// all threads, clears the memory and terminates the program.
void terminate_program2(int endBy___)
{
 std::string s1;
 //------string with reason for terminating---------------------
 switch(endBy___)
 {
  case 1: s1="Program terminated by <ctrl>c";
   break;
  case 2: s1="Program terminated by MQTT";
   break;
  case 3: s1="Program terminated by main()";
   break;
  case 4: s1="Program terminated by application";
   break;
  case 5: s1="Program terminated by sms";
   break;
  default: s1=std::to_string(endBy___);
   s1="Program terminated by unknown reason #"+s1;
   break;
 }
 //------generate time string-----------------------------------
 std::string sTime="";
 if(g_base.timeShouldBeAdded) sTime=" ("+g_base.getDateTime()+")";
 //------print reason for terminating---------------------------
 std::this_thread::sleep_for(std::chrono::milliseconds(100));
 fprintf(stdout, ("\n"+s1+sTime+"\n").c_str());
 //-----clean up------------------------------------------------
 mosquitto_destroy(g_mosq);
 mosquitto_lib_cleanup();
 kill(g_pid_main, SIGTERM);
 exit(endBy___);
}

//_______terminate programm (part 1)____________________________
// This function starts the ending of the program:
// * Either the end message is sent (if this was set in the
//    config file). 
// * or it goes directly to the 2nd part of the program end.
void terminate_program(int endBy___)
{
 if(g_prt) fprintf(stdout, "\nExit program... ");
 g_endBy___=endBy___;
 //------Possibility for cleanup before end of program----------
 f4OnExit(g_mosq, endBy___);
 //------send end MQTT message?---------------------------------
 if(g_base.msgMqttEnd.topic.length()>0)
 {//-----yes, send an end message-------------------------------
  try {
   //....generate time string...................................
   std::string sTime="";
   if(g_base.timeShouldBeAdded) sTime=" ("+g_base.getDateTime()+")";
   //....set qos (Quality of Service): 0 = At most once,
   //    1 = At least once, 2 = Exactly once
   int qos=2;
   std::string s2=g_base.msgMqttEnd.payload+sTime;
   //....now send end mqtt end message..........................
   int ret=mosquitto_publish(g_mosq, NULL,g_base.msgMqttEnd.topic.c_str(),
   s2.length(), s2.c_str(), qos, g_base.msgMqttEnd.retain);
   //....Error handling.........................................
   if(ret!=0) 
   {
    if(g_prt) fprintf(stdout, "MQTT end message NOT sent. Error=%i",ret);
    terminate_program2(endBy___);
   }
   else 
   {//...message sent successfully..............................
    if(g_prt) fprintf(stdout, "MQTT end message sent.");
   }
  } catch(std::string& error) { //.....error handling...........
   fprintf(stderr,"Error while publishing end message: %s\n",error);
   terminate_program2(endBy___);
  }
 }
 else // don´t send end message.................................
 {//-----continue to the 2nd part of the program end------------
  terminate_program2(endBy___);
 }
 if(g_prt) fprintf(stdout, "\n");      // next line
}

//_______handler for signal SIG*** (e.g. <ctrl>c)_______________
void my_signal_handler(int signum)
{
 if(signum==SIGINT) { terminate_program(1); }
 //exit(98);
}

//_____print log message________________________________________
void m4h_log_callback(struct mosquitto *g_mosq,
 void *userdata, int level, const char *str)
{
 switch(level)
 {
  case MOSQ_LOG_DEBUG:
  case MOSQ_LOG_INFO:
  case MOSQ_LOG_NOTICE:
   break;
  case MOSQ_LOG_WARNING:
  case MOSQ_LOG_ERR:
  default:
   if(g_prt) fprintf(stderr, "LOG-Message %i: %s\n", level, str);
 }
}

//_____connect info_____________________________________________
void m4h_connect_callback(struct mosquitto *g_mosq,
 void *userdata, int result)
{
 if(!result)
 { //-----Subscribe topic on successful connect-----------------
  mosquitto_subscribe(g_mosq,NULL,M4H_SUBSCRIBE,2);  //2=QOS=exact 1x
  } else {
  if(g_prt) fprintf(stderr, "Connect failed\n");
 }
}

//_____message call back routine________________________________
// called by every subscribed message
// parameter will be destroyed after return!
void m4h_msg_callback(struct mosquitto *g_mosq, void *userdata,
 const struct mosquitto_message *message)
{
 int iRet;
 //======PART 1: plausibility check topic name==================
 if(message->topic==NULL) return;
 if(strlen(message->topic)<1) return;
 //if(g_prt) printf("1: %s | %s\n", message->topic, message->payload);
 //======PART 2: external callback function=====================
 // original topic name, payload may be empty...
 std::string sTopic=message->topic;
 std::string sPayload="";
 if(message->payload!=NULL) sPayload=(char*)message->payload;
 std::thread mythread2(f3OnMessage, g_mosq, sTopic, sPayload);
 mythread2.detach();
 //======PART 3: act on "default" payloads======================
 //------version------------------------------------------------
 if(sTopic==g_base.msgVersion.topicIn) 
 {
  if(g_base.msgVersion.payloadIn.length()==0 || 
     g_base.msgVersion.payloadIn==sPayload)
  {
   try
   {
    std::string s1=g_base.msgVersion.payloadOut;
    iRet=mosquitto_publish(g_mosq, NULL,g_base.msgVersion.topicOut.c_str(),
     s1.length(), s1.c_str(), 0, 0);
    if(iRet!=0) {
     if(g_prt) fprintf(stderr, "Could not send version answer. MQTT error=%i\n",iRet);
    }
   } catch(std::string& error) {
     fprintf(stderr,"Error while publishing answer: %s\n",error);
   }
  }
  return;
 }
  //------reload config file-------------------------------------
 if(sTopic==g_base.msgReadConf.topicIn)
 {
  try
  {
   iRet=-1;
   int lenP = sPayload.length();
   if(lenP<1) return;
   std::string pfConf1=g_base.msgReadConf.payloadIn;
   if(sPayload!="?" && lenP>2) {
    pfConf1=sPayload;
    g_base.msgReadConf.payloadIn=sPayload;
   }
   std::string s1=g_base.msgReadConf.payloadOut;
   bool bRet=g_base.readConfig(pfConf1);
   if(g_prt) g_base.show();
   bRet=bRet&&f2Init(pfConf1);
   if(bRet) s1+=" OK";
   else s1+=" ERROR";
   s1+=" (file: "+pfConf1+")";
   iRet=mosquitto_publish(g_mosq, NULL,g_base.msgReadConf.topicOut.c_str(),
    s1.length(), s1.c_str(), 0, 0);
   if(iRet!=0) {
    if(g_prt) fprintf(stderr, "Could not send read config answer. MQTT error=%i\n",iRet);
   }
  } catch(std::string& error) {
    fprintf(stderr,"Error while publishing answer: %s\n",error);
  }
  return;
 }
 //------check for command "end program by MQTT"----------------
 if(sTopic==g_base.msgProgEnd.topic)
 {
  if(g_base.msgProgEnd.payload.length()>0 &&
     g_base.msgProgEnd.payload==sPayload)
  {
   terminate_program(2);          // start process end of program
  }
 }
 //-----receive sent end message--------------------------------
 // check: sent MQTT message "End of program" was received again
 if(sTopic==g_base.msgMqttEnd.topic)
 {
  if(g_endBy___>0)
  {
   //.....cut away date and time (if available)..................
   int lenP=g_base.msgMqttEnd.payload.length();
   if(lenP>0)
   {
    std::string s1=sPayload.substr(0,lenP);
    if(s1==g_base.msgMqttEnd.payload) 
     terminate_program2(g_endBy___); // continue process end of prog
   }
  }
 }
 //-----for any other payload-----------------------------------
 return;
}

//_____main program_____________________________________________
int main(int argc, char *argv[])
{ 
 int  argOK=-99;                       // !=0 print help text
 bool bRet=false;                      // bool return value
 char host[]=M4H_HOST;                 // host=localhost
 char *phost=host;                     // pointer to hostname
 int  port=M4H_PORT;                   // mqtt port
 std::string pfConf=_CONF_PFILE_;      // path+file config
 std::string s1;                       // help string
 //------check argument(s)--------------------------------------
 if(argc==1) argOK=0;
 if(argc==2) {
  if(argv[1][0]=='-') argOK=1;         // help or quiet
  else { pfConf=argv[1]; argOK=0; }    // only config file
 }
 if(argc==3 && argv[1][0]=='-') { argOK=1; pfConf=argv[2]; }
 if(argc==3 && argv[2][0]=='-') { argOK=2; pfConf=argv[1]; }
 if(argOK==1 ||argOK==2)
 {
  switch(argv[argOK][1])
  {
   case 'q': g_prt=false; argOK=0; break;
   case 'h': argOK=-1; break;
   default: break;
  }
 }
 //------if params not ok: help text----------------------------
 if(argOK!=0) { f1PrintHelptext(); return -1; }

 //------init signal handler, prepare the end of the program----
 signal(SIGINT, my_signal_handler);    // <std c> handler
 g_pid_main=getpid();                  // pid of main program

 //------read config data---------------------------------------
 bRet=g_base.readConfig(pfConf);
 if(g_prt) std::cout<< "Read config file "<<pfConf<<": ";
 if(g_prt && bRet)  std::cout<< "OK"<<std::endl;
 if(g_prt && !bRet) std::cout<< "NOT OK"<<std::endl;
 if(g_prt) g_base.show();

 //------init extension (or read extension config from file)----
 f2Init(pfConf);
 //------init network and mqtt----------------------------------
 mosquitto_lib_init();
 g_mosq=mosquitto_new(NULL, true, NULL);
 if(!g_mosq)
 {
  fprintf(stderr, "Error: Out of memory.\n");
  return(1);
 }
 mosquitto_threaded_set(g_mosq, true);
 mosquitto_log_callback_set(g_mosq, m4h_log_callback);
 mosquitto_connect_callback_set(g_mosq, m4h_connect_callback);
 mosquitto_message_callback_set(g_mosq, m4h_msg_callback);
 //mosquitto_subscribe_callback_set(mosq, m4h_subscribe_callback);
 if(g_prt) fprintf(stdout, "Try to connect to mosquitto...\n");
 while(mosquitto_connect(g_mosq, host, port, M4H_KEEPALIVE)!=MOSQ_ERR_SUCCESS)
 {
  if(g_prt) { fputc('.', stdout); fflush(stdout); }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 }
 if(g_prt) fprintf(stdout,"Connected: Waiting for topics...\n");
 //------start a second external endless loop-------------------
 std::thread mythread1(f5Periodic, g_mosq);
 mythread1.detach();
 //------send start MQTT message--------------------------------
 if(g_base.msgMqttStart.topic.length()>0)
 {
  try {
   int qos=2;
   s1="";
   if(g_base.timeShouldBeAdded) s1=" ("+g_base.getDateTime()+")";
   s1=g_base.msgMqttStart.payload+s1;
   int ret=mosquitto_publish(g_mosq, NULL,g_base.msgMqttStart.topic.c_str(),
    s1.length(), s1.c_str(), qos, g_base.msgMqttStart.retain);
   if(ret!=0) {
    if(g_prt) fprintf(stderr, "Could not send start message. MQTT send error=%i\n",ret);
   } 
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } catch(std::string& error) {
   fprintf(stderr,"Error while publishing answer: %s\n",error);
  }
 }
 //-----endless loop, terminate program with <ctrl>c------------
 mosquitto_loop_forever(g_mosq, -1, 1);
 if(g_prt) fprintf(stdout, "main(): after loop_forever...\n");
 terminate_program(3);
 return(99);
}