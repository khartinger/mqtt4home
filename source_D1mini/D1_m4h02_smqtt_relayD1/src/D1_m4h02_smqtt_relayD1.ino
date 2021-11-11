//_____D1_m4h02_smqtt_relayD1.ino________________khartinger_____
// This program for a D1 mini or ESP32 D1mini is used to switch 
// a relay via MQTT, using the class `SimpleMqtt` as a base.
// Function of this program:
// [1] Connect to given WiFi and MQTT broker
// [2] Send a start message
// [3] Use "topic base" stored in eeprom or topic TOPIC_BASE
// [4] Answer all get requests in TOPIC_GET
// [5] Do all set requests in TOPIC_SET and answer them, e.g.
//     toggle lamp: -t ci/lamp/1/set/lamp -m -1
// Class SimpleMqtt extends class PubSubClient for easy use.
// All commands of the PubSubClient class can still be used.
// Note: When PubSubClient lib is installed,
//       delete PubSubClient files in directory src/simplemqtt
// Hardware: 
// (1) WeMos D1 mini (OR ESP32 D1 mini)
// (2) Relay shield (@D1)
//
// Important: Example needs a MQTT-broker!
// Created by Karl Hartinger, November 11, 2021.
// Changes:
// 2021-11-11 New
// Released into the public domain.

#define D1MINI          1              // ESP8266 D1mini +pro
//#define ESP32D1         2              // ESP32 D1mini
#include "src/simplemqtt/D1_class_SimpleMqtt.h"

#define  VERSION_M4H02  "2021-11-11 D1_m4h02_smqtt_relayD1"
#define  TOPIC_BASE     "ci/lamp/1"
#define  TOPIC_GET      "?,help,version,ip,topicbase,eeprom,lamp"
#define  TOPIC_SET      "topicbase,eeprom,lamp"
#define  TOPIC_SUB      ""
#define  TOPIC_PUB      ""

//_____sensors, actors, global variables________________________
#if defined(ESP32) || defined(ESP32D1)
 #define RELAY_PIN      22                  // D1=22
 #define BLUELED_PIN    2                   // led pin D4=IO2
 #define BLUELED_ON     1                   // ESP32 1
 #define BLUELED_OFF    0                   // ESP32 0
#else
 #define RELAY_PIN      D1                  // D1=22
 #define BLUELED_PIN    D4                  // led pin D4=IO2
 #define BLUELED_ON     0                   // D1mini 0
 #define BLUELED_OFF    1                   // D1mini 1
#endif
 #define RELAY_ON       1                   //
 #define RELAY_OFF      0                   //
int      ledVal_=BLUELED_ON;                // pin value
int      relayVal_=RELAY_OFF;               // relay off

//_____MQTT communication_______________________________________
//SimpleMqtt client("..ssid..", "..password..","mqttservername");
SimpleMqtt client(String("Raspi11"),String("12345678"),String("10.1.1.1"),String(TOPIC_BASE));

//_____MQTT: inspect all subscribed incoming messages___________
void callback(char* topic, byte* payload, unsigned int length)
{
 client.callback_(topic, payload, length);  // must be called!
}

//_____answer get requests______________________________________
// sPayload: payload to message from TOPIC_GET
// auto answer: for help (+), version, ip (can be overwritten)
// return: ret answer payload for get request
String simpleGet(String sPayload)
{
 if(sPayload=="version") return String(VERSION_M4H02);
 //-------------------------------------------------------------
 if(sPayload=="topicbase") return client.getsTopicBase();
 //-------------------------------------------------------------
 if(sPayload=="eeprom") {
  int iResult;
  String s1=client.eepromReadTopicBase(iResult);
  if(iResult==-2) return String("Error: No topic base stored");
  if(iResult<0) return String("Error # ")+String(iResult);
  return s1;
 }
 //-------------------------------------------------------------
 if(sPayload=="lamp") {
  if(relayVal_==RELAY_ON) return String("on"); 
  else return String("off"); 
 }
 //-------------------------------------------------------------
 return String("");                         // wrong Get command
}

//_____execute set requests_____________________________________
// sTopic from TOPIC_SET, sPayload: payload to topic
// return: ret answer payload for set command
String simpleSet(String sTopic, String sPayload)
{
 //-------------------------------------------------------------
 if(sTopic=="topicbase") {                  // new topic base?
  client.changeTopicBase(sPayload);         // change base
  return client.getsTopicBase();            // return new base
 }
 //-------------------------------------------------------------
 if(sTopic=="eeprom") {                     // erase eeprom?
  if(sPayload=="0" || sPayload=="erase") {  // payload OK?
  if(client.eepromEraseTopicBase()) return "erased";
  }
  return "not erased";                      // return answer
 }
 //-------------------------------------------------------------
 if(sTopic=="lamp") {                       // switch blue led?
  if(sPayload=="on" || sPayload=="1") relayVal_=RELAY_ON;
  else {                                    // other command
   if(sPayload=="off" || sPayload=="0") relayVal_=RELAY_OFF;
   else {
    if(sPayload=="toggle" || sPayload=="-1")
      relayVal_=1-relayVal_;                // toggle led
   }
  }
  digitalWrite(RELAY_PIN,relayVal_);         // turn led on/off
  if(relayVal_==RELAY_ON) return String("on"); // return answer
  return String("off");                     // return answer
 }
 //-------------------------------------------------------------
 return String("");                         // wrong set command
}

//_____execute sub requests_____________________________________
// sTopic from TOPIC_SUB, sPayload: payload to topic
// return: no automatic answer
void simpleSub(String sTopic, String sPayload)
{
}

//_____SETUP____________________________________________________
void setup() {
  //-----Serial, just for debug----------------------------------
 {Serial.begin(115200); Serial.println("\nsetup(): --Start--");}
 //-----init pins-----------------------------------------------
 pinMode(BLUELED_PIN, OUTPUT);              // set pin to output
 digitalWrite(BLUELED_PIN,ledVal_);         // turn led on
 pinMode(RELAY_PIN, OUTPUT);                // set pin to output
 digitalWrite(RELAY_PIN,relayVal_);         // turn relay off
 //-----prepare mqtt start--------------------------------------
 client.setTopicBaseDefault(TOPIC_BASE);    // default base
 client.begin();                            // setup objects
 client.setCallback(callback);              // mqtt receiver
 client.setTopics(TOPIC_GET,TOPIC_SET,TOPIC_SUB,TOPIC_PUB);
 //-----connect to WiFi and MQTT broker-------------------------
 Serial.println("Waiting for aconnection to WiFi and MQTT broker...");
 while(!client.connectWiFiMQTT()) yield();
 Serial.println("setup(): topicBase="+client.getsTopicBase());
 Serial.println("setup(): --Finished--");
}

//_____LOOP_____________________________________________________
void loop() {
 client.doLoop();                           // mqtt loop
 if(client.isMQTTConnected()) ledVal_ = BLUELED_OFF;
 else ledVal_ = BLUELED_ON;
 digitalWrite(BLUELED_PIN,ledVal_);         // turn led on/off
 delay(100);
}
