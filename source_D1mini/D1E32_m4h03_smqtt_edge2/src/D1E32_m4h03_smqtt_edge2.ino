//_______D1E32_m4h03_smqtt_edge2.ino_____________khartinger_____
// Power measurement through pulse counting and MQTT info.
// This program for a D1 mini or ESP32 mini is used to 
// * count pulses (falling edge) at pin D5 (IO14) and 
// * send periodic MQTT messages with a "power" value that is 
//   calculated in function getsWatt().
// The program function in detail:
// [1] Connect to given WiFi and MQTT broker
// [2] Send a start message
// [3] Use "topic base" stored in eeprom or topic TOPIC_BASE
// [4] Answer all get requests in TOPIC_GET e.g. for help
//     -t info/test/1/get -m ?
// [5] Do all set requests in TOPIC_SET and answer them, e.g.
//     set a new topic base: 
//     -t info/test/1/set/topicbase  -m info/test/2
// [6] Show the power value on the 0.66" OLED display
// Class SimpleMqtt extends class PubSubClient for easy use.
// All commands of the PubSubClient class can still be used.
// Note: When PubSubClient lib is installed,
//       delete PubSubClient files in directory src/simplemqtt
// Hardware: 
// (1) WeMos D1 mini (OR ESP32 D1 mini)
// (2) Button shield or photo transistor @ D5 (not D3)
// (3) 0,66" OLED Shield
//
// Important: Example needs a MQTT-broker!
// Created by Karl Hartinger, September 06, 2022.
// Changes:
// 2022-09-06 New
// 2024-08-13 Customisation for D1 mini and ESP32, addition of
//            OLED 0.66", change to power calculation and MQTT
// Released into the public domain.

//#define D1MINI          1              // ESP8266 D1mini +pro
#define ESP32D1         2              // ESP32 D1mini

#include <Arduino.h>
#include "src/screen1b/D1_class_Screen1b.h"
#include "src/simplemqtt/D1_class_SimpleMqtt.h"

// *************************************************************
// 1     Definitions, declarations and global variables
// *************************************************************

#define  DEBUG03        true  //false
#define  VERSION_M4H03  "2024-08-13 D1E32_m4h03_smqtt_edge2"
#define  FUNCTION_M4H03 "Count pulses, calc watts, send MQTT message"
#define  TOPIC_BASE     "info/test/1"
#define  TOPIC_GET      "?,help,version,ip,topicbase,eeprom,sec,watt,watt_"
#define  TOPIC_SET      "topicbase,eeprom0,sec"
#define  TOPIC_SUB      ""
#define  TOPIC_PUB      "info/test/1/ret/watt5"

//_____sensors, actors, global variables________________________
#if defined(ESP32) || defined(ESP32D1)
 #include "esp_timer.h"  // ESP32 spezifische Timerbibliothek
 IRAM_ATTR void myTimer_ISR(void *pArg);
 esp_timer_handle_t myTimer;
 const esp_timer_create_args_t timer_args1 = {
  .callback = &myTimer_ISR,                 // pointer callback fkt
  .name = "timer1"
 };
 #define IN_PIN         18                  // D5=IO18// D3=IO17
 #define BLUELED_PIN    2                   // led pin D4=IO2
 #define BLUELED_ON     1                   // ESP32 1
 #define BLUELED_OFF    0                   // ESP32 0
#else
  extern "C" {
  #include "user_interface.h"
 }
 os_timer_t    myTimer;
 #define IN_PIN         D5                  // D5=14 // D3=0
 #define BLUELED_PIN    D4                  // led pin D4=IO2
 #define BLUELED_ON     0                   // D1mini 0
 #define BLUELED_OFF    1                   // D1mini 1
#endif

//_______Global values for updating the screen__________________
// 0.66" OLED display with SSD1306, 64x48 pixel, I2C
#define  SCREEN_TITLE        "WATT"
#define  SCREEN_LINE_MAX     4
//#define  SCREEN_LINE_LEN     10
//       buffer for lines for screen update
String   aScreenText[SCREEN_LINE_MAX];
//       align per line: 'c'=center, 'l'=left, 'r'=right
char     aScreenAlign[SCREEN_LINE_MAX]={'c','c','r','c'};
#define  SCREEN_LINE_3_NO    " NO WLAN! "
#define  SCREEN_LINE_4       "11.08.2024"
Screen1  screen_;

