# RasPi: Reaktion auf bestimmte Schl&uuml;ssel in (JSON-)Payloads
Programm: m4hPayload, Version 2021-08-30   
[--> English Version](./readme.md "English Version")   

## Zweck des Programms
Das Programm `m4hPayload` dient als Grundlage f&uuml;r ein C++ Programm, das   
* beim Finden eines bestimmten Schl&uuml;ssels in der Payload einer Nachricht   
* eine MQTT-Antwort-Nachricht versenden kann.   

_Beispiel_: Unterschreitet der Wert des Schl&uuml;ssels "`battery`" __eines beliebigen Topics__ (!) einen bestimmten Wert, so wird eine Warnungs-Nachricht geschickt.   

### Grunds&auml;tzlicher Aufbau des Programms
Das Programm  `m4hPayload` ist so aufgebaut, dass   

*  die Bearbeitung des Schl&uuml;ssels lediglich in der Methode   
   `bool Payload::editPayload(Payload1 &p1, std::string sValue)`   
   der Datei `C_Payload.hpp` erg&auml;nzt werden muss (als C++ Code) und   

*  Details zur Bearbeitung eines Schl&uuml;ssels in der Konfigurationsdatei (zB `m4h.conf`) stehen.   

Durch diese Aufteilung ist es m&ouml;glich, nachtr&auml;gliche Anpassungen lediglich in der Konfigurationsdatei durchf&uuml;hren zu m&uuml;ssen.   

## Beispiel: Erg&auml;nzen einer "Frostwarnung"
Das Programm  `m4hPayload` soll um folgende Funktionalit&auml;t erg&auml;nzt werden:   

Unterschreitet die Temperatur eines Sensors einen Wert von 3 Grad Celsius, so soll folgende Nachricht versendet werden:   
`-t info/frost_warning -m <in> (<value>degree)`   
wobei `<in>` durch das Topic des Temperatursensors und `<value>` durch den aktuelle Temperaturwert ersetzt werden soll.   

_Anmerkung_: Durch &Auml;ndern des Topics in `send/sms ...` k&ouml;nnte zB auch das Versenden einer SMS erreicht werden...   

### L&ouml;sung Teil 1: Eintrag in der Konfigurationsdatei   
In der Konfigurationsdatei m&uuml;ssen in einer Sektion `[payload]` drei 
Informationen angegeben werden:   
* `key: ` Der Schl&uuml;ssel, nach dem in der (JSON-formatierten) Payload gesucht werden soll ( hier `temperature`)
* `if:  ` Die Bedingung, die erf&uuml;llt werden soll. Diese besteht aus 3 Teilen:   
   1. Vergleichszeichen (hier `<`), 
   2. Wert, mit dem der hereinkommende Wert verglichen werden soll (hier `3`) und
   3. Text, der ausgegeben werden soll, wenn die Bedingung erf&uuml;llt ist   
   (hier "`<in> (<value> *C)`", wobei `<in>` durch das Topic und `<value>` durch den Temperaturwert automatisch ersetzt werden).
* `out: ` dem Topic f&uuml;r die ausgehende Nachricht (hier `info/frost_warning`).   
_Anmerkung_: Die Payload steht beim `if:`.   

Ergebnis:   
```   
[payload]
key:    temperature
if:     <3 <in> (<value> *C)
out:    info/frost_warning
```   
   
### L&ouml;sung Teil 2: C++ Code   
Folgender C++ Code muss in der Datei `C_Payload.hpp` in der Methode 
 `bool Payload::editPayload(Payload1 &p1, std::string sValue)` vor der Zeile   
 ` //=========== add your code here... ======================`   
 eingef&uuml;gt werden:   
