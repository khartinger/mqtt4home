Letzte &Auml;nderung: 15.8.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: MQTT-Programme in C++</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h07_RasPiCppBase_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
* Die Vorteile eines modularen Aufbaus von Systemen zu schätzen.   
* Die Teile der C++ Vorlage `m4hBase` zu kennen.   
* Die Progammvorlage  `m4hBase` kompilieren, starten und testen können.

## Übersicht
Um Systeme übersichtlich zu gestalten, sollten sie modular aufgebaut sein. Modular bedeutet dabei, dass eine bestimmte Funktion von genau einem Modul realisiert wird und Module bei Bedarf dazugegeben oder weggelassen werden können.   
Bei Multi-Tasking Systemen wie Linux ist Modularität einfach durch Kommandozeilen-Programme bzw. -Skripte zu erreichen.   
Kommandozeilen-Programme können in verschiedenen Programmiersprachen geschrieben werden, wobei bei eingen Beispielen C++ verwendet wird. Der Vorteil von C++ besteht darin, dass die Programme nach dem Kompilieren wie Systemaufrufe verwendet werden können.   
Da das Erstellen von MQTT-Anwendungen in C++ nicht ganz einfach ist, wird hier eine Vorlage `m4hBase`verwendet, die bestimmte Basisfunktionalitäten bereits abdeckt. Die Vorlage besteht aus vier Dateien:   
* __`m4hMain.cpp`__   
Diese Datei enthält das Hauptprogramm und Funktionen für das Arbeiten mit MQTT, die <strg>C Behandlung, das Programmende usw.
* __`m4hBase.h`__ und __`m4hBase.cpp`__   
   In diesen beiden Dateien sind folgende Klassen enthalten:   
   * `Message` und `Message2` zur MQTT-Nachrichten-Speicherung,   
   * `Conf` zum Laden der Konfigurationsdatei sowie zum Bearbeiten der Einträge,   
   * `M4hBase` zur Bereitstellung von Basis-Funktionalitäten   
     (Version, MQTT-Meldung beim Programmstart und/oder Ende, ...).   
* __`m4hExtension.hpp`__   
Die Datei enthält die fünf Funktionen, mit deren Hilfe die Benutzerfunktion realisiert wird.

## Basisfunktionen der Vorlage `m4hBase`
1. Lesen von Einstellungen aus der Konfigurationsdatei m4h.conf.   
2. Möglichkeit, eine andere Konfigurationsdatei anzugeben   
   (beim Starten des Programms auf der Kommandozeile).   
3. Beantwortung einer Anfrage nach der Programmversion.   
   Vorgabe für die Anfrage: Topic "m4hBase/get", Payload "version"   
   Vorgabe für die Antwort: Topic "m4hBase/ret/version", Payload "2021-08-15"   
4. Senden (oder Nicht-Senden) einer MQTT-Nachricht beim Programmstart und/oder dem Programmende.   
5. Verwendung der globalen Objekte g_base, g_prt, g_mosq
6. Möglichkeit, das Programm durch eine MQTT-Nachricht zu beenden, die in der Konfigurationsdatei definiert ist (Schlüssel "progend" in der Konfigurationsdatei).
7. Beenden des Programms mit <Strg>c.

## Realisierung der Benutzer-Funktionen in der Datei `m4hExtension.hpp`
In der Datei `m4hExtension.hpp` müssen folgende fünf Funktionen definiert werden:   
* `void f1PrintHelptext() { }`
* `void f2Init(std::string pfConf) { }`
* `void f3OnMessage(struct mosquitto *mosq, std::string topic, std::string payload) { }`
* `void f4OnExit(struct mosquitto *mosq, int reason) { }`
* `void f5Periodic(struct mosquitto *mosq) { }`
Für die Basis-Anwendung "Anzeige der eintreffenden MQTT-Nachrichten" kann die Datei folgendermaßen aussehen:   
```   
//_____m4hExtension.hpp__________________________khartinger_____
// g++ m4hMain.cpp m4hBase.cpp -o m4hBase -lmosquitto -lpthread
// *  This program demonstrates a basic implementation for the
//    m4hBase template.
// 1. show which five functions must be provided by the user.
// 2. print all incoming messages (so you can see that the 
//    program is working ;))
// *  All functions of m4hBase are also available.
//    (For more information see file m4hMain.cpp.)
// *  m4hMain.cpp must have a line "#include "m4hExtension.hpp"
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-15 First release
// Released into the public domain.

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
void f2Init(std::string pfConf)
{
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

## Erstellung des Kommandozeilenprogramms `m4hBase`
Der Quellcode der vier Dateien befindet sich unter []().



## Test des Kommandozeilenprogramms `m4hBase`
