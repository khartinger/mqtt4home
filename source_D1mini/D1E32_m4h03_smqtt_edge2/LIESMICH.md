<a href="./README.md">==> English version</a>   
Letzte &Auml;nderung: 13.8.2024 <a name="up"></a>   
<h1>ESP32: Leistungsmessung durch Impulsz&auml;hlung und MQTT-Info</h1>   

# Ziel
Dieses Programm f&uuml;r einen D1mini oder ESP32 realisiert eine Leistungsmessung durch Impulsz&auml;hlung, zeigt die Leistung auf einem 0,66 Zoll gro&szlig;en OLED an und sendet regelm&auml;&szlig;ig MQTT-Nachrichten mit der gemessenen Leistung.   

Zur Messung des elektrischen Energieverbrauchs von Haushalten werden in &Ouml;sterreich Z&auml;hler der franz&ouml;sischen Firma Sagemcom wie zB der S210 (einphasig) oder TR210-D-r (dreiphasig) verwendet. Diese Z&auml;hler haben eine 
LCD-Anzeige und zwei LEDs, die die Wirk- und Blindenergie durch Impuse anzeigen (1000 Impulse/kWh bzw. 1000 Impulse/kVArh). Die erste LED kann zur Leistungsmessung durch Impulsz&auml;hlung verwendet werden.   

