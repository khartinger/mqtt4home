Letzte &Auml;nderung: 24.1.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Senden und Empfangen von SMS &uuml;ber MQTT in C++</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h340_RasPiCppSms_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hSms">==> Code @ GitHub</a><hr>

# Worum geht es?
M&ouml;chte man bei der Hausautomation in bestimmten Situationen eine SMS erhalten oder Dinge &uuml;ber SMS steuern, so ist das hier vorgestellte Programm `m4hSms` daf&uuml;r sehr hilfreich:   
* Das Programm `m4hSms` wandelt SMS in MQTT-Nachrichten um und umgekehrt.   
* Viele Eigenschaften des Programms k&ouml;nnen in der Konfigurationsdatei (`m4h.conf`) festgelegt werden.   
* Mit ein wenig C++ Kenntnissen kann das Programm einfach an eigene Bed&uuml;rfnisse angepasst werden.   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Wie werden SMS und MQTT-Nachrichten ineinander umgewandelt?](#a10)   
2. [Welche Hilfsmittel werden ben&ouml;tigt? (Stand August 2021)](#a20)   
3. [Wie verbindet man das SIM-Modul mit dem RasPi?](#a30)   
4. [Wie stellt man fest, an welchem Anschluss das SIM-Modul h&auml;ngt?](#a40)   
5. [Wie erstellt man das Programm m4hSms?](#a50)   
6. [Wie kann man das Programm konfigurieren?](#a60)   
7. [Wie testet man das Programm m4hSms?](#a70)   

<a name="a10"></a>[Zum Seitenanfang](#up)   

# Wie werden SMS und MQTT-Nachrichten ineinander umgewandelt?
Das folgende Schema skizziert die Umwandlung von MQTT in SMS und umgekehrt. Die MQTT-Topics zum Senden (bei `sub:`), f&uuml;r die R&uuml;ckantwort (`subret:`) und f&uuml;r das Empfangen von SMS (bei `pub:`) k&ouml;nnen frei gew&auml;hlt werden und stehen in der Konfigurationsdatei `m4h.conf`.   

![m4hSms1](./images/m4hSms_mqtt-sms.png "m4hSms function")   
_Bild 1: Funktionsschema von `m4hSms`_   

## Senden einer MQTT-Nachricht als SMS
* Das __Topic__ zum Senden einer SMS wird in der Konfigurationsdatei `m4h.conf`, Abschnitt `[sms]`, Schl&uuml;ssel `sub:` definiert (zB `sub: sms/send`).   
* Die __Payload__ zum Senden einer SMS muss aus der Telefonnummer (ohne Leerzeichen!), einem nachfolgenden Leerzeichen (als Trennzeichen) und dem SMS-Text bestehen.   

_Beispiel: Nachricht als SMS versenden_   
* `mosquitto_pub -h 10.1.1.1 -t sms/send -m "+43680XXXXX This SMS was sent via m4hSMS :-)"`   
  _Anmerkung 1_: Die Telefonnummer (`+43680XXXXX`) muss durch eine in `m4h.conf` freigegebene Nummer (unter `to:`) ersetzt werden.   
  _Anmerkung 2_: Das Programm `m4hSms` muss auf dem RasPi laufen ;)   

Unter dem Topic `subret:` kommt eine Antwort-Nachricht, ob das Senden erfolgreich war (Payload beginnt mit `SMS sent ...` oder `SMS NOT sent ...`).   

## Umwandeln einer SMS in eine MQTT-Nachricht
* Ein __normaler SMS-Text__ wird unter dem in der Konfigurationsdatei `m4h.conf`, Abschnitt `[sms]`, Schl&uuml;ssel `pub:` (zB `pub: sms/received`) angegebenen Topic als Nachricht versendet.   
Mit Hilfe des Eintrags `pubNum: true` wird festgelegt, dass auch die Telefonnummer und Datum+Uhrzeit der SMS in der Payload mitgeschickt werden.   

* __MQTT-formatierter SMS-Text__ enth&auml;lt die Abk&uuml;rzungen `-t` (f&uuml;r Topic), `-m` (f&uuml;r Nachricht) und optional `-r` (f&uuml;r retain) und wird als entsprechende Nachricht ver&ouml;ffentlicht.   
_Beispiel_:   
`-t info/test -m SMS test message`   
ergibt eine Nachricht mit dem Topic `info/test` und der Payload `SMS test message`.   
Anmerkung: Die SMS muss von einer in `m4h.conf` freigegebenen Nummer (unter `from:`) kommen.

* M&ouml;gliche __Befehle__ m&uuml;ssen in der Konfigurationsdatei `m4h.conf` im Abschnitt `[sms]` angef&uuml;hrt sein. Sie bestehen aus dem Schl&uuml;ssel (zB `cmdversion:`) und dem SMS-Text f&uuml;r den Befehl (zB `-version-`). Das hei&szlig;t: Schickt man eine SMS mit dem Text `-version-` an das Programm `m4hSms`, so erh&auml;lt man eine Antwort-SMS mit der Versionsnummer.   

<a name="a20"></a>[Zum Seitenanfang](#up)   

# Welche Hilfsmittel werden ben&ouml;tigt? (Stand August 2021)
* _Hardware_: RasPi mit laufenden Broker (zB mosquitto)   
* _Hardware_: ein SIM808 Modul   
  zB SIM808 Modul GSM mit GPS Antenne f&uuml;r 3G 4G SIM Karte zB von [AliExpress](https://de.aliexpress.com/item/1005002384541464.html?spm=a2g0s.9042311.0.0.5c824c4dqUu43E) oder [Amazon](https://www.amazon.de/dp/B09CM8TSX9/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B09CM8TSX9&pd_rd_w=yNPjf&pf_rd_p=4f2ceb27-95e9-46ab-8808-db390b56ec01&pd_rd_wg=Muvvr&pf_rd_r=3NTH9ZQRZNE3VBZKD1YV&pd_rd_r=58ca39f9-b9f0-40b5-9b14-7f5d8a37ce1d&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzN1A5OTA0NUNBTTA2JmVuY3J5cHRlZElkPUEwMzcxNzc3MlkxMFpaUTBTRjhYMSZlbmNyeXB0ZWRBZElkPUEwNzU2MTYzMjRJSlNTREJMTjVHSiZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* _Hardware_: 4 Dr&auml;hte   
  zB Breadboard Jumper Dr&auml;hte weiblich auf weiblich 4polig zB von [Amazon](https://www.amazon.de/Female-Female-Male-Female-Male-Male-Steckbr%C3%BCcken-Drahtbr%C3%BCcken-bunt/dp/B01EV70C78/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Breadboard+Jumper+Dr%C3%A4hte&qid=1629911840&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzRVMzTE5FRDJTWjJSJmVuY3J5cHRlZElkPUEwODQwNzA5MUZGVVRTNVE1WDBBTiZlbmNyeXB0ZWRBZElkPUExMDAwNjg1MUFSNzFTUlhGM0hSOSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* _Hardware_: ein USB zu TTL Seriell Adapter   
  DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C mit FTDI FT232RL zB von [Amazon](https://www.amazon.de/gp/product/B07BBPX8B8/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1) oder DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C5 zB von [Amazon](https://www.amazon.de/DSD-TECH-SH-U09C5-Konverterkabel-Unterst%C3%BCtzung/dp/B07WX2DSVB/ref=sr_1_10?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=DSD+TECH+USB+zu+TTL+Seriell+Adapter+Konverter&qid=1629910857&s=computers&sr=1-10)   
* _Hardware_: USB-Verl&auml;ngerungskabel USB A Stecker auf A Buchse   
  zB [PIPIKA USB 3.0 Verl&auml;ngerung von Amazon](https://www.amazon.de/Verl%C3%A4ngerung-Verl%C3%A4ngerungskabel-Superschnelle-Vergoldeten-Kartenleseger%C3%A4t/dp/B08BHWJLLS/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=USB+verl%C3%A4ngerungskabel+A-Buchse+1m&qid=1629911382&smid=A3PTYKYXFT73EG&sr=8-4)   
* _Hardware_: SIM-Karte   
zB [in &Ouml;sterreich HOT von Hofer/Aldi](https://www.hot.at/tarife.html?gclid=CjwKCAjw1JeJBhB9EiwAV612y80Gd6MxAkyvJRJ2BAeTtinSp9OnIBslTwBgd_B-iJANhbW7v5TQSBoCckgQAvD_BwE) mit Tarif "HoT flex-unser Basistarif ohne Fixkosten".   
---   
* _Software_: Internetzugang zu GitHub   
* _Software_: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* _Software_: MQTT-Clientprogramme `mosquitto_sub` und `mosquitto_pub`   
  (werden bei der Mosquitto-Broker-Installation mit installiert).   
* _Software_: Eventuell   
   [Visual Studio Code](https://code.visualstudio.com/) und   
   [WinSCP](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   

<a name="a30"></a>[Zum Seitenanfang](#up)   

# Wie verbindet man das SIM-Modul mit dem RasPi?
Zum Senden und Empfangen von SMS ist zumindest ein SIM-Modul und eine SIM-Karte erforderlich. Das SIM-Modul kann entweder
* an die Pins der seriellen Schnittstelle des RasPi angeschlossen werden (Pin 2-5V, Pin 6-GND, Pin 8-TxD0 und Pin 10-RxD0), wobei eine Pegelanpassung 5V/3V3 erforderlich ist, oder   
* &uuml;ber einen USB-Serial-Adapter am RasPi angeschlossen werden.   

Der Anschluss &uuml;ber einen USB-Serial-Adapter ist zwar teurer (da die Kosten f&uuml;r den Adapter anfallen), daf&uuml;r aber einfacher.   

![Modul SIM808 mit USB-Serial-Adapter](./images/210825_SIM808_480b.png "Modul SIM808 mit USB-Serial-Adapter")   
_Bild 2: USB-Serial-Adapter SH-U09C, Modul SIM808 und GSM-Antenne (von links nach rechts)_   
   
Der Jumper auf dem USB-Serial-Adapter muss auf 5V gesteckt werden, da die Versorgungsspannung des SIM-Moduls zwischen 5 und 18V betragen muss. (3,3V ist zu wenig.)  

Beim Verbinden des SIM-Moduls mit dem USB-Serial-Adapter oder RasPi ist darauf zu achten, dass die Pins TxD mit RxD und RxD mit TxD verbunden werden ("auskreuzen" der Datenleitungen).   

![USB-SIM-connection](./images/m4hSms_connect.png "USB-SIM-connection")   
_Bild 3: Verbindung zwischen USB-Serial-Adapter und SIM-Modul_   

__Nicht vergessen__: Zum Senden und Empfangen von SMS wird eine SIM-Karte ben&ouml;tigt, wobei f&uuml;r viele Anwendungen eine Prepaid-Karte ausreicht, bei der keine Fixkosten anfallen. Die SIM-Karte wird in den SIM-Modul eingelegt (siehe Bild 1, links unten im Modul).   

<a name="a40"></a>[Zum Seitenanfang](#up)   

# Wie stellt man fest, an welchem Anschluss das SIM-Modul h&auml;ngt?   
Sind mehrere USB-Ger&auml;te am RasPi angeschlossen, ist nicht immer klar, mit welchem logischen Port (`/dev/ttyS0`, `/dev/ttyACM0`, `/dev/ttyUSB0`, `/dev/ttyUSB1` usw.) das SIM-Modul verbunden ist. Dies kann man mit Hilfe des Programms [`
m4hFindSimModule`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hFindSimModule) feststellen:   

1. Mit Putty eine Verbindung zum RasPi herstellen.   
2. Auf dem RasPi ein Verzeichnis f&uuml;r das Programm anlegen und hineinwechseln:   
  `mkdir ~/m4hFindSimModule`   
  `cd ~/m4hFindSimModule`   
3. Eine leere Datei erstellen:   
  `nano m4hFindSimModule.cpp`   
4. Den [Quellcode f&uuml;r `m4hFindSimModule`](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hFindSimModule/m4hFindSimModule.cpp) auf Github mit [Raw] &lt;strg&gt; a markieren, mit &lt;strg&gt; c kopieren und in die leere Datei einf&uuml;gen (rechte Maustaste).   
5. Die Datei speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
6. Die ausf&uuml;hrbare Datei erzeugen:   
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
Die Erstellung der ausf&uuml;hrbaren Datei erfolgt gleich wie es im Kapitel [Raspberry Pi: Hilfreiche Einzel-Programme in C++](m4h08_RasPiCppDemos.md) beispielhaft beschrieben wurde:   
1. Erstellen eines Projektverzeichnisses `mkdir ~/m4hSms`   
2. Wechseln in das Projektverzeichnis `cd ~/m4hSms`   
3. Kopieren der Projektdateien von GitHub in das Projektverzeichnis   
4. Erstellen der ausf&uuml;hrbaren Datei `m4hSms`  
  `g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread`   
5. Starten des Programms   
  `~/m4hSms/m4hSms`   

Beenden des Programms zB mit der Tastenkombination &lt;Strg&gt;c   

<a name="a60"></a>[Zum Seitenanfang](#up)   

# Wie kann man das Programm konfigurieren?

Die Konfiguration des Programms m4hSms erfolgt mit Hilfe der Konfigurationsdatei `m4h.conf`. Diese kann mit dem Texteditor Nano bearbeitet werden, zB   
`nano ~/m4hSms/m4h.conf`   

In der Sektion `[sms]` sind folgende Eintr&auml;ge erforderlich:   

* `device:` Schnittstelle, an der das SIM-Modul angeschlossen ist.   
  M&ouml;gliche Werte sind `ttyS0` f&uuml;r die serielle Schnittstelle oder `ttyACM0`, `ttyUSB0`, `ttyUSB1` usw. f&uuml;r die USB-Schnittstelle.   

* `from: ` Auflistung der Telefonnummern, von denen SMS empfangen werden d&uuml;rfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).

* `to: `  Auflistung der Telefonnummern, zu denen SMS geschickt werden d&uuml;rfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).

* `sub: ` Topic f&uuml;r das Senden einer SMS. Die Payload muss die Telefonnummer, ein Leerzeichen und dann den gew&uuml;nschten SMS-Text enthalten.   

* `subret: ` Topic f&uuml;r eine Antwortnachricht nach dem Senden einer SMS.   

* `pub: ` Topic, unter dem ankommende SMS verschickt werden, sofern kein Topic in der SMS angegeben wurde.   
   Aufbau einer SMS mit vorgegebenem Topic: `-t topic -m payload`   
   `-t ` steht f&uuml;r Topic, `-m ` steht f&uuml;r die Payload. Folgt noch ein `-r`, so wird das Retain-Flag gesetzt.   
---   
Folgende Eintr&auml;ge sind optional:   

* `pubNum: ` Gibt an, ob bei einer empfangenen SMS in der Payload zus&auml;tzlich zum SMS-Text auch die Telefonnummer und Datum+Uhrzeit angegeben werden soll.   
  Vorgabe: `false` (d.h. nur SMS-Text als Payload verwenden).

* `smsStart: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` gestartet wurde, zB   
  `smsStart: +43..... program m4hSms started!`   
  F&uuml;r jede Telefonnummer muss eine eigene Zeile verwendet werden.   
  Vorgabe: keine Start-SMS   

* `smsEnd: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` beendet wurde, zB   
  `smsEnd: +43..... program m4hSms finished!`   
  F&uuml;r jede Telefonnummer muss eine eigene Zeile verwendet werden.   
  Vorgabe: keine End-SMS   

* `cmdversion:` definert einen SMS-Text, der eine Antwort-SMS mit der Versionsnummer des Programms `m4hSms` bewirkt.   

* `cmdend:` definert einen SMS-Text, der das Beenden des Programms `m4hSms` bewirkt.   

* `cmdreload:` definert einen SMS-Text, der das neuerliche Laden der Konfigurationsdatei `m4h.conf` bewirkt.   

* `cmdcredit:` definert einen SMS-Text, der das Abfragen des Guthabens beim Provider bewirkt und den Betrag (in Euro) als Antwort-SMS zur&uuml;ckschickt.   
   
_Anmerkung_: Alle Schl&uuml;ssel wie `device`, `from`, `to` usw. sind im Quellcode der Datei `C_Sms.hpp` mit   
`#define SMS_..._KEY ...`   
definiert und k&ouml;nnen leicht an eigene Bed&uuml;rfnisse angepasst werden.   

### Beispiel f&uuml;r eine Konfigurationsdatei:   

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
* SIM-Modul mit g&uuml;ltiger SIM-Karte am RasPi anschlie&szlig;en ([siehe oben](#30)).   
* Broker auf dem Raspi mit der IP `10.1.1.1` l&auml;uft.   
### Software
* Vor dem Start des Programms m&uuml;ssen in die Konfigurationsdatei `m4h.conf` im Abschnitt `[sms]` zumindest die Schnittstelle (`device:`) und die Telefonummern eingetragen werden, die zum Senden (`to:`) und Empfangen (`from:`) von SMS berechtigt sind (siehe auch die [Konfigurationshinweise](#a60) weiter oben).   
* Programm starten:   
  `~/m4hSms/m4hSms`   
* In einem ersten Terminalfenster `mosquitto_sub` starten, um alle Nachrichten mitverfolgen zu k&ouml;nnen:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   

### SMS senden
In einem (zweiten) Terminalfenster (auf dem PC oder RasPi) folgendes eingeben:   
`mosquitto_pub -h 10.1.1.1 -t sms/send -m "+43... My first SMS-test"`   
_Wichtig_: Die Nummer `+43...` durch eine berechtigte Telefonnummer ersetzen!   

Im ersten Terminalfenster sieht man folgende Nachrichten   
`sms/send +43XXXXXXXXXXX My first SMS-test`   
`sms/send/ret SMS sent (+43XXXXXXXXXXX: My first SMS-test)`   
und man erh&auml;lt die entsprechende SMS.   

Vergisst man, die Telefonnummer auszubessern, sieht man im ersten Terminalfenster folgende Nachrichten:   
`sms/send +43... My first SMS-test`   
`sms/send/ret SMS NOT sent #6 not allowed (+43...: My first SMS-test)`   

### SMS empfangen
Auf einem Smartphone (mit berechtigter Telefonnummer!) folgende SMS schreiben und an das SIM-Modul schicken:   
`This is a test SMS from my smartphone`   

Nach kurzer Zeit liest man folgende MQTT-Nachricht im ersten Terminfenster:   
`sms/received +43XXXXXXXXXXX\n24.01.2022 11:30:29\nThis is a test SMS from my smartphone`   
_Anmerkung_: In der Konfigurationsdatei steht die Zeile `pubNum: true`, daher wird die Telefonnummer (hier mit XXX beschreiben) und Datum+Uhrzeit angezeigt.   

Sollte die Vue-Komponente `CiSmsIn` in einem Browser laufen, kann man auch hier die SMS lesen.   

## Guthaben abfragen
&Uuml;ber das Smartphone kann man auch das Guthaben der SIM-Karte im SIM-Modul abfragen. Dazu schickt man an das SIM-Modul eine SMS mit folgenden Inhalt:   
`-credit-`   
Nach kurzer Zeit erh&auml;lt man eine SMS mit dem entsprechenden Euro-Betrag.   

Im Terminalfenster werden folgende Nachrichten abgezeigt:   
`sms/send/ret SMS sent (6700: GUTHABEN)`   
`sms/send/ret SMS sent (+43XXXXXXXXXXX: 15,08 Euro)`   

[Zum Seitenanfang](#up)   
