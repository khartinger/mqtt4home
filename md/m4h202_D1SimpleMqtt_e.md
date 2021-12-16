Last modified: 2021-11-12   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1mini: Class SimpleMqtt</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h202_D1SimpleMqtt.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Introduction
A protocol very well suited for home automation is MQTT because it is flexible and easy to use. It is message-oriented, with each message consisting of topic and payload. Topics can be chosen arbitrarily and can be structured hierarchically similar to directories.   
   
In order for a D1 mini (or ESP32 D1mini) to send or receive MQTT messages, an MQTT library is required. In these examples, [`PubSubClient`](https://github.com/knolleary/pubsubclient) by Nick O'Leary is used as the base.   
   
The class `SimpleMqtt` extends `PubSubClient` with some functionalities to facilitate the creation of D1 mini systems for home automation.   

## Agreements for using `SimpleMqtt`
To be able to process messages easily, the class `SimpleMqtt` defines the following topic structure:   
* __Base-Topic__: each system participant has its own "name" or topic it "listens" to, the "base-topic" (`topicbase`) e.g. `led/1` or `led/2`.
* __Query-Topics__ are used to query information and consist of the topicbase with the addition `/get`. Which value is queried is in the payload.   
Example: `mosquitto_pub -t led/1/get -m blueled`
* __control-topics__ set parameters of a system participant and consist of the topicbase with the addition `/set/...`. The value to be processed is in the payload.   
Example: `mosquitto_pub -t led/1/set/blueled -m 1`
* __response messages__ contain the result of queries or control commands and consist of the topicbase with the addition `/ret/...`. The return value is in the payload.   
Example: Topic `led/1/ret/blueled` e.g. with the payload `0` or `1`.
   
In addition to these agreements, any other topics can be received and sent.   
   
## Advantages of using the "SimpleMqtt" class
Two features have proven to be particularly handy in practice:   
1. sending a startup message when the D1 mini boots.   
2. possibility to change the base topic via an MQTT message.   

This means: If you don't know the base topic of a system (e.g. button etc.), you just have to reset it and look at the MQTT messages to see which start topic was sent ;)
### Further advantages of the class `SimpleMqtt` are:   
* Assigning arbitrary names (base topic) for each D1 mini without having to customize the program. (The name is stored in the EEPROM).
* Definition of get-/set-/sub- and pub-topics in one constant each (`TOPIC_GET`, `TOPIC_SET`, `TOPIC_SUB`, `TOPIC_PUB`)
* Automatic response to get requests:   
  | payload | response message |   
  | ------- | ----------------- |   
  | help or ? | list of possible get/set/sub and pub topics |   
  | version | SimpleMqtt version or program version |   
  | ip | the IP of the system participant assigned by the RasPi |   
* simple storage of values in EEPROM (e.g. parameters for sensors)
   
## Program structure
D1 mini programs require the following project structure:   
  | Directory name | Contents (files) |   
  | --- | --- |   
  | xxx | xxx.ino <br>eventually README.md |   
  | xxx/src/simplemqtt | `D1_class_SimpleMqtt.cpp`, `D1_class_SimpleMqtt.h`,<br> `PubSubClient.cpp`, `PubSubClient.h` |   
  
_Example for Arduino IDE_:   
[https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1)   
_Example for Visual Studion Code_:   
[https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1_vsc](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1_vsc)   

The following listing `D1_m4h01_smqtt_template1.ino` can be used as a basis for your own programs:   
```   
//_____D1_m4h01_smqtt_template1.ino______________khartinger_____
// Template to create a MQTT application using SimpleMqtt
// Function of this program:
// [1] Connect to given WiFi and MQTT broker
// [2] Use "topic base" stored in eeprom or topic "test/smqtt"
// [3] Automatic (build in) answers for messages 
//     -t test/smqtt/get -m help
//     -t test/smqtt/get -m version
//     -t test/smqtt/get -m ip
// [4] Answer the following requests with a topic like
//     test/smqtt/ret/...:
//     -t test/smqtt/get -m topicbase
//     -t test/smqtt/get -m eeprom
// [5] Change topic base (e.g. to test/newtopic)
//     -t test/smqtt/set/topicbase -m test/newtopic
// [6] Erase topic base stored in EEPROM by
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
 //if(DEBUG75)
 {Serial.begin(115200); Serial.println("\nsetup(): --Start--");}
 //-----init pins-----------------------------------------------
 //-----prepare mqtt start--------------------------------------
 // Use setTopicBaseDefault() (or constructor) instead of 
 // setTopicBase(), to use topic base from eeprom on restart ;)
 client.setTopicBaseDefault(TOPIC_BASE);    // default base
 client.begin();                            // setup objects
 client.setCallback(callback);              // mqtt receiver
 client.setTopics(TOPIC_GET,TOPIC_SET,TOPIC_SUB,TOPIC_PUB);
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
```   

## Test of the template
### Required tools
* Hardware: D1 mini or ESP32 mini
* Hardware: Laptop/PC with WLAN reception and Mosquitto software installed
* Software: Arduino development environment or Visual Studio Code with at least PlatformIO IDE and C/C++ extensions installed

### Preparation
1. laptop/PC: create project `D1_m4h01_smqtt_template1`   
[---> source code see https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1)   

2. laptop/PC: build and upload the program to the D1 mini.   

3. start RasPi with the Mosquitto-Broker (if not already done).   

4. laptop/PC: Connect to the WLAN network `RasPi11` (password `12345678`).   

5. laptop/PC: open a command window ("command prompt"):   
`cmd.exe`
in the start menu.   
Change to the correct drive and Mosquitto directory:   
`c:`   
`cd /programs/mosquitto`.

6. laptop/PC: Allow the display of all messages:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
Use the switch `-h` to specify the IP address of the Raspi.   

7. laptop/PC: open a second command prompt window:   
`cmd.exe`
in the start menu.   
Change to the correct drive and Mosquitto directory:   
`c:`   
`cd /programs/mosquitto`

### Performing the template test
1. D1 mini: Press reset button   
   Display in first prompt window:   
   `info/start/mqtt {"topicbase": "test/smqtt", "IP": "10.1.1.160"}`   

2. query help   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m ?`   
   _display in window 1_:   
   `test/smqtt/get help`   
   `test/smqtt/ret/help`   
   `get: ?|help|version|ip|topicbase|eeprom|`   
   `set: topicbase|eeprom|`   
   `sub:`   

3. query version   
   _enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m version`   
   _display in window 1_:   
   `test/smqtt/get version`   
   `test/smqtt/ret/version 2021-11-11 D1_m4h01_smqtt_template1`   

4. query IP address of D1 mini   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m ip`   
   _display in window 1_:   
   `test/smqtt/get ip`   
   `test/smqtt/ret/ip 10.1.1.160`   

5. query base-topic of D1 mini   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m topicbase`   
   _display in window 1_:   
   `test/smqtt/get topicbase`   
   `test/smqtt/ret/topicbase test/smqtt`   

6. query EEPROM content of the D1 mini   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m eeprom`   
   _display in window 1_:   
   `test/smqtt/get eeprom`   
   `test/smqtt/ret/eeprom Error: No topic base stored`   

7. change base topic to `test/newtopic`   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/set/topicbase -m test/newtopic`   
   _display in window 1_:   
   `test/smqtt/set/topicbase test/newtopic`   
   `test/newtopic/ret/topicbase test/newtopic`   
   Note: The new topicbase will be used immediately.

8. D1 mini: Press reset button   
   _Display in window 1_:   
   `info/start/mqtt {"topicbase": "test/newtopic", "IP": "10.1.1.160"}`   

9. query version   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/newtopic/get -m version`.   
   Note: Use the changed topicbase ;)   
   _display in window 1_:   
   `test/newtopic/get version`   
   `test/newtopic/ret/version 2021-11-11 D1_m4h01_smqtt_template1`   

10. delete EEPROM content of D1 mini   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/newtopic/set/eeprom -m 0`   
   _display in window 1_:   
   `test/newtopic/set/eeprom 0`   
   `test/newtopic/ret/eeprom erased`   
   Note: The topicbase stored in the EEPROM has been erased, but the old topicbase is still valid until the D1 mini is restarted!

---   

## Class diagram 'SimpleMqtt

| class SimpleMqtt | extends PubSubClient |
| ---------------- | -------------------- |
| + SimpleMqtt() | set default values: |
| + SimpleMqtt(char* ssid, char* pwd) | |
| + SimpleMqtt(char* ssid, char* pwd, char* mqtt_server, char* topicbase) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server, String topicbase) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server, String topicbase, String clientname) | |
| ~ void setup() | Set properties. Call by constructor.             |
| ~ bool begin(int iSource) | Set more properties. Call by constructor.     |
| + bool begin() | Initialize objects, read topicbase from EEPROM. |

