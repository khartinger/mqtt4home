Letzte &Auml;nderung: 31.7.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: MQTT Relay mit PubSubClient</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h04_D1mqttRelayD1_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Ziele
* Kennenlernen der Klasse [`pubsubclient`](https://github.com/knolleary/pubsubclient) zur Verarbeitung von MQTT-Nachrichten mit dem D1 mini oder ESP32 mini.
* Erstellen eines Programmes f&uuml;r den D1 mini oder ESP32 mini zum Steuern eines Relais(-Shields) &uuml;ber MQTT.
* Testen des MQTT-Relais mit dem PC.

## Erforderliche Hilfsmittel
* Hardware: D1 mini oder ESP32 mini
* Hardware: Relay-Shield (Ansteuerung &uuml;ber Pin D1=GPIO 22)
* Hardware: Laptop/PC mit WLAN-Empfang und installierter Mosquitto-Software
* Software: Arduino-Entwicklungsumgebung oder Visual Studio Code mit zumindest installierten Erweiterungen PlatformIO IDE und C/C++

&nbsp;   
## MQTT-Nachrichten und die Klasse "pubsubclient"
Die Bearbeitung von MQTT-Nachrichten gliedert sich in vier Schritte:   
1. Festlegen der Netzwerk-Parameter
2. Verbinden mit dem MQTT-Broker
3. Empfangen von Nachrichten ("subscribe")
4. Senden von Nachrichten ("publish")

### ad 1. Festlegen der Netzwerk-Parameter
* Bereits beim [Einrichten des RasPi als Access-Point](m4h02_RasPiAccessPoint.md) werden folgende Parameter festgelegt:   

|                 |                 |   
| --------------- | --------------- |   
| WLAN-Name:      | `Raspi11`       |   
| WLAN-Passwort:  | `12345678`      |   
| IP-Adresse:     | `10.1.1.1`      |   
Zus&auml;tzlich muss noch ein Client-Name festgelegt werden, zB `D1mini_Relay1`.   

* Im Programmcode sieht das zB folgenderma&szlig;en aus:   

```
const char* ssid = "Raspi11";
const char* password = "12345678";
const char* mqtt_server = "10.1.1.1";
String clientId=String("D1mini_Relay1");

WiFiClient espClient;
PubSubClient client(espClient);
```

### ad 2. Verbinden mit dem Netzwerk und dem MQTT-Broker
* Zum Verbinden mit dem Netzwerk ist zumindest folgender (blockierender) Code erforderlich:   
```
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) { delay(200); }
```
* Wurde die Verbindung mit dem Netzwerk erfolgreich hergestellt, m&uuml;ssen (in der Funktion `setup()`) die Server-IP, der Port und der Name der Callback-Routine festgelegt werden:   
```
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
```
* Die Verbindung zum Broker sowie das Abonnieren ("subscribe") von Nachrichten erfolgt durch folgende Befehle: 
```
 if(client.connect(clientId.c_str())) { //...connected...
  client.subscribe("#");
 }
```
Anmerkung: Mit `"#"` werden alle MQTT-Nachrichten abonniert.   

* Die &Uuml;berpr&uuml;fung, ob der D1 mini (noch) mit dem Broker verbunden ist, erfolgt durch folgenden Befehl:
```
if (client.connected()) {
//...
}
```

### ad 3. Empfangen von Nachrichten
F&uuml;r das Empfangen von Nachrichten sind zwei Punkte erforderlich:   
1. In der Funktion `loop()` MUSS regelm&auml;&szlig;ig die Methode `client.loop()` aufgerufen werden:
```
 if (client.connected()) {
  client.loop();                            // must be called!
 }
```
2. Die eigentliche Bearbeitung aller abonnierten Nachrichten erfolgt in der Funktion `callback`:
```
void callback(char* topic, byte* payload, unsigned int length) {
//...
}
```
Da die Payload beliebige Zeichen (auch das 0-Zeichen!) enthalten kann, ist zus&auml;tzlich noch die L&auml;nge der Payload angegeben.

### ad 4. Senden von Nachrichten
Das Senden von Nachrichten erfolgt mit einer der Methoden "`publish()`", die als Parameter zumindest das Sende-Topic und die Payload ("Nachricht") enthalten muss. Weiters kann man angeben, ob sich der Broker die Nachricht merken soll (3. Parameter = retain = auf `true` setzen).   
&nbsp;

## Erstellen des Programms "MQTT-Relay"
### Funktionen des Programmes im Detail
1. Versuch der Herstellung der Verbindung zum gegebenen WiFi und MQTT-Broker.
2. Einschalten der blauen LED, wenn die Verbindung besteht.
3. Abonnieren des Topics `relay/1/set` mit den m&ouml;glichen Inhalten ("payload") 0, 1 oder -1.
4. Wird das Topic `relay/1/set` empfangen,
   * wird das Relais entsprechend der Payload geschaltet: 0=aus, 1=ein, -1=um.
   * wird eine Nachricht mit dem Topic `relay/1/ret` und der Payload entsprechend dem neuen Status des Relais geschickt (0=aus, 1=ein, -1=um).
5. Falls die Verbindung zum Broker unterbrochen wird, versucht der D1 mini, sie nach einer  Wartezeit von 4 Sekunden (`MQTT_RECONNECT_MS`) wieder herzustellen.

### Codierung

Die vollst&auml;ndige Codierung befindet sich auf GitHub unter   
[https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1](https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1)   

&nbsp;

## Test des MQTT-Relais
1. D1 mini: Hochladen des Programms [`D1_Ex60_mqtt_relayD1`](https://github.com/khartinger/D1mini/tree/master/D1_Ex60_mqtt_relayD1).   

2. RasPi mit dem Mosquitto-Broker starten (falls dies noch nicht erfolgt ist).   

3. Laptop/PC: Mit dem WLAN-Netz `Raspi11` verbinden (Passwort `12345678`).   

4. Laptop/PC: Ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
```cmd.exe```
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
```c:```   
```cd /programme/mosquitto```

5. Laptop/PC: Die Anzeige aller Nachrichten zulassen:   
```mosquitto_sub -h 10.1.1.1 -t "#" -v```   
Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

6. Laptop/PC: Ein zweites Eingabeaufforderungs-Fenster &ouml;ffnen:   
```cmd.exe```
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
```c:```   
```cd /programme/mosquitto```

7. Senden des Relais-Umschalte-Kommandos vom zweiten Eingabeaufforderungs-Fenster aus:    
```mosquitto_pub -h 10.1.1.1 -t relay/1/set -m -1```   
Im ersten Eingabeaufforderungs-Fenster erscheinen folgende zwei Zeilen:   
```relay/1/set -1```   
```relay/1/ret 1```   
Au&szlig;erdem m&uuml;sste das Relais umgeschaltet haben (Relais-LED leuchtet).   
