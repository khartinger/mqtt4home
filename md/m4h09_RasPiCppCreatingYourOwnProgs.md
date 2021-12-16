Letzte &Auml;nderung: 22.8.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Erstellen eigener Programme in C++</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h09_RasPiCppCreatingYourOwnProgs_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
* Eigene Programme mit Hilfe der Vorlage `m4hBase` erstellen.   
   

## Erforderliche Hilfsmittel
* Hardware: RasPi
* Software: Internetzugang zu GitHub
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop
* Software: Eventuell   
   [Visual Studio Code](https://code.visualstudio.com/) und   
   [WinSCP](https://winscp.net/eng/docs/lang:de) zur Daten&uuml;bertragung vom PC/Laptop zum RasPi   

## Erstellen eines eigenen Programms
Das Erstellen eines eigenen Hilfsrogramms soll an Hand des Beispiels `m4hDemo1` gezeigt werden. Dabei ist es am einfachsten, das Programm am PC in Visual Studio Code vorzubereiten, danach auf das RasPi zu übertragen und dort zu kompilieren. (Es wäre auch möglich, mit Visual Studio Code direkt am RasPi zu arbeiten.)    

### Forderungen
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
1. RasPi: Erstellen eines Projektverzeichnisses `m4hDemo1`:   
`mkdir ~/m4hDemo1`   

2. RasPi: Wechseln in dieses Verzeichnis:   
`cd ~/m4hDemo1`   

3. RasPi: Hineinkopieren der Vorlagedateien (zB von `m4hXxx`) in dieses Verzeichnis:   
`cp ~/m4hXxx/*.* ~/m4hDemo1`   

4. RasPi: Umbenennen der Datei C_Xxx.hpp in Demo1.hpp   
`mv C_Xxx.hpp C_Demo1.hpp`   

5. PC: Herunterladen des Verzeichnisses `~/m4hDemo1` vom RasPi auf den PC mit Hilfe von WinSCP und   
   &Ouml;ffnen des Projekt-Ordners `m4hDemo1` __auf dem PC__ mit Visual Studio Code (Menü DAtei - Ordner öffnen...)

6. Umbenennen der Dummy-Bezeichnung Xxx   
Beim Umbenennen der Dummy-Bezeichnung `Xxx` in `Demo1` MUSS in den Dateien `C_Cxx.hpp`, `m4hExtension.hpp` und `m4h.conf` die Gro&szlig;-/Kleinschreibung beachtet werden, dh. das Umbenennen muss in drei Schritten erfolgen:
   1. Xxx durch Demo1 ersetzen (Klassenbezeichnung)   
   2. xxx durch demo1 ersetzen (globale Variable)   
   3. XXX durch DEMO1 ersetzen (Definitionen)   

   In Visual Studio Code erfolgt das Ersetzen im Menü   
   `Bearbeiten - In Dateien ersetzen`   
   __Wichtig__: Im Suchfeld __`Aa`__ aktivieren!

7. Zeilen mit `DEMO1_DEMO_KEY` und `_demo_` l&ouml;schen   
   
---   
   
### L&ouml;sung der Anforderungen
8. Forderung {a}: Konfigurationsdatei erg&auml;nzen   
in der Datei `m4h.conf` folgendes eintragen:   
   ```   
   [demo1]
   startvalue: 20
   ```   

9. Forderung {b}: Konfigurationsdaten einlesen   
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
   
10. Forderung {c}: "Anzeige aller Nachrichten"   
* C_Demo1.hpp: In der Methode `onMessage()` erg&auml;nzen:   
   `std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;`   
* m4hExtension.hpp - Kontrolle: In der Funktion f3OnMessage() muss folgende Zeile stehen:   
  `g_demo1.onMessage(mosq, topic, payload);`   
   
11. Forderung {d}: "Reagieren auf eine Nachricht"   
C_Demo1.hpp: In der Methode `onMessage()` erg&auml;nzen:   
```   
 if(topic=="m4hDemo1/get" && payload=="keys") {
  publish(mosq, "m4hDemo1/ret/keys", getKeys(), false);
 }
```   

12. Forderung {e}: Herunterz&auml;hlen vom Startwert   
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

13. Forderung {f}: Ende-Meldung ausgeben   
C_Demo1.hpp: In der Methode `onExit()` erg&auml;nzen:   
`std::cout<<"Thank you for using m4hDemo1!"<<std::endl;`   
   

14. PC: Hinaufkopieren der Projekt-Dateien vom PC auf das RasPi mit WinSCP.   
---   
&nbsp;
###   Erzeugen des Hilfsprogramms

15. Erstellen der ausf&uuml;hrbaren Datei   
```g++ m4hMain.cpp m4hBase.cpp -o m4hDemo1 -lmosquitto -lpthread```   
_Anmerkung 1_: Im Verzeichnis `~/m4hDemo1` wurde die Datei `m4hDemo1` erzeugt.   
_Anmerkung 2_: Die Befehlszeile, mit der die ausf&uuml;hrbare Datei erzeugt wird, steht immer in der Datei `m4hExtension.hpp` und `C_Xxx.hpp`   
   

16. Testen bzw. Ausf&uuml;hren der Datei:   
```./m4hDemo1``` 

17. Bereitstellung der Datei f&uuml;r alle User   
Soll die Datei von jedem User und aus jedem Verzeichnis heraus verwendet werden k&ouml;nnen, sind folgende Schritte erforderlich:   
```sudo cp ~/m4hDemo1/m4hDemo1 /usr/local/bin```   
```sudo chown root /usr/local/bin/m4hDemo1```   
```sudo chmod 777 /usr/local/bin/m4hDemo1```   

18. Automatisches Starten der Datei beim RasPi-Start   
* Datei /usr/local/bin/autostart.sh &ouml;ffnen:   
`sudo nano /usr/local/bin/autostart.sh`   
* Zwischen den printf-Anweisungen einf&uuml;gen:   
`/usr/local/bin/m4hDemo1 &`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
_Anmerkung_: Das "kaufmännisches-und"-Zeichen (Ampersand) & am Ende der Zeile ist Absicht!   
(siehe auch Abschnitt "Eigene Autostart-Datei `autostart.sh`" in [m4h01_RasPiInstall.md](https://github.com/khartinger/mqtt4home/blob/main/m4h01_RasPiInstall.md) )
