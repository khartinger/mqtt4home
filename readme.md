<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>MQTT for domestic/home use :)</h1>
Last modified: 2021-07-31<br>
<a href="liesmich.md">==> German version</a>&nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>


## Welcome to mqtt4home
This project describes in many examples how to network sensors and actuators at home using the MQTT protocol. A Raspberry Pi (RasPi for short) serves as the control center ("broker").<br>
Information about the MQTT protocol is available at [https://mqtt.org/](https://mqtt.org/).   

### General Assumptions

* Sensors and actuators are connected to the control center (RasPi) via radio (WiFi or ZigBee).
* The WiFi network is provided by the RasPi.
* If control via the Internet is desired, Internet access must be provided separately. (Router or Alcatel Linkhub HH71VM with HOT SIM card etc.).

### Required hardware

* Raspberry Pi with accessories (power supply, monitor, keyboard etc.),   
e.g. starter kit from [Reichelt (incl. 7" touch display)](https://www.reichelt.at/at/de/raspberry-pi-4-b-4gb-inkl-7-touch-display-gehaeuse-rpi4-bdl-4gb-7td-p291393.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCSgJ1AiDo2X72WmgAzl4nAxSPFg4yIhT37OJYww45nqSir0qp0gd-waAjhcEALw_wcB) or [Amazon (without display)](https://www.amazon.de/Raspberry-SD-Karte-Ultimatives-Quad-Core-unterst%C3%BCtzt/dp/B082PSBBMM/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=raspberry+pi+4+set&qid=1627196094&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFXQ0QzNDU2UU9QVTYmZW5jcnlwdGVkSWQ9QTA1ODg3NDgyTUlMWFpXWjNCWTZJJmVuY3J5cHRlZEFkSWQ9QTA1MDMyMjUzMElTVENaSEJHWTNLJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==).   

* D1 mini e.g. from [Reichelt](https://www.reichelt.at/at/de/d1-mini-esp8266-v2-0-d1-mini-p253978.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCQ36slwX3O8fPPdBLJoEg1YwBGT9tDJQfl-IjdhpwqzUj0-DTPbe8saAswVEALw_wcB) or [Amazon](https://www.amazon.de/AZDelivery-D1-Mini-ESP8266-12E-kompatibel/dp/B01N9RXGHY/ref=asc_df_B01N9RXGHY/?tag=googshopde-21&linkCode=df0&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456&psc=1&th=1&psc=1&tag=&ref=&adgrpid=65257070361&hvpone=&hvptwo=&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456).   

* Further required tools, such as ZigBee stick, ZigBee sensors, GSM modem or software etc., are described with the respective examples.   

---   

## Contents
1. [Install Raspberry Pi (2021-07-31)](m4h01_RasPiInstall_e.md)
2. [Raspberry Pi as access point (2021-07-31)](m4h02_RasPiAccessPoint_e.md)
3. [Raspberry Pi as MQTT broker (2021-07-31)](m4h03_RasPiMQTTBroker_e.md)

---   

4. [D1mini: MQTT Relay with PubSubClient (2021-07-31)](m4h04_D1mqttRelayD1_e.md)
5. [D1mini: The SimpleMqtt class (2021-07-31)](m4h05_D1SimpleMQTT_e.md)