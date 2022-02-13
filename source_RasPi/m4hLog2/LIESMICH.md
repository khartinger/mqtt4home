Letzte &Auml;nderung: 13.02.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1><i>RasPi m4hLog2</i>: Schreiben von MQTT-Nachrichten in Dateien (Log-Files)</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# Worum geht es?
Das Programm `m4hLog2` schreibt MQTT-Nachrichten in Dateien. Dabei werden in getrennten Verzeichnissen zwei Dateiarten angelegt:   
1. Log-Datei: Diese Dateien werden für jedes Topic getrennt angelegt und monatlich neu erstellt.   
  Sie dokumentieren, wann, wie oft und mit welcher Payload Nachrichten verschickt wurden.   
2. Last-Datei: Hier wird pro Topic eine Datei angelegt, die nur das zuletzt empfangene Topic enthält. Über das Änderungsdatum der Datei kann ermittelt werden, wann das Topic zuletzt gesendet wurde. Weiters ist ein einfacher Zugriff für andere Programme (zB PHP-Webseiten) möglich.   

Der Dateiname besteht bei Log-Dateien aus dem Topic + Punkt + Jahr + Monat + ".log", wobei Leerzeichen in Topics durch Unterstriche und Schrägstriche durch das At-Zeichen ersetzt werden. Last-Dateinamen bestehen nur aus dem Topic.   

_Beispiel_:   
Die im Februar 2022 versendete Nachricht `-t z2m/ret/health -m Zigbee2mqtt-health is perfect.` wird folgendermaßen abgespeichert:   

Log-Datei:   
* Dateiname:   `z2m@ret@health.2202.log`   
* Dateiinhalt: `11.02.22 09:16:16 | z2m@ret@health |  Zigbee2mqtt-health is perfect.`   
* Standardverzeichnis: `./log/`   

