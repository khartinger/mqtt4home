Last modified: 2021-11-11   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>D1 mini: Switching a relay via MQTT - Version 2</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h203_D1smqttRelayD1.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Mission
This program for a D1 mini or ESP32 D1mini is used to switch a relay via MQTT, using the class 'SimpleMqtt' as a base.   
### Assumptions
| Assumption for ...    | value           |   
| --------------------- | --------------- |   
| ... WLAN name         | `Raspi11`       |   
| ... WLAN password     | `12345678`      |   
| ... IP address        | `10.1.1.1`      |   
| ... Base-Topic        | `ci/lamp/1`     |   
| ... Relay pin         | D1 or GPIO 22   |   
   
### Function
After startup the blue LED is switched on and the D1 mini tries to connect to the set broker. If this succeeds, it switches off the blue LED and a start message is sent (e.g. `info/start/mqtt {"topicbase": "test/smqtt", "IP": "10.1.1.160"}`). The following messages are used to switch the relay:   
| topic                  | payload | result               |   
|------------------------|---------|----------------------|   
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
