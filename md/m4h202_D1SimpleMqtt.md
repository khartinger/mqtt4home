Letzte &Auml;nderung: 31.7.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: Die Klasse SimpleMqtt</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h202_D1SimpleMqtt_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
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
| + int setTopicGet(String sAllGet) | Setze alle (durch Komma getrennte) get-Topics |   
| + int setTopicSet(String sAllSet) | Setze alle (durch Komma getrennte) set-Topics |   
| + int setTopicSub(String sAllSub) | Setze alle (durch Komma getrennte) sub-Topics |   
| + int setTopicPub(String sAllPub) | Setze alle (durch Komma getrennte) pub-Topics |   
| + int setTopicGet(String sAllGet, String sAllRetainedGet) | Setze alle (durch Komma getrennte) get-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + int setTopicSet(String sAllSet, String sAllRetainedSet) | Setze alle (durch Komma getrennte) set-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + int setTopicPub(String sAllPub, String sAllRetainedPub) | Setze alle (durch Komma getrennte) pub-Topics plus Komma getrenntem Retain- String (0=false, 1=true) |   
| + boolean setRetainedIndex(String sType, int index, boolean bRetained) | Setze das Retain-Flag für das Element mit dem gegebenen Index in a..Get, a..Set oder a..Pub |   
| + String  getsRetainedAll() | Setze alle Retain-Flags |   
