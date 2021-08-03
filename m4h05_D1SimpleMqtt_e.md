Last modified: 2021-07-31   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1mini: Class SimpleMqtt</h1>
<a href="readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h05_D1SimpleMqtt.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets





// The SimpleMqtt class is suitable for D1 mini (ESP8266)
// and ESP32 and extends the class PubSubClient 
// to make MQTT easy to use.
// * For this purpose a "base" topic (topicbase, default is
//   simplemqtt/default) is defined, which can be extended
//    by the following keywords:
// /get Request of the value specified in the payload
//      e.g. version query:
//      -t simplemqtt/default/get -m version
// /set Set a value e.g. 
//      -t simplemqtt/default/set/topicbase -m new_base
// * In response you get a MQTT message with the keyword /ret,
//    e.g. -t simplemqtt/default/ret/version -m 2020-12-08
// * Furthermore, all (arbitrary) topics defined in the constant
//   TOPIC_SUB are subscribed.
// * Furthermore, all commands of the PubSubClient class can 
//   still be used.
//
// In the user program the following things must be implemented:
// [1] Define the type of board (1=D1mini, 2=D1_ESP32)
//     #define  D1MINI         1            // ESP8266 D1mini +pro
// [2] Define the topic base
//     #define TOPIC_BASE "simplemqtt/default"
// [3] Define the topics the program should respond to 
//     for set/get/sub and pub requests
//     (key words comma separated or empty string) e.g.
//     #define TOPIC_GET  "help,version,ip,topicbase,eeprom,led"
//     #define TOPIC_SET  "topicbase,eeprom,led"
//     #define TOPIC_SUB  ""
//     #define TOPIC_PUB  ""
// [4] Create an object that will be used in the user program
//     SimpleMqtt client("ssid", "password", "mqttservername|ip");
// [5] Create a callback routine for incoming messages
//     void callback(char* topic, byte* payload, unsigned int length)
//     { client.callback_(topic, payload, length); }
// [6] Create functions to answer incomming MQTT requests
//     String simpleGet(String sPayload) { return String(""); }
//     String simpleSet(String sTopic, String sPayload) { return String(""); }
//     void   simpleSub(String sTopic, String sPayload) { }
// [7] Call method "doLoop()" in the main loop:
//     client.doLoop();
//     It MUST be called, otherwise no MQTT messages are processed!