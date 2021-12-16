Letzte &Auml;nderung: 11.11.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>D1 mini: Schalten eines Relais via MQTT - Version 2</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h203_D1smqttRelayD1_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Mission / Ziel
Dieses Programm f&uuml;r einen D1 mini oder ESP32 D1mini dient zum Schalten eines Relais via MQTT, wobei als Basis die Klasse `SimpleMqtt` verwendet wird.   
### Annahmen
| Annahme f&uuml;r ...             | Wert            |   
| --------------------------- | --------------- |   
| ... WLAN name               | `Raspi11`       |   
| ... WLAN password &nbsp;    | `12345678`      |   
| ... IP address              | `10.1.1.1`      |   
| ... Basis-Topic             | `ci/lamp/1`     |   
| ... Relais-Pin              | D1 bzw. GPIO 22 |   
   
### Funktion
Nach dem Start wird die blaue LED eingeschaltet und der D1 mini versucht sich mit dem eingestellten Broker zu verbinden. Gelingt dies, schaltet er die blaue LED ab und es wird eine Startnachricht gesendet (zB `info/start/mqtt {"topicbase":"ci/lamp/1","IP":"10.1.1.160"}`). Zum Schalten des Relais dienen folgende Nachrichten:   
| Topic                  | Payload | Ergebnis                  |   
|------------------------|:-------:|---------------------------|   
| `ci/lamp/1/set/lamp`   |    1    | Lampe/Relais ein          |   
| `ci/lamp/1/set/lamp`   |    0    | Lampe/Relais aus          |   
| `ci/lamp/1/set/lamp`   |   -1    | Lampe/Relais umgeschaltet |   

### M&ouml;gliche Erweiterungen des Programms:
* Statusanzeige der WLAN- und MQTT-Verbindung &uuml;ber (zB verschiedenfarbige) LEDs
* Kontrolle, ob das Relais wirklich geschaltet hat, zB durch das Messen des Verbraucherstromes

## Erforderliche Hilfsmittel
* Hardware: D1 mini oder ESP32 mini
* Hardware: Relay-Shield (Ansteuerung &uuml;ber Pin D1=GPIO 22)
* Hardware: Laptop/PC mit WLAN-Empfang und installierter Mosquitto-Software
* Software: Arduino-Entwicklungsumgebung oder Visual Studio Code mit zumindest installierten Erweiterungen PlatformIO IDE und C/C++

## Erstellen des Programms
Ausgehend vom [smqtt_template1](https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h01_smqtt_template1/D1_m4h01_smqtt_template1.ino) m&uuml;ssen folgende &Auml;nderungen bzw. Erg&auml;nzungen vorgenommen werden:   
1. &Auml;nderung der Definitionen f&uuml;r das Basis-Topic und die GET- und SET-Topics:   
```   
#define  VERSION_M4H02  "2021-11-11 D1_m4h02_smqtt_relayD1"
#define  TOPIC_BASE     "ci/lamp/1"
#define  TOPIC_GET      "?,help,version,ip,topicbase,eeprom,lamp"
#define  TOPIC_SET      "topicbase,eeprom,lamp"
```   
   
2. Definitionen f&uuml;r die blaue LED und das Relais:   
```   
//_____sensors, actors, global variables________________________
#if defined(ESP32) || defined(ESP32D1)
 #define RELAY_PIN      22                  // D1=22
 #define BLUELED_PIN    2                   // led pin D4=IO2
 #define BLUELED_ON     1                   // ESP32 1
 #define BLUELED_OFF    0                   // ESP32 0
#else
 #define RELAY_PIN      D1                  // D1=22
 #define BLUELED_PIN    D4                  // led pin D4=IO2
 #define BLUELED_ON     0                   // D1mini 0
 #define BLUELED_OFF    1                   // D1mini 1
#endif
 #define RELAY_ON       1                   //
 #define RELAY_OFF      0                   //
int      ledVal_=BLUELED_ON;                // pin value
int      relayVal_=RELAY_OFF;               // relay off
```   
   
3. Erg&auml;nzung in der Funktion `String simpleGet(String sPayload)` vor der Zeile    
`return String("");                         // wrong set command`   
   Beantwortung der GET-Anfrage "lamp":   
```   
 //-------------------------------------------------------------
 if(sPayload=="lamp") {
  if(relayVal_==RELAY_ON) return String("on"); 
  else return String("off"); 
 }
```   
   
4. Erg&auml;nzung in der Funktion `String simpleSet(String sTopic, String sPayload)` vor der Zeile    
`return String("");                         // wrong set command`   
   Bearbeitung der SET-Anfrage "lamp":   