Last-Datei:   
* Dateiname:   `z2m@ret@health`   
* Dateiinhalt: ` Zigbee2mqtt-health is perfect. `   
* Standardverzeichnis: `./data/`   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Welche Hilfsmittel benötige ich für dieses Projekt?](#a10)   
2. [Wie verwende ich dieses Programm?](#a20)   
3. [Welche Konfigurationsmöglichkeiten gibt es?](#a30)   
4. [Wie übersetze und teste ich das Programm?](#a40)   
5. [Wie ist das Programm codiert?](#a90)   

<a name="a10"></a>[_Zum Seitenanfang_](#up)   

# Welche Hilfsmittel benötige ich für dieses Projekt?
Theoretisch könnten alle Arbeitsschritte auf dem RasPi durchgeführt werden, praktisch ist das Verwenden eines PC/Laptop sinnvoll. Daher wird folgendes benötigt:   

* Hardware: Raspberry Pi als Access Point (WLAN Raspi11, PW 12345678) mit der IP 10.1.1.1, auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)   
---   
* Hardware: PC oder Laptop mit Internetzugang, Browser   
* Software: Terminal-Programm [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
---   
* Software: Visual Studio Code ("VSC"), das f&uuml;r C++-Anwendungen vorbereitet ist.   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi.   
* Software: Die MQTT-Klient-Programme `mosquitto_sub` und ` mosquitto_pub` (auf dem PC oder RasPi installiert) zum Testen des Programms.   

<a name="a20"></a>[_Zum Seitenanfang_](#up)   

# Wie verwende ich dieses Programm?
## Standardfunktion
Liegt das Programm bereits kompiliert im Verzeichnis `~/m4hLog2` vor, muss es lediglich durch folgende Eingabe in putty oder einer RasPi-Konsole gestartet werden:   
`~/m4hLog2/m4hLog2`   
_Anmerkung_: Zum Erzeugen der ausführbaren Datei siehe Kapitel [Wie übersetze und teste ich das Programm?](#a40).

Nach dem Einlesen der Konfigurationsdatei (Vorgabe `./m4h.conf`) wird die Verbindung zum Broker hergestellt:   

```   
Read config file ./m4h.conf: OK
=====[base]===========================
config file         | ./m4h.conf
all keys            | versionin|versionout|mqttstart|mqttend|progend|readconfin|readconfout|addtime
version (in)        | -t m4hLog2/get -m version
version (out)       | -t m4hLog2/ret/version -m 2022-02-13
mqtt @ start (out,*)| -t info/start -m m4hLog2
mqtt @ end (out,*)  | -t info/end__ -m m4hLog2
progend by mqtt (in)| -t m4hLog2/set -m ...end...
reload conf-file(in)| -t m4hbase/set/conf -m ./m4h.conf
reload conf-fil(out)| -t m4hbase/ret/conf -m Read config:
         * add time | true
=====[log2]==============================
config file         | ./m4h.conf
all keys            | pathlog|pathlast|skiplog|timein|timeout
pathlog             | ./log/
pathlast            | ./last/
timein              | m4hLog2/get/time
timeout             | m4hLog2/ret/time
file time format    | %Y%m%d %H%M%S
.....log: topics to skip..............
z2m/bridge/info,z2m/bridge/devices,z2m/bridge/logging
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

Das Programm schreibt ab jetzt empfangene Nachrichten in die oben beschriebenen Dateien.   
Programmende mit &lt;strg&gt;c   

## Abfrage des letzten Datum+Uhrzeit-Wertes einer Nachricht
1. Am PC ein Terminalfenster (`cmd.exe`) oder putty-Fenster oder am RasPi eine Konsole &ouml;ffnen in das Mosquitto-Verzeichnis wechseln und das Programm zum Anzeigen von MQTT-Nachrichten starten:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   
2. Am PC ein zweites Terminalfenster (`cmd.exe`) oder putty-Fenster oder am RasPi eine zweite Konsole &ouml;ffnen in das Mosquitto-Verzeichnis wechseln.   
Zum Testen kann man zuerst eine MQTT-Nachricht versenden, zB Abfrage der Version des Programms:   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get -m version`   
3. Im zweiten Konsolen-Fenster die Nachricht für die Abfrage des Datums und der Uhrzeit abschicken:   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get/time -m m4hLog2/get`   
5. Im ersten Konsolen-Fenster sieht man folgende Nachrichten:   
  `m4hLog2/get/time m4hLog2/get`   
  `m4hLog2/ret/time 20220213 120500 | m4hLog2/get`   
  Das bedeutet: Die Payload enthält das Datum (`20220213`), die Uhrzeit (`120500`) und das Topic (`m4hLog2/get`), für das die Werte abgefragt wurden.   

<a name="a30"></a>[_Zum Seitenanfang_](#up)   

# Welche Konfigurationsmöglichkeiten gibt es?   

Folgende Schlüsselwörter sind möglich:   
`pathlog|pathlast|skiplog|timein|timeout`   

## pathlog
Angabe des Verzeichnisses (Pfades) für die Log-Dateien (History-Dateien).   

_Beispiel:_   
`pathlog:  ./log/`   

## pathlast
Angabe des Verzeichnisses (Pfades) für die Last-Dateien.   

_Beispiel:_   
`pathlast: ./last/`   

## skiplog
Angabe aller Topics, für die keine Log-Dateien (History) erstellt werden sollen. Die Topics werden durch Beistriche getrennt.   
Die Last-Dateien werden für diese Topics trotzdem erstellt, da man sonst das letzte Datum nicht abfragen könnte.   

_Beispiel:_   
`skiplog: z2m/bridge/info,z2m/bridge/devices,z2m/bridge/logging`   

## timein
Topic für die Abfrage des letzten Datum+Zeit-Wertes einer Nachricht. Das Topic der abzufragenden Zeit muss in der Abfrage-Nachricht als Payload angegeben werden.   

_Beispiel:_   
`timeIn:   m4hLog2/get/time`   
Abfrage-Nachricht insgesamt zB   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get/time -m info/start`   

## timeout
Topic zum Senden der Antwort zur letzten Datum+Zeit-Abfrage einer Nachricht. Als zusätzlicher Wert kann ein Zeitformat angegeben werden.   
Wird kein Zeitformat angegeben, wird das Vorgabe-Format aus `C_Log2.hpp` verwendet:   
`#define  LOG2_TIMEFORMAT     "%Y%m%d %H%M%S"`   

_Beispiele:_   
`timeOut:  m4hLog2/ret/time`   
oder   
`timeOut:  m4hLog2/ret/time %d.%m.%Y %H:%M:%S`   

<a name="a40"></a>[_Zum Seitenanfang_](#up)   

# Wie übersetze und teste ich das Programm?
Befinden sich bereits alle Dateien des Projekts im Verzeichnis `~/m4hLog2`, muss lediglich die ausführbare Datei durch folgende Eingabe in putty oder einer RasPi-Konsole erzeugt werden:   
`g++ m4hMain.cpp m4hBase.cpp -o m4hLog2 -lmosquitto -lpthread -std=c++17`   

Ist dies nicht der Fall, so erfolgt das Übersetzen des C++ Hilfsprogramms wie in der Anleitung ["RasPi: Welche C++ Hilfsprogramme gibt es und wie bekomme ich sie zum Laufen?"](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30) beschrieben.   
Linkadresse: [https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30)   

Zum Testen des Programms kann Punkt ["Wie verwende ich dieses Programm?"](#a20) dieser Anleitung verwendet werden.   

<a name="a90"></a>[_Zum Seitenanfang_](#up)   

# Wie ist das Programm codiert?   
Eine Übersicht über die Dateien bietet das folgende Bild, wobei `Xxx` durch `Log2` zu ersetzen ist ;)   
![m4hBase files](./images/rpi_m4hBase_files2.png "m4hBase files")   
_Bild 1: Dateien zur Erstellung eines C++ Hilfsprogramme_   

Für die Codierung der Datei `C_Log2.hpp` siehe [https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLog2/C_Log2.hpp](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLog2/C_Log2.hpp)   

[_Zum Seitenanfang_](#up)   
