Letzte Änderung: 3.2.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Raspberry Pi: Zeitliche Überwachung von MQTT-Sensoren</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# Worum geht es?
Viele Sensoren senden Messwerte und gehen dann in einen Sleep-Modus, um Energie zu sparen. Bei batteriebetriebenen Sensoren geht irgendwann die Energie zu Ende und sie können nichts mehr senden oder sie verlieren die Netzwerkverbindung.   
Das Projekt `m4hWdog` ("Watchdog") hilft, diesen Fehlerfall zu erkennen. Es schickt eine MQTT-Nachricht, wenn ein Sensor innerhalb einer vorgegebenen Zeit keine Nachricht mehr gesendet hat.   
In einer Konfigurationsdatei wird festgelegt, innerhalb welcher Zeitspanne eine Nachricht von einem bestimmten Topic eintreffen muss.   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Welche Dinge ben&ouml;tige ich für dieses Projekt?](#a10)   
2. [Wie verwende ich dieses Programm?](#a20)

99. [Wie würde ich selbst dieses Programm erstellen?](#a99)   

<a name="a10"></a>[Zum Seitenanfang](#up)   
# Welche Dinge ben&ouml;tige ich für dieses Projekt?
* Hardware: PC oder Laptop mit Internetzugang, Browser   
* Hardware: Raspberry Pi als Access Point (WLAN Raspi11, PW 12345678) mit der IP 10.1.1.1, auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)   
* Software: Visual Studio Code ("VSC"), das f&uuml;r C++-Anwendungen vorbereitet ist.   
* Software: Terminal-Programm [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   
* Software: Die MQTT-Klient-Programme `mosquitto_sub` und ` mosquitto_pub` (auf dem PC oder RasPi installiert)   

<a name="a20"></a>[Zum Seitenanfang](#up)   
# Wie verwende ich dieses Programm?   
Die Registrierung der zu überwachenden Sensoren erfolgt in der Konfigurationsdatei im Abschnitt `[wdog]`. Für jeden Sensor muss eine Zeile mit dem Key "in:" und dem Wert Topic + Leerzeichen + maximale Antwortzeit im Format `[HHHH]H:MM:SS` angelegt werden, wobei die Stundenzahl (nahezu) beliebig sein kann, zb 0 bis zu 8760 (= 365*24 = 1 Jahr) oder mehr.   
_Beispiel_: Der Sensor `z2m/schalter_2` soll mindestens 1x am Tag betätigt werden. Eintrag in der Konfigurationsdatei:   
`[wdog]`   
`in:  z2m/schalter_2 24:00:00`   

Mit dem Key `out` kann festgelegt werden, unter welchem Topic die Warnung verschickt werden soll.   
Beispiel:   
`out: m4hWdog/attention Sensor <in> missing!`   
Der Platzhalter `<in>` wird durch den entsprechenden Sensornamen ersetzt.   

<a name="a99"></a>[Zum Seitenanfang](#up)   
# Wie würde ich selbst dieses Programm erstellen?   
## Anlegen des Projektes
1. Windows: Anlegen des Projektordners `m4hWdog`   
2. Windows: Kopieren der Dateien der Vorlage `m4hWdog` in den Ordner `m4hWdog`   
3. Visual Studio Code (VSC) starten   
4. In VSC den Projektordner öffnen   
   Menü Datei - Ordner öffnen... : Im Dialog in den Ordner wechseln und [Ordner auswählen] drücken   
5. Ändern des Dateinamens `C_Wdog.hpp` in `C_Wdog.hpp`   
   Im Explorer-Fenster mit der rechten Maustaste auf `C_Wdog.hpp` klicken, "Umbenennen" wählen und `C_Wdog.hpp` eingeben   
6. In allen Dateien die Namen `WDOG`, `Wdog` und `wdog` in `WDOG`, `Wdog` und `wdog` umbenennen.   
   Menü Bearbeiten - In Dateien ersetzen   
   __WICHTIG__: IM Suchen-Eingabefeld das Symbol "Aa" aktivieren, damit die Groß-/Kleinschreibung berücksichtigt wird!   
   Es werden ca. 23 Ersetzungen in 2 Dateien bzw. 35 Ersetzungen in 4 Dateien und 9 Ersetzungen in 4 Dateien durchgeführt.   
7. Formatierungen stc. anpassen (falls gewünscht)   
   * Da die neuen Bezeichnuntgen `Wdog` um einen Buchstaben mehr haben als `Xxx` sind auch die Spalten bei Kommentaren um eine Stelle verschoben...   
   * Datum ausbessern auf das aktuelle Datum (Datei `C_Wdog.hpp`, `m4hExtension.hpp`, `m4h.conf`)   
8. Alle Dateien speichern   
   Menü Datei - Alles speichern

Das Projekt kann bereits auf das RasPi übertragen und dort gestestet werden:   
* RasPi starten, PC mit dem RasPi über WLAN verbinden.   
* Übertragen des Verzeichnisses `m4hWdog` vom PC zum RasPi (mit _WinSCP_)   
* Mittels _Putty_ auf das RasPi zugreifen (User `pi_`, PW `pass`) und in den Projektordner wechseln:   
  `cd ~/m4hWdog`   
* _Putty_: Ausführbares Programm erzeugen:   
  `g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread`   
* _Putty_: Das Programm ausführen:   
  `./m4hWdog`   
* PC: Terminalfenster (`cmd.exe`) öffnen und das Programm zum Anzeigen von MQTT-Nachrichten starten:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* PC: Ein zweites Terminalfenster (`cmd.exe`) öffnen und eine MQTT-Nachricht senden:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/get -m version`   
  Im ersten Terminalfenster erscheinen die Meldungen   
  `m4hWdog/get version`   
  `m4hWdog/ret/version 2022-02-02`   
* PC: Das Programm mittels MQTT-Befehl beenden:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/set -m ...end...`   
  Im ersten Terminalfenster erscheinen die Meldungen   
  `m4hWdog/set ...end...`   
  `info/end__ m4hWdog (02.02.2022 18:13:19)`   
  bzw. in _Putty_ erscheinen die Meldungen   
  `Exit program... MQTT end message sent.`   
  `Program terminated by MQTT (02.02.2022 18:13:19)`   
  `Beendet`   

## Klasse "WdogIn1" für Sensor-Daten
Da für jeden zu überwachenden Sensor ein Topic und eine Zeitdauer gespeichert werden muss, wird dafür eine eigene Klasse `WdogIn1` definiert.   

## Konstante definieren
Um das Programm möglichst flexibel benutzen zu können, wird in der Datei `C_Wdog.hpp` für jeden Schlüssel und jeden Wert der Konfigurationsdatei eine Konstante und eine Variable definiert:   
```   
#define  WDOG_OUT_KEY        "out"
#define  WDOG_OUT            "m4hWdog/attention"
#define  WDOG_IN_KEY         "in"
```   
und in der Klasse `Wdog`   
```   
 std::string wdog_out_key;             // topic out key
 std::string wdog_out;                 // topic out value
 std::string wdog_in_key;              // topic in key
 std::vector<WdogIn1>vIn;              // topic in values
```   

## Einlesen der Konfigurationsdatei (readConfig)   


## Anzeige der eingelesenen Daten (show)   


## Reaktion auf eintreffende Nachrichten (onMessage)





---   
# ..ToDo..   

---    

&nbsp;

---   
---------------------------------
---   
# ---
Möchte man bei der Hausautomation in bestimmten Situationen eine SMS erhalten oder Dinge über SMS steuern, so ist das hier vorgestellte Programm `m4hSms` dafür sehr hilfreich:   
* Das Programm `m4hSms` wandelt SMS in MQTT-Nachrichten um und umgekehrt.   
* Viele Eigenschaften des Programms können in der Konfigurationsdatei (`m4h.conf`) festgelegt werden.   
* Mit ein wenig C++ Kenntnissen kann das Programm einfach an eigene Bedürfnisse angepasst werden.   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Wie werden SMS und MQTT-Nachrichten ineinander umgewandelt?](#a10)   
2. [Welche Hilfsmittel werden benötigt? (Stand August 2021)](#a20)   
3. [Wie verbindet man das SIM-Modul mit dem RasPi?](#a30)   
4. [Wie stellt man fest, an welchem Anschluss das SIM-Modul hängt?](#a40)   
5. [Wie erstellt man das Programm m4hSms?](#a50)   
6. [Wie kann man das Programm konfigurieren?](#a60)   
7. [Wie testet man das Programm m4hSms?](#a70)   

<a name="aa10"></a>[Zum Seitenanfang](#up)   

# Wie werden SMS und MQTT-Nachrichten ineinander umgewandelt?
Das folgende Schema skizziert die Umwandlung von MQTT in SMS und umgekehrt. Die MQTT-Topics zum Senden (bei `sub:`), für die Rückantwort (`subret:`) und für das Empfangen von SMS (bei `pub:`) können frei gewählt werden und stehen in der Konfigurationsdatei `m4h.conf`.   

![m4hSms1](./images/m4hSms_mqtt-sms.png "m4hSms function")   
_Bild 1: Funktionsschema von `m4hSms`_   

## Senden einer MQTT-Nachricht als SMS
* Das __Topic__ zum Senden einer SMS wird in der Konfigurationsdatei `m4h.conf`, Abschnitt `[sms]`, Schlüssel `sub:` definiert (zB `sub: sms/send`).   
* Die __Payload__ zum Senden einer SMS muss aus der Telefonnummer (ohne Leerzeichen!), einem nachfolgenden Leerzeichen (als Trennzeichen) und dem SMS-Text bestehen.   

_Beispiel: Nachricht als SMS versenden_   
* `mosquitto_pub -h 10.1.1.1 -t sms/send -m "+43680WDOGXX This SMS was sent via m4hSMS :-)"`   
  _Anmerkung 1_: Die Telefonnummer (`+43680WDOGXX`) muss durch eine in `m4h.conf` freigegebene Nummer (unter `to:`) ersetzt werden.   
  _Anmerkung 2_: Das Programm `m4hSms` muss auf dem RasPi laufen ;)   

Unter dem Topic `subret:` kommt eine Antwort-Nachricht, ob das Senden erfolgreich war (Payload beginnt mit `SMS sent ...` oder `SMS NOT sent ...`).   

## Umwandeln einer SMS in eine MQTT-Nachricht
* Ein __normaler SMS-Text__ wird unter dem in der Konfigurationsdatei `m4h.conf`, Abschnitt `[sms]`, Schlüssel `pub:` (zB `pub: sms/received`) angegebenen Topic als Nachricht versendet.   
Mit Hilfe des Eintrags `pubNum: true` wird festgelegt, dass auch die Telefonnummer und Datum+Uhrzeit der SMS in der Payload mitgeschickt werden.   

* __MQTT-formatierter SMS-Text__ enthält die Abkürzungen `-t` (für Topic), `-m` (für Nachricht) und optional `-r` (für retain) und wird als entsprechende Nachricht veröffentlicht.   
_Beispiel_:   
`-t info/test -m SMS test message`   
ergibt eine Nachricht mit dem Topic `info/test` und der Payload `SMS test message`.   
Anmerkung: Die SMS muss von einer in `m4h.conf` freigegebenen Nummer (unter `from:`) kommen.

* Mögliche __Befehle__ müssen in der Konfigurationsdatei `m4h.conf` im Abschnitt `[sms]` angeführt sein. Sie bestehen aus dem Schlüssel (zB `cmdversion:`) und dem SMS-Text für den Befehl (zB `-version-`). Das heißt: Schickt man eine SMS mit dem Text `-version-` an das Programm `m4hSms`, so erhält man eine Antwort-SMS mit der Versionsnummer.   

<a name="aa20"></a>[Zum Seitenanfang](#up)   

# Welche Hilfsmittel werden benötigt? (Stand August 2021)
* _Hardware_: RasPi mit laufenden Broker (zB mosquitto)   
* _Hardware_: ein SIM808 Modul   
  zB SIM808 Modul GSM mit GPS Antenne für 3G 4G SIM Karte zB von [AliExpress](https://de.aliexpress.com/item/1005002384541464.html?spm=a2g0s.9042311.0.0.5c824c4dqUu43E) oder [Amazon](https://www.amazon.de/dp/B09CM8TSX9/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B09CM8TSX9&pd_rd_w=yNPjf&pf_rd_p=4f2ceb27-95e9-46ab-8808-db390b56ec01&pd_rd_wg=Muvvr&pf_rd_r=3NTH9ZQRZNE3VBZKD1YV&pd_rd_r=58ca39f9-b9f0-40b5-9b14-7f5d8a37ce1d&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzN1A5OTA0NUNBTTA2JmVuY3J5cHRlZElkPUEwMzcxNzc3MlkxMFpaUTBTRjhYMSZlbmNyeXB0ZWRBZElkPUEwNzU2MTYzMjRJSlNTREJMTjVHSiZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* _Hardware_: 4 Drähte   
  zB Breadboard Jumper Drähte weiblich auf weiblich 4polig zB von [Amazon](https://www.amazon.de/Female-Female-Male-Female-Male-Male-Steckbr%C3%BCcken-Drahtbr%C3%BCcken-bunt/dp/B01EV70C78/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Breadboard+Jumper+Dr%C3%A4hte&qid=1629911840&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzRVMzTE5FRDJTWjJSJmVuY3J5cHRlZElkPUEwODQwNzA5MUZGVVRTNVE1WDBBTiZlbmNyeXB0ZWRBZElkPUExMDAwNjg1MUFSNzFTUlhGM0hSOSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* _Hardware_: ein USB zu TTL Seriell Adapter   
  DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C mit FTDI FT232RL zB von [Amazon](https://www.amazon.de/gp/product/B07BBPX8B8/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1) oder DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C5 zB von [Amazon](https://www.amazon.de/DSD-TECH-SH-U09C5-Konverterkabel-Unterst%C3%BCtzung/dp/B07WX2DSVB/ref=sr_1_10?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=DSD+TECH+USB+zu+TTL+Seriell+Adapter+Konverter&qid=1629910857&s=computers&sr=1-10)   
* _Hardware_: USB-Verlängerungskabel USB A Stecker auf A Buchse   
  zB [PIPIKA USB 3.0 Verlängerung von Amazon](https://www.amazon.de/Verl%C3%A4ngerung-Verl%C3%A4ngerungskabel-Superschnelle-Vergoldeten-Kartenleseger%C3%A4t/dp/B08BHWJLLS/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=USB+verl%C3%A4ngerungskabel+A-Buchse+1m&qid=1629911382&smid=A3PTYKYXFT73EG&sr=8-4)   
* _Hardware_: SIM-Karte   
zB [in Österreich HOT von Hofer/Aldi](https://www.hot.at/tarife.html?gclid=CjwKCAjw1JeJBhB9EiwAV612y80Gd6MxAkyvJRJ2BAeTtinSp9OnIBslTwBgd_B-iJANhbW7v5TQSBoCckgQAvD_BwE) mit Tarif "HoT flex-unser Basistarif ohne Fixkosten".   
---   
* _Software_: Internetzugang zu GitHub   
* _Software_: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* _Software_: MQTT-Clientprogramme `mosquitto_sub` und `mosquitto_pub`   
  (werden bei der Mosquitto-Broker-Installation mit installiert).   
* _Software_: Eventuell   
   [Visual Studio Code](https://code.visualstudio.com/) und   
   [WinSCP](https://winscp.net/eng/docs/lang:de) zur Datenübertragung vom PC/Laptop zum RasPi   

<a name="a30"></a>[Zum Seitenanfang](#up)   

# Wie verbindet man das SIM-Modul mit dem RasPi?
Zum Senden und Empfangen von SMS ist zumindest ein SIM-Modul und eine SIM-Karte erforderlich. Das SIM-Modul kann entweder
* an die Pins der seriellen Schnittstelle des RasPi angeschlossen werden (Pin 2-5V, Pin 6-GND, Pin 8-TxD0 und Pin 10-RxD0), wobei eine Pegelanpassung 5V/3V3 erforderlich ist, oder   
* über einen USB-Serial-Adapter am RasPi angeschlossen werden.   

Der Anschluss über einen USB-Serial-Adapter ist zwar teurer (da die Kosten für den Adapter anfallen), dafür aber einfacher.   

![Modul SIM808 mit USB-Serial-Adapter](./images/210825_SIM808_480b.png "Modul SIM808 mit USB-Serial-Adapter")   
_Bild 2: USB-Serial-Adapter SH-U09C, Modul SIM808 und GSM-Antenne (von links nach rechts)_   
   
Der Jumper auf dem USB-Serial-Adapter muss auf 5V gesteckt werden, da die Versorgungsspannung des SIM-Moduls zwischen 5 und 18V betragen muss. (3,3V ist zu wenig.)  

Beim Verbinden des SIM-Moduls mit dem USB-Serial-Adapter oder RasPi ist darauf zu achten, dass die Pins TxD mit RxD und RxD mit TxD verbunden werden ("auskreuzen" der Datenleitungen).   

![USB-SIM-connection](./images/m4hSms_connect.png "USB-SIM-connection")   
_Bild 3: Verbindung zwischen USB-Serial-Adapter und SIM-Modul_   

__Nicht vergessen__: Zum Senden und Empfangen von SMS wird eine SIM-Karte benötigt, wobei für viele Anwendungen eine Prepaid-Karte ausreicht, bei der keine Fixkosten anfallen. Die SIM-Karte wird in den SIM-Modul eingelegt (siehe Bild 1, links unten im Modul).   

<a name="a40"></a>[Zum Seitenanfang](#up)   

# Wie stellt man fest, an welchem Anschluss das SIM-Modul hängt?   
Sind mehrere USB-Geräte am RasPi angeschlossen, ist nicht immer klar, mit welchem logischen Port (`/dev/ttyS0`, `/dev/ttyACM0`, `/dev/ttyUSB0`, `/dev/ttyUSB1` usw.) das SIM-Modul verbunden ist. Dies kann man mit Hilfe des Programms [`
m4hFindSimModule`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hFindSimModule) feststellen:   

1. Mit Putty eine Verbindung zum RasPi herstellen.   
2. Auf dem RasPi ein Verzeichnis für das Programm anlegen und hineinwechseln:   
  `mkdir ~/m4hFindSimModule`   
  `cd ~/m4hFindSimModule`   
3. Eine leere Datei erstellen:   
  `nano m4hFindSimModule.cpp`   
4. Den [Quellcode für `m4hFindSimModule`](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hFindSimModule/m4hFindSimModule.cpp) auf Github mit [Raw] &lt;strg&gt; a markieren, mit &lt;strg&gt; c kopieren und in die leere Datei einfügen (rechte Maustaste).   
5. Die Datei speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
6. Die ausführbare Datei erzeugen:   
   `g++ m4hFindSimModule.cpp -o m4hFindSimModule`   
7. Das Programm starten:   
  `~/m4hFindSimModule/m4hFindSimModule`   

Ergebnis (zum Beispiel):   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM-Modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
In diesem Beispiel ist der Device-Name also `/dev/ttyUSB0`.   

&nbsp;   
<a name="a50"></a>[Zum Seitenanfang](#up)   

# Wie erstellt man das Programm m4hSms?
Die Erstellung der ausführbaren Datei erfolgt gleich wie es im Kapitel [Raspberry Pi: Hilfreiche Einzel-Programme in C++](m4h08_RasPiCppDemos.md) beispielhaft beschrieben wurde:   
1. Erstellen eines Projektverzeichnisses `mkdir ~/m4hSms`   
2. Wechseln in das Projektverzeichnis `cd ~/m4hSms`   
3. Kopieren der Projektdateien von GitHub in das Projektverzeichnis   
4. Erstellen der ausführbaren Datei `m4hSms`  
  `g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread`   
5. Starten des Programms   
  `~/m4hSms/m4hSms`   

Beenden des Programms zB mit der Tastenkombination &lt;Strg&gt;c   

<a name="a60"></a>[Zum Seitenanfang](#up)   

# Wie kann man das Programm konfigurieren?

Die Konfiguration des Programms m4hSms erfolgt mit Hilfe der Konfigurationsdatei `m4h.conf`. Diese kann mit dem Texteditor Nano bearbeitet werden, zB   
`nano ~/m4hSms/m4h.conf`   

In der Sektion `[sms]` sind folgende Einträge erforderlich:   

* `device:` Schnittstelle, an der das SIM-Modul angeschlossen ist.   
  Mögliche Werte sind `ttyS0` für die serielle Schnittstelle oder `ttyACM0`, `ttyUSB0`, `ttyUSB1` usw. für die USB-Schnittstelle.   

* `from: ` Auflistung der Telefonnummern, von denen SMS empfangen werden dürfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).

* `to: `  Auflistung der Telefonnummern, zu denen SMS geschickt werden dürfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).

* `sub: ` Topic für das Senden einer SMS. Die Payload muss die Telefonnummer, ein Leerzeichen und dann den gewünschten SMS-Text enthalten.   

* `subret: ` Topic für eine Antwortnachricht nach dem Senden einer SMS.   

* `pub: ` Topic, unter dem ankommende SMS verschickt werden, sofern kein Topic in der SMS angegeben wurde.   
   Aufbau einer SMS mit vorgegebenem Topic: `-t topic -m payload`   
   `-t ` steht für Topic, `-m ` steht für die Payload. Folgt noch ein `-r`, so wird das Retain-Flag gesetzt.   
---   
Folgende Einträge sind optional:   

* `pubNum: ` Gibt an, ob bei einer empfangenen SMS in der Payload zusätzlich zum SMS-Text auch die Telefonnummer und Datum+Uhrzeit angegeben werden soll.   
  Vorgabe: `false` (d.h. nur SMS-Text als Payload verwenden).

* `smsStart: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` gestartet wurde, zB   
  `smsStart: +43..... program m4hSms started!`   
  Für jede Telefonnummer muss eine eigene Zeile verwendet werden.   
  Vorgabe: keine Start-SMS   

* `smsEnd: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` beendet wurde, zB   
  `smsEnd: +43..... program m4hSms finished!`   
  Für jede Telefonnummer muss eine eigene Zeile verwendet werden.   
  Vorgabe: keine End-SMS   

* `cmdversion:` definert einen SMS-Text, der eine Antwort-SMS mit der Versionsnummer des Programms `m4hSms` bewirkt.   

* `cmdend:` definert einen SMS-Text, der das Beenden des Programms `m4hSms` bewirkt.   

* `cmdreload:` definert einen SMS-Text, der das neuerliche Laden der Konfigurationsdatei `m4h.conf` bewirkt.   

* `cmdcredit:` definert einen SMS-Text, der das Abfragen des Guthabens beim Provider bewirkt und den Betrag (in Euro) als Antwort-SMS zurückschickt.   
   
_Anmerkung_: Alle Schlüssel wie `device`, `from`, `to` usw. sind im Quellcode der Datei `C_Sms.hpp` mit   
`#define SMS_..._KEY ...`   
definiert und können leicht an eigene Bedürfnisse angepasst werden.   

### Beispiel für eine Konfigurationsdatei:   

```   
________m4h.conf________________________________khartinger_____
# Configuration file for mqtt4home
# 2021-08-27

[base]
versionIn:  m4hSms/get version
versionOut: m4hSms/ret/version 2021-08-27
mqttStart:  info/start m4hSms
mqttEnd:    info/end__ m4hSms
ProgEnd:    m4hSms/set -end-
addTime:    true

[sms]
# (serial) interface for sim module (ttyS0, ttyUSB0, ttyUSB1)
#device: /dev/ttyS0
device: /dev/ttyUSB0
# authorisized phone numbers
from: 6700,+43.....
to: 6700,+43.....
# mqtt base topics for sending and receiving
# sub-topic with payload num txt --> sms
# --> sms send result published under "subret:"
# sms (cmd, plain)          --> publish with topic pub
# sms (-t topic -m payload) --> publish topic payload
sub: sms/send
subret: sms/send/ret
pub: sms/received
# payload for received sms with phonenumber date+time sms-text
pubNum: true

smsStart: +43..... program m4hSms started!
smsEnd:   +43..... program m4hSms finished!
# sms or mqtt commands to which the program responds
cmdversion: -version-
cmdend:     -end-
cmdreload:  -reload-
cmdcredit:  -credit-
#netid:      T-Mobile A
#netphone:   6700
#nettext:    GUTHABEN
```   

_Anmerkung_: Zeilen mit # am Beginn sind Kommentarzeilen und werden ignoriert.   

<a name="a70"></a>[Zum Seitenanfang](#up)   

# Wie testet man das Programm m4hSms?
## Vorbereitung
### Hardware
* SIM-Modul mit gültiger SIM-Karte am RasPi anschließen ([siehe oben](#30)).   
* Broker auf dem Raspi mit der IP `10.1.1.1` läuft.   
### Software
* Vor dem Start des Programms müssen in die Konfigurationsdatei `m4h.conf` im Abschnitt `[sms]` zumindest die Schnittstelle (`device:`) und die Telefonummern eingetragen werden, die zum Senden (`to:`) und Empfangen (`from:`) von SMS berechtigt sind (siehe auch die [Konfigurationshinweise](#a60) weiter oben).   
* Programm starten:   
  `~/m4hSms/m4hSms`   
* In einem ersten Terminalfenster `mosquitto_sub` starten, um alle Nachrichten mitverfolgen zu können:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   

### SMS senden
In einem (zweiten) Terminalfenster (auf dem PC oder RasPi) folgendes eingeben:   
`mosquitto_pub -h 10.1.1.1 -t sms/send -m "+43... My first SMS-test"`   
_Wichtig_: Die Nummer `+43...` durch eine berechtigte Telefonnummer ersetzen!   

Im ersten Terminalfenster sieht man folgende Nachrichten   
`sms/send +43WDOGWDOGWDOGXX My first SMS-test`   
`sms/send/ret SMS sent (+43WDOGWDOGWDOGXX: My first SMS-test)`   
und man erhält die entsprechende SMS.   

Vergisst man, die Telefonnummer auszubessern, sieht man im ersten Terminalfenster folgende Nachrichten:   
`sms/send +43... My first SMS-test`   
`sms/send/ret SMS NOT sent #6 not allowed (+43...: My first SMS-test)`   

### SMS empfangen
Auf einem Smartphone (mit berechtigter Telefonnummer!) folgende SMS schreiben und an das SIM-Modul schicken:   
`This is a test SMS from my smartphone`   

Nach kurzer Zeit liest man folgende MQTT-Nachricht im ersten Terminfenster:   
`sms/received +43WDOGWDOGWDOGXX\n24.01.2022 11:30:29\nThis is a test SMS from my smartphone`   
_Anmerkung_: In der Konfigurationsdatei steht die Zeile `pubNum: true`, daher wird die Telefonnummer (hier mit WDOG beschreiben) und Datum+Uhrzeit angezeigt.   

Sollte die Vue-Komponente `CiSmsIn` in einem Browser laufen, kann man auch hier die SMS lesen.   

## Guthaben abfragen
Über das Smartphone kann man auch das Guthaben der SIM-Karte im SIM-Modul abfragen. Dazu schickt man an das SIM-Modul eine SMS mit folgenden Inhalt:   
`-credit-`   
Nach kurzer Zeit erhält man eine SMS mit dem entsprechenden Euro-Betrag.   

Im Terminalfenster werden folgende Nachrichten abgezeigt:   
`sms/send/ret SMS sent (6700: GUTHABEN)`   
`sms/send/ret SMS sent (+43WDOGWDOGWDOGXX: 15,08 Euro)`   

[Zum Seitenanfang](#up)   