String getsWatt5();

//_______interrupt utilities____________________________________
#define SEC_BETW_MSGS   300                 // every 5min auto MQTT msg
#define LED_ON_ZSEC     3                   // on edge 0,3s blue led on
#define DEBOUNCE_MS     100                 // delay

volatile bool isrSendMessage5 = false;      // send 5 min message
volatile unsigned long isrMillisLast=0;     // last ms value
volatile unsigned long isrPulseCounter=0;   // number of pulses
volatile long isrWatt1;                     // current power
volatile int  iLedOnZsec=0;                 // blue led on after pulse
int           secBetwMsgs=SEC_BETW_MSGS;    // secs between messages
unsigned long millisStart5=millis();        // starttime of 5min meas

// *************************************************************
// 2     Interrupt Functions
// *************************************************************

//_______io interrupt service routine (falling edge)____________
// uses millisLast, millisNum, millisSum, iLedOnZsec,
//      DEBOUNCE_MS, LED_ON_ZSEC
IRAM_ATTR void io_ISR(void) {
 unsigned long millisNow=millis();          // ISR start time
 unsigned long millisDiff=millisNow-isrMillisLast;
 //------debounce push button-----------------------------------
 if(millisDiff < DEBOUNCE_MS) return;
 //------command: turn blue led on------------------------------
 iLedOnZsec=LED_ON_ZSEC;
 //------add one measuring point--------------------------------
 isrPulseCounter++;
 // if(millisDiff!=0) isrWatt1=3600000/millisDiff;
 isrWatt1=3600000/millisDiff;
 //------prepare next measurement-------------------------------
 isrMillisLast=millisNow;
}

//_______mytimer1 interrupt service routine_____________________
// uses bSendMessage
IRAM_ATTR void myTimer_ISR(void *pArg) {
 isrSendMessage5=true;
}

//_______stop and (re)start timer interrupt_____________________
// uses: myTimer, secBetwMsgs
void startMyTimerInterrupt() {
 #if defined(ESP32) || defined(ESP32D1)
  esp_timer_stop(myTimer);                       //stop timer
  esp_timer_delete(myTimer);                     //delete timer
  esp_timer_create(&timer_args1, &myTimer);      //create timer
  esp_timer_start_periodic(myTimer, secBetwMsgs*1000000); // usec
 #else
  os_timer_disarm(&myTimer);                     //stopp timer
  os_timer_setfn(&myTimer, myTimer_ISR, NULL);   //NULL:no param
  os_timer_arm(&myTimer, secBetwMsgs*1000, true); // repeat
 #endif
}

// *************************************************************
// 3     MQTT Functions
// *************************************************************

//_______MQTT communication_____________________________________
//SimpleMqtt client("..ssid..", "..password..","mqttservername");
SimpleMqtt client(String("Raspi11"),String("12345678"),String("10.1.1.1"),String(TOPIC_BASE));


//_______MQTT: inspect all subscribed incoming messages_________
void callback(char* topic, byte* payload, unsigned int length)
{
 client.callback_(topic, payload, length);  // must be called!
}

//_______answer get requests____________________________________
// sPayload: payload to message from TOPIC_GET
// auto answer: for help (+), version, ip (can be overwritten)
// return: ret answer payload for get request
String simpleGet(String sPayload)
{
 String p1;                            // help string
 //-------------------------------------------------------------
 if((sPayload=="?")||(sPayload=="help"))
  return "+"+String(VERSION_M4H03)+": "+String(FUNCTION_M4H03);
 //-------------------------------------------------------------
 if(sPayload=="version") return String(VERSION_M4H03);
 //-------------------------------------------------------------
 if(sPayload=="topicbase") return client.getsTopicBase();
 //-------------------------------------------------------------
 if(sPayload=="eeprom") {
  int iResult1, iResult2;
  String s1=client.eepromReadTopicBase(iResult1);
  String s2=client.eepromReadMyData(iResult2);
  p1="{\"topic\":\"";
  if(iResult1>=0) p1+=s1;
  else
  {
   if(iResult1==-2) p1+="(no topic base)";
   else p1+=String("Error_#")+String(iResult1);
  }
  p1+="\",\"data\":\"";
  if(s2=="") p1+="(no data)";
  else p1+=s2;
  p1+="\"}";
  return p1;
 }
 //-------------------------------------------------------------
 if(sPayload=="sec") {
  return String(secBetwMsgs);
 }
 //-------------------------------------------------------------
 if(sPayload=="watt") {
  return String(isrWatt1);
 }
 //-------------------------------------------------------------
 if(sPayload=="watt_") {
  return getsWatt5();
 }
 //-------------------------------------------------------------
 return String("");                         // wrong Get command
}

