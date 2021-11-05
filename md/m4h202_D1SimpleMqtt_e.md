Last modified: 2021-08-30   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1mini: Class SimpleMqtt</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h202_D1SimpleMqtt.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Know the structure of Topics when using the `SimpleMqtt` class.
* To be able to use the class `SimpleMqtt` to process MQTT messages.

* Creating a program for the D1 mini or ESP32 mini to control the blue LED using SimpleMqtt.
* Testing the SimpleMqtt led program with the PC.

## Required tools
* Hardware: D1 mini or ESP32 mini
* Hardware: Laptop/PC with WLAN reception and installed Mosquitto software
* Software: Arduino development environment or Visual Studio Code with at least installed extensions PlatformIO IDE and C/C++

&nbsp;   
## The "SimpleMqtt" class
### General
The SimpleMqtt class is suitable for D1 mini (ESP8266) and ESP32 and extends the class 'PubSubClient' to make MQTT easy to use.   
The protocol MQTT is message oriented, where each message consists of Topic and Payload. Topics can be chosen arbitrarily and can be structured hierarchically similar to directories.   
To be able to process messages easily, the class `SimpleMqtt` defines the following topic structure:   
* Each system participant has its own __topicbase__ (`topicbase`) e.g. `led/1`.
* __Query-topics__ consist of the base-topic and the addition `/get`. Which value is queried is in the payload.   
Example: `mosquitto_pub -t led/1/get -m blueled`.
* __control-topics__ consist of the base-topic and the `/set/...` addition. A value to be processed is in the payload.   
Example: `mosquitto_pub -t led/1/set/blueled -m 1`.
* __response messages__ consist of the base topic and the `/ret/...` addition, where the return value is in the payload.   
Example: Topic `led/1/ret/blueled` e.g. with the payload `0` or `1`.
* Additionally, any topics can be received and sent.

### Advantages of using the `SimpleMqtt` class
Advantages of the 'SimpleMqtt' class are:   
* Assigning a separate name (base topic) for each D1 mini without having to customize the program. (The name is stored in the EEPROM).
* Definition of the get-/set-/sub- and pub-topics in one constant each (`TOPIC_GET`, `TOPIC_SET`, `TOPIC_SUB`, `TOPIC_PUB`)
* Automatic response to get requests with the payload   
  |     |     |   
  | --- | --- |   
  | help or ? | display implemented get/set/sub and pub topics |   
  | version | SimpleMqtt version or program version |   
  | ip | the IP of the system participant assigned by the RasPi |   
| simple storage of values in EEPROM (e.g. parameters for sensors)

## class diagram 'SimpleMqtt

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
