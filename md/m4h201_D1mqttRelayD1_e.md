Last modified: 2021-11-05   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1mini: MQTT Relay with PubSubClient</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h201_D1mqttRelayD1.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Get to know the class [`pubsubclient`](https://github.com/knolleary/pubsubclient) for processing MQTT messages with the D1 mini or ESP32 mini.
* Creating a program for the D1 mini or ESP32 mini to control a relay(-shield) via MQTT.
* Testing of the MQTT relay with the PC.

## Required tools
* Hardware: D1 mini or ESP32 mini
* Hardware: Relay-Shield (control via pin D1=GPIO 22)
* Hardware: Laptop/PC with WLAN reception and installed Mosquitto software
* Software: Arduino development environment or Visual Studio Code with at least installed extensions PlatformIO IDE and C/C++

&nbsp;   
## MQTT messages and the "pubsubclient" class
The processing of MQTT messages is divided into four steps:   
1. setting the network parameters
2. connecting to the MQTT broker
3. receiving messages ("subscribe")
4. sending messages ("publish")

### ad 1. setting the network parameters
* The following parameters are already set during [Setting up the RasPi as an access point](m4h02_RasPiAccessPoint.md):   

|                 |                 |   
| --------------- | --------------- |   
| WLAN name:      | `Raspi11`       |   
| WLAN password:  | `12345678`      |   
| IP address:     | `10.1.1.1`      |   
Additionally, a client name must be specified, e.g. `D1mini_Relay1`.   

* In the program code, for example, this looks like this:   

```
const char* ssid = "Raspi11";
const char* password = "12345678";
const char* mqtt_server = "10.1.1.1";
String clientId=String("D1mini_Relay1");

WiFiClient espClient;
PubSubClient client(espClient);
```

### ad 2. Connecting to the network and the MQTT broker
* At least the following (blocking) code is required to connect to the network:   
```
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) { delay(200); }
```
* If the connection to the network was successfully established, the server IP, port and name of the callback routine must be set (in the `setup()` function):   
```
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
```
* The connection to the broker as well as the subscription ("subscribe") of messages is done by the following commands: 
```
 if(client.connect(clientId.c_str())) { //...connected...
  client.subscribe("#");
 }
```
Note: With `"#"` all MQTT messages are subscribed.   

* The check whether the D1 mini is (still) connected to the broker is done by the following command:
```
if (client.connected()) {
//...
}
```

### ad 3. receiving messages
Two points are required for receiving messages:   
1. In the `loop()` function, the `client.loop()` method MUST be called regularly:
```
 if (client.connected()) {
  client.loop();                            // must be called!
 }
```
2. The actual processing of all subscribed messages is done in the function `callback`:
```
void callback(char* topic, byte* payload, unsigned int length) {
//...
}
```
Since the payload can contain any character (including the 0 character!), the length of the payload is also specified.

### ad 4. sending messages
Sending messages is done with one of the methods `publish()`, which must contain at least the send topic and the payload ("message") as parameters. Furthermore you can specify if the broker should remember the message (3rd parameter = retain = set to `true`).   
&nbsp;

## Creating the "MQTT-Relay" program
### Functions of the program in detail
1. Attempt to establish connection to the given WiFi and MQTT broker.
2. Turning on the blue LED when the connection is established.
3. Subscribe to the topic `relay/1/set` with the possible contents ( payload ) 0, 1 or -1.
4. If the topic `relay/1/set` is received,
   * the relay is switched according to the payload: 0=off, 1=on, -1=um.
   * a message with the topic `relay/1/ret` and the payload is sent according to the new status of the relay (0=off, 1=on, -1=um).
5. If the connection to the broker is interrupted, the D1 mini will try to reconnect it after a waiting time of 4 seconds (`MQTT_RECONNECT_MS`).

### Coding

The full coding can be found on GitHub at.   
[https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1](https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1)   

&nbsp;

## Test of the MQTT relay.
1. D1 mini: Upload the program [`D1_Ex60_mqtt_relayD1`](https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1).   

2. Start RasPi with the Mosquitto broker (if this has not already been done).   

3. Laptop/PC: connect to the WLAN network `Raspi11` (password `12345678`).   

4. Laptop/PC: open a command window ("command prompt"):   
```cmd.exe```
in the start menu.   
Change to the correct drive and Mosquitto directory:   
```c:```   
```cd /programs/mosquitto```

5. Laptop/PC: Allow the display of all messages:   
```mosquitto_sub -h 10.1.1.1 -t "#" -v```   
Use the switch `-h` to specify the IP address of the Raspi.   

6. Laptop/PC: Open a second command prompt window:   
```cmd.exe```
in the start menu.   
Change to the correct drive and Mosquitto directory:   
```c:```   
```cd /programs/mosquitto```

7. Send the relay switching command from the second prompt window:    
```mosquitto_pub -h 10.1.1.1 -t relay/1/set -m -1```   
In the first prompt window, the following two lines appear:   
```relay/1/set -1```   
```relay/1/ret 1```   
In addition, the relay should have switched (relay LED is lit).   
