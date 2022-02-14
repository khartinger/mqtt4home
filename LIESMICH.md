Letzte &Auml;nderung: 03.02.2022 <a name="up"></a>   
<table><tr><td><img src="md/logo/mqtt4home_96.png"></img></td><td>
<h1>MQTT f&uuml;r den Hausgebrauch :)</h1>
<a href="README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="#inhalt">Direkt zur Inhalts&uuml;bersicht</a><hr>

# Willkommen auf mqtt4home   

__*Wie kann ich Sensoren und Aktoren zu Hause miteinander verbinden?*__   
Wer sich diese Frage stellt, ist hier genau richtig. In f&uuml;nf verschiedenen Bereichen 
 werden in vielen Beispielen Fragen zum Thema "Homeautomation" beantwortet:   
1. Bereich "__Zentrale__": Wie setze ich ein Raspberry Pi (kurz RasPi) auf, damit es als Steuerzentrale bzw. MQTT-Broker arbeitet?   
2. Bereich "__D1mini__": Wie kann ich D1mini- oder ESP32mini-Mikrocontroller als Steuerelemente im System verwenden?   
3. Bereich "__Hilfsprogramme__": Wie erstelle ich Stand-Alone-Hilfsprogramme, die bestimmte Aufgaben durchf&uuml;hren?   
4. Bereich "__Zigbee__": Wie kann ich ZigBee-Komponenten in mein System einbauen?   
5. Bereich "__Visualisierung__": Wie kann ich Komponenten &uuml;ber eine grafische Oberfl&auml;che bedienen?   

