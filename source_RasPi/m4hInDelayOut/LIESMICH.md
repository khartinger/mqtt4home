Letzte &Auml;nderung: 13.02.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>m4hInDelayOut: Empfangen und Senden von MQTT-Nachrichten</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

## Zweck des Programms
Dieses Programm verwendet `m4hBase` als Basis, um nach dem Eintreffen einer bestimmten Nachricht zeitverz&ouml;gert eine (andere) Nachricht auszuschicken.   
Das Topic der eingehenden Nachricht muss bekannt sein.   
### Weiters gilt
1. Jede eingehende und (dazugeh&ouml;rende) ausgehende Nachricht m&uuml;ssen in der Konfigurationsdatei (Vorgabe `m4h.conf`) in jeweils einer Sektion `[indelayout]` definiert werden.   
(Schl&uuml;sselwort `in` bzw. `out`)   
2. F&uuml;r die ausgehende Nachricht kann festgelegt werden, ob das Retain-Flag gesetzt werden soll oder nicht.   
(Schl&uuml;sselwort `retain`, Vorgabe: `false`)   
3. F&uuml;r die ausgehende Nachricht kann festgelegt werden, ob die selbe Nachricht mehrmals hintereinander gesendet werden darf.   
(Schl&uuml;sselwort `allowsame`, Vorgabe: `false`)   
4. Die Verz&ouml;gerungszeit kann entweder fix in der Konfigurationsdatei oder dynamisch in der Payload angegeben werden.   
(Schl&uuml;sselwort `delay`, Vorgabe `0`ms)   
5. Wie der Inhalt der Payload interpretiert werden soll kann &uuml;ber das Schl&uuml;sselwort `action` gesteuert werden (siehe unten).
6. Bestimmte Platzhalter werden in der ausgehenden Payload durch entsprechende Werte ersetzt:   
```<in> | <text> | <delay> | <value> | <invert> | <brokertime> | <time> ```   
   `<in>` = eingehendes Topic, `<delay>` = Verz&ouml;gerungszeit in ms, `<brokertime>` = aktuelle Zeit im Format "yyyymmdd HHMMSS", `<time>` = aktuelle Zeit im deutschen Format "dd.mm.yyyy HH:MM:SS"
7. Das Programm ist beliebig erweiterbar (siehe Beispiel unten).   

### Implementierte "action:" Parameter
* `text ........ ` Die Payload entspricht dem Platzhalter `<text>`   
* `delay ....... ` Die Payload ist die Verz&ouml;gerungszeit in Millisekunden   
* `delaytext ... ` Die Payload besteht aus der Verz&ouml;gerungszeit (in ms) und dem Text   
* `setvalue .... ` Der danach stehende Wert ist als Platzhalter `<value>` verf&uuml;gbar   
* `invert ...... ` Die nachfolgenden zwei Werte werden mit der eingehenden Payload verglichen und der andere Wert als `<invert>` zur Verf&uuml;gung gestellt   
   
### Liste der Schl&uuml;sselw&ouml;rter in der Konfigurationsdatei
`in | delay | action | out | retain | allowsame`   

&nbsp;
   
## Beispiele f&uuml;r die Konfiguration
Um den nachfolgenden Text einfacher zu gestalten, werden Nachrichten folgenderma&szlig;en dargestellt:   
`-t topic -m payload`   

### Beispiel 1: Verz&ouml;gertes Weiterschicken einer Nachricht
Auf die eingehende Nachricht `-t /test/1/get -m name` soll nach vier Sekunden eine Antwort-Nachricht `-t /test/1/ret/name -m "Program name: m4hInDelayOut (4000 ms)."` gesendet werden.   
Der Eintrag in der Konfigurationsdatei `m4h.conf` dazu sieht folgenderma&szlig;en aus:   
```   
[indelayout]
in:     test/1/get name
delay:  4000
out:    test/1/ret/name Program name: m4hInDelayOut (<delay> ms).
```   
Das Programm zerlegt automatisch die beiden Zeilen `in:` und `out:` in __Schl&uuml;ssel__ (Zeichen bis zum Doppelpunkt dh. in bzw. out), __Topic__ (Zeichen - ohne Leerzeichen - nach dem Doppelpunkt bis zum 1. Leerzeichen) und __Payload__ (nach dem Leerzeichen bis zum Ende der Zeile).