//_______execute set requests___________________________________
// sTopic from TOPIC_SET, sPayload: payload to topic
// return: ret answer payload for set command
String simpleSet(String sTopic, String sPayload)
{
 String p1;                                 // help string
 //-------------------------------------------------------------
 if(sTopic=="topicbase") {                  // new topic base?
  client.changeTopicBase(sPayload);         // change base
  return client.getsTopicBase();            // return new base
 }
 //-------------------------------------------------------------
 if(sTopic=="eeprom0") {                    // erase eeprom?
  if(sPayload=="?") return "erase...1=topicBase|2=myData|3=all";
  if(sPayload=="2" || sPayload=="3" || sPayload=="all")
  {
   if(client.eepromEraseMyData()) p1="my data erased";
   else p1="my data NOT erased";            // return answer
  }
  if(sPayload=="1" || sPayload=="3" || sPayload=="all")
  {
   if(p1!="") p1=", "+p1;
   if(client.eepromEraseTopicBase()) p1="topicbase erased"+p1;
   else p1="topicbase NOT erased"+p1;       // return answer
  }
  return p1;
 }
 //-------------------------------------------------------------
 if(sTopic=="sec") {
  int iSec=sPayload.toInt();
  if(iSec>=5 && iSec<=6870)
  {
   //....
   secBetwMsgs=iSec;                        // new message value
   startMyTimerInterrupt();                 // restart timer
   //....
   p1=sPayload+"     ";
   p1=p1.substring(0,5);
   String s1=String("TS=");
   client.eepromWriteMyData(s1+p1);
   
   p1=String("new_limit_");
   return p1+sPayload+"_sec";
  } else
  {
   return String("limit_not_set_(5..6870)");
  }
 }
 //-------------------------------------------------------------
 return String("");                         // wrong set command
}

//_______execute sub requests___________________________________
// sTopic from TOPIC_SUB, sPayload: payload to topic
// return: no automatic answer
void simpleSub(String sTopic, String sPayload)
{
}

// *************************************************************
// 4     Display Functions
// *************************************************************

//_______Save line content, write it to display and show screen_
// line_: 1 ... SCREEN_LINE_MAX (4)
// uses: screen_, aScreenText[]
void showLine(int line_, String text_) {
 if(line_<1 || line_>SCREEN_LINE_MAX) return;
 aScreenText[line_-1]=text_;
 screen_.screen112(line_,aScreenText[line_-1],aScreenAlign[line_-1]);
}

//_______Refresh screen to avoid damage_________________________
// uses: screen_, aScreenText[]
void refreshScreen() {
 screen_.screen112Clear(1,aScreenText[0],aScreenAlign[0]);
 screen_.screen112(2,aScreenText[1],aScreenAlign[1]);
 screen_.screen112(3,aScreenText[2],aScreenAlign[2]);
 screen_.screen112(4,aScreenText[3],aScreenAlign[3]);
}

// *************************************************************
// 5     Other Functions
// *************************************************************

//_______build power (watt) string______________________________
String getsWatt5()
{
 unsigned long watt=0;
 unsigned long ms = millis()-millisStart5;
 if(isrPulseCounter>0) {
  if(ms!=0) watt=(unsigned long) (3600000*isrPulseCounter/ms);
  else watt=(unsigned long) 99999;
 } else {
  watt=0;
 }
 String s1 = String(watt);
 return s1;
}

// *************************************************************
// 6     Standard Functions
// *************************************************************