Um das Thema einzuschr&auml;nken, wird in dieser Programmsammlung als Verbindungsmedium ein WLAN mit TCP-IP und MQTT-Protokoll festgelegt.   
Informationen zum MQTT-Protokoll gibt es auf [https://mqtt.org/](https://mqtt.org/).   

# Voraussetzungen

F&uuml;r das Bearbeiten des sehr weitl&auml;ufigen Themas "Homeautomation" sind einige Grundkenntnisse von Vorteil:   
* Etwas Basiswissen zum Raspberry Pi und zum Betriebssystem Linux (f&uuml;r die Bereiche __Zentrale__ und __ZigBee__)   
* Grundkenntnisse in C/C++ (f&uuml;r die Bereiche __D1mini__ und __Hilfsprogramme__)   
* Grundkenntnisse in HTML, CSS, JavaScript bzw. Node.js (f&uuml;r den Bereich __Visualisierung__)   
* Grundkenntnisse in Elektronik und L&ouml;ten (f&uuml;r den Bereich __D1mini__)   

Die Beispiele sind h&auml;ufig Schritt-f&uuml;r-Schritt-Anleitungen, die gut nachvollziehbar sein sollten.   
Welche Hard- und Software f&uuml;r die Bespiele ben&ouml;tigt wird, steht bei den jeweiligen Erkl&auml;rungen dabei.   
   
# Allgemeine Annahmen   

* Sensoren und Aktoren sind mit der Zentrale (RasPi) &uuml;ber Funk (WiFi oder ZigBee) verbunden.
* Das WiFi-Netzwerk wird vom RasPi zur Verf&uuml;gung gestellt.
* Wird eine Steuerung &uuml;ber das Internet gew&uuml;nscht, so muss der Internet-Zugang extra zur Verf&uuml;gung gestellt werden. (Router oder Alcatel Linkhub HH71VM mit HOT SIM-Karte etc.)

# Erforderliche Hardware   

* Raspberry Pi mit Zubeh&ouml;r (Netzteil, Monitor, Tastatur etc.),   
zB Starter-Kit von [Reichelt (incl. 7" Touch-Display)](https://www.reichelt.at/at/de/raspberry-pi-4-b-4gb-inkl-7-touch-display-gehaeuse-rpi4-bdl-4gb-7td-p291393.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCSgJ1AiDo2X72WmgAzl4nAxSPFg4yIhT37OJYww45nqSir0qp0gd-waAjhcEALw_wcB) oder 
[Amazon (ohne Display)](https://www.amazon.de/Raspberry-SD-Karte-Ultimatives-Quad-Core-unterst%C3%BCtzt/dp/B082PSBBMM/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=raspberry+pi+4+set&qid=1627196094&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFXQ0QzNDU2UU9QVTYmZW5jcnlwdGVkSWQ9QTA1ODg3NDgyTUlMWFpXWjNCWTZJJmVuY3J5cHRlZEFkSWQ9QTA1MDMyMjUzMElTVENaSEJHWTNLJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==).   
* D1 mini zB von [Reichelt](https://www.reichelt.at/at/de/d1-mini-esp8266-v2-0-d1-mini-p253978.html?PROVID=2807&gclid=Cj0KCQjw9O6HBhCrARIsADx5qCQ36slwX3O8fPPdBLJoEg1YwBGT9tDJQfl-IjdhpwqzUj0-DTPbe8saAswVEALw_wcB) oder [Amazon](https://www.amazon.de/AZDelivery-D1-Mini-ESP8266-12E-kompatibel/dp/B01N9RXGHY/ref=asc_df_B01N9RXGHY/?tag=googshopde-21&linkCode=df0&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456&psc=1&th=1&psc=1&tag=&ref=&adgrpid=65257070361&hvpone=&hvptwo=&hvadid=309008177512&hvpos=&hvnetw=g&hvrand=15638866670190817706&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1000900&hvtargid=pla-378671262456).   

* Weitere ben&ouml;tigte Hilfsmittel, wie ZigBee-Stick, ZigBee-Sensoren, GSM-Modem oder Software etc., werden bei den jeweiligen Beispielen beschrieben.   

---   

<a name="#inhalt"></a>   

# Inhalt
## Bereich "Zentrale"
* [Raspberry Pi installieren (2021-07-31)](md/m4h01_RasPiInstall.md)
* [Raspberry Pi als Access-Point (2021-07-31)](md/m4h02_RasPiAccessPoint.md)
* [Raspberry Pi als MQTT-Broker (2021-07-31)](md/m4h03_RasPiMQTTBroker.md)
* [Raspberry Pi: Regelm&auml;&szlig;iges Senden von MQTT-Nachrichten (2021-10-12)](md/m4h104_RasPi_crontab.md)
* [Vue-MQTT: Wie erstellt man ein CI-Symbol? (Beispiel: Lampe) (2021-12-23)](md/m4h520_Vue_ci_mqtt_lamp1.md)

---   

## Bereich "D1mini"
* [D1mini: MQTT Relais mit PubSubClient (2021-07-31)](md/m4h201_D1mqttRelayD1.md)
* [D1mini: Die Klasse SimpleMqtt (2021-07-31)](md/m4h202_D1SimpleMqtt.md)
* [D1mini: Schalten eines Relais via MQTT - Version 2 (2021-11-08)](md/m4h203_D1smqttRelayD1.md)

---   

## Bereich "Hilfsprogramme"
* [RasPi: Welche C++ Hilfsprogramme gibt es und wie bekomme ich sie zum Laufen? (2022-02-11)](md/m4h310_RasPiCppDemos.md)   
* [RasPi: Wie erstelle ich ein eigenes C++ Hilfsprogramm? (2022-02-12)](md/m4h320_RasPiCppCreatingYourOwnProgs.md)   
* [RasPi: Vorlage f&uuml;r MQTT-Programme in C++ (2021-08-21)](md/m4h07_RasPiCppBase.md)   
* [RasPi: Senden und Empfangen von SMS &uuml;ber MQTT in C++ (2021-08-27)](md/m4h340_RasPiCppSms.md)   
* [RasPi: Zeitliche &Uuml;berwachung von MQTT-Sensoren (Watchdog) in C++ (2022-02-03)](md/m4h350_RasPiCppWatchdog.md)   
* [RasPi: Filtern bzw. Blockieren von MQTT-Nachrichten (2022-02-14)](md/m4h360_RasPiCppInBlockOut.md)   

## Bereich "ZigBee"
* [Wie verbindet man Zigbee-Ger&auml;te mit MQTT? (2022-01-15)](md/m4h301_zbInstall.md)

## Bereich "Visualisierung"
* [Erstes Vue-Programm (ohne Visual Studio Code) (2021-09-15)](md/m4h501_Vue_Hello.md)
* [Vue+VSC: Erste Anwendung mit Visual Studio Code (2021-10-12)](md/m4h502_Vue_Hello_VSC.md)
* [Vue: Erste MQTT Anwendung "all-in-one" (2021-10-15)](md/m4h505_Mqtt_mini.md)
* [Wie erstellt man einfach eine Vue-MQTT-App? (2021-12-13)](md/m4h510_Vue_ci_mqtt_mini.md)
* [MQTT-Vue-Komponenten zum Verbinden, Ver&ouml;ffentlichen, Abonnieren und Anzeigen (2021-12-19)](md/m4h515_Vue_ci_mqtt_conpubsublast.md)
* [Vue-MQTT: Wie erstellt man ein CI-Symbol? (Beispiel: CiLamp) (2021-12-23)](md/m4h520_Vue_ci_mqtt_lamp1.md)
* [Vue+VSC: Schalten von Lampen mit MQTT (2021-12-25)](md/m4h525_Vue_ci_mqtt_lamp_button1.md)
* [Vue+VSC: Wie erstellt man ein Vue Projekt mit mehreren Web-Seiten? (2021-12-26)](md/m4h535_Vue_ci_mqtt_multipage1.md)   
* [__Vue+VSC: Beispiele f&uuml;r Ci-Symbole (2022-01-20)__](md/m4h550_Vue_ci_mqtt_all.md)   
* [Vue+VSC: Wie zeichnet man einen Hausplan? (CiWall-Elemente) (2022-01-04)](md/m4h560_Vue_ci_mqtt_wall1.md)   
* [Vue+VSC: Wie zeichnet man Türen und Fenster in einem Hausplan? (CiDoor, CiWindow, CiBlind) (2022-01-08)](md/m4h565_Vue_ci_mqtt_door_etc.md)   

[Zum Seitenanfang](#up)   
