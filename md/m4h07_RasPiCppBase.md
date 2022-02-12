Letzte &Auml;nderung: 19.8.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Vorlage für MQTT-Programme in C++</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h07_RasPiCppBase_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
* Mit Hilfe der Vorlage `m4hBase` soll ein einfaches C++ Programm erstellt und getestet werden, das alle eintreffenden MQTT-Nachrichten anzeigt.   
* Der Zweck der einzelnen Teile der C++ Vorlage `m4hBase` soll erl&auml;utert werden.   
## Erforderliche Hilfsmittel
* Hardware: RasPi
* Software: Internetzugang zu GitHub
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop
* Software: Eventuell [WinSCP](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   

## &Uuml;bersicht
Um Systeme &uuml;bersichtlich zu gestalten, sollten sie modular aufgebaut sein. Modular bedeutet dabei, dass eine bestimmte Funktion von genau einem Modul realisiert wird und Module bei Bedarf dazugegeben oder weggelassen werden k&ouml;nnen.   
In dieser Anleitung werden mit Hilfe der Vorlage `m4hBase` C++ Programme erstellt, die genau eine Aufgabe erfüllen und wie Betriebssystemaufrufe verwendet werden k&ouml;nnen.   
   
## Was kann die Vorlage `m4hBase` leisten?
1. Lesen von Einstellungen aus der Konfigurationsdatei m4h.conf.   
2. M&ouml;glichkeit, eine andere Konfigurationsdatei anzugeben   
   (beim Starten des Programms auf der Kommandozeile).   
3. Beantwortung einer Anfrage nach der Programmversion.   
   Vorgabe f&uuml;r die Anfrage: Topic "m4hBase/get", Payload "version"   
   Vorgabe f&uuml;r die Antwort: Topic "m4hBase/ret/version", Payload "2021-08-15"   
4. Senden (oder Nicht-Senden) einer MQTT-Nachricht beim Programmstart und/oder dem Programmende.   
5. Verwendung der globalen Objekte g_base, g_prt, g_mosq
6. M&ouml;glichkeit, das Programm durch eine MQTT-Nachricht zu beenden, die in der Konfigurationsdatei definiert ist (Schl&uuml;ssel "progend" in der Konfigurationsdatei).
7. Beenden des Programms mit &lt;strg&gt;c.
   
## Aufbau der C++ Vorlage `m4hBase`
 Die Vorlage besteht aus drei fixen Dateien (bleiben immer gleich) und (mindestens) zwei Dateien, die bearbeitet werden m&uuml;ssen:   
### Fixe Dateien
* __`m4hMain.cpp`__ (Hauptprogramm)   
Diese Datei enth&auml;lt das Hauptprogramm mit den Aufrufen aller Funktionen, die in der Erweiterungsdatei definiert werden m&uuml;ssen. Weitere Aufgaben sind das Starten von MQTT, die &lt;strg&gt;c Behandlung, das Beenden des Programmes usw.
* __`m4hBase.h`__ und __`m4hBase.cpp`__ (Basisklassen)   
   In diesen beiden Dateien sind folgende Klassen definiert:   
   * `Message` und `Message2` zur MQTT-Nachrichten-Speicherung,   
   * `Conf` zum Laden der Konfigurationsdatei sowie zum Bearbeiten der Eintr&auml;ge,   
   * `M4hBase` zur Bereitstellung von Basis-Funktionalit&auml;ten   
     (Version, MQTT-Meldung beim Programmstart und/oder Ende, ...).   

   Au&szlig;erdem enthalten die Dateien ein globales Objekt `g_base`, mit dem auf die Methoden der Klasse `M4hBase` zugegriffen werden kann.
### Dateien, die bearbeitet werden m&uuml;ssen
* __`m4hExtension.hpp`_ (Erweiterung)_   
Die Datei enth&auml;lt die f&uuml;nf Funktionen, mit deren Hilfe die Benutzerfunktion realisiert wird.   
* __`m4h.conf`__ (Konfigurationsdatei)   
Mit der Konfigurationsdatei kann das Verhalten der Programme gesteuert werden. Der Aufbau der Datei besteht aus Sektionen (in eckigen Klammern[]) und Schl&uuml;ssel:Wert-Paaren.   
   
Bei vielen Programmen ist es au&szlig;erdem sinnvoll, weitere Funktionalit&auml;ten in einer "Arbeitsklasse" zu realisieren.
&nbsp;   
## Bearbeitung der Erweiterungsdatei `m4hExtension.hpp`
In der Datei `m4hExtension.hpp` m&uuml;ssen folgende f&uuml;nf Funktionen definiert werden:   
* `void f1PrintHelptext() { }`   
* `bool f2Init(std::string pfConf) { }`   
* `void f3OnMessage(struct mosquitto *mosq, std::string topic, std::string payload) { }`   
* `void f4OnExit(struct mosquitto *mosq, int reason) { }`   
* `void f5Periodic(struct mosquitto *mosq) { }`   

F&uuml;r die Basis-Anwendung "Anzeige der eintreffenden MQTT-Nachrichten" kann die Datei zB folgenderma&szlig;en aussehen:   
```   
//_____m4hExtension.hpp__________________________khartinger_____
// ...
#include "mosquitto.h"            // mosquitto_* functions
#include "m4hBase.h"              // m4h basic functions

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);    // myMqttPeriodicExec

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hBase [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4hLocal.conf)\n");
 fprintf(stdout, "Purpose: (1) Demo for m4hBase\n");
 fprintf(stdout, "         (2) Program shows all incomming messages.\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2021-08-15");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
bool f2Init(std::string pfConf)
{
 return true;
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //------print all received messages----------------------------
 std::cout<<topic<<" | "<<payload<<std::endl;
}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
}

//_______for periodic actions (a parallel thread)_______________
void f5Periodic(struct mosquitto *mosq)
{
 bool bDoPeriodic=true;                // do "endless"
 int  iEnd=4;                          // reason for end
 while(bDoPeriodic) //-----"endless"----------------------------
 { //...Do something...
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 };
 terminate_program(iEnd);
}
```   
_Listing 1:  Beispiel f&uuml;r m4hExtension.hpp_   
   

---   
## Erstellung des Kommandozeilenprogramms `m4hBase`
### Vorbereitung
1. Erstellen eines Verzeichnisses f&uuml;r den Programmcode auf dem RasPi:   
```mkdir ~/m4hBase```

2. Wechseln in dieses Verzeichnis:   
```cd ~/m4hBase```
### Projektdateien kopieren
3. Herunterladen des Quellcodes der Projektdateien.   
Der Quellcode der Dateien befindet sich unter [https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBase](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBase).   
4. Kopieren der Dateien ins Projektverzeichnis `m4hBase`   
(Vom PC zur RasPi zB mit `WinSCP`)   

### Oder: Erstellen der einzelnen Dateien mit `nano`
Alternativ kann man die Dateien auch mit dem Texteditor `nano` erzeugen und den Quellcode Datei f&uuml;r Datei hineinkopieren:   
* Putty starten und mit dem RasPi verbinden
* Ins Projektverzeichnis wechseln   
  ```cd ~/m4hBase```
* Eine Quellcode-Datei, zB `m4h.conf`, auf GitHub anklicken, [Raw] dr&uuml;cken, Quellcode kopieren
* Auf dem RasPi eingeben:
```nano ./m4h.conf```   
* Den Quellcode mit der rechten Taste in `nano`einf&uuml;gen
* Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

Die Vorgangsweise ab dem 3. Punkt f&uuml;r die Dateien `m4hBase.cpp`, `m4hBase.h`, `m4hExtension.hpp` und `m4hMain.cpp` wiederholen.   

### Quellcode kompilieren
5. Das Kompilieren erfolgt mit folgendem Befehl:   
```g++ m4hMain.cpp m4hBase.cpp -o m4hBase -lmosquitto -lpthread```   
Im Verzeichnis `~/m4hBase` wurde die Datei `m4hBase` erzeugt.   

_Anmerkung 1_: Damit das Kompilieren erfolgreich ist, muss die Mosquitto-Bibliothek installiert sein:   
```sudo apt-get install libmosquitto-dev```   
 (siehe auch [https://github.com/khartinger/mqtt4home/blob/main/m4h03_RasPiMQTTBroker.md](https://github.com/khartinger/mqtt4home/blob/main/m4h03_RasPiMQTTBroker.md) )   

&nbsp;   
## Test des Kommandozeilenprogramms `m4hBase`
Zum Testen des Programms ben&ouml;tigt man ein Putty-Fenster und ein PC-Eingabeaufforderungs-Fenster:
1. Das neu erstellte Programm in der Konsole oder einem putty-Fenster starten:   
```./m4hBase```  
Ergebnis:   
```   
Read config file ./m4h.conf: OK
-----[base]---------------------------
config file         | ./m4h.conf
section name        | base
version (in)        | -t m4hBase/get -m version
version (out)       | -t m4hBase/ret/version -m 2021-08-15
mqtt @ start (out,*)| -t info/start -m m4hBase -r
mqtt @ end (out,*)  | -t info/end -m m4hBase -r
progend by mqtt (in)| -t m4hBase/set -m ...end...
         * add time | true
Try to connect to mosquitto...
Connected: Waiting for topics...
info/start | m4hBase (15.08.2021 19:29:10)
```   

2. Am PC ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
```cmd.exe```
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
```c:```   
```cd /programme/mosquitto```

3. Nachricht vom PC-Eingabeaufforderungs-Fenster senden   
```mosquitto_pub -h 10.1.1.1 -t Test1 -m "Hallo vom PC!"```   
Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

4. In der Konsole oder dem putty-Fenster erscheint (eventuell nach kurzer Zeit) die entsprechende Nachricht:<br>
```Test1 | Hallo vom PC```   

Beendet man das Programm mit &lt;ctrl&gt;c, so erh&auml;lt man folgende Meldungen:   
```
^C
Exit program... MQTT end message sent.
info/end | m4hBase (15.08.2021 19:32:19)

Program terminated by <ctrl>c (15.08.2021 19:32:19)
Beendet
```