//_______SETUP__________________________________________________
void setup() {
 //------Serial, just for debug---------------------------------
 if(DEBUG03) 
 {Serial.begin(115200); Serial.println("\nsetup(): --Start--");}
 //------init pins----------------------------------------------
 pinMode(IN_PIN, INPUT);                    // set pin to output
 pinMode(IN_PIN, INPUT_PULLUP);             // use pull up resistor
 pinMode(BLUELED_PIN, OUTPUT);              // set pin to output
 digitalWrite(BLUELED_PIN, BLUELED_OFF);    // turn led off
 //------init screen--------------------------------------------
 screen_.begin();
 aScreenText[0]=SCREEN_TITLE;
 aScreenText[1]="0";
 aScreenText[2]=SCREEN_LINE_3_NO;
 aScreenText[3]=SCREEN_LINE_4;
 refreshScreen();
 //------io interrupt-------------------------------------------
 attachInterrupt(digitalPinToInterrupt(IN_PIN), io_ISR, RISING); 
 // or CHANGE, FALLING, RISING
 //------(try to) read sec between messages from eeprom---------
 int iResult;
 String sMyData=client.eepromReadMyData(iResult);
 if(iResult>0)
 {
  int len1=sMyData.length();
  if(len1>3)
  {
   if(sMyData.substring(0,3)=="TS=")
   {
    String s1=sMyData.substring(3,8);
    int iSec=s1.toInt();
    if(DEBUG03) Serial.println("sec between messages from EEPROM: "+s1);
    secBetwMsgs=iSec;
   }
  }
 }
 //------prepare mqtt start-------------------------------------
 client.setLanguage('d');                   // German
 client.setTopicBaseDefault(TOPIC_BASE);    // default base
 client.begin();                            // setup objects
 client.setCallback(callback);              // mqtt receiver
 client.setTopics(TOPIC_GET,TOPIC_SET,TOPIC_SUB,TOPIC_PUB);
 //------connect to WiFi and MQTT broker------------------------
 if(DEBUG03) Serial.println("Waiting for a connection to WiFi and MQTT broker...");
 while(!client.connectWiFiMQTT()) yield();
 if(DEBUG03) Serial.println("setup(): topicBase="+client.getsTopicBase());
 showLine(3,String(isrPulseCounter));
 //------start timer: secs between MQTT messages----------------
 startMyTimerInterrupt();
 //------send sec value-----------------------------------------
 client.simpleMqttDo("get", "sec", "");
 if(DEBUG03) Serial.println("setup(): --Finished--");
}

bool bResetCounter=true;
unsigned long pulseCounterOld=-1;
long watt1Old=-1;

//_______LOOP___________________________________________________
void loop() {
 //======(1) make at the beginning of the loop ...==============
 //======(2) do, independent on the network, ...================
 if(iLedOnZsec>0)
 {//----------turn blue led on----------------------------------
  digitalWrite(BLUELED_PIN, BLUELED_ON);    // turn led on
  iLedOnZsec--;
  //..........just for debug....................................
  if(DEBUG03) {
   if((iLedOnZsec+1)==LED_ON_ZSEC) {
    Serial.print(" isrPulseCounter="); Serial.print(isrPulseCounter);
   }
  }
 } else
 {//----------turn blue led off---------------------------------
  digitalWrite(BLUELED_PIN, BLUELED_OFF);   // turn led off
 }
 
 //-----------screen: update pulse counter----------------------
 if(pulseCounterOld!=isrPulseCounter)
 {
  showLine(3,String(isrPulseCounter));
  pulseCounterOld=isrPulseCounter;
 }
 //-----------screen: update watt-------------------------------
 if(watt1Old!=isrWatt1) {
  showLine(2,String(isrWatt1));
  watt1Old=isrWatt1;
 }
 
 //----------send every 5min MQTT message----------------------
 if(isrSendMessage5)
 {
  isrSendMessage5=false;
  String sWatt=getsWatt5();
  showLine(2,sWatt);
  isrWatt1=(long)sWatt.toInt();
  client.simpleMqttDo("pub", "test/watt/ret/watt5", sWatt);
  client.simpleMqttDo("get", "watt", "");
  if(DEBUG03) Serial.println("Send message: "+sWatt);
  bResetCounter=true;
  refreshScreen();
 }
 //======(3) process mqtt actions===============================
 client.doLoop();                           // mqtt loop
 //======(4) do things after mqtt access========================
 if(bResetCounter) { //......do this AFTER client.doLoop() !....
  //..........prepair next 5min power measurement...............
  millisStart5=millis();
  bResetCounter=false;
  isrPulseCounter=0;
  showLine(3,String(isrPulseCounter));
 }
 //======(5) do at the end of the loop ...======================
 delay(100);
}
