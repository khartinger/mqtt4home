Letzte &Auml;nderung: 19.8.2021   
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
* Senden der aktuellen Broker-Zeit, wenn eine bestimmte Nachricht empfangen wurde (`m4hBrokertime`)   
* Mitschreiben aller MQTT-Nachrichten in Dateien (`m4hLogM`)   
* Mitschreiben aller MQTT-Nachrichten in Dateien und zus&auml;tzliches Abspeichern der jeweils letzten Nachricht in einer eigenen Datei (f&uuml;r eine schnellere Weiterverarbeitung) (`m4hLog2`)   
* Senden und Empfangen von SMS mit MQTT-Nachrichten (`m4hSms`)   
* Vorlage zur Erstellung eigenen Anwendungen (`m4hXxx`)   
   

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

## Erstellen eigener Programme
### Forderungen
Das Erstellen eines eigenen Programmes soll an Hand des Beispiels `m4hDemo1` gezeigt werden:   
* {a} In der Konfigurationsdatei soll eine Sektion `[demo1]` definiert und ein "Startwert" (Schl&uuml;ssel `startvalue`) von `20` vorgegeben werden.
k&ouml;nnen.   
* {b} Das Programm soll beim Start die Konfigurationsdaten einlesen   
(Datei `m4h.conf`, Schl&uuml;ssel `startvalue`).   
* {c} Das Programm soll alle ankommenden Nachrichten anzeigen.   
* {d} Das Programm soll auf die Nachricht `-t m4hDemo1/get -m keys` mit der Nachricht `-t m4hDemo1/ret/keys -m [keys]` antworten.   
   `[keys]` sollen alle Keys sein.   
* {e} Das Programm soll ausgehend von einem Startwert (default 30) im Sekundentakt bis 0 herunterz&auml;hlen und dann das Programm beenden.   
* {f} Bei Ende des Programms soll die Meldung `Thank you for using m4hDemo1!` ausgegeben werden.

### Vorbereitung der L&ouml;sung   
1. Erstellen eines Projektverzeichnisses `m4hDemo1`:   
`mkdir ~/m4hDemo1`   

2. Wechseln in dieses Verzeichnis:   
`cd ~/m4hDemo1`   

3. Hineinkopieren der Vorlagedateien (zB von `m4hXxx`) in dieses Verzeichnis:   
`cp ~/m4hXxx/*.* ~/m4hDemo1`   

4. Umbenennen der Datei C_Xxx.hpp in Demo1.hpp   
`mv C_Xxx.hpp C_Demo1.hpp`   

5. Umbenennen der Dummy-Bezeichnung Xxx   
Beim Umbenennen der Dummy-Bezeichnung `Xxx` in `Demo1` MUSS in den Dateien `C_Cxx.hpp`, `m4hExtension.hpp` und `m4h.conf` die Gro&szlig;-/Kleinschreibung beachtet werden, dh. das Umbenennen muss in drei Schritten erfolgen:
   1. Xxx durch Demo1 ersetzen (Klassenbezeichnung)
   2. xxx durch demo1 ersetzen (globale Variable)
   3. XXX durch DEMO1 ersetzen (Definitionen)

6. Zeilen mit `DEMO1_DEMO_KEY` und `_demo_` l&ouml;schen   

---
### L&ouml;sung der Anforderungen
7. Forderung {a}: Konfigurationsdatei erg&auml;nzen   
in der Datei `m4h.conf` folgendes eintragen:   
   ```   
   [demo1]
   startvalue: 20
   ```   

8. Forderung {b}: Konfigurationsdaten einlesen   
Das Einlesen der Konfigurationsdaten erfordert mehrere Schritte:   
* C_Demo1.hpp: Anlegen von Konstanten f&uuml;r den Startwert:   
   ```   
   #define DEMO1_STARTVALUE_KEY "startvalue"   
   #define DEMO1_STARTVALUE     30
   ```   

* C_Demo1.hpp: Eigenschaft f&uuml;r den Startwert deklarieren   
  `int startvalue;`   
  und in der Methode `setDefaults()` initialisieren:   
   `startvalue=DEMO1_STARTVALUE;`
   `keys=std::string(DEMO1_STARTVALUE_KEY);`

* C_Demo1.hpp: Methode zur R&uuml;ckgabe des Startwertes definieren:   
  `int getStartvalue() { return startvalue; }`   

* C_Demo1.hpp: Startwert aus der Konfigurationdatei einlesen (falls definiert).   In der Methode `readConfig(std::string pfConf)` nach der Zeile   
` if(v1.size()<1) return false;         // no valid lines read`   
folgendes einf&uuml;gen:   
```   
 //------for every line in section------------------------------
 bool bRet=false;
 for(int i=0; i<v1.size(); i++) {
  //-----get key and value--------------------------------------
  std::string sKey="", sVal="";
  std::string s1=v1.at(i);
  if(!conf.split2String(s1, sKey, sVal, ':')) continue;
  conf.delExtBlank(sKey);
  conf.delExtBlank(sVal);
  conf.strToLower(sKey);
  //-----search key---------------------------------------------
  if(sKey==DEMO1_STARTVALUE_KEY) {
   try{ // string to int
    int temp=std::stoi(sVal);
    startvalue=temp;
    bRet=true;
   } catch(std::invalid_argument) {};
  }
 }
 return bRet;
}
```   

* C_Demo1.hpp: Startwert anzeigen.   
In der Methode `show()` erg&auml;nzen:   
```   
std::cout<<DEMO1_STARTVALUE_KEY<<"          | "<<startvalue<<std::endl;
```   

* m4hExtension.hpp: Einlesen der Konfigurationsdatei   
In der Datei `m4hExtension.hpp` in der Funktion `f2Init()` folgende Zeilen erg&auml;nzen:   
```
 g_demo1.readConfig(pfConf);            // {a} read config data
 if(g_prt) g_demo1.show();              // show config values Demo1
```   
   
9. Forderung {c}: "Anzeige aller Nachrichten"   
* C_Demo1.hpp: In der Methode `onMessage()` erg&auml;nzen:   
   `std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;`   
* m4hExtension.hpp - Kontrolle: In der Funktion f3OnMessage() muss folgende Zeile stehen:   
  `g_demo1.onMessage(mosq, topic, payload);`   
   
10. Forderung {d}: "Reagieren auf eine Nachricht"   
C_Demo1.hpp: In der Methode `onMessage()` erg&auml;nzen:   
```   
 if(topic=="m4hDemo1/get" && payload=="keys") {
  publish(mosq, "m4hDemo1/ret/keys", getKeys(), false);
 }
```   

11. Forderung {e}: Herunterz&auml;hlen vom Startwert   
In der Datei `m4hExtension.hpp` in der Funktion `f5Periodic` erg&auml;nzen:   
```   
 int  iSec=g_demo1.getStartvalue();
 char cSec[16];
 //------count down iSec and exit program on 0------------------
 while(iSec>0) //-----"endless"---------------------------------
 { 
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  sprintf(cSec," %d ",iSec);
  fputs(cSec, stdout); fflush(stdout);
  iSec--;
 };
 printf(" 0 ");
 terminate_program(iEnd);
}
```   

12. Forderung {f}: Ende-Meldung ausgeben   
C_Demo1.hpp: In der Methode `onExit()` erg&auml;nzen:   
`std::cout<<"Thank you for using m4hDemo1!"<<std::endl;`   
