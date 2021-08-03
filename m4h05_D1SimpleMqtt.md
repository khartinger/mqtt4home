Letzte &Auml;nderung: 31.7.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: Die Klasse SimpleMqtt</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h05_D1SimpleMqtt_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Ziele
* Den Aufbau von Topics bei der Nutzung der Klasse `SimpleMqtt` kennen.
* Die Klasse `SimpleMqtt` zur Verarbeitung von MQTT-Nachrichten verwenden können.

* Erstellen eines Programmes f&uuml;r den D1 mini oder ESP32 mini zum Steuern der blauen LED mit SimpleMqtt.
* Testen des SimpleMqtt-Led-Programms mit dem PC.

## Erforderliche Hilfsmittel
* Hardware: D1 mini oder ESP32 mini
* Hardware: Laptop/PC mit WLAN-Empfang und installierter Mosquitto-Software
* Software: Arduino-Entwicklungsumgebung oder Visual Studio Code mit zumindest installierten Erweiterungen PlatformIO IDE und C/C++

&nbsp;   
## Die Klasse "SimpleMqtt"
### Allgemeines
Die SimpleMqtt Klasse ist für D1 mini (ESP8266) und ESP32 geeignet und erweitert die Klasse `PubSubClient`, um MQTT einfach nutzbar zu machen.   
Das Protokoll MQTT ist Nachrichten-orientiert, wobei jede Nachricht aus Topic (Thema) und Payload (Nutzlast) besteht. Topics können beliebig gewählt werden und ähnlich wie Verzeichnisse hierarchisch aufgebaut sein.   
Um Nachrichten einfach verarbeiten zu können, definiert die Klasse `SimpleMqtt` folgenden Topic-Aufbau:   
* Jeder Systemteilnehmer hat ein eigenes __Basis-Topic__ (`topicbase`) zB `led/1`.
* __Anfrage-Topics__ bestehen aus dem Basis-Topic und dem Zusatz `/get`. Welcher Wert abgefragt wird, steht in der Payload.   
Beispiel: `mosquitto_pub -t led/1/get -m blueled`
* __Steuer-Topics__ bestehen aus dem Basis-Topic und dem Zusatz `/set/...`. Ein zu verarbeitender Wert steht in der Payload.   
Beispiel: `mosquitto_pub -t led/1/set/blueled -m 1`
* __Antwort-Nachrichten__ bestehen aus dem Basis-Topic und dem Zusatz `/ret/...`, wobei der Rückgabewert in der Payload steht.   
Beispiel: Topic `led/1/ret/blueled` zB mit der Payload `0` oder `1`
* Zusätzlich können beliebige Topics empfangen und gesendet werden.

### Vorteile bei der Verwendung der Klasse "SimpleMqtt"
Vorteile der Klasse `SimpleMqtt` sind:   
* Vergabe eines eigenen Namens (Basis-Topic) für jeden D1 mini ohne das Programm anpassen zu müssen. (Der Name wird im EEPROM gespeichert.)
* Definition der get-/set-/sub- und pub-Topics in jeweils einer Konstanten (`TOPIC_GET`, `TOPIC_SET`, `TOPIC_SUB`, `TOPIC_PUB`)
* Automatische Antwort auf get-Anfragen mit der Payload   
  |     |     |   
  | --- | --- |   
  | help oder ? | Anzeige der implementierten get-/set-/sub- und pub-Topics |   
  | version     | SimpleMqtt-Version oder Programm-Version           |   
  | ip          | die vom RasPi zugewiesene IP des Systemteilnehmers |   
* Einfache Speicherung von Werten im EEPROM (zB Parameter für Sensoren)

## Klassendiagramm `SimpleMqtt`

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
| + void setEepromSize(int eepromSize)   | Setze die benötigte EEPROM-Größe (maximal 4096) |
| + void setWiFiWaitingTime(int ms)      | Maximale Wartezeit in Millisekunden, um sich mit dem WiFi zu verbinden (min. 100ms). |
| + void setWiFiConnectingCounter(int number) | Anzahl der maximalen connectingWiFi()-Aufrufe nach Aufruf von connectingWiFiBegin() |
| + void setMQTTClientName(String sName) | Setze MQTT Clientname |
| + void allowMQTTStartInfo(bool allow)  | Erlaube das Senden der Startinformation dieses D1 mini. |
| + String  getMQTTClientName()          | Hole MQTT Clientname |
| + String  getsState()                  | Hole MQTT Client-Status als String  |
| + String  getsSSID()                   | Hole WiFi-Netzwerk-Namen als String |
| + virtual String getsLocalIP()         | Lies Client-IP und gib sie als String zurück |
| + String  getsMyIP()                   | Hole die gespeicherte Client-IP-Adresse als String |
| + String  getsMac()                    | Hole die MAC Adresse des D1 mini als String |
| + String  getsTopicBase()              | Hole das Basis-Topic |