&nbsp;

| General setter and getter methods      |     |
| -------------------------------------- | --- |
| + void setLanguage(char language) | Set language (default e=English, d=German |
| + void setEepromSize(int eepromSize) | Set the required EEPROM size (maximum 4096) |
| + void setWiFiWaitingTime(int ms) | Maximum waiting time in milliseconds to connect to WiFi (min. 100ms). |
| + void setWiFiConnectingCounter(int number) | Maximum number of connectingWiFi() calls after calling connectingWiFiBegin() |
| + void setMQTTClientName(String sName) | Set MQTT client name |
| + void allowMQTTStartInfo(bool allow) | Allow sending the startup information of this D1 mini. |
| + String getMQTTClientName() | Get MQTT client name |
| + String getsState() | Get MQTT client state as string |
| + string getsSSID() | Get WiFi network name as string |
| + virtual String getsLocalIP() | Get client IP and return it as string |
| + String getsMyIP() | Get the stored client IP address as string |
| + String getsMac() | Get the MAC address of the D1 mini as string |
| + String getsTopicBase() | Get the base topic |

&nbsp;

| Methods for the MQTT Topics |     |
| --------------------------- | --- |
| + int setTopicBase(String topicBase) | Set base topic in RAM and EEPROM WITHOUT subscribing to messages |   
| + int setTopicBaseDefault(String topicBaseDefault) | Set default value for base topic |   
| + void setTopics(String sAllGet, String sAllSet, String sAllSub, String sAllPub) | Set all (comma-separated) topics |   
| + int setTopicGet(String sAllGet) | Set all (comma-separated) get-topics |   
| + int setTopicSet(String sAllSet) | Set all (comma-separated) set-topics |   
| + int setTopicSub(String sAllSub) | Set all (comma-separated) sub-Topics |   
| + int setTopicPub(String sAllPub) | Set all (comma-separated) pub-topics |   
| + int setTopicGet(String sAllGet, String sAllRetainedGet) | Set all (comma-separated) get-topics plus comma-separated retain string (0=false, 1=true) |   
| + int setTopicSet(String sAllSet, String sAllRetainedSet) | Set all (comma separated) set-topics plus comma separated retain string (0=false, 1=true) |   
| + int setTopicPub(String sAllPub, String sAllRetainedPub) | Set all (comma separated) pub-topics plus comma separated retain string (0=false, 1=true) |   
| + boolean setRetainedIndex(String sType, int index, boolean bRetained) | Set the retain flag for the element with the given index in a..Get, a..Set or a..Pub |   
| + String getsRetainedAll() | Set all retain flags |   
