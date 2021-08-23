# m4hSubPub: Empfangen und Senden von MQTT-Nachrichten
## Zweck des Programms
Dieses Programm verwendet `m4hBase` als Basis , um folgenden Aufgaben zu erf&uuml;llen:
1. Nach dem Empfang einer Nachricht wird eine bestimmte Zeit gewartet und dann eine Antwortnachricht gesendet.
2. Jede eingehende und (dazugeh&ouml;rende) ausgehende Nachricht sowie die Wartezeit m&uuml;ssen in der Konfigurationsdatei (Vorgabe `m4h.conf`) in jeweils einer Sektion `[subpub]` definiert werden.
3. F&uuml;r die ausgehende Nachricht kann festgelegt werden, ob das Retain-Flag gesetzt werden soll oder nicht (Schl&uuml;sselwort retain, Vorgabe: false)
4. Das Programm ist so aufgebaut, dass es einfach zur Verarbeitung von Laufzeit-Daten erweitert werden kann (JSON-formatierte eingehende Payload oder action-Key). Eingebaute Beispiele sind:
   * Mitschicken der Wartezeit in der Payload {"delay": 4000}
   * Mitschicken der ausgehenden Payload {"text": "new payload out :)"}
   * Schicken der aktuellen Uhrzeit {"do":"brokertime"} oder {"do":"time"}
   * Invertiertes Weiterschicken der eingehenden Payload 0 bzw. 1 als 1 bzw. 0 {"do":"invert01"}
   

## Beispiele f&uuml;r die Konfiguration
Um den nachfolgenden Text einfacher zu gestalten, werden Nachrichten folgenderma&szlig;en dargestellt:   
`-t topic -m payload`   

### Beispiel 1: Verz&ouml;gertes Weiterschicken einer Nachricht
Auf die eingehende Nachricht `-t /test/1/get -m name` soll nach zwei Sekunden eine Antwort-Nachricht `-t /test/1/ret/name -m "my name is m4hSubPub"` gesendet werden.   
Der Eintrag in der Konfigurationsdatei `m4h.conf` dazu sieht folgenderma&szlig;en aus:   
```   
[subpub]
in:     /test/1/get name
out:    /test/1/ret/name my name is m4hSubPub
delay:  2000
```   
Das Programm zerlegt automatisch die ersten beiden Programmzeilen in __Schl&uuml;ssel__ (Zeichen bis zum Doppelpunkt dh. in bzw. out), __Topic__ (Zeichen - ohne Leerzeichen - nach dem Doppelpunkt bis zum 1. Leerzeichen) und __Payload__ (nach dem Leerzeichen bis zum Ende der Zeile).

### Beispiel 2: Weiterschicken einer Nachricht nach mitgeschickter Wartezeit
Nach dem Eintreffen einer Nachricht `/test/2` soll die angegebene Zeit (zB 3 Sekunden) gewartet und eine Antwort mit Topic `/test/2/ret` und mitgeschickter Payload (zB `waited 3 seconds :)`) geschickt werden.   
Eintrag in der Konfigurationsdatei `m4h.conf`:    
```   
[subpub]
# payload e.g. "{\"delay\":3000,\"text\":\"waited 3 seconds :)\"}"
in:     /test/2
out:    /test/2/ret
```   

### Beispiel 3: Abfrage der Broker-Zeit
Auf die eingehende Nachricht `-t /test/3/get -m time` soll eine Antwort-Nachricht `-t /test/3/ret/time` mit Datum und Uhrzeit als Payload gesendet werden. Weiters soll die Nachricht beim Broker gespeichert werden (retain: true).   
Eintrag in der Konfigurationsdatei `m4h.conf`:    
```   
[subpub]
in:     /test/3/get time
out:    /test/3/ret/time
action: {"do":"brokertime"}
retain: true
```   
Anmerkung: `action: {"do":"time"}` liefert die Zeit im deutschen Format dd.mm.yyyy HH:MM:SS