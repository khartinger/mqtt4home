Letzte &Auml;nderung: 11.10.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi als MQTT-Broker</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
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
1. Paketquellen aktualisieren   
   ```
   sudo apt update
   ```
2. Mosquitto Server installieren   
   ```
   sudo apt install -y mosquitto
   ```
3. Mosquitto Clients installieren (Publisher und Subscriber)   
   ```
   sudo apt install -y mosquitto-clients
   ```
4. Mosquitto C-Bibliothek installieren   
   ```
   sudo apt install -y libmosquitto-dev
   ```
5. Mosquitto beim Start automatisch aktivieren   
   ```
   sudo systemctl enable mosquitto
   ```

### Konfigurationsdatei erstellen
Zum Testen vom PC aus muss eine Konfigurationsdatei erstellt werden, da `mosquitto` standardmäßig lokal läuft.   
```
sudo nano /etc/mosquitto/conf.d/default.conf
```
Inhalt:   
```
listener 1883
protocol mqtt
listener 1884
protocol websockets
allow_anonymous true
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

### Test der MQTT Installation auf dem RasPi
Zum Testen der Installation ben&ouml;tigt man drei Dinge:   
1. Ein laufendes Programm, das Nachrichten weiterschickt ("Broker").
2. Einen Nachrichten-Sender ("Publisher").
3. Einen Nachrichten-Empf&auml;nger ("Subscriber").

Dazu geht man folgenderma&szlig;en vor:   
* Zuerst muss der MQTT-Broker gestartet werden:   
  `sudo systemctl start mosquitto`   

* Als N&auml;chstes wird ein Datenempf&auml;nger (Subscriber) eingerichtet, der auf Nachrichten des Typs "test1" h&ouml;rt:   
 `mosquitto_sub -d -t test1`   
 _`Client mosqsub/3152-raspberryp sending CONNECT`_   
 _`Client mosqsub/3152-raspberryp received CONNACK`_   
 _`Client mosqsub/3152-raspberryp sending SUBSCRIBE (Mid: 1, Topic: Test1, QoS: 0)`_   
 _`Client mosqsub/3152-raspberryp received SUBACK`_   
 _`Subscribed (mid: 1): 0`_   

Befindet man sich nicht auf dem Rechner, auf dem der Broker läuft, muss man zusätzlich die IP-Adresse des Servers angeben, zB.   
   `mosquitto_sub -h 10.1.1.1 -d -t test1`   

* Senden einer Nachricht.   
  Zu diesem Zweck muss ein zweites Konsolenfenster (zB durch Dr&uuml;cken von &lt;Alt&gt;&lt;F2&gt;) oder eine zweite Datenverbindung mittels putty ge&ouml;ffnet werden. In dieses Fenster gibt man folgenden Befehl ein:   
   `mosquitto_pub -d -r -t test1 -m 'Hello from Publisher!'`   
   _`Client mosqpub/3194-raspberryp sending CONNECT`_   
   _`Client mosqpub/3194-raspberryp received CONNACK`_   
   _`Client mosqpub/3194-raspberryp sending PUBLISH (d0, q0, r0, m1, 'Test1', ... (20 bytes))`_   
   _`Client mosqpub/3194-raspberryp sending DISCONNECT`_   

Sobald man die Nachricht abschickt, erscheint sie im Subscriber-Fenster :)   
Die Option `-d` bedeutet "Enable debug messages", dh. alle Meldungen werden ausgegeben. Dies bewirkt auch, dass jede Minute die PINGREQ-Nachricht angezeigt wird. M&ouml;chte man nur die Nachrichten sehen, so muss man -d weglassen ;)   
Der Schalter `-r` bewirkt, dass die letzte Nachricht im Broker zwischengespeichert wird und so einem Client "nachgeschickt" werden kann, wenn dieser gerade offline war.   
   
Eine detaillierte Dokumentation zu Mosquitto findet man unter [http://mosquitto.org/man/](http://mosquitto.org/man/).   

### Mosquitto-Installation auf dem PC/Laptop
Das Installationsprogramm f&uuml;r Windows (und andere Betriebssysteme) kann von folgender Seite heruntergeladen werden:   
[`https://mosquitto.org/download/`](https://mosquitto.org/download/)

