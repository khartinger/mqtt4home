Letzte &Auml;nderung: 12.10.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Periodisches Senden von MQTT-Nachrichten</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h104_RasPi_crontab_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
* Versenden von MQTT-Nachrichten zu bestimmten Zeitpunkten.

## Ben&ouml;tigte Hilfsmittel
* Hardware: RasPi
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop

## Realisierung
Zur periodischen Ausf&uuml;hrung von Aktionen stellt das Betriebssystem Linux den Dienst `crontab` zu Verf&uuml;gung, der automatisch Skripte oder Programme starten kann.   
Es bietet sich daher an, diesen Dienst zum Senden von MQTT-Nachrichten zu verwenden.   
Zahlreiche Informationen zu `crontab` findet man im Internet, zB   
* [https://wiki.ubuntuusers.de/Cron/ (2021-10-12)](https://wiki.ubuntuusers.de/Cron/)   
* [https://www.stetic.com/developer/cronjob-linux-tutorial-und-crontab-syntax/ (2021-10-12)](https://www.stetic.com/developer/cronjob-linux-tutorial-und-crontab-syntax/)   
* [https://crontab.guru/ (2021-10-12)](https://crontab.guru/)   

### Beispiel "Rollo"
* Das &Ouml;ffnen einer Rollo soll jeden Tag um 07:10 Uhr durch den MQTT-Befehl `rollo/motor` mit der der Payload `1` gestartet werden.   
* Um 19:30 Uhr soll die Rollo heruntergelassen werden (gleiches Topic, Payload `-1`).   
* Eine Minute nach dem Start einer Aktion soll der Motor wieder gestoppt werden (Payload `0`).   

Annahme: Das Senden von MQTT-Befehlen erfolgt mit dem Programm `mosquitto_pub`   
### L&ouml;sung
* Crontab starten (Parameter e = edit):   
   `crontab -e`
* Eintragen folgender Befehle in die Tabelle:   
```   
10  7 * * * mosquitto_pub -t rollo/motor -m 1
11  7 * * * mosquitto_pub -t rollo/motor -m 0
30 19 * * * mosquitto_pub -t rollo/motor -m -1
31 19 * * * mosquitto_pub -t rollo/motor -m 0
```   
* Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