## &Uuml;bersicht
* [Erforderliche Hardware](#a10)   
* [Kurzanleitung](#a20)   
* [Berechnung der Leistung](#a30)   
* [Selbstbau-Board "NE555-Schmitt-Trigger"](#a40)   
* [Programmdetails](#a50)   

<a name="a10"></a>   

# Erforderliche Hardware
1. D1 mini oder ESP32 D1 mini   
2. 1x [Selbstbau-Board D1_NE555-Schmitt-Trigger](#a40) mit lichtempfindlichem Widerstand (LDR) oder Foto-LED (Foto-Transistor)   
3. 1x OLED-Display mit SSD1306 controller (zB 0,66" Display mit 64x48 Pixel)   

![ESP32 Leistungsmessung](./images/300_D1_NE555_ST_power.png "ESP32 Leistungsmessung")   
_Bild 1: ESP32 mit D1_NE555-Schmitt-Trigger und 0,66" OLED Shield_   

[Zum Seitenanfang](#up)
<a name="a20"></a>   

# Kurzanleitung
## Vorbereitung der Hardware

1. Buchsenleisten auf das D1 mini oder ESP32 D1 mini Prozessor-Board l&ouml;ten.   
2. Selbstbau-Board NE555-Schmitt-Trigger laut Anleitung [(siehe unten)](#a40) zusammenbauen.   
3. Selbstbau-Board NE555-Schmitt-Trigger und 0,66"-OLED-Shield auf das Prozessor-Board stecken.   
4. Lichtsensor (LDR oder Foto-LED) anschlie&szlig;en.   
5. Prozessor-Board an den USB-Anschluss des PCs/Laptops anschlie&szlig;en.   

## Vorbereitung der Software
1. Dieses Projekt herunterladen und in Visual Studio Code (mit installiertem PlatformIO) &ouml;ffnen.   

2. Im Programm `D1E32_m4h03_smqtt_edge2.ino` die f&uuml;r den gew&auml;hlen Prozessor richtige `#define`-Zeile w&auml;hlen (andere Zeile auskommentieren), zB:   
```   
#define D1MINI          1              // ESP8266 D1mini +pro
//#define ESP32D1         2              // ESP32 D1mini
```   

3. In der Fu&szlig;zeile durch Klick auf `env:` den Prozessor w&auml;hlen (`env:d1_mini` oder `env:wemos_d1_mini32`)

4. In folgender Zeile die WLAN-Daten eingeben:   
```   
SimpleMqtt client(String("Raspi11"),String("12345678"),String("10.1.1.1"),String(TOPIC_BASE));
```   
  * Parameter 1: Name des WLAN (WiFi)   
  * Parameter 2: Passwort des WLAN (WiFi)   
  * Parameter 3: IP des MQTT-Brokers   
  * Parameter 4: Basis-Topic f&uuml;r die MQTT-Kommunikation   

5. Projekt kompilieren und auf den Prozessor laden (Symbol Pfeil nach rechts `->`). Der Prozessor startet automatisch das Programm.   

Nach dem Programmstart sucht der D1 mini/ESP32 nach dem angegebenen WLAN und meldet sich beim MQTT-Broker an.   

Der Prozessor sendet nun periodisch die Leistungswerte.   

M&ouml;chte man den aktuellen Stand der Leistungsmessung wissen, so kann man diesen durch folgende Nachricht anfordern:   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt`   
Antwort zB:
`test/watt/ret/watt 123`   

__Annahmen__:   
* Die Mosquitto-Clients sind auf dem Rechner installiert.   
* Das Basis-Topic lautet `test/watt`   

[Zum Seitenanfang](#up)
<a name="a30"></a>   

# Berechnung der Leistung
## Annahme
Z&auml;hlerkonstante z = 1000 Impulse/kWh = 1 Impuls/Wh   

## Berechnung der Momentanleistung P<sub>1</sub>
Die Momentanleistung P<sub>1</sub> ist die Leistung, die sich aus der Zeit &Delta;t<sub>1</sub> zwischen zwei Impulsen ergibt:   
z = 1 Impuls/Wh   
z = 1 Impuls / (3600 Ws)   
3600 = 1 / (P1*t1)   
P<sub>1</sub> = 3600 / &Delta;t<sub>1</sub> &nbsp; &nbsp; . . . . . mit P<sub>1</sub> in Watt und &Delta;t<sub>1</sub> in Sekunden.   

_Berechnungsbeispiele:_   

| Zeit zwischen zwei Impulsen | Leistung | Strom @ 230V |   
| --------------------------- | -------- | ------------ |   
|   0,1 s        |   36 kW | 156,52 A |   
|   0,2 s        |   18 kW |  78,26 A |   
|   0,5 s        | 7200 W  |  31,30 A |   
|   1 s          | 3600 W  |  15,65 A |   
|   2 s          | 1800 W  |   7,83 A |   
|   5 s          |  720 W  |   3,13 A |   
|  10 s          |  360 W  |   1,57 A |   
|  20 s          |  180 W  |   0,78 A |   
|  30 s          |  120 W  |   0,52 A |   
|  60 s (1 min)  |   60 W  |   0,26 A |   
| 100 s          |   36 W  |   0,16 A |   
| 200 s          |   18 W  |   0,08 A |   
| 300 s (5 min)  |   12 W  |   0,05 A |   
| 600 s (10 min) |    6 W  |   0,03 A |   
| 900 s (15 min) |    4 W  |   0,02 A |   

_Man erkennt_: f&uuml;r kleine Leistungen wird die Dauer zwischen zwei Impulsen sehr lang, f&uuml;r eine Leistung von P1 = 0 W wird die Zeit sogar "unendlich".   

## Mittlere Leistung f&uuml;r einen Zeitraum &Delta;t<sub>n</sub>
Will man die (mittlere) Leistung f&uuml;r einen fixen Zeitraum &Delta;t<sub>n</sub> bestimmen, so z&auml;hlt man die Impulse n in diesem Zeitraum und dividiert dann durch diese Zeit:   
P<sub>n</sub> = 3600 * n / &Delta;t<sub>n</sub> &nbsp; &nbsp; . . . . . mit P<sub>n</sub> in Watt und &Delta;t<sub>n</sub> in Sekunden   

## MQTT-Nachrichten zur Leistungsmessung
Mit Hilfe von MQTT-Anfragen k&ouml;nnen die Momentan- und die mittlere Leistung abgefragt werden:    
__Beispiel__: Abfrage der Momentanleistung   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt`   
Antwort zB:   
`test/watt/ret/watt 123`   

__Beispiel__: Abfrage der mittleren Leistung   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt_`   
Antwort zB:   
`test/watt/ret/watt 123`   
_Anmerkung_: Die Antwort erfolgt mit `/ret/watt` und nicht `/ret/watt_`   

Weiters wird alle 5 Minuten automatisch die mittlere Leistung versendet:   
`test/watt/ret/watt5 123`   
_Anmerkung_: Die Nachricht erfolgt mit `/ret/watt5`   

[Zum Seitenanfang](#up)
<a name="a40"></a>   

# Selbstbau-Board "NE555-Schmitt-Trigger"
Das Selbstbau-Board "NE555-Schmitt-Trigger" dient dazu, das Ausgangssignal des Sensors (LDR) zu sch&auml;rfen. Dies ist sinnvoll, weil die Leistungsberechnungssoftware mit Flankenerkennung arbeitet.   

![D1_NE555_ST Schaltung](./images/300_D1_NE555_ST_circuit.png "D1_NE555_ST Schaltung")   
_Bild 2: Schaltung des D1_NE555_ST Shields (Boards)_   

_Bauteile:_   
* 1x NE555 mit 8-poliger Fassung   
* 1x LDR (Lichtempfindlicher Widerstand) mit 2 k&Omega; bei mittlerer Helligkeit   
* 1x kleiner Trimmer (stehend) R1 mit 10 k&Omega;   
* 1x Kondensator C1 mit 10 nF   
* 1x Kondensator C2 mit 100 nF   
* 1x 2polige Stiftleiste J1, 90° abgewinkelt   
* Low-Current-LED D1 (3mm, 2mA) und Vorwiderstand 1 k&Omega;   

Die Ausgangsspannung des LM555 liegt laut Datenblatt bei einer Versorgungsspannung von 5V bei 3,3V (gemessen 3,6V), sodass die Ausgangsspannung direkt an den Eingangspin des Prozessors gelegt werden kann.   
Die Schaltschwellen des Schmitt-Triggers liegen bei 1/3 bzw. 2/3 der Versorgungsspannung, also bei ca. 1,7V und 3,3V. Mit Hilfe des Trimmers R1 
k&ouml;nnen die Schaltzeitpunkte an die Umgebungshelligkeit angepasst werden.   
Die Low-Current-LED D1 (2mA) mit Vorwiderstand R2 dient zur Anzeige des Schaltzustandes und kann auch weggelassen werden.   
Die beiden Kondensatoren C1 und C2 verhindern, dass die Schaltung hochfrequent schwingt.   
Sollte der LDR hochohmiger sein, so muss man einen 50 k&Omega; Trimmer verwenden.   

Der Aufbau der Schaltung kann auf einem D1 mini Protoboard erfolgen.   

![D1_NE555_ST Protoboard](./images/300_D1_NE555_ST_shield.png "D1_NE555_ST Protoboard")   
_Bild 3: Aufbau des D1_NE555_ST Shields auf einem D1 mini Protoboard_   

[Zum Seitenanfang](#up)
<a name="a50"></a>   

# Programmdetails

## Entwicklungsumgebung
Das Programm wurde in Visual Studio Code und PlatformIO erstellt.   

## Programmteile
Das Programm besteht aus sechs Abschnitten.   

### 1. Definitionen, Deklarationen und globale Variable
Hier werden hardware-spezifische Dinge definiert, je nachdem, ob man einen D1 mini oder ESP32 verwendet, zB ob die LED leuchtet, wenn der Ansteuer-Pin auf 0 (D1 mini) oder 1 (ESP32) liegt.   
Anschlie&szlig;end folgen Definitionenen f&uuml;r den OLED-Bildschirm.   

### 2. Interrupt Funktionen
Dieser Abschnitt beinhaltet Definitionen und globale Variable f&uuml;r die Verwendung in den Interrupt-Routinen sowie die Interrupt-Programme selbst.   
Die Funktion `io_ISR` wird bei jeder steigenden Flanke an D5 aufgerufen, `myTimer_ISR` alle 5 Minuten.   
Die Funktion `startMyTimerInterrupt()` startet den Timer, wobei die Codierung abh&auml;ngig vom verwendeten Prozessor ist.   

### 3. MQTT Funktionen
Dieser Bereich erstellt ein SimpleMqtt-Objekt und definert die Reaktion des Programms auf alle MQTT-Nachrichten.    

### 4. Anzeige- (Display-) Funktionen
Die beiden Funktionen `showLine(int line_, String text_)` und `refreshScreen()` vereinfachen die Ansteuerung der OLED-Anzeige.   
Mit `refreshScreen()` wird alle f&uuml;nf Minuten der OLED-Bildschirm gel&ouml;scht und neu gezeichnet, wodurch ein Einbrennen von Bildschirminhalten etwas verhindert wird.   

### 5. Weitere Funktionen
Die Funktion `getsWatt5()` berechnet die mittlere Leistung der letzten 5 Minuten und gibt den Wert als String zur&uuml;ck.   

### 6. Standardfunktionen
Dieser Abschnitt enth&auml;lt die Standardfunktionen `setup()` und `loop()`.   

[Zum Seitenanfang](#up)
