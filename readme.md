Last modified: 2021-09-03
<table><tr><td><img src="md/logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>MQTT for domestic/home use :)</h1>
<a href="liesmich.md">==> German version</a>&nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Welcome to mqtt4home

__*How can I connect sensors and actuators at home?*__   
If you're asking yourself this question, you've come to the right place. In five different sections 
 questions about "home automation" are answered in many examples:   
1. area "__Central__": How do I set up a Raspberry Pi (RasPi for short) so that it works as a control center respectively MQTT broker?   
2. area "__D1mini__": How can I use D1mini or ESP32mini microcontrollers as control elements in the system?   
3. area "__auxiliary programs__": How do I create stand-alone utility programs that perform specific tasks?   
4. area "__Zigbee__": How can I incorporate ZigBee components into my system?   
5. area "__Visualization__": How can I operate components via a graphical user interface?   

To limit the topic, this program collection defines a WLAN with TCP-IP and MQTT protocol as the connection medium.   
Information about the MQTT protocol can be found at [https://mqtt.org/](https://mqtt.org/).   

## Prerequisites

For working on the very broad topic "Homeautomation" some basic knowledge is advantageous:   
* Some basic knowledge of the Raspberry Pi and the Linux operating system (for the __Central__ and __ZigBee__ areas).   
* Basic knowledge in C/C++ (for the areas __D1mini__ and __auxiliary programs__)   
* Basic knowledge of JavaScript or Node.js (for the __visualization__ area).   

The examples are often step-by-step instructions that should be easy to follow.   
The hardware and software required for the examples is listed with the respective explanations.   

## General Assumptions

* Sensors and actuators are connected to the control center (RasPi) via radio (WiFi or ZigBee).
* The WiFi network is provided by the RasPi.
* If control via the Internet is desired, Internet access must be provided separately. (Router or Alcatel Linkhub HH71VM with HOT SIM card etc.).

## Required hardware

* Raspberry Pi with accessories (power supply, monitor, keyboard etc.),   
e.g. starter kit from [Reichelt (incl. 7" touch display)](https://www.reichelt.at/at/de/raspberry-pi-4-b-4gb-inkl-7-touch-display-gehaeuse-rpi4-bdl-4gb-7td-p291393.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCSgJ1AiDo2X72WmgAzl4nAxSPFg4yIhT37OJYww45nqSir0qp0gd-waAjhcEALw_wcB) or [Amazon (without display)](https://www.amazon.de/Raspberry-SD-Karte-Ultimatives-Quad-Core-unterst%C3%BCtzt/dp/B082PSBBMM/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=raspberry+pi+4+set&qid=1627196094&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFXQ0QzNDU2UU9QVTYmZW5jcnlwdGVkSWQ9QTA1ODg3NDgyTUlMWFpXWjNCWTZJJmVuY3J5cHRlZEFkSWQ9QTA1MDMyMjUzMElTVENaSEJHWTNLJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==).   

* D1 mini e.g. from [Reichelt](https://www.reichelt.at/at/de/d1-mini-esp8266-v2-0-d1-mini-p253978.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCQ36slwX3O8fPPdBLJoEg1YwBGT9tDJQfl-IjdhpwqzUj0-DTPbe8saAswVEALw_wcB) or [Amazon](https://www.amazon.de/AZDelivery-D1-Mini-ESP8266-12E-kompatibel/dp/B01N9RXGHY/ref=asc_df_B01N9RXGHY/?tag=googshopde-21&linkCode=df0&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456&psc=1&th=1&psc=1&tag=&ref=&adgrpid=65257070361&hvpone=&hvptwo=&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456).   

* Further required tools, such as ZigBee stick, ZigBee sensors, GSM modem or software etc., are described with the respective examples.   

---   

## Content
### Area "Central":
1. [Install Raspberry Pi (2021-07-31)](md/m4h01_RasPiInstall_e.md)
2. [Raspberry Pi as access point (2021-07-31)](md/m4h02_RasPiAccessPoint_e.md)
3. [Raspberry Pi as MQTT broker (2021-07-31)](md/m4h03_RasPiMQTTBroker_e.md)

---   
### Area "D1mini"
4. [D1mini: MQTT Relay with PubSubClient (2021-07-31)](hd/m4h04_D1mqttRelayD1_e.md)
5. [D1mini: The SimpleMqtt class (2021-07-31)](md/m4h05_D1SimpleMqtt_e.md)

---   
### Area "auxiliary programs"
6. [RasPi: Template for MQTT programs in C++ (2021-08-21)](md/m4h07_RasPiCppBase_e.md)
7. [RasPi: Helpful single programs in C++ (2021-08-21)](md/m4h08_RasPiCppDemos_e.md)
8. [RasPi: Create your own programs in C++ (2021-08-21)](md/m4h09_RasPiCppCreatingYourOwnProgs_e.md)
9. [RasPi: Sending and receiving SMS via MQTT in C++ (2021-08-27)](md/m4h10_RasPiCppSms_e.md)

### Area "Zigbee"

### Area "Visualization"
