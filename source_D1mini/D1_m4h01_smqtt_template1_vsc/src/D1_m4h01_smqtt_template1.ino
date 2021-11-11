//_____D1_m4h01_smqtt_template1.ino______________khartinger_____
// Template to create a MQTT application using SimpleMqtt
// Function of this program:
// [1] Connect to given WiFi and MQTT broker
// [2] Automatically send an MQTT message message at startup
// [3] Use "topic base" stored in eeprom or topic "test/smqtt"
// [4] Automatic (build in) answers for messages 
//     -t test/smqtt/get -m help
//     -t test/smqtt/get -m version
//     -t test/smqtt/get -m ip
// [5] Answer the following requests with a topic like
//     test/smqtt/ret/...:
//     -t test/smqtt/get -m topicbase
//     -t test/smqtt/get -m eeprom
// [6] Change topic base (e.g. to test/newtopic)
//     -t test/smqtt/set/topicbase -m test/newtopic
// [7] Erase topic base stored in EEPROM by
//     -t test/smqtt/set/eeprom -m erase (or -m 0)
//     On next start topic base is set to default value test/smqtt
// Class SimpleMqtt extends class PubSubClient for easy use.
// All commands of the PubSubClient class can still be used.
// Note: When PubSubClient lib is installed,
//       delete PubSubClient files in directory src/simplemqtt
// Hardware: 
// (1) WeMos D1 mini (OR ESP32 D1 mini)
//
// Important: Example needs a MQTT-broker!
// Created by Karl Hartinger, November 11, 2021.
// Changes:
// 2021-11-11 New
// Released into the public domain.
#define D1MINI          1              // ESP8266 D1mini +pro
//#define ESP32D1         2              // ESP32 D1mini
#include "src/simplemqtt/D1_class_SimpleMqtt.h"

#define  VERSION_M4H01  "2021-11-11 D1_m4h01_smqtt_template1"
#define  TOPIC_BASE     "test/smqtt"
#define  TOPIC_GET      "?,help,version,ip,topicbase,eeprom"
#define  TOPIC_SET      "topicbase,eeprom"
#define  TOPIC_SUB      ""
#define  TOPIC_PUB      ""

//_____sensors, actors, global variables________________________
#if defined(ESP32) || defined(ESP32D1)
#else
#endif

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
 if(sPayload=="version") return String(VERSION_M4H01);
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
  return "not erased";                 // return answer
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
 //-----prepare mqtt start--------------------------------------
 client.setTopicBaseDefault(TOPIC_BASE);    // default base
 client.begin();                            // setup objects
 client.setCallback(callback);              // mqtt receiver
 client.setTopics(TOPIC_GET, TOPIC_SET, TOPIC_SUB, TOPIC_PUB);
 //-----connect to WiFi and MQTT broker-------------------------
 Serial.println("Waiting for connection to WiFi and MQTT broker...");
 while(!client.connectWiFiMQTT()) yield();
 Serial.println("setup(): topicBase="+client.getsTopicBase());
 Serial.println("setup(): --Finished--");
}

//_____LOOP_____________________________________________________
void loop() {
 client.doLoop();                           // mqtt loop
 delay(100);
}
