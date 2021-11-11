Letzte &Auml;nderung: 10.11.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: Die Klasse SimpleMqtt</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h202_D1SimpleMqtt_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Einleitung
Ein f&uuml;r die Heimautomation sehr gut geeignetes Protokoll ist MQTT, weil es flexibel und einfach zu verwenden ist. Es ist Nachrichten-orientiert, wobei jede Nachricht aus Topic (Thema) und Payload (Nutzlast) besteht. Topics k&ouml;nnen beliebig gew&auml;hlt werden und &auml;hnlich wie Verzeichnisse hierarchisch aufgebaut sein.   
   
Damit ein D1 mini (oder ESP32 D1mini) MQTT-Nachrichten senden oder empfangen kann, ben&ouml;tigt man eine MQTT-Bibliothek. In diesen Beispielen wird als Basis [`PubSubClient`](https://github.com/knolleary/pubsubclient) von Nick O´Leary verwendet.   
   
Die Klasse `SimpleMqtt` erweitert `PubSubClient` um einige Funktionalit&auml;ten, um das Erstellen von D1 mini-Systemen f&uuml;r die Heimautomation zu erleichtern.   
   
## Vereinbarungen zur Nutzung von `SimpleMqtt`
Um Nachrichten einfach verarbeiten zu k&ouml;nnen, definiert die Klasse `SimpleMqtt` folgenden Topic-Aufbau:   
* __Basis-Topic__: Jeder Systemteilnehmer hat einen eigenen "Namen" bzw. ein Topic auf das er "h&ouml;rt", das "Basis-Topic" (`topicbase`) zB `led/1` oder `led/2`.
* __Anfrage-Topics__ dienen zum Abfragen von Informationen und bestehen aus dem Basis-Topic mit dem Zusatz `/get`. Welcher Wert abgefragt wird, steht in der Payload.   
Beispiel: `mosquitto_pub -t led/1/get -m blueled`
* __Steuer-Topics__ setzen Parameter eines Systemteilnehmers und bestehen aus dem Basis-Topic mit dem Zusatz `/set/...`. Der zu verarbeitende Wert steht in der Payload.   
Beispiel: `mosquitto_pub -t led/1/set/blueled -m 1`
* __Antwort-Nachrichten__ enthalten das Ergebnis von Anfragen oder Steuerbefehlen und bestehen aus dem Basis-Topic mit dem Zusatz `/ret/...`. Der R&uuml;ckgabewert steht in der Payload.   
Beispiel: Topic `led/1/ret/blueled` zB mit der Payload `0` oder `1`
   
Zus&auml;tzlich zu diesen Vereinbarungen k&ouml;nnen aber auch beliebige weitere Topics empfangen und gesendet werden.   
   
## Vorteile bei der Verwendung der Klasse "SimpleMqtt"
Vorteile der Klasse `SimpleMqtt` sind:   
* Vergabe eines eigenen Namens (Basis-Topic) f&uuml;r jeden D1 mini ohne das Programm anpassen zu m&uuml;ssen. (Der Name wird im EEPROM gespeichert.)
* Definition der get-/set-/sub- und pub-Topics in jeweils einer Konstanten (`TOPIC_GET`, `TOPIC_SET`, `TOPIC_SUB`, `TOPIC_PUB`)
* Automatische Antwort auf get-Anfragen mit der Payload   
  |     |     |   
  | --- | --- |   
  | help oder ? | Anzeige der implementierten get-/set-/sub- und pub-Topics |   
  | version     | SimpleMqtt-Version oder Programm-Version           |   
  | ip          | die vom RasPi zugewiesene IP des Systemteilnehmers |   
* Einfache Speicherung von Werten im EEPROM (zB Parameter f&uuml;r Sensoren)
   
## Programmger&uuml;st
D1 mini Programme setzen folgende Projektstruktur voraus:   
  | Verzeichnisname   | Inhalt (Dateien)    |   
  | --- | --- |   
  | xxx | xxx.ino   <br>eventuell readme.md |   
  | xxx/src/simplemqtt | `D1_class_SimpleMqtt.cpp`, `D1_class_SimpleMqtt.h`,<br>   `PubSubClient.cpp`, `PubSubClient.h`   |   
  

Das folgende Listing `D1_m4h01_smqtt_template1.ino` ist die Basis f&uuml;r eigene Programme:   
```   
//_____D1_m4h01_smqtt_template1.ino______________khartinger_____
// Template to create a MQTT application using SimpleMqtt
// Function of this program:
// [1] Connect to given WiFi and MQTT broker
// [2] Automatically send an MQTT message message at startup
// [3] Use "topic base" stored in eeprom or topic "test/smqtt"
// [4] Automatic (build in) answers for messages 
//     -t test/smqtt/get -m help
//     -t test/smqtt/get -m version
//     -t test/smqtt/get -m ip
// [5] Answer the following requests with a topic like
//     test/smqtt/ret/...:
//     -t test/smqtt/get -m topicbase
//     -t test/smqtt/get -m eeprom
// [6] Change topic base (e.g. to test/newtopic)
//     -t test/smqtt/set/topicbase -m test/newtopic
// [7] Erase topic base stored in EEPROM by
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
 {Serial.begin(115200); Serial.println("\nsetup(): --Start--");}
 //-----init pins-----------------------------------------------
 //-----prepare mqtt start--------------------------------------
 client.setTopicBaseDefault(TOPIC_BASE);    // default base
 client.begin();                            // setup objects
 client.setCallback(callback);              // mqtt receiver
 client.setTopics(TOPIC_GET, TOPIC_SET, TOPIC_SUB, TOPIC_PUB);
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

## Test des Templates
### Erforderliche Hilfsmittel
* Hardware: D1 mini oder ESP32 mini
* Hardware: Laptop/PC mit WLAN-Empfang und installierter Mosquitto-Software
* Software: Arduino-Entwicklungsumgebung oder Visual Studio Code mit zumindest installierten Erweiterungen PlatformIO IDE und C/C++

### Vorbereitung
1. Laptop/PC: Erstellen des Projekts `D1_m4h01_smqtt_template1`   
[---> Quellcode siehe https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h01_smqtt_template1)   

2. Laptop/PC: Build und Upload des Programms auf den D1 mini.   

3. RasPi mit dem Mosquitto-Broker starten (falls dies noch nicht erfolgt ist).   

4. Laptop/PC: Mit dem WLAN-Netz `Raspi11` verbinden (Passwort `12345678`).   

5. Laptop/PC: Ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
`cmd.exe`
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
`c:`   
`cd /programme/mosquitto`

6. Laptop/PC: Die Anzeige aller Nachrichten zulassen:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

7. Laptop/PC: Ein zweites Eingabeaufforderungs-Fenster &ouml;ffnen:   
`cmd.exe`
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
`c:`   
`cd /programme/mosquitto`

### Durchf&uuml;hreung des Template-Tests
1. D1 mini: Reset-Knopf dr&uuml;cken   
   Anzeige im ersten Eingabeaufforderungs-Fenster:   
   `info/start/mqtt {"topicbase":"test/smqtt","IP":"10.1.1.160"}`   

2. Hilfe abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m ?`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/get help`   
   `test/smqtt/ret/help`   
   `get: ?|help|version|ip|topicbase|eeprom|`   
   `set: topicbase|eeprom|`   
   `sub:`   

3. Version abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m version`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/get version`   
   `test/smqtt/ret/version 2021-11-11 D1_m4h01_smqtt_template1`   

4. IP-Adresse des D1 mini abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m ip`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/get ip`   
   `test/smqtt/ret/ip 10.1.1.160`   

5. Basis-Topic des D1 mini abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m topicbase`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/get topicbase`   
   `test/smqtt/ret/topicbase test/smqtt`   

6. EEPROM-Inhalt des D1 mini abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/get -m eeprom`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/get eeprom`   
   `test/smqtt/ret/eeprom Error: No topic base stored`   

7. Basis-Topic &auml;ndern auf `test/newtopic`   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/smqtt/set/topicbase -m test/newtopic`   
   _Anzeige im Fenster 1_:   
   `test/smqtt/set/topicbase test/newtopic`   
   `test/newtopic/ret/topicbase test/newtopic`   
   Beachte: Das neue Basis-Topic ist sofort wirksam.

8. D1 mini: Reset-Knopf dr&uuml;cken   
   _Anzeige im Fenster 1_:   
   `info/start/mqtt {"topicbase":"test/newtopic","IP":"10.1.1.160"}`   

9. Version abfragen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/newtopic/get -m version`   
   Beachte: Das ge&auml;nderte Basis-Topic verwenden ;)   
   _Anzeige im Fenster 1_:   
   `test/newtopic/get version`   
   `test/newtopic/ret/version 2021-11-11 D1_m4h01_smqtt_template1`   

10. EEPROM-Inhalt des D1 mini l&ouml;schen   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t test/newtopic/set/eeprom -m 0`   
   _Anzeige im Fenster 1_:   
   `test/newtopic/set/eeprom 0`   
   `test/newtopic/ret/eeprom erased`   
   Beachte: Das im EEPROM gespeicherte Basis-Topic wurde zwar gel&ouml;scht, bis zum Neustart des D1 minis gilt aber immer noch das alte Basis-Topic!

---   

# Klassendiagramm `SimpleMqtt`

| Klasse SimpleMqtt                   | erweitert PubSubClient                            |
| ----------------------------------- | ------------------------------------------------- |
| + SimpleMqtt()                      | Setze Vorgabewerte: |
| + SimpleMqtt(char* ssid, char* pwd) | |
| + SimpleMqtt(char* ssid, char* pwd, char* mqtt_server, char* topicbase) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server, String topicbase) | |
| + SimpleMqtt(String sssid, String spwd, String smqtt_server, String topicbase, String clientname) | |
| ~ void setup()                      | Eigenschaften setzen. Aufruf durch Konstruktor.             |
| ~ bool begin(int iSource)           | Weitere Eigenschaften setzen. Aufruf durch Konstruktor.     |
| + bool begin()                      | Initialisiere Objekte, lies topicbase aus dem EEPROM. |