&nbsp;

| Methoden für die MQTT-Topics |     |
| ---------------------------- | --- |
| + int setTopicBase(String topicBasis) | Setze Basis-Topic im RAM und EEPROM OHNE neuerliches Abonnieren ("subscribe") von Nachrichten |
| + int setTopicBaseDefault(String topicBasisDefault) | Setze Vorgabewert für Basis-Topic |
| + void setTopics(String sAllGet, String sAllSet, String sAllSub, String sAllPub) | Setze alle (durch Komma getrennte) Topics |



  //_____set all get-topics as comma separated strings__________
| + int setTopicGet(String sAllGet) | |
  //_____set all set-topics as comma separated strings__________
| + int setTopicSet(String sAllSet) | |
  //_____set all sub(scribe)-topics as comma separated strings__
| + int   setTopicSub(String sAllSub) | |
  //_____set all pub(lish)-topics as comma separated strings____
| + int   setTopicPub(String sAllPub) | |
  
  //_____set all get-topics as comma separated strings__________
  //     plus comma separated retained string (0=false, 1=true)
| + int   setTopicGet(String sAllGet, String sAllRetainedGet) | |
  //_____set all set-topics as comma separated strings__________
  //     plus comma separated retained string (0=false, 1=true)
| + int   setTopicSet(String sAllSet, String sAllRetainedSet) | |
  //_____set all pub(lish)-topics as comma separated strings____
  //     plus comma separated retained string (0=false, 1=true)
| + int   setTopicPub(String sAllPub, String sAllRetainedPub) | |
  //_____set retained for index in a..Get|a..Set|a..Pub_________
| + boolean setRetainedIndex(String sType, int index, boolean bRetained) | |
  //_____ret all retained flags as string_______________________
| + String  getsRetainedAll() | |







Zu diesem Zweck wird ein "Basis"-Topic ("`topicbase`") definiert, das durch Schlüsselwörter erweitert werden kann um die folgenden Schlüsselwörter erweitert werden kann: /get Abfrage des in der Nutzlast angegebenen Wertes z.B. Abfrage der Version: -t simplemqtt/default/get -m version /set Setzen eines Wertes z.B.. -t simplemqtt/default/set/topicbase -m new_base
    Als Antwort erhalten Sie eine MQTT-Nachricht mit dem Schlüsselwort /ret, z.B. -t simplemqtt/default/ret/version -m 2020-12-08
    Außerdem werden alle (beliebigen) Topics, die in der Konstante TOPIC_SUB definiert sind, abonniert.
    Weiterhin können alle Befehle der Klasse PubSubClient weiterhin verwendet werden. // Im Anwenderprogramm müssen die folgenden Dinge implementiert werden: [1] Definieren Sie den Typ der Karte (1=D1mini, 2=D1_ESP32) #define D1MINI 1 ESP8266 D1mini +pro [2] Definieren Sie die Themenbasis #define TOPIC_BASE "simplemqtt/default" [3] Definieren Sie die Topics, auf die das Programm reagieren soll für set/get/sub und pub Anfragen (Schlüsselwörter durch Komma getrennt oder Leerstring) z.B. #define TOPIC_GET "help,version,ip,topicbase,eeprom,led" #define TOPIC_SET "topicbase,eeprom,led" #define TOPIC_SUB "" #define TOPIC_PUB "" [4] Erzeugen eines Objekts, das im Anwenderprogramm verwendet werden soll SimpleMqtt client("ssid", "password", "mqttservername|ip"); [5] Erstellen Sie eine Callback-Routine für eingehende Nachrichten void callback(char* topic, byte* payload, unsigned int length) { client.callback_(topic, payload, length); } [6] Funktionen zur Beantwortung eingehender MQTT-Anfragen erstellen String simpleGet(String sPayload) { return String(""); } String simpleSet(String sTopic, String sPayload) { return String(""); } void simpleSub(String sTopic, String sPayload) { } [7] Aufruf der Methode "doLoop()" in der Hauptschleife: client.doLoop(); Sie MUSS aufgerufen werden, sonst werden keine MQTT-Nachrichten verarbeitet! // Hinweis: Wenn die PubSubClient-Klasse registriert (installiert) ist in der IDE registriert (installiert) ist, werden die PubSubClient-Dateien im Verzeichnis src/simplemqtt gelöscht werden.