```   
 //===========process payload key "temperature"=================
 if(p1.key=="temperature")
 {//----------skip given topics---------------------------------
  for(int ii=0; ii<p1.vSkip.size(); ii++) {
   if(p1.m2.topicIn==p1.vSkip.at(ii)) return false;
  }
  //----------process condition---------------------------------
  try { //----string to double----------------------------------
   double dValIn=0, dValConf=0;        // temperature as double
   double temp=std::stod(sValue);      // convert incomming value
   dValIn=temp;                        // store value
   temp=std::stod(p1.ifVal);           // convert config value
   dValConf=temp;                      // store value
   //---------process condition---------------------------------
   switch(p1.ifCond) {                 // type of comparison chars
    case 1:                            // 1 means '<' in config
     if(dValIn<dValConf) {             // compare temperatures
      sPay+=p1.ifText;                 // payload = if text
      bRet=true;                       // yes, send warning msg
     }                                 // end if(dValIn<dValConf)
     break;                            // end of case 1
    default: break;                    // all other comp chars
   }                                   // end switch
  } catch(std::invalid_argument) {};   // do nothing on error
 } // end process payload key "temperature"
```   

__*Erkl&auml;rung*__:   
Eigenschaften:   
* `p1` ist ein Objekt der Klasse `Payload1`, das alle Informationen der aktuellen Nachricht und der aktuellen Sektion enth&auml;lt.   
* `sValue` ist der aktuelle Wert zum Schl&uuml;ssel (`temperature`).   

Code:   
* `if(p1.key=="temperature"){}` stellt den Block dar, in dem der Schl&uuml;ssel `temperature` bearbeitet wird.   
* In der for-Schleife werden alle Topics, die in der Konfigurationsdatei in der Sektion unter `skip:` aufgelistet sind, mit dem aktuellen Topic verglichen. Stimmt das Topic &uuml;berein, wird die Verarbeitung abgebrochen (`return false;`)
* Im try-catch-Block werden zuerst die beiden Temperaturwerte in Double-Zahlen umgewandelt. Bei einem Fehler wird zu catch gesprungen und der nachfolgende Code wird nicht ausgef&uuml;hrt.
* Der `switch(p1.ifCond)`-Block unterscheidet zwischen den verschiedenen Vergleichszeichen, wobei beim Einlesen der Konfigurationsdatei jedem Zeichen eine Ganzzahl zugeordnet wird:   
1 bedeutet `<`, 2 `<=`, 3 `=`, 4 `!=`, 5 `>=` und 6 `>`   
* In `case 1:` werden die beiden Temperaturen verglichen. Ist die Temperatur zu klein, wird der if-Text an die Payload angeh&auml;ngt und mit `bRet=true;` das Senden der Antwort-Nachricht erlaubt.   


### Test
Erzeugen der ausf&uuml;hrbaren Datei durch Eingabe von   
`g++ m4hMain.cpp m4hBase.cpp -o m4hPayload -lmosquitto -lpthread`   
Nach dem Starten des Programmes durch   
`pi_@raspi:~/m4hPayload $ ./m4hPayload`   
erh&auml;lt man folgende Ausgabe:   
```   
Read config file ./m4h.conf: OK
-----[base]---------------------------
config file         | ./m4h.conf
section name        | base
version (in)        | -t m4hPayload/get -m version
version (out)       | -t m4hPayload/ret/version -m 2021-08-30
mqtt @ start (out,*)| -t info/start -m m4hPayload
mqtt @ end (out,*)  | -t info/end__ -m m4hPayload
progend by mqtt (in)| -t m4hPayload/set -m -end-
         * add time | true
-----[payload]------------------------
config file         | ./m4h.conf
all keys            | in|skip|key|if|else|out|retain|<in>|<value>
.....Payloads to check................
topic-in: (all) | skip: (none)
     key: temperature | if <3 <in> (<value> *C) else  | OUT: -t info/frost_warning -m
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

Sendet man eine Nachricht, zB   
`mosquitto_pub -t zigbee/sensor1 -m "{\"battery\":100,\"humidity\":54.17,\"linkquality\":36,\"pressure\":974.4,\"temperature\":2.32,\"voltage\":3035}"`   
so erh&auml;lt man folgende MQTT-Nachrichten:   
```   
zigbee/sensor1 {"battery":100,"humidity":54.17,"linkquality":36,"pressure":974.4,"temperature":2.32,"voltage":3035}
info/frost_warning zigbee/sensor1 (2.32 *C)
```   
_Hinweis_: Die Payload zB des Temperatursensors TS0201 (TuYa Temperature & humidity sensor with display) ist folgenderma&szlig;en aufgebaut:   
`{"battery":100,"humidity":54.17,"linkquality":36,"pressure":974.4,"temperature":22.32,"voltage":3035}`   