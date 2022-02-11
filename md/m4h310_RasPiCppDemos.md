Letzte &Auml;nderung: 11.2.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Raspberry Pi: Welche C++ Hilfsprogramme gibt es und wie bekomme ich sie zum Laufen?</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h310_RasPiCppDemos_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# Worum geht es?
In der Heimautomation gibt es verschiedene Möglichkeiten, Abläufe zu steuern. 
Oft verwendet man dabei vorgefertigte Systeme, die nur mehr parametrisiert werden müssen. Der Nachteil dabei ist, dass nur die Dinge möglich sind, die vom System vorgesehen sind.   

Die hier vorgestellten Hilfsprogramme (bzw. auch selbst erstellte Module) gehen einen anderen Weg:   
* Alle Hilfsprogramme bestehen aus dem gleichen Basisprogramm und einem Funktionsmodul.   
* Jedes Hilfsprogramm (bzw. der Funktionsmodul) löst (genau) eine Aufgabe.   
* Die Ein- und Ausgabe jedes Programms erfolgt über MQTT Nachrichten.   
* Die Eigenschaften eines Hilfsprogramms werden über eine Konfigurationsdatei festgelegt.   

Da jedes Hilfsprogramm eine ausführbare Datei ist und über MQTT-Nachrichten gesteuert wird, kann es auch gemeinsam mit vorgefertigten Systemen verwendet werden.   
Weiteres ist es möglich, verschiedene Module zu einem Gesamtprogramm zusammenzufassen und so seine eigene Haussteuerung zu realisieren.   

## Diese Anleitung beantwortet folgende Fragen:   
1. [Welche Dinge benötige ich für das Erstellen eines ausführbaren Hilfsprogramms?](#a10)   
2. [Welche vorgefertigte Hilfsprogramme gibt es auf GitHub?](#a20)   
3. [Wie erstelle ich ein ausführbares Hilfsprogramm?](#a30)   
4. [Aus welchen Dateien besteht das Basissystem?](#a90)   

<a name="a10"></a>[_Zum Seitenanfang_](#up)   
# Welche Dinge benötige ich für das Erstellen eines ausführbaren Hilfsprogramms?
* Hardware: PC oder Laptop mit Internetzugang, Browser   
* Hardware: Raspberry Pi als Access Point (WLAN Raspi11, PW 12345678) mit der IP 10.1.1.1, auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)   
* Software: Visual Studio Code ("VSC"), das f&uuml;r C++-Anwendungen vorbereitet ist.   
* Software: Terminal-Programm [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   
* Software: Die MQTT-Klient-Programme `mosquitto_sub` und ` mosquitto_pub` (auf dem PC oder RasPi installiert)   

<a name="a20"></a>[_Zum Seitenanfang_](#up)   
# Welche vorgefertigte Hilfsprogramme gibt es auf GitHub?   
* Die Hilfsprogramme findet man auf GitHub im Verzeichnis [mqtt4home/source_RasPi](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi).   
* Genauere Informationen zu den Programmen gibt es in den jeweiligen Projekt-Verzeichnissen.   
* Zur einfachen Darstellung von Nachrichten wird die Syntax des Brokers "Mosquitto" verwendet:   
   `-t topic -m payload`   
* Viele Topics und Payloads können in der Konfigurationsdatei (zB `m4h.conf`) an eigene Bedürfnisse angepasst werden.   

Die folgende Aufzählung ist alphabetisch geordnet (und daher nicht nach Wichtigkeit).   

## Abfrage des Zustands (der "Gesundheit") des zigbee2mqtt-Programms
Name: [`m4hAdZigbee2mqtt`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hAdZigbee2mqtt)   
Das Programm sendet eine "Gesundheits"-Anfrage an das Programm `zigbee2mqtt` und wandelt die Antwort in eine leichter verarbeitbare Antwort um.   
OUT zigbee2mqtt: `-t zb/bridge/request/health_check -m ""`   
IN  zigbee2mqtt: `zb/bridge/response/health_check {"data":{"healthy":true},"status":"ok"}`   
IN: `-t z2m/get -m health`   
OUT: `-t z2m/ret/health -m Zigbee2mqtt-health is perfect.`   

## C++ Basisprogramm
Name: [`m4hBase`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBase)   
Das C++ Basisprogramm stellt die Verbindung zum MQTT Broker und zu verschiedenen Funktionsmodulen zur Verfügung.   
Es ermöglicht das Neuladen der Konfigurationsdatei und bietet verschiedene Möglichkeiten zum Beenden des Programmes an (Tastenkombination &lt;strg&gt;c, MQTT-Nachricht, Ende nach periodischer Ausführung eines Vorgangs).   
Details stehen auch [am Ende dieser Anleitung](#a90).   

## Abfrage der Brokerzeit   
Name: [`m4hBrokertime`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBrokertime)   
Zweck: Auf Anfrage sendet das Programm die aktuelle Brokerzeit zurück. Anwendung zB bei D1mini-Systemen, die keine Echtzeituhr (RTC) habe und trotzdem das aktuelle Datum bzw. die Uhrzeit anzeigen wollen.   
IN : `-t getTime -m ?`   
OUT: `-t brokertime -m yyyymmdd HHMMSS`   
Das Zeitformat `yyyymmdd HHMMSS` bedeutet yyyy=Jahr, mm=Monat, dd=Tag, HH=Stunde, MM=Minute, SS=Sekunde.   
Alterative: Das Programm `InDelayOut`.   

## Demoprogramm   
Name: [`m4hDemo1`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hDemo1)   
Das fertige Demo-Programm, dessen Erstellung in [`RasPi: Create your own programs in C++`](https://github.com/khartinger/mqtt4home/blob/main/md/m4h09_RasPiCppCreatingYourOwnProgs_e.md) beschrieben wird.   

## 
Name: [`m4hFindSimModule`]()   

##
Name: [`m4hInDelayOut`]()   

##
Name: [`m4hLog2`]()   
##
Name: [`m4hLogM`]()   
##
Name: [`m4hPayload`]()   
##
Name: [`m4hSms`]()   
##
Name: [`m4hWdog`]()   
##
Name: [`m4hXxx`]()   


<a name="a30"></a>[_Zum Seitenanfang_](#up)   
# Wie erstelle ich ein ausführbares Hilfsprogramm?](#a30)   


<a name="a90"></a>[_Zum Seitenanfang_](#up)   
#  Aus welchen Dateien besteht das Basissystem?   



---   




# Wie &uuml;bersetze und teste ich das Programm m4hWdog?

Die Erstellung der ausf&uuml;hrbaren Datei erfolgt gleich wie es im Kapitel [Raspberry Pi: Hilfreiche Einzel-Programme in C++](m4h08_RasPiCppDemos.md) beispielhaft beschrieben wurde:   
1. Erstellen eines Projektverzeichnisses `mkdir ~/m4hWdog`   
2. Wechseln in das Projektverzeichnis `cd ~/m4hWdog`   
3. Kopieren der Projektdateien von GitHub in das Projektverzeichnis   
4. Erstellen der ausf&uuml;hrbaren Datei `m4hWdog`  
  `g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread`   
5. Starten des Programms   
  `~/m4hWdog/m4hWdog`   

Beenden des Programms zB mit der Tastenkombination &lt;Strg&gt;c   

## Test des Programms
1. Am PC ein Terminalfenster (`cmd.exe`) oder putty-Fenster oder am RasPi eine Konsole &ouml;ffnen und das Programm zum Anzeigen von MQTT-Nachrichten starten:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   

2. Starten des Programms zB &uuml;ber ein Putty-Fenster   
  `~/m4hWdog/m4hWdog`   

3. Tut man nichts, erscheint im Putty-Fenster alle 10 Sekunden folgende Ausgabe:   
`ERROR! Watchdog timeout m4h/button_2`   
Weiters wird im ersten Terminalfenster alle 10 Sekunden folgende MQTT-Nachricht angezeigt:   
`m4hWdog/attention Sensor m4h/button_2 missing!`   

4. Dr&uuml;ckt man innerhalb der 10 Sekunden den Taster `m4h/button_2` so erscheint im Putty-Fenster die Meldung:   
`Update secLast: m4h/button_2`   
und im ersten Terminalfenster die Nachricht des Tasters.   

5. Hat man keinen Taster zur Verf&uuml;gung, so kann man am PC ein zweites Terminalfenster (`cmd.exe`) oder putty-Fenster oder am RasPi eine zweite Konsole &ouml;ffnen und eine Taster-Nachricht simulieren:   
`mosquitto_pub -h 10.1.1.1 -t m4h/button_2 -m anything`   
Mit jeder Nachricht wird der Watchdog-Timer neu gestartet.   

<a name="a90"></a>[_Zum Seitenanfang_](#up)   
# Wie ist das Programm m4hWdog codiert?   
## Anlegen des Projektes
1. Windows: Anlegen des Projektordners `m4hWdog`   
2. Windows: Kopieren der Dateien der Vorlage `m4hWdog` in den Ordner `m4hWdog`   
3. Visual Studio Code (VSC) starten   
4. In VSC den Projektordner &ouml;ffnen   
   Men&uuml; Datei - Ordner &ouml;ffnen... : Im Dialog in den Ordner wechseln und [Ordner ausw&auml;hlen] dr&uuml;cken   
5. &auml;ndern des Dateinamens `C_Wdog.hpp` in `C_Wdog.hpp`   
   Im Explorer-Fenster mit der rechten Maustaste auf `C_Wdog.hpp` klicken, "Umbenennen" w&auml;hlen und `C_Wdog.hpp` eingeben   
6. In allen Dateien die Namen `XXX`, `Xxx` und `xxx` in `WDOG`, `Wdog` und `wdog` umbenennen.   
   Men&uuml; Bearbeiten - In Dateien ersetzen   
   __WICHTIG__: IM Suchen-Eingabefeld das Symbol "Aa" aktivieren, damit die Gro&szlig;-/Kleinschreibung ber&uuml;cksichtigt wird!   
   Es werden ca. 23 Ersetzungen in 2 Dateien bzw. 35 Ersetzungen in 4 Dateien und 9 Ersetzungen in 4 Dateien durchgef&uuml;hrt.   
7. Formatierungen stc. anpassen (falls gew&uuml;nscht)   
   * Da die neuen Bezeichnuntgen `Wdog` um einen Buchstaben mehr haben als `Xxx` sind auch die Spalten bei Kommentaren um eine Stelle verschoben...   
   * Datum ausbessern auf das aktuelle Datum (Datei `C_Wdog.hpp`, `m4hExtension.hpp`, `m4h.conf`)   
8. Alle Dateien speichern   
   Men&uuml; Datei - Alles speichern

Das Projekt kann bereits auf das RasPi &uuml;bertragen und dort gestestet werden:   
* RasPi starten, PC mit dem RasPi &uuml;ber WLAN verbinden.   
* &uuml;bertragen des Verzeichnisses `m4hWdog` vom PC zum RasPi (mit _WinSCP_)   
* Mittels _Putty_ auf das RasPi zugreifen (User `pi_`, PW `pass`) und in den Projektordner wechseln:   
  `cd ~/m4hWdog`   
* _Putty_: Ausf&uuml;hrbares Programm erzeugen:   
  `g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread`   
* _Putty_: Das Programm ausf&uuml;hren:   
  `./m4hWdog`   
* PC: Terminalfenster (`cmd.exe`) &ouml;ffnen und das Programm zum Anzeigen von MQTT-Nachrichten starten:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* PC: Ein zweites Terminalfenster (`cmd.exe`) &ouml;ffnen und eine MQTT-Nachricht senden:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/get -m version`   
  Im ersten Terminalfenster erscheinen die Meldungen   
  `m4hWdog/get version`   
  `m4hWdog/ret/version 2022-02-03`   
* PC: Das Programm mittels MQTT-Befehl beenden:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/set -m ...end...`   
  Im ersten Terminalfenster erscheinen die Meldungen   
  `m4hWdog/set ...end...`   
  `info/end__ m4hWdog (03.02.2022 18:13:19)`   
  bzw. in _Putty_ erscheinen die Meldungen   
  `Exit program... MQTT end message sent.`   
  `Program terminated by MQTT (03.02.2022 18:13:19)`   
  `Beendet`   

## Zus&auml;tzliche Klasse "WdogIn1" f&uuml;r Sensor-Daten
Da f&uuml;r jeden zu &uuml;berwachenden Sensor   
* ein Topic,   
* die Watchdog-Zeitdauer und   
* der Zeitpunkt des letzten Aufrufs    

gespeichert werden muss, wird daf&uuml;r eine eigene Klasse `WdogIn1` definiert. Der Einfachheit halber werden alle Eigenschaften als `public` definiert.   

## Konstante definieren
Um das Programm m&ouml;glichst flexibel benutzen zu k&ouml;nnen, wird in der Datei `C_Wdog.hpp` f&uuml;r jeden Schl&uuml;ssel und jeden Wert der Konfigurationsdatei eine Konstante und eine Variable definiert:   
```   
#define  WDOG_OUT_KEY        "out"
#define  WDOG_OUT_TOPIC      "m4hWdog/attention"
#define  WDOG_OUT_PAYLOAD    "Sensor <in> missing!"
#define  WDOG_IN_KEY         "in"
```   
_WICHTIG:_ Keys d&uuml;rfen nur Kleinbuchstaben enthalten!   

In der Klasse `Wdog` werden die dazugeh&ouml;rigen Eigenschaften definiert:   
```   
 std::string wdog_out_key;             // topic out key
 std::string wdog_out_topic;           // topic out
 std::string wdog_out_payload;         // value out
 std::string wdog_in_key;              // topic in key
 std::vector<WdogIn1>vIn;              // topic in values
```   

Zus&auml;tzlich werden noch Konstante f&uuml;r den Topic-In-Platzhalter ("`<in>`") sowie den Minimal- und Maximalwert f&uuml;r die Watchdog-Zeit definiert.   

Im Allgemeinen werden die "Demo"-Eintr&auml;ge in der Datei `C_Wdog.hpp` nicht ben&ouml;tigt und k&ouml;nnen auskommentiert oder gel&ouml;scht werden.   

## Vorgabewerte setzen (setDefaults)
In der Methode `setDefaults()` werden den Variablen die Vorgabewerte zugewiesen und der String `keys` gebildet, der alle Keys anzeigen kann.   
```   
 wdog_out_key     = WDOG_OUT_KEY;      // topic out key
 wdog_out_topic   = WDOG_OUT_TOPIC;    // topic out value
 wdog_out_payload = WDOG_OUT_PAYLOAD;  // payload out
 wdog_in_key      = WDOG_IN_KEY;       // topic out
 vIn.clear();                          // topic in values
 keys=std::string(WDOG_OUT_KEY);
 keys+="|"+std::string(WDOG_IN_KEY);
```   

## Einlesen der Konfigurationsdatei (readConfig)   
In der Methode `readConfig()` werden im vorgegebenen Code bereits alle Zeilen des Abschnitts `[wdog]` der Konfigurationsdatei in den Vektor `v1` eingelesen.   

Nun muss das Auswerten der Zeilen erg&auml;nzt werden:   

* Jede Zeile wird in ihren Schl&uuml;ssel und den Wert zerlegt (mit Hilfe des Doppelpunktes). F&uuml;hrende bzw. nacheilende Leerzeichen werden entfernt.   
* Der Wert in der Zeile `out:` wird in Topic und Text zerlegt (durch das erste Leerzeichen).   
  Fehlt die Textangabe, wird der Vorgabewert verwendet.   
* Jede Zeile `in:` wird in Topic und Zeit-Text sHMS zerlegt (durch das erste Leerzeichen). Danach wird der String `sHMS` in Stunde, Minute und Sekunde zerlegt, die Strings in Zahlen umgewandelt und die Sekundenzahl berechnet.   
  Topic und Zeit werden in ein `WdogIn1`-Objekt umgewandelt und in den Vektor `vIn` eingeh&auml;ngt.   
  Fehlt die Zeitangabe oder ist sie fehlerhaft, wird das Topic ignoriert.   

## Anzeige der eingelesenen Daten (show)   
Die Anzeige der eingelesenen Daten erfolgt in der &uuml;blichen Weise:   
```   
-----[wdog]------------------------------
config file         | ./m4h.conf
all keys            | out|in
out                 | -t m4hWdog/attention -m Sensor <in> missing!
-----in----------------------------------
m4h/button_2: 10sec | test/t3723: 3723sec | test/t259200: 259200sec |
```   

Codierung:   
```   
 std::cout<<wdog_out_key<<"                 | -t "<<wdog_out_topic<<" -m "<<wdog_out_payload<<std::endl;
 std::cout<<"-----in----------------------------------"<<std::endl;
 for(int i=0; i<vIn.size(); i++) {
  std::cout<<vIn.at(i).topicIn<<": "<<vIn.at(i).secDiff<<"sec | ";
 }
 std::cout<<std::endl;
```   

## Reaktion auf eintreffende Nachrichten (onMessage)
Trifft eine Nachricht ein, wird das Topic mit allen gespeicherten Topics verglichen. Wird es gefunden, wird der Zeitpunkt des letzten Aufrufs auf die aktuelle Zeit gesetzt und die Methode verlassen.   

Codierung:   

```   
//_______act on messages..._____________________________________
bool Wdog::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 int num=vIn.size();
 time_t now;
 time(&now);                           // get sec since 1.1.1970
 //------for all sensors----------------------------------------
 for(int i=0; i<num; i++){
  if(topic==vIn.at(i).topicIn) 
  {//----topic for watchdog found-------------------------------
   vIn.at(i).secLast=now;              // update sensor
   if(g_prt) std::cout<<"Update secLast: "<<vIn.at(i).topicIn<<std::endl;
   return true;
  }
 }
 return false;
}
```   

## Periodische Watchdog-Kontrolle
F&uuml;r die periodische Kontrolle, ob die maximale Watchdog-Zeit &uuml;berschritten wurde, wird die Methode `periodic` definiert. Diese muss zB jede Sekunde von der Funktion `f5Periodic` der Datei `m4hExtension.hpp` aufgerufen werden.   

* Zuerst wird die aktuelle Zeit ermittelt ("`now`")   
* In einer Schleife wird f&uuml;r jeden Sensor kontrolliert, ob die maximale Watchdog-Zeit erreicht oder &uuml;berschritten wurde.   
* Ist dies der Fall, wird der Zeitpunkt des letzten Aufrufs aktualisiert, die Ausgabe-Payload erzeugt und die MQTT-Nachricht gesendet.   

[_Zum Seitenanfang_](#up)   