### RasPi: Anzeige einer vom Laptop/PC gesendeten Nachricht
1. In einer Konsole oder in einem putty-Fenster einen Subscriber am RasPi starten, der auf alle Nachrichten horcht:   
`mosquitto_sub -t "#" -v`

2. Am PC ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
   `cmd.exe`   
   im Startmen&uuml; eingeben.   
   Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
   `c:`   
   `cd /programme/mosquitto`

3. Nachricht vom PC-Eingabeaufforderungs-Fenster senden   
   `mosquitto_pub -h 192.168.1.2 -t Test1 -m "Hallo vom PC!"`   
   Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

Im Subscribe-Fenster (Putty- oder RasPi-Konsole) erscheint (eventuell nach kurzer Zeit) die entsprechende Nachricht:   
```
pi_@raspi:~ $ mosquitto_sub -t "#" -v
Test1 Hallo vom PC!
```

## Mosquitto für Websockets einrichten
Siehe auch: [http://blog.ithasu.org/2016/05/enabling-and-using-websockets-on-mosquitto/](http://blog.ithasu.org/2016/05/enabling-and-using-websockets-on-mosquitto/)
Dazu muss ein weiterer Port (zB 1884) definiert werden, der auf Websocket-Anfragen wartet. Die Einstellungen wurden bereits vorhin in der Konfigurationsdatei `/etc/mosquitto/conf.d/default.conf` durchgeführt.
   
## Hilfreiches zu Mosquitto

* Broker stoppen   
   `sudo /etc/init.d/mosquitto stop`   
   _`[ ok ] Stopping mosquitto (via systemctl): mosquitto.service.`_   

* Broker starten   
   `sudo /etc/init.d/mosquitto start`   
   _`[ ok ] Starting mosquitto (via systemctl): mosquitto.service.`_   

* Alle Topics und deren Wert ("Payload") anzeigen   
   `mosquitto_sub -t "#" -v`   
   _`Test1 D1mini message #1971`_   
   _`button 1`_   
   _`counter 21_`   
   `-t "#"` bedeutet alle Topics und deren Untertopics anzeigen,   
   `-v` bedeutet, auch das Topic anzeigen.   

* Pr&uuml;fen, ob Mosquitto l&auml;uft   
  `systemctl status mosquitto.service`   

Im Normalfall wird der MQTT-Server nach der Installation so eingerichtet, dass er bei jedem Neustart automatisch gestartet wird. Sollte dies nicht der Fall sein, kann man den Startbefehl f&uuml;r Mosquitto bei jedem Systemstart automatisch ausf&uuml;hren, indem man die Zeile   
`sudo /etc/init.d/mosquitto start &`   
in die Autostart-Datei einf&uuml;gt.
(Das Et-Zeichen & ist Absicht und bedeutet, dass die Ausf&uuml;hrung der Autostart-Datei weitergehen soll.)

* L&ouml;schen einer bestimmten auf dem Broker gespeicherten Nachricht (retain)   
  Senden einer Nachricht mit dem entsprechenden Topic und den Schaltern `-n -r -d` (statt der Payload `-m`)   
  _Beispiel_: L&ouml;schen der Nachricht `test1`   
  `mosquitto_pub -h 10.1.1.1 -t test1 -n -r -d`   
&nbsp;   
---   

## Hilfreiche Links zu MQTT
* L&ouml;schen __aller__ auf dem Broker gespeicherten Nachrichten (retain):   
[https://community.openhab.org/t/clearing-mqtt-retained-messages/58221](https://community.openhab.org/t/clearing-mqtt-retained-messages/58221)   
```
sudo service mosquitto stop   
sudo rm /var/lib/mosquitto/mosquitto.db   
sudo service mosquitto start   
```
_Anmerkung_: Alle Nachrichten sind in der Datei `/var/lib/mosquitto/mosquitto.db` gespeichert.

* Dokumentation zu libmosquitto   
[https://mosquitto.org/api/files/mosquitto-h.html](https://mosquitto.org/api/files/mosquitto-h.html)

