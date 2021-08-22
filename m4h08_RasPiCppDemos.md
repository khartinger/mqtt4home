Letzte &Auml;nderung: 22.8.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Hilfreiche Einzel-Programme in C++</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h08_RasPiCppDemos_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
* Vorhandene Einzel-Programme in C++ kompilieren, starten und testen k&ouml;nnen.   
* Eigene Programme mit Hilfe der Vorlage `m4hBase` erstellen.   
   

## Erforderliche Hilfsmittel
* Hardware: RasPi
* Software: Internetzugang zu GitHub
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop
* Software: Eventuell [WinSCP](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   
   

## &Uuml;bersicht
Auf GitHub sind im Verzeichnis [mqtt4home/source_RasPi](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi) einige C++ Beispiele f&uuml;r folgende Aufgaben verf&uuml;gbar:   
* `m4hBrokertime`: Senden der aktuellen Broker-Zeit, wenn eine bestimmte Nachricht empfangen wurde.   
* `m4hLogM`: Mitschreiben aller MQTT-Nachrichten in Dateien   
* `m4hLog2`: Mitschreiben aller MQTT-Nachrichten in Dateien und zus&auml;tzliches Abspeichern der jeweils letzten Nachricht in einer eigenen Datei (f&uuml;r eine schnellere Weiterverarbeitung)    
* `m4hSubPub`: Nach dem Empfang einer Nachricht wird eine angegebene Zeit gewartet und danach eine Antwort-Nachricht gesendet.   
* `m4hSms`: Senden und Empfangen von SMS mit MQTT-Nachrichten   
* `m4hXxx`: Vorlage zur Erstellung eigenen Anwendungen   
   
## Beispiel: Erstellen eines Hilfsprogramms
Die Vorgangsweise zum Erstellen eines Programmes soll am Beispiel `m4hBrokertime` gezeigt werden. Die Arbeiten k&ouml;nnen direkt auf dem RasPi oder &uuml;ber `putty` durchgef&uuml;hrt werden.   

1. Erstellen des Projekverzeichnisses auf dem RasPi   
```mkdir ~/m4hBrokertime```   

2. Wechseln in dieses Verzeichnis:   
```cd ~/m4hBrokertime```

3. Kopieren der Projektdateien von GitHub   
(Dateien: `C_Brokertime.hpp`, `m4h.conf`, `mh4Base.cpp`, `m4hBase.h`, `m4hExtension.hpp`, `m4hMain.cpp`)

4. Erstellen der ausf&uuml;hrbaren Datei   
```g++ m4hMain.cpp m4hBase.cpp -o m4hBrokertime -lmosquitto -lpthread```   
_Anmerkung 1_: Im Verzeichnis `~/m4hBrokertime` wurde die Datei `m4hBrokertime` erzeugt.   
_Anmerkung 2_: Die Befehlszeile, mit der die ausf&uuml;hrbare Datei erzeugt wird, steht immer in der Datei `m4hExtension.hpp` und `C_Xxx.hpp`   
   
5. Testen bzw. Ausf&uuml;hren der Datei:   
```./m4hBrokertime``` 

6. Bereitstellung der Datei f&uuml;r alle User   
Soll die Datei von jedem User und aus jedem Verzeichnis heraus verwendet werden k&ouml;nnen, sind folgende Schritte erforderlich:   
```sudo cp ~/m4hBrokertime/m4hBrokertime /usr/local/bin```   
```sudo chown root /usr/local/bin/m4hBrokertime```   
```sudo chmod 777 /usr/local/bin/m4hBrokertime```   

7. Automatisches Starten der Datei beim RasPi-Start   
* Datei /usr/local/bin/autostart.sh &ouml;ffnen:   
`sudo nano /usr/local/bin/autostart.sh`   
* Zwischen den printf-Anweisungen einf&uuml;gen:   
`/usr/local/bin/m4hBrokertime &`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
(siehe auch Abschnitt "Eigene Autostart-Datei `autostart.sh`" in [m4h01_RasPiInstall.md](https://github.com/khartinger/mqtt4home/blob/main/m4h01_RasPiInstall.md) )