&nbsp;

| Allgemeine Setter- und Getter-Methoden |     |
| -------------------------------------- | --- |
| + void setLanguage(char language)      | Setze Sprache (Vorgabe e=Englisch, d=Deutsch |
| + void setEepromSize(int eepromSize)   | Setze die ben&ouml;tigte EEPROM-Gr&ouml;&szlig;e (maximal 4096) |
| + void setWiFiWaitingTime(int ms)      | Maximale Wartezeit in Millisekunden, um sich mit dem WiFi zu verbinden (min. 100ms). |
| + void setWiFiConnectingCounter(int number) | Anzahl der maximalen connectingWiFi()-Aufrufe nach Aufruf von connectingWiFiBegin() |
| + void setMQTTClientName(String sName) | Setze MQTT Clientname |
| + void allowMQTTStartInfo(bool allow)  | Erlaube das Senden der Startinformation dieses D1 mini. |
| + String  getMQTTClientName()          | Hole MQTT Clientname |
| + String  getsState()                  | Hole MQTT Client-Status als String  |
| + String  getsSSID()                   | Hole WiFi-Netzwerk-Namen als String |
| + virtual String getsLocalIP()         | Lies Client-IP und gib sie als String zur&uuml;ck |
| + String  getsMyIP()                   | Hole die gespeicherte Client-IP-Adresse als String |
| + String  getsMac()                    | Hole die MAC Adresse des D1 mini als String |
| + String  getsTopicBase()              | Hole das Basis-Topic |

&nbsp;

| Methoden f&uuml;r die MQTT-Topics |     |
| ---------------------------- | --- |
| + int setTopicBase(String topicBasis) | Setze Basis-Topic im RAM und EEPROM OHNE neuerliches Abonnieren ("subscribe") von Nachrichten |   
| + int setTopicBaseDefault(String topicBasisDefault) | Setze Vorgabewert f&uuml;r Basis-Topic |   
| + void setTopics(String sAllGet, String sAllSet, String sAllSub, String sAllPub) | Setze alle (durch Komma getrennte) Topics |   
| + int setTopicGet(String sAllGet) | Setze alle (durch Komma getrennte) get-Topics |   
| + int setTopicSet(String sAllSet) | Setze alle (durch Komma getrennte) set-Topics |   
| + int setTopicSub(String sAllSub) | Setze alle (durch Komma getrennte) sub-Topics |   
| + int setTopicPub(String sAllPub) | Setze alle (durch Komma getrennte) pub-Topics |   
| + int setTopicGet(String sAllGet, String sAllRetainedGet) | Setze alle (durch Komma getrennte) get-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + int setTopicSet(String sAllSet, String sAllRetainedSet) | Setze alle (durch Komma getrennte) set-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + int setTopicPub(String sAllPub, String sAllRetainedPub) | Setze alle (durch Komma getrennte) pub-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + boolean setRetainedIndex(String sType, int index, boolean bRetained) | Setze das Retain-Flag f&uuml;r das Element mit dem gegebenen Index in a..Get, a..Set oder a..Pub |   
| + String  getsRetainedAll() | Setze alle Retain-Flags |   
