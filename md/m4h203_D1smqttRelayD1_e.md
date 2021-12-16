Last modified: 2021-11-11   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1 mini: Switching a relay via MQTT - Version 2</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h203_D1smqttRelayD1.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Mission
This program for a D1 mini or ESP32 D1mini is used to switch a relay via MQTT, using the class `SimpleMqtt` as a base.   
### Assumptions
| Assumption for ...    | value           |   
| --------------------- | --------------- |   
| ... WLAN name         | `Raspi11`       |   
| ... WLAN password     | `12345678`      |   
| ... IP address        | `10.1.1.1`      |   
| ... Base-Topic        | `ci/lamp/1`     |   
| ... Relay pin         | D1 or GPIO 22   |   
   
### Function
After startup the blue LED is switched on and the D1 mini tries to connect to the set broker. If this succeeds, it switches off the blue LED and a start message is sent (e.g. `info/start/mqtt {"topicbase": "ci/lamp/1", "IP": "10.1.1.160"}`). The following messages are used to switch the relay:   
| topic                  | payload | result               |   
|------------------------|:-------:|----------------------|   
| `ci/lamp/1/set/lamp`   |    1    | lamp/relay on        |   
| `ci/lamp/1/set/lamp`   |    0    | lamp/relay off       |   
| `ci/lamp/1/set/lamp`   |   -1    | lamp/relay toggled   |   

### Possible extensions of the program:
* Status display of the WLAN and MQTT connection via (e.g. different colored) LEDs
* Check if the relay has really switched, e.g. by measuring the load current

## Required tools
* Hardware: D1 mini or ESP32 mini
* Hardware: Relay-Shield (control via pin D1=GPIO 22)
* Hardware: Laptop/PC with WLAN reception and installed Mosquitto software
* Software: Arduino development environment or Visual Studio Code with at least installed extensions PlatformIO IDE and C/C++

## Creating the program
Starting from the [smqtt_template1](https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h01_smqtt_template1/D1_m4h01_smqtt_template1.ino), the following changes or additions must be made:   
1. change the definitions for the base topic and the GET and SET topics:   
```   
#define VERSION_M4H02 "2021-11-11 D1_m4h02_smqtt_relayD1"
#define TOPIC_BASE "ci/lamp/1"
#define TOPIC_GET "?,help,version,ip,topicbase,eeprom,lamp"
#define TOPIC_SET "topicbase,eeprom,lamp"
```   
   
2. definitions for the blue LED and the relay:   
```   
//_____sensors, actors, global variables________________________
#if defined(ESP32) || defined(ESP32D1)
 #define RELAY_PIN 22 // D1=22
 #define BLUELED_PIN 2 // led pin D4=IO2
 #define BLUELED_ON 1 // ESP32 1
 #define BLUELED_OFF 0 // ESP32 0
#else
 #define RELAY_PIN D1 // D1=22
 #define BLUELED_PIN D4 // led pin D4=IO2
 #define BLUELED_ON 0 // D1mini 0
 #define BLUELED_OFF 1 // D1mini 1
#endif
 #define RELAY_ON 1 //
 #define RELAY_OFF 0 //
int ledVal_=BLUELED_ON; // pin value
int relayVal_=RELAY_OFF; // relay off
```   
   
3. addition in function `String simpleGet(String sPayload)` before line    
`return String(""); // wrong set command`   
   Reply to GET request `lamp`:   
```   
 //-------------------------------------------------------------
 if(sPayload=="lamp") {
  if(relayVal_==RELAY_ON) return String("on"); 
  else return String("off"); 
 }
```   

4. addition in the function `String simpleSet(String sTopic, String sPayload)` before the line    
`return String(""); // wrong set command`.   
   Processing SET request `lamp`:   
```   
 //-------------------------------------------------------------
 if(sTopic=="lamp") { // switch blue led?
  if(sPayload=="on" || sPayload=="1") relayVal_=RELAY_ON;
  else { // other command
   if(sPayload=="off" || sPayload=="0") relayVal_=RELAY_OFF;
   else {
    if(sPayload=="toggle" || sPayload=="-1")
      relayVal_=1-relayVal_; // toggle led
   }
  }
  digitalWrite(RELAY_PIN,relayVal_); // turn led on/off
  if(relayVal_==RELAY_ON) return String("on"); // return answer
  return String("off"); // return answer
 }
```   

5. addition in the setup function in the section "init pins":   
```   
 //-----init pins-----------------------------------------------
 pinMode(BLUELED_PIN, OUTPUT); // set pin to output
 digitalWrite(BLUELED_PIN,ledVal_); // turn led on
 pinMode(RELAY_PIN, OUTPUT); // set pin to output
 digitalWrite(RELAY_PIN,relayVal_); // turn relay off
```   

6. change the loop function so that the blue LED is turned on when the connection to the broker is lost:   
```   
//_____LOOP_____________________________________________________
void loop() {
 client.doLoop(); // mqtt loop
 if(client.isMQTTConnected()) ledVal_ = BLUELED_OFF;
 else ledVal_ = BLUELED_ON;
 digitalWrite(BLUELED_PIN,ledVal_); // turn led on/off
 delay(100);
}
```   

The complete listing can be found on [https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h02_smqtt_relayD1/src/D1_m4h02_smqtt_relayD1.ino](https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h02_smqtt_relayD1/src/D1_m4h02_smqtt_relayD1.ino).

## Testing the program
### Preparation
1. laptop/PC: create the project `D1_m4h02_smqtt_relayD1`   
[---> source code see https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h02_smqtt_relayD1](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h02_smqtt_relayD1)

2. laptop/PC: build and upload the program to the D1 mini.   

3. start RasPi with the Mosquitto-Broker (if this has not been done yet).   

4. laptop/PC: Connect to the WLAN network `RasPi11` (password `12345678`).   

5. laptop/PC: open a command window ("command prompt"): Enter   
`cmd.exe`
in the start menu.   
Change to the correct drive and Mosquitto directory:   
`c:`   
`cd /programs/mosquitto`.

6. laptop/PC: Allow the display of all messages:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
Use the switch `-h` to specify the IP address of the Raspi.   

7. laptop/PC: open a second command prompt window: Enter   
`cmd.exe`
in the start menu.   
Change to the correct drive and Mosquitto directory:   
`c:`   
`cd /programs/mosquitto`

### Performing the program test
1. D1 mini: press reset button   
   Wait until the blue LED goes out. Then   
   Display in the first prompt window:   
   `info/start/mqtt {"topicbase":"ci/lamp/1","IP":"10.1.1.169"}`   

2. switch on relay   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m 1`   
   _Input in window 1_:   
   `ci/lamp/1/set/lamp 1`   
   `ci/lamp/1/ret/lamp on`   
   The relay should be on now ;)   

3. switch off relay   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m 0`   
   _Input in window 1_:   
   `ci/lamp/1/set/lamp 0`   
   `ci/lamp/1/ret/lamp off`   
   The relay should be off now ;)   

4. switch relay   
   _Enter in window 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m -1`   
   _Input in window 1_:   
   `ci/lamp/1/set/lamp -1`   
   `ci/lamp/1/ret/lamp on`   
   The relay should be on now ;)   