### Beispiel 2: Senden einer Nachricht mit der aktuellen Zeit
Nach dem Eintreffen der Nachricht `-t test/1/get -m time` soll eine Nachricht mit der aktuellen Zeit im deutschen Format ausgeschickt werden.   
```   
[indelayout]
in:     test/1/get time
out:    test/1/ret/time time (german format): <time>
```   

### Beispiel 3: Ausschaltverz&ouml;gerung
Nach dem Einschalten einer Lampe durch die Nachricht `-t test/1/set/lamp -m 1` soll sie nach 5 Sekunden durch die Payload `0` ausgeschaltet werden.   
```   
[indelayout]
in:     test/1/set/lamp
delay:  5000
action: setvalue 0
out:    test/1/set/lamp <value>
```   

### Beispiel 4: Brokertime
Nach dem Eintreffen der Nachricht `-t getTime -m ?` soll eine Nachricht mit der aktuellen Zeit als "Brokertime" im Format "yyyymmdd HHMMSS" ausgeschickt und gespeichert werden (retain=true).   
```   
[indelayout]
in:     getTime ?
out:    brokertime <brokertime>
retain: true
```   

### Beispiel 5: Verz&ouml;gertes Weiterschicken einer Payload unter anderem Topic
Die Payload der Nachricht `-t test/2/set/text -m "Hallo Welt!"` soll nach 4 Sekunden unter dem Topic `test/2/ret/text` weitergeschickt werden.   
```   
[indelayout]
in:     test/2/set/text
delay:  4000
action: text
out:    test/2/ret/text <text>
```   

### Beispiel 6: Verz&ouml;gerungszeit als Payload
Nach dem Eintreffen der Nachricht `-t test/3/delay/set -m 3000` soll nach der angegebenen Zeit (3000ms = 3 Sekunden) die in der Konfigurationsdatei angegebene Nachricht ausgeschickt werden.   
```   
[indelayout]
in:     test/3/delay/set
action: delay
out:    test/3/delay/ret Payload was delay time (Delay: <delay>ms)
```   

### Beispiel 7: Die eintreffende Payload enth&auml;lt Verz&ouml;gerungszeit und Text
Nach dem Eintreffen der Nachricht `-t test/4 -m "3000 Waited for 3 seconds :)"` soll nach 3 Sekunden der angegebene Text ("Waited for 3 seconds :)") unter dem Topic `test/4/ret` ver&ouml;ffentlicht werden.
```   
[indelayout]
in:      test/4
action:  delaytext
out:     test/4/ret <text>
```   
Die Aktion "delaytext" zerlegt die eintreffende Payload mit Hilde des ersten Leerzeichens in eine Verz&ouml;gerungszeit (3000) und den Rest als Text (in `<text>`).

### Beispiel 8: Invertiertes Weitersenden einer Payload
Die Payload einer eintreffenden Nachricht (zB `-t test/5/invert -m 1`) wird mit den beiden in der Konfigurationsdatei gespeicherten Werte verglichen. Bei &Uuml;bereinstimmung mit dem einen Wert wird der andere Wert im Platzhalter `<invert>` zur Verf&uuml;gung gestellt und kann zB unter dem Topic `test/5/invert/ret` weitergeschickt werden.   
```   
in:     test/5/invert
delay:  2000
action: invert 0 1
out:    test/5/invert/ret <invert>
retain: false
```   

### Beispiel 9: Wiederholtes Senden einer Nachricht
Setzt man in der Konfigurationsdatei die Eintr&auml;ge von `in:` und `out:` gleich, so kann es zu einem endlosen Senden der Nachricht kommen. Als Vorgabe ist das nicht erlaubt. M&ouml;chte man absichtlich das endlose Senden einer Nachricht erlauben, muss das Schl&uuml;sselwort `allowsame` auf `true` gesetzt werden.   
```   
[indelayout]
in:    test/6 -1
delay: 2000
out:   test/6 -1
allowsame: true
#allowsame: false
```   
Anwendungsbeispiele k&ouml;nnten sein: zB das regelm&auml;&szlig;ige Ansto&szlig;en/Abfragen eines Sensors oder ein Blinklicht usw.

