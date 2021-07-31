######
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: Die Klasse SimpleMqtt</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h04_D1SimpleMQTT_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

Die SimpleMqtt Klasse ist geeignet für D1 mini (ESP8266)
und ESP32 und erweitert die Klassen PubSubClient und
SimpleMqtt, um MQTT einfach nutzbar zu machen.
* Zu diesem Zweck wird ein "Basis"-Thema (topicbase, Standard ist
simplemqtt/default) definiert, das durch folgende Schlüsselwörter erweitert werden kann
um die folgenden Schlüsselwörter erweitert werden kann:
/get Abfrage des in der Nutzlast angegebenen Wertes
z.B. Abfrage der Version:
-t simplemqtt/default/get -m version
/set Setzen eines Wertes z.B.. 
-t simplemqtt/default/set/topicbase -m new_base
* Als Antwort erhalten Sie eine MQTT-Nachricht mit dem Schlüsselwort /ret,
z.B. -t simplemqtt/default/ret/version -m 2020-12-08
* Außerdem werden alle (beliebigen) Topics, die in der Konstante
TOPIC_SUB definiert sind, abonniert.
* Weiterhin können alle Befehle der Klasse PubSubClient 
weiterhin verwendet werden.
//
Im Anwenderprogramm müssen die folgenden Dinge implementiert werden:
[1] Definieren Sie den Typ der Karte (1=D1mini, 2=D1_ESP32)
#define D1MINI 1 ESP8266 D1mini +pro
[2] Definieren Sie die Themenbasis
#define TOPIC_BASE "simplemqtt/default"
[3] Definieren Sie die Topics, auf die das Programm reagieren soll 
für set/get/sub und pub Anfragen
(Schlüsselwörter durch Komma getrennt oder Leerstring) z.B.
#define TOPIC_GET "help,version,ip,topicbase,eeprom,led"
#define TOPIC_SET "topicbase,eeprom,led"
#define TOPIC_SUB ""
#define TOPIC_PUB ""
[4] Erzeugen eines Objekts, das im Anwenderprogramm verwendet werden soll
SimpleMqtt client("ssid", "password", "mqttservername|ip");
[5] Erstellen Sie eine Callback-Routine für eingehende Nachrichten
void callback(char* topic, byte* payload, unsigned int length)
{ client.callback_(topic, payload, length); }
[6] Funktionen zur Beantwortung eingehender MQTT-Anfragen erstellen
String simpleGet(String sPayload) { return String(""); }
String simpleSet(String sTopic, String sPayload) { return String(""); }
void simpleSub(String sTopic, String sPayload) { }
[7] Aufruf der Methode "doLoop()" in der Hauptschleife:
client.doLoop();
Sie MUSS aufgerufen werden, sonst werden keine MQTT-Nachrichten verarbeitet!
//
Hinweis: Wenn die PubSubClient-Klasse registriert (installiert) ist
in der IDE registriert (installiert) ist, werden die PubSubClient-Dateien im Verzeichnis
src/simplemqtt gelöscht werden.
