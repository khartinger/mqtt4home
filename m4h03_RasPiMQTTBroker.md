######
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi als MQTT-Broker</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h03_RasPiMQTTBroker_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
Zur Verwendung von MQTT gibt es verschiedene Bibliotheken. In diesem Fall wird das Open-Source-Projekt Mosquitto verwendet, da es einen Broker, einen Sende- und einen Empfangs-Client zur Verf&uuml;gung stellt und sowohl unter Linux als auch Windows l&auml;uft.   
Die Ziele im Detail:   
* Mosquitto auf dem RasPi installieren.
* Mosquitto auf dem PC/Laptop installieren.
* MQTT-Nachrichten vom PC und RasPi senden und auf beiden Ger&auml;ten anzeigen.

## Ben&ouml;tigte Hilfsmittel
* Hardware: RasPi mit Internet-Zugriff
* Hardware: PC/Laptop mit Internet-Zugriff
 
## Vorgangsweise
### Installation des MQTT Brokers und der Clients auf dem RasPi
1. Mosquitto Server installieren   
   ```
   sudo apt-get install mosquitto
   ```
2. Mosquitto Clients installieren (Publisher und Subscriber)   
   ```
   sudo apt-get install mosquitto-clients
   ```
3. Mosquitto C-Bibliothek installieren   
   ```
   sudo apt-get install libmosquitto-dev
   ```

### Test der MQTT Installation auf dem RasPi
Zum Testen der Installation ben&ouml;tigt man drei Dinge:   
1. Ein laufendes Programm, das Nachrichten weiterschickt ("Broker").
2. Einen Nachrichten-Sender ("Publisher").
3. Einen Nachrichten-Empf&auml;nger ("Subscriber").

Dazu geht man folgenderma&szlig;en vor:   
* Zuerst muss der MQTT-Broker gestartet werden:   
  `sudo /etc/init.d/mosquitto start`   
  `[ ok ] Starting mosquitto (via systemctl): mosquitto.service.`

* Als N&auml;chstes wird ein Datenempf&auml;nger (Subscriber) eingerichtet, der auf Nachrichten des Typs "Test1" h&ouml;rt:   
 `mosquitto_sub -d -t Test1`   
 `Client mosqsub/3152-raspberryp sending CONNECT`   
 `Client mosqsub/3152-raspberryp received CONNACK`   
 `Client mosqsub/3152-raspberryp sending SUBSCRIBE (Mid: 1, Topic: Test1, QoS: 0)`   
 `Client mosqsub/3152-raspberryp received SUBACK`   
 `Subscribed (mid: 1): 0`   

* Senden einer Nachricht.   
  Zu diesem Zweck muss ein zweites Konsolenfenster (zB durch Dr&uuml;cken von &lt;Alt&gt;&lt;F2&gt;) oder eine zweite Datenverbindung mittels putty ge&ouml;ffnet werden. In dieses Fenster gibt man folgenden Befehl ein:   
  `mosquitto_pub -d -r -t Test1 -m 'Hallo vom Publisher!'`   
  `Client mosqpub/3194-raspberryp sending CONNECT`   
  `Client mosqpub/3194-raspberryp received CONNACK`   
  `Client mosqpub/3194-raspberryp sending PUBLISH (d0, q0, r0, m1, 'Test1', ... (20 bytes))`   
  `Client mosqpub/3194-raspberryp sending DISCONNECT`   

Sobald man die Nachricht abschickt, erscheint sie im Subscriber-Fenster :)   
Die Option `-d` bedeutet "Enable debug messages", dh. alle Meldungen werden ausgegeben. Dies bewirkt auch, dass jede Minute die PINGREQ-Nachricht angezeigt wird. M&ouml;chte man nur die Nachrichten sehen, so muss man -d weglassen ;)   
Der Schalter `-r` bewirkt, dass die letzte Nachricht im Broker zwischengespeichert wird und so einem Client "nachgeschickt" werden kann, wenn dieser gerade offline war.   
   
Eine detaillierte Dokumentation zu Mosquitto findet man unter [http://mosquitto.org/man/](http://mosquitto.org/man/).   

### Mosquitto-Installation auf dem PC/Laptop
Das Installationsprogramm f&uuml;r Windows (und andere Betriebssysteme) kann von folgender Seite heruntergeladen werden:   
[`https://mosquitto.org/download/`](https://mosquitto.org/download/)


### RasPi: Anzeige einer vom Laptop/PC gesendeten Nachricht
1. In einer Konsole oder in einem putty-Fenster einen Subscriber am RasPi starten, der auf alle Nachrichten horcht:   
```mosquitto_sub -t "#" -v```

2. Am PC ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
```cmd.exe```
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
```c:```   
```cd /programme/mosquitto```

3. Nachricht vom PC-Eingabeaufforderungs-Fenster senden   
```mosquitto_pub -h 192.168.1.2 -t Test1 -m "Hallo vom PC!"```   
Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

Im Subscribe-Fenster (Putty- oder RasPi-Konsole) erscheint (eventuell nach kurzer Zeit) die entsprechende Nachricht:<br>
```
pi_@raspi:~ $ mosquitto_sub -t "#" -v
Test1 Hallo vom PC!
```

## Hilfreiches zu Mosquitto

* Broker stoppen   
  `sudo /etc/init.d/mosquitto stop`   
  `[ ok ] Stopping mosquitto (via systemctl): mosquitto.service.`   

* Broker starten   
  `sudo /etc/init.d/mosquitto start`   
  `[ ok ] Starting mosquitto (via systemctl): mosquitto.service.`   

* Alle Topics und deren Wert ("Payload") anzeigen   
  `mosquitto_sub -t "#" -v`   
  `Test1 D1mini message #1971`   
  `button 1`   
  `counter 21`   
  `-t "#"` bedeutet alle Topics und deren Untertopics anzeigen,   
  `-v` bedeutet, auch das Topic anzeigen.   

* Pr&uuml;fen, ob Mosquitto l&auml;uft   
  `systemctl status mosquitto.service`   

Im Normalfall wird der MQTT-Server nach der Installation so eingerichtet, dass er bei jedem Neustart automatisch gestartet wird. Sollte dies nicht der Fall sein, kann man den Startbefehl f&uuml;r Mosquitto bei jedem Systemstart automatisch ausf&uuml;hren, indem man die Zeile   
`sudo /etc/init.d/mosquitto start &`   
in die Autostart-Datei einf&uuml;gt.
(Das Et-Zeichen & ist Absicht und bedeutet, dass die Ausf&uuml;hrung der Autostart-Datei weitergehen soll.)