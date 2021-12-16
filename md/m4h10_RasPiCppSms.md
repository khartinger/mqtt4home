Letzte &Auml;nderung: 27.8.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Senden und Empfangen von SMS &uuml;ber MQTT in C++</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h10_RasPiCppSms_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
M&ouml;chte man bei der Hausautomation in bestimmten Situationen eine SMS erhalten oder Dinge &uuml;ber SMS steuern, so ist das hier vorgestellte Programm `m4hSms` daf&uuml;r sehr hilfreich:   
* Das Programm `m4hSms` wandelt SMS in MQTT-Nachrichten um und umgekehrt.   
* Viele Eigenschaften des Programms k&ouml;nnen in der Konfigurationsdatei (`m4h.conf`) festgelegt werden.   
* Mit ein wenig C++ Kenntnissen kann das Programm leicht an eigene Bed&uuml;rfnisse angepasst werden.   
## Erforderliche Hilfsmittel (Stand August 2021)
* Hardware: RasPi   
* Hardware: SIM808 Modul GSM mit GPS Antenne f&uuml;r 3G 4G SIM Karte zB von [AliExpress](https://de.aliexpress.com/item/1005002384541464.html?spm=a2g0s.9042311.0.0.5c824c4dqUu43E) oder [Amazon](https://www.amazon.de/dp/B09CM8TSX9/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B09CM8TSX9&pd_rd_w=yNPjf&pf_rd_p=4f2ceb27-95e9-46ab-8808-db390b56ec01&pd_rd_wg=Muvvr&pf_rd_r=3NTH9ZQRZNE3VBZKD1YV&pd_rd_r=58ca39f9-b9f0-40b5-9b14-7f5d8a37ce1d&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzN1A5OTA0NUNBTTA2JmVuY3J5cHRlZElkPUEwMzcxNzc3MlkxMFpaUTBTRjhYMSZlbmNyeXB0ZWRBZElkPUEwNzU2MTYzMjRJSlNTREJMTjVHSiZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* Hardware: Breadboard Jumper Dr&auml;hte weiblich auf weiblich 4polig zB von [Amazon](https://www.amazon.de/Female-Female-Male-Female-Male-Male-Steckbr%C3%BCcken-Drahtbr%C3%BCcken-bunt/dp/B01EV70C78/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Breadboard+Jumper+Dr%C3%A4hte&qid=1629911840&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzRVMzTE5FRDJTWjJSJmVuY3J5cHRlZElkPUEwODQwNzA5MUZGVVRTNVE1WDBBTiZlbmNyeXB0ZWRBZElkPUExMDAwNjg1MUFSNzFTUlhGM0hSOSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* Hardware: DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C mit FTDI FT232RL zB von [Amazon](https://www.amazon.de/gp/product/B07BBPX8B8/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1) oder DSD TECH USB zu TTL Seriell Adapter Konverter SH-U09C5 zB von [Amazon](https://www.amazon.de/DSD-TECH-SH-U09C5-Konverterkabel-Unterst%C3%BCtzung/dp/B07WX2DSVB/ref=sr_1_10?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=DSD+TECH+USB+zu+TTL+Seriell+Adapter+Konverter&qid=1629910857&s=computers&sr=1-10)   
* Hardware: USB-Verl&auml;ngerungskabel USB A Stecker auf A Buchse zB [PIPIKA USB 3.0 Verl&auml;ngerung von Amazon](https://www.amazon.de/Verl%C3%A4ngerung-Verl%C3%A4ngerungskabel-Superschnelle-Vergoldeten-Kartenleseger%C3%A4t/dp/B08BHWJLLS/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=USB+verl%C3%A4ngerungskabel+A-Buchse+1m&qid=1629911382&smid=A3PTYKYXFT73EG&sr=8-4)   
* Hardware: SIM-Karte zB [in &ouml;sterreich HOT von Hofer/Aldi](https://www.hot.at/tarife.html?gclid=CjwKCAjw1JeJBhB9EiwAV612y80Gd6MxAkyvJRJ2BAeTtinSp9OnIBslTwBgd_B-iJANhbW7v5TQSBoCckgQAvD_BwE) mit Tarif "HoT flex-unser Basistarif ohne Fixkosten".   
---   
* Software: Internetzugang zu GitHub   
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* Software: Eventuell   
   [Visual Studio Code](https://code.visualstudio.com/) und   
   [WinSCP](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   


## Beschreibung der Hardware
Zum Senden und Empfangen von SMS ist zumindest ein SIM-Modul und eine SIM-Karte erforderlich. Das SIM-Modul kann entweder
* an die Pins des RasPi angeschlossen werden (Pin 2-5V, Pin 6-GND, Pin 8-TxD0 und Pin 10-RxD0), wobei eine Pegelanpassung 5V/3V3 erforderlich ist, oder   
* &uuml;ber einen USB-Serial-Adapter am RasPi angeschlossen werden.   

Der Anschluss &uuml;ber einen USB-Serial-Adapter ist zwar teurer (da die Kosten f&uuml;r den Adapter anfallen), daf&uuml;r aber einfacher.   

![Modul SIM808 mit USB-Serial-Adapter](./images/210825_SIM808_480b.png "Modul SIM808 mit USB-Serial-Adapter")   
_Bild 1: USB-Serial-Adapter SH-U09C, Modul SIM808 und GSM-Antenne (von links nach rechts)_   
   
Der Jumper auf dem USB-Serial-Adapter muss auf 5V gesteckt werden, da die Versorgungsspannung des SIM-Moduls zwischen 5 und 18V betragen muss. (3,3V ist zu wenig.)  

Beim Verbinden des SIM-Moduls mit dem USB-Serial-Adapter oder RasPi ist darauf zu achten, dass die Pins TxD mit RxD und RxD mit TxD verbunden werden ("auskreuzen" der Datenleitungen).   

Weiters wird eine SIM-Karte ben&ouml;tigt, wobei f&uuml;r viele Anwendungen eine Prepaid-Karte ausreicht, bei der keine Fixkosten anfallen. Die SIM-Karte wird in den SIM-Modul eingelegt (siehe Bild 1, links unten im Modul).   

Hat man das SIM-Modul am RasPi angesteckt, so kann man mit Hilfe des Programms [`
m4hFindSimModule`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hFindSimModule) feststellen, als welches "Device" das Modul angesprochen werden kann:   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM-Modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
In diesem Beispiel ist der Device-Name also `/dev/ttyUSB0`.   
_Anmerkung_: Das Erzeugen des Programmes `m4hFindSimModule` erfolgt mit   
`g++ m4hFindSimModule.cpp -o m4hFindSimModule`.   

&nbsp;   
## Erstellung des Programms m4hSms
Die Erstellung der ausf&uuml;hrbaren Datei erfolgt gleich wie es im Kapitel [Raspberry Pi: Hilfreiche Einzel-Programme in C++](m4h08_RasPiCppDemos.md) beispielhaft beschrieben wurde:   
1. Erstellen eines Projektverzeichnisses `mkdir ~/m4hSms`   
2. Wechseln in das Projektverzeichnis `cd ~/m4hSms`   
3. Kopieren der Projektdateien von GitHub in das Projektverzeichnis   
4. Erstellen der ausf&uuml;hrbaren Datei `m4hSms`  
   `g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread`   

## Beschreibung der Konfigurationsm&ouml;glichkeiten
Die Konfiguration des Programms m4hSms erfolgt mit Hilfe der Konfigurationsdatei `m4h.conf`. Dabei sind in der Sektion `[sms]` folgende Eintr&auml;ge erforderlich:   
* `device:` Schnittstelle, an der das SIM-Modul angeschlossen ist. M&ouml;gliche Werte sind `ttyS0` f&uuml;r die serielle Schnittstelle oder `ttyUSB0`, `ttyUSB1` usw. f&uuml;r die USB-Schnittstelle.   
* `from: ` Auflistung der Telefonnummern, von denen SMS empfangen werden d&uuml;rfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).
* `to: `  Auflistung der Telefonnummern, zu denen SMS geschickt werden d&uuml;rfen   
   (Telefonnummern durch Beistriche getrennt, Nummern beginnen mit +...).
* `sub: ` Topic f&uuml;r das Senden einer SMS. Die Payload muss die Telefonnummer, ein Leerzeichen und dann den gew&uuml;nschten SMS-Text enthalten.   
* `pub: ` Topic, unter dem ankommende SMS verschickt werden, sofern kein Topic in der SMS angegeben wurde.   
   Aufbau einer SMS mit vorgegebenem Topic: `-t topic -m payload`   
   `-t ` steht f&uuml;r Topic, `-m ` steht f&uuml;r die Payload. Folgt noch ein `-r`, so wird das Retain-Flag gesetzt.   
---   
Folgende Eintr&auml;ge sind optional:   
* `smsStart: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` gestartet wurde, zB `smsStart: +43..... program m4hSms started!`   
* `smsEnd: ` Senden einer SMS an die angegebene Telefonnummer, wenn das Programm `m4hSms` beendet wurde, zB `smsEnd: +43..... program m4hSms finished!`   
* `cmdversion:` definert einen SMS-Text, der eine Antwort-SMS mit der Versionsnummer des Programms `m4hSms` bewirkt.   
* `cmdend:` definert einen SMS-Text, der das Beenden des Programms `m4hSms` bewirkt.   
* `cmdreload:` definert einen SMS-Text, der das neuerliche Laden der Konfigurationsdatei `m4h.conf` bewirkt.   
* `cmdcredit:` definert einen SMS-Text, der das Abfragen des Guthabens beim Provider bewirkt und den Betrag (in Euro) als Antwort-SMS zur&uuml;ckschickt.   

   
_Anmerkung_: Alle Schl&uuml;ssel wie `device`, `from`, `to` usw. sind im Quellcode der Datei `C_Sms.hpp` mit   
`#define SMS_..._KEY ...`   
definiert und k&ouml;nnen leicht an eigene Bed&uuml;rfnisse angepasst werden.   

### Ausf&uuml;hrliche Beispiel-Konfiguration:
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
# --> sms send result published under pub
# sms (cmd, plain)          --> publish with topic pub
# sms (-t topic -m payload) --> publish topic payload
sub: sms/send
pub: sms/send/ret
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