## Erweiterung des Programms
Das Erweitern des Programms soll an einem einfachen Beispiel gezeigt werden:   
Die Payload soll als ganze Zahl interpretiert werden und die um 1 erh&ouml;hte Zahl soll als Platzhalter `<inc>` zur Verf&uuml;gung stehen. Wird ein in der Konfigurationsdatei angegebener Grenzwert &uuml;berschritten oder ist die Payload keine Ganzzahl, wird `<inc>` auf `0` gesetzt.   
Der Eintrag in der Konfigurationsdatei soll unter dem Topic `test/counter` einen Endlos-Z&auml;hler von 0 bis 12 im 3-Sekundentakt erzeugen.   

### L&ouml;sung f&uuml;r die Konfigurationsdatei
```   
[indelayout]
in:        test/counter
delay:     3000
action:    inc 12
out:       <in> <inc>
allowsame: true
```   
### Erg&auml;nzungen in der Datei C_InDelayOut.hpp   
   
1. Bei den bestehenden `#define`-Anweisungen folgendes erg&auml;nzen:   
   ```
   #define IDO_PLAHO_INC        "<inc>"
   #define IDO_ACT_INC          "inc"
   ```   
   _Anmerkung_: Die Definitionen des Platzhalters ("PLAHO") und des "action"-Kommandos ("ACT") dienen nur dazu, das Programm &uuml;bersichtlich zu halten ;)   

2. In der Methode `void InDelayOut::threadFunctionDelay()` am Beginn nach `std::string s1;                       // help value` eine Variable ´sInc´ zum Zwischenspeichern des `<val>`-Wertes einf&uuml;gen:   
   ```   
   std::string sInc="0";                 // increment value
   ```   

3. In der Methode `void InDelayOut::threadFunctionDelay()` zB nach dem Block `if(actionKey==IDO_ACT_INVERT) {}` den Block zum Erh&ouml;hen des Payload-Wertes einf&uuml;gen:   
   ```   
   if(actionKey==IDO_ACT_INC) 
   {//---------increment payloadIn-------------------------------
    try{
     int iPayloadIn=std::stoi(cpay);    // payload in as int
     int iIncmax=std::stoi(actionVal);  // config max value as int
     iPayloadIn++;                      // increment value
     if(iPayloadIn>iIncmax) temp=0;     // limit check
     sInc=std::to_string(iPayloadIn);   // convert to string
    } catch(std::string& error) { return; }
   }
   ```   

4. In der Methode `void InDelayOut::threadFunctionDelay()` noch __VOR__ der Zeile `if(m2d.payloadOut.length()<1) return;` das Ersetzen des Platzhalters durchf&uuml;hren:   
   ```   
   g_utils.replaceAll(m2d.payloadOut,IDO_PLAHO_INC,sInc);
   ```   

### &Uuml;bersetzen und testen des Programms
1. &Uuml;bersetzen des Programms:   
   `g++ m4hMain.cpp m4hBase.cpp -o m4hInDelayOut -lmosquitto -lpthread
`   
2. Starten des Programms:   
   `./m4hInDelayOut`   
   
3. Starten des Z&auml;hlers durch zB folgende MQTT-Nachricht:   
   `mosquitto_pub -t test/counter -m 10`    
   _Anmerkung_: 10 ist ein Beispiel f&uuml;r den Startwert   
   Ergebnis in einer zweiten Konsole (mit `mosquitto_sub -t "#" -v`):   
   ```   
   info/start m4hInDelayOut (06.09.2021 16:47:10)
   test/counter 10
   test/counter 11
   test/counter 12
   test/counter 0
   test/counter 1
   info/end__ m4hInDelayOut (06.09.2021 16:47:27)
   ```   