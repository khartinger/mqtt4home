Letzte &Auml;nderung: 20.09.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1><i>RasPi m4hLcd</i>: &Auml;ndern der LCD-Helligkeit durch eine MQTT-Nachricht</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# Worum geht es?
Mit Hilfe des Programms `m4hLcd` kann die Helligkeit eines Rasperry Pi LCDs durch einen MQTT-Befehl ver&auml;ndert werden.   

## Wie erfolgt die LCD Helligkeitseinstellung?
Dazu wird ein Helligkeitswert von 12 bis 255 in die Datei   
`/sys/class/backlight/rpi_backlight/brightness`   
geschrieben.   
__*Wichtig:*__ Die oben genannte Datei muss f&uuml;r alle User zug&auml;nglich sein. Dies erfolgt durch auf dem Raspi folgende Linux-Befehle:   
`sudo chown root /sys/class/backlight/rpi_backlight/brightness`   
`sudo chmod 777 /sys/class/backlight/rpi_backlight/brightness`   
`sudo chmod u+s /sys/class/backlight/rpi_backlight/brightness`   

## MQTT-Befehle
Das Setzen des Helligkeitswertes (zB auf 64) erfolgt mit Hilfe des folgenden MQTT-Befehls:   
`mosquitto_pub -t lcd/set/brightness -m 64`   
Als Antwort erh&auml;lt man entweder   
`lcd/ret/brightness 64`   
oder, falls der Wert (zB 1234) nicht gesetzt werden konnte:   
`lcd/ret/brightness 1234_not_set!_(12...255)`   
   
Weiters kann der aktuelle Helligkeitswert folgenderma&szlig;en abgefragt werden:   
`mosquitto_pub -t lcd/get -m brightness`   
Ergebnis:   
`lcd/ret/brightness 64`   

__*&Auml;ndern der Topics*__   
Alle Topics k&ouml;nnen in der Konfigurationsdatei `m4h.conf` ge&auml;ndert werden, zB   
```   
[lcd]
getin:  info/lcd/get
getout: info/lcd/ret
setin:  info/lcd/set
setout: info/lcd/ret
```   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Welche Hilfsmittel ben&ouml;tige ich f&uuml;r dieses Projekt?](#a10)   
2. [Wie verwende ich dieses Programm?](#a20)   
3. [Welche Konfigurationsm&ouml;glichkeiten gibt es?](#a30)   
4. [Wie &uuml;bersetze und teste ich das Programm?](#a40)   
5. [Wie ist das Programm codiert?](#a90)   

<a name="a10"></a>[_Zum Seitenanfang_](#up)   

# Welche Hilfsmittel ben&ouml;tige ich f&uuml;r dieses Projekt?
Theoretisch k&ouml;nnten alle Arbeitsschritte auf dem RasPi durchgef&uuml;hrt werden, praktisch ist das Verwenden eines PC/Laptop sinnvoll. Daher wird folgendes ben&ouml;tigt:   

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
Liegt das Programm bereits kompiliert im Verzeichnis `~/m4hLcd` vor, muss man in dieses Verzeichnis wechseln und das Programm durch folgende Eingabe in putty oder einer RasPi-Konsole starten:   
`cd ~/m4hLcd`   
`./m4hLcd`   
_Anmerkung_: Zum Erzeugen der ausf&uuml;hrbaren Datei siehe Kapitel "[Wie &uuml;bersetze und teste ich das Programm?](#a40)".

Nach dem Einlesen der Konfigurationsdatei (Vorgabe `./m4h.conf`) wird die Verbindung zum Broker hergestellt:   

```   
Read config file ./m4h.conf: OK
=====[base]===========================
config file         | ./m4h.conf
all keys            | versionin|versionout|mqttstart|mqttend|progend|readconfin|readconfout|addtime
version (in)        | -t m4hLcd/get -m version
version (out)       | -t m4hLcd/ret/version -m 2022-02-10
mqtt @ start (out,*)| -t info/start -m m4hLcd
mqtt @ end (out,*)  | -t info/end__ -m m4hLcd
progend by mqtt (in)| -t m4hLcd/set/end -m ..true..
reload conf-file(in)| -t m4hLcd/set/conf -m ./m4h.conf
reload conf-fil(out)| -t m4hLcd/ret/conf -m Reload Config file:
         * add time | true
=====[lcd]==============================
config file         | ./m4h.conf
all keys            | getin|getout|setin|setout
.....lcd: get message(s)..............
get topic (in)      | info/lcd/get
get possible payload| ?|help|brightness
get return topic    | info/lcd/ret/
.....lcd: set message(s)..............
set topic base      | info/lcd/set
set possible values | brightness
set return topic    | info/lcd/ret/
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

Das Programm wartet jetzt auf eine get- oder set-Nachricht...   
Programmende mit &lt;strg&gt;c   

<a name="a30"></a>[_Zum Seitenanfang_](#up)   

# Welche Konfigurationsm&ouml;glichkeiten gibt es?   

Folgende Schl&uuml;sselw&ouml;rter sind m&ouml;glich:   
`getin|getout|setin|setout`   

## getin
Angabe des Topics f&uuml;r den get-Befehl.   

_Beispiel:_   
`getin:  info/lcd/get`   

## getout
Angabe des Topics f&uuml;r die Antwort des get-Befehls.   

_Beispiel:_   
`getout:  info/lcd/ret`   

## setin
Angabe des Topics f&uuml;r den set-Befehl.   

_Beispiel:_   
`setin:  info/lcd/set`   

## setout
Angabe des Topics f&uuml;r die Antwort des set-Befehls. Dieses Topic ist meist gleich wie das f&uuml;r die get-Antwort.   

_Beispiel:_   
`setout:  info/lcd/ret`   

<a name="a40"></a>[_Zum Seitenanfang_](#up)   

# Wie &uuml;bersetze und teste ich das Programm?
Befinden sich bereits alle Dateien des Projekts im Verzeichnis `~/m4hLcd`, muss lediglich die ausf&uuml;hrbare Datei durch folgende Eingabe in putty oder einer RasPi-Konsole erzeugt werden:   
`g++ m4hMain.cpp m4hBase.cpp -o m4hLcd -lmosquitto -lpthread`   

Ist dies nicht der Fall, so erfolgt das &Uuml;bersetzen des C++ Hilfsprogramms wie in der Anleitung ["RasPi: Welche C++ Hilfsprogramme gibt es und wie bekomme ich sie zum Laufen?"](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30) beschrieben.   
Linkadresse: [https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30)   

Zum Testen des Programms kann Punkt ["Wie verwende ich dieses Programm?"](#a20) dieser Anleitung verwendet werden.   

<a name="a90"></a>[_Zum Seitenanfang_](#up)   

# Wie ist das Programm codiert?   
Eine &Uuml;bersicht &uuml;ber die Dateien bietet das folgende Bild, wobei `Xxx` durch `Lcd` zu ersetzen ist ;)   
![m4hBase files](./images/rpi_m4hBase_files2.png "m4hBase files")   
_Bild 1: Dateien zur Erstellung eines C++ Hilfsprogramme_   

F&uuml;r die Codierung der Datei `C_Lcd.hpp` siehe [https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLcd/C_Lcd.hpp](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLcd/C_Lcd.hpp)   

[_Zum Seitenanfang_](#up)   