```   
 //-------------------------------------------------------------
 if(sTopic=="lamp") {                       // switch blue led?
  if(sPayload=="on" || sPayload=="1") relayVal_=RELAY_ON;
  else {                                    // other command
   if(sPayload=="off" || sPayload=="0") relayVal_=RELAY_OFF;
   else {
    if(sPayload=="toggle" || sPayload=="-1")
      relayVal_=1-relayVal_;                // toggle led
   }
  }
  digitalWrite(RELAY_PIN,relayVal_);         // turn led on/off
  if(relayVal_==RELAY_ON) return String("on"); // return answer
  return String("off");                     // return answer
 }
```   

5. Erg&auml;nzung in der setup-Funktion im Abschnitt "init pins":   
```   
 //-----init pins-----------------------------------------------
 pinMode(BLUELED_PIN, OUTPUT);              // set pin to output
 digitalWrite(BLUELED_PIN,ledVal_);         // turn led on
 pinMode(RELAY_PIN, OUTPUT);                // set pin to output
 digitalWrite(RELAY_PIN,relayVal_);         // turn relay off
```   

6. &Auml;nderung der loop-Funktion so, dass die blaue LED eingeschaltet wird, sobald die Verbindung zum Broker verloren geht:   
```   
//_____LOOP_____________________________________________________
void loop() {
 client.doLoop();                           // mqtt loop
 if(client.isMQTTConnected()) ledVal_ = BLUELED_OFF;
 else ledVal_ = BLUELED_ON;
 digitalWrite(BLUELED_PIN,ledVal_);         // turn led on/off
 delay(100);
}
```   

Das vollst&auml;ndige Listing befinet sich auf [https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h02_smqtt_relayD1/src/D1_m4h02_smqtt_relayD1.ino](https://github.com/khartinger/mqtt4home/blob/main/source_D1mini/D1_m4h02_smqtt_relayD1/src/D1_m4h02_smqtt_relayD1.ino).

## Test des Programms
### Vorbereitung
1. Laptop/PC: Erstellen des Projekts `D1_m4h02_smqtt_relayD1`   
[---> Quellcode siehe https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h02_smqtt_relayD1](https://github.com/khartinger/mqtt4home/tree/main/source_D1mini/D1_m4h02_smqtt_relayD1)

2. Laptop/PC: Build und Upload des Programms auf den D1 mini.   

3. RasPi mit dem Mosquitto-Broker starten (falls dies noch nicht erfolgt ist).   

4. Laptop/PC: Mit dem WLAN-Netz `Raspi11` verbinden (Passwort `12345678`).   

5. Laptop/PC: Ein Kommando-Fenster ("Eingabeaufforderung") &ouml;ffnen:   
`cmd.exe`
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
`c:`   
`cd /programme/mosquitto`

6. Laptop/PC: Die Anzeige aller Nachrichten zulassen:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
Mit dem Schalter `-h` wird die IP-Adresse des Raspi angegeben.   

7. Laptop/PC: Ein zweites Eingabeaufforderungs-Fenster &ouml;ffnen:   
`cmd.exe`
im Startmen&uuml; eingeben.   
Ins richtige Laufwerk und Mosquitto-Verzeichnis wechseln:   
`c:`   
`cd /programme/mosquitto`

### Durchf&uuml;hrung des Programm-Tests
1. D1 mini: Reset-Knopf dr&uuml;cken   
   Warten, bis die blaue LED erlischt. Danach   
   Anzeige im ersten Eingabeaufforderungs-Fenster:   
   `info/start/mqtt {"topicbase":"ci/lamp/1","IP":"10.1.1.169"}`   

2. Relais einschalten   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m 1`   
   _Anzeige im Fenster 1_:   
   `ci/lamp/1/set/lamp 1`   
   `ci/lamp/1/ret/lamp on`   
   Das Relais sollte jetzt eingeschaltet sein ;)   

3. Relais ausschalten   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m 0`   
   _Anzeige im Fenster 1_:   
   `ci/lamp/1/set/lamp 0`   
   `ci/lamp/1/ret/lamp off`   
   Das Relais sollte jetzt ausgeschaltet sein ;)   

4. Relais umschalten   
   _Eingabe im Fenster 2_:   
   `mosquitto_pub -h 10.1.1.1 -t ci/lamp/1/set/lamp -m -1`   
   _Anzeige im Fenster 1_:   
   `ci/lamp/1/set/lamp -1`   
   `ci/lamp/1/ret/lamp on`   
   Das Relais sollte jetzt eingeschaltet sein ;)   
