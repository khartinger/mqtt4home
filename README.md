Last modified: 2021-12-20 <a name="up"></a>   
<table><tr><td><img src="md/logo/mqtt4home_96.png"></td><td>
<h1>MQTT for domestic/home use :)</h1>
<a href="LIESMICH.md">==> German version</a>&nbsp; &nbsp; &nbsp; 
</td></tr></table>   
<a href="#content1">Directly to the content overview</a>   

---   

# Welcome to mqtt4home

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

# Prerequisites

For working on the very broad topic "Homeautomation" some basic knowledge is advantageous:   
* Some basic knowledge of the Raspberry Pi and the Linux operating system (for the __Central__ and __ZigBee__ areas).   
* Basic knowledge in C/C++ (for the areas __D1mini__ and __auxiliary programs__).   
* Basic knowledge of HTML, CSS, JavaScript or Node.js (for the __visualization__ area).   
* Basic knowledge of electronics and soldering (for the __D1mini__ area).   

The examples are often step-by-step instructions that should be easy to follow.   
The hardware and software required for the examples is listed with the respective explanations.   

# General Assumptions

* Sensors and actuators are connected to the control center (RasPi) via radio (WiFi or ZigBee).
* The WiFi network is provided by the RasPi.
* If control via the Internet is desired, Internet access must be provided separately. (Router or Alcatel Linkhub HH71VM with HOT SIM card etc.).

# Required hardware

* Raspberry Pi with accessories (power supply, monitor, keyboard etc.),   
e.g. starter kit from [Reichelt (incl. 7" touch display)](https://www.reichelt.at/at/de/raspberry-pi-4-b-4gb-inkl-7-touch-display-gehaeuse-rpi4-bdl-4gb-7td-p291393.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCSgJ1AiDo2X72WmgAzl4nAxSPFg4yIhT37OJYww45nqSir0qp0gd-waAjhcEALw_wcB) or [Amazon (without display)](https://www.amazon.de/Raspberry-SD-Karte-Ultimatives-Quad-Core-unterst%C3%BCtzt/dp/B082PSBBMM/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=raspberry+pi+4+set&qid=1627196094&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFXQ0QzNDU2UU9QVTYmZW5jcnlwdGVkSWQ9QTA1ODg3NDgyTUlMWFpXWjNCWTZJJmVuY3J5cHRlZEFkSWQ9QTA1MDMyMjUzMElTVENaSEJHWTNLJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==).   

* D1 mini e.g. from [Reichelt](https://www.reichelt.at/at/de/d1-mini-esp8266-v2-0-d1-mini-p253978.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCQ36slwX3O8fPPdBLJoEg1YwBGT9tDJQfl-IjdhpwqzUj0-DTPbe8saAswVEALw_wcB) or [Amazon](https://www.amazon.de/AZDelivery-D1-Mini-ESP8266-12E-kompatibel/dp/B01N9RXGHY/ref=asc_df_B01N9RXGHY/?tag=googshopde-21&linkCode=df0&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456&psc=1&th=1&psc=1&tag=&ref=&adgrpid=65257070361&hvpone=&hvptwo=&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456).   

* Further required tools, such as ZigBee stick, ZigBee sensors, GSM modem or software etc., are described with the respective examples.   

---   

<a name="#content1"></a>   

# Content
## Area "Central":
* [Install Raspberry Pi (2021-07-31)](md/m4h01_RasPiInstall_e.md)
* [Raspberry Pi as access point (2021-07-31)](md/m4h02_RasPiAccessPoint_e.md)
* [Raspberry Pi as MQTT broker (2021-07-31)](md/m4h03_RasPiMQTTBroker_e.md)
* [Raspberry Pi: Sending MQTT messages regularly (2021-10-12)](md/m4h104_RasPi_crontab_e.md)

---   

## Area "D1mini"
* [D1mini: MQTT Relay with PubSubClient (2021-07-31)](md/m4h201_D1mqttRelayD1_e.md)
* [D1mini: The SimpleMqtt class (2021-07-31)](md/m4h202_D1SimpleMqtt_e.md)
* [D1mini: Switching a relay via MQTT - version 2 (2021-11-08)](md/m4h203_D1smqttRelayD1_e.md)

---   

## Area "auxiliary programs"
* [RasPi: Template for MQTT programs in C++ (2021-08-21)](md/m4h07_RasPiCppBase_e.md)
* [RasPi: Helpful single programs in C++ (2021-08-21)](md/m4h08_RasPiCppDemos_e.md)
* [RasPi: Create your own programs in C++ (2021-08-21)](md/m4h09_RasPiCppCreatingYourOwnProgs_e.md)
* [RasPi: Sending and receiving SMS via MQTT in C++ (2021-08-27)](md/m4h10_RasPiCppSms_e.md)

## Area "Zigbee"

## Area "Visualization"
* [First Vue program (without Visual Studio code) (2021-09-15)](md/m4h501_Vue_Hello_e.md)
* [Vue+VSC: First application with Visual Studio Code (2021-10-12)](md/m4h502_Vue_Hello_VSC_e.md)
* [Vue: First MQTT application "all-in-one" (2021-10-15)](md/m4h505_Mqtt_mini_e.md)
* [How to create a Vue MQTT app easily? (2021-12-13)](md/m4h510_Vue_ci_mqtt_mini_e.md)
* [MQTT Vue components for connecting, publishing, subscribing, and viewing (2021-12-19)](md/m4h515_Vue_ci_mqtt_conpubsublast_e.md)

---   

* [Vue+VSC+RasPi: Sending and automatically receiving MQTT messages (2021-10-11)](md/m4h504_Vue_PubSub2_e.md)
* [Vue+VSC: Sending and receiving MQTT messages (2021-10-03)](md/m4h505_Vue_Mqtt1_e.md)
* [How to create a Vue MQTT lamp symbol? (2021-12-09)](source_Vue/vue_mqtt_lamp1)
* [Vue: Switching lamps with MQTT (2021-11-05)](md/m4h506_Vue_Mqtt2_Lamp_e.md)
* [How to make a Vue app with multiple web pages? (2021-12-08)](source_Vue/vue_mqtt3_3webpages)   

[To the top of the page](#up)   
