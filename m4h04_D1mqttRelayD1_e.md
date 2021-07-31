######
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: MQTT Relay mit PubSubClient</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h04_D1mqttRelayD1_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Ziele
* Kennenlernen der Klasse [`pubsubclient`](https://github.com/knolleary/pubsubclient) zur Verarbeitung von MQTT-Nachrichten mit dem D1 mini oder ESP32 mini.
* Erstellen eines Programmes für den D1 mini oder ESP32 mini zum Steuern eines Relais(-Shields) &uuml;ber MQTT.
* Testen des MQTT-Relais mit dem PC.

## Erforderliche Hilfsmittel
* Hardware: D1 mini oder ESP32 mini
* Hardware: Relay-Shield (Ansteuerung über Pin D1=GPIO 22)
* Software: Arduino-Entwicklungsumgebung oder Visual Studio Code mit zumindest installierten Erweiterungen PlatformIO IDE und C/C++

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
Zusätzlich muss noch ein Client-Name festgelegt werden, zB `D1mini_Relay1`.   


* Im Programmcode sieht das zB folgendermaßen aus:   

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
* Wurde die Verbindung mit dem Netzwerk erfolgreich hergestellt, müssen (in der Funktion `setup()`) die Server-IP, der Port und der Name der Callback-Routine festgelegt werden:   
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

* Die &Uuml;berprüfung, ob der D1 mini (noch) mit dem Broker verbunden ist, erfolgt durch folgenden Befehl:
```
if (client.connected()) {
//...
}
```


### ad 3. Empfangen von Nachrichten
Für das Empfangen von Nachrichten sind zwei Punkte erforderlich:   
1. In der Funktion `loop()` MUSS regelmäßig die Methode `client.loop()` aufgerufen werden:
```
 if (client.connected()) {
  client.loop();                            // must be called!
 }
```
2. Die eigentliche Bearbeitung aller abonnierten Nachrichten erfolgt in der Funktion 
```
void callback(char* topic, byte* payload, unsigned int length) {
//...
}
```
Da die Payload beliebige Zeichen (auch das 0-Zeichen!) enthalten kann, ist zusätzlich noch die Länge der Payload angegeben.

### ad 4. Senden von Nachrichten
Das Senden von Nachrichten erfolgt mit einer der Methoden "publish()", die als Parameter zumindest das Sende-Topic und die Payload ("Nachricht") enthalten muss. Weiters kann man angeben, ob sich der Broker die Nachricht merken soll (3. Parameter = retain = auf `true` setzen).   
&nbsp;

## Erstellen des Programms "MQTT-Relay"
### Funktionen des Programmes im Detail
1. Connect to given WiFi and MQTT broker.
2. If connected, turn on blue LED.
3. Subscribe topic "relay/1/set" payload 0|1|-1.
4. If the topic is received,
   * switch the relay 0=off, 1=on, -1=toggle.
   * send a message with topic `relay/1/ret` and payload new relay state (0=off, 1=on, -1=toggle).
5. If connection is lost, D1 mini tries to reconnect.

 
## Test des MQTT-Relais
