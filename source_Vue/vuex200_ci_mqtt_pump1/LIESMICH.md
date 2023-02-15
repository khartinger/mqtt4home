Letzte &Auml;nderung: 15.2.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiPump1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiPump1`-Symbols.   
Allgemein gilt:   
* Das `CiPump1`-Symbol wartet auf MQTT-Nachrichten einer Pumpensteuerung und zeigt diese an.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiPump1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex200_ci_mqtt_pump1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Das CiPump1-Symbol wartet auf MQTT-Nachrichten der folgenden Art, die zB von Zigbee-Sensoren gesendet werden oder - zur Simulation - mit dem Programm `mosquitto_pub` in einer Windows-Konsole eingegeben werden k&ouml;nnen:   
  * `mosquitto_pub -h 10.1.1.1 -t  z2m/t/pump/-1 -m "{\"sensor\":\"ok-Sensor\",\"pump\":0,\"status\":\"OK HIGH\"}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/pump/-1/time -m "20230214 192233"`   
Die Werte werden entsprechend angezeigt.   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Ci-Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiPump1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![CiPump1-Varianten](./images/vuex200_all_pump1.png "CiPump1-Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiPump1-Darstellungen_   

## CiPump1-Aufruf
Das Zeichnen eines CiPump1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiPump1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiPump1 :x="50" :y="80" sid="pump1" :border="3" :fx="2" :fy="1.5" colorH="white"></CiPump1>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiPump1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiPump1-Symbols](./images/vuex200_pump1_location1.png "Ort eines CiPump1-Symbols")   
_Bild 2: Ort eines CiPump1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")

## Rand eines Symbols (border)
Alle CI-Symbole sind im Normalfall au&szlig;en 100x100 Einheiten gro&szlig; und haben nach innen einen fixen Rand von 5 Einheiten. Die individuelle "Arbeitsfl&auml;che" eines Symbols ist also 90 x 90 Einheiten gro&szlig;.   
Durch den Rand ist es m&ouml;glich, Symbole direkt nebeneinander in einem 100er-Raster anzuordnen, ohne dass sich die Symbol-Zeichnungen optisch ber&uuml;hren.   
Will man den Rand eines CI-Symbols darstellen, so gibt es dazu vier M&ouml;glichkeiten, die &uuml;ber das Attribut `:border=` eingestellt werden:   

| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `:border="0"` | Es wird kein Rand dargestellt. |   
| `:border="1"` | Es wird nur der &auml;u&szlig;ere Rand des Symbols dargestellt (= gesamter Platzbedarf des Symbols). |   
| `:border="2"` | Es wird nur der innere Rand des Symbols dargestellt (= der Zeichenbereich f&uuml;r das Symbol). |   
| `:border="3"` | Es wird der Rand des Symbols gelb angezeigt. |   

Vorgabe ist `:border="2"`   

## Textangaben im CiPump1-Symbol
Jedes `CiPump1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Zeichenbereich (= Textbereich)   

### Kopfzeile   
Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`.   

### Fu&szlig;zeile
1. Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt.   
2. Enth&auml;lt die empfangene MQTT-Nachricht eine "status"-Angabe, so wird dieser Wert angezeigt.   
3. Ist keiner der Punkte 1 und 2 erf&uuml;llt, wird eine leere Zeile angezeigt.   

### Zeichenbereich
Der Zeichenbereich enth&auml;lt drei Zeilen (Zeile 2 bis 4), die folgendes anzeigen:   
#### Zeile 2   
* Enth&auml;lt die empfangene MQTT-Nachricht eine "sensor"-Angabe, so wird dieser Wert links angezeigt.   
* In der Mitte befindet sich das Pumpensymbol.   
* Rechts wird der MQTT-"pump"-Wert als Text angezeigt (`ON`, `OFF` oder `?`)   
#### Zeile 3   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public pump1s: Array<Pump1>` (Datei `CiPump12Controller.ts`), so wird "`unknown`" angezeigt.   
2. Gibt es einen Datumswert, so wird dieser angezeigt (Format `dd.mm.yy`).   
3. Ansonsten wird `Date?` angezeigt.   
#### Zeile 4   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public pump1s: Array<Pump1>` (Datei `CiPump12Controller.ts`), so wird "`sid`" angezeigt.   
2. Gibt es einen Uhrzeit-Wert, wird dieser angezeigt (Format `HH:MM:SS`).   
3. Ansonsten wird `Time?` angezeigt.   

## Strecken eines Symbols (fx, fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Bild zeigt:   
![Gestrecktes CiPump1-Symbol](./images/vuex200_pump1_stretch1.png "Gestrecktes CiPump1-Symbol")   
_Bild 3: Gestrecktes CiPump1-Symbol_   

Die linken beiden CiPump1-Symbole sind um den Faktor fx=2 gestreckt (ergibt 2*100 = 200 Einheiten), das dritte Symbol ist um den Faktor fy=2 gesteckt und das rechte Symbol ist um fx=2 und fy=2 gestreckt, d.h., es ist mit einer Gr&ouml;&szlig;e von 200x200 doppelt so gro&szlig; wie das Original.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiPump1 :x="50" :y="70"  sid="pump1" :fx="2" :fy="1"></CiPump1>
  <CiPump1 :x="50" :y="170" sid="pumpX" :fx="2" :fy="1"></CiPump1>
  <CiPump1 :x="250" :y="70" sid="pump1" :fx="1" :fy="2"></CiPump1>
  <CiPump1 :x="350" :y="70" sid="pump1" :fx="2" :fy="2"></CiPump1>
  </svg>
</template>
```   

## Farbe des CiPump1-Symbols (colorH, colorF)
### Farbe des Zeichenbereichs   
Der Zeichenbereich wird abh&auml;ngig vom Pumpenzustand eingef&auml;rbt:   
| Zustand | Farbbezeichnung     | Farbe        |   
| ------- | ------------------- | ------------ |   
| `on`    | `colorPumpOn`       | hellgr&uuml;n     |   
| `off`   | `colorPumpOff`      | hellgrau     |   
| `?`     | `geof.colorUnknown` | mittelblau   |   

* Tritt ein Fehler auf, wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   
* Ist der Status "ALARM" wird die Hintergrundfarbe auf rot gesetzt. (Farbe "`colorAlarm`" aus der Datei `Geo.ts`)   
* Ist der Status "ERROR" wird die Hintergrundfarbe auf rot gesetzt. (Farbe "`colorError`" aus der Datei `Geo.ts`)   

### Farbe der Kopfzeile   
Mit Hilfe des Attributs `colorH=` kann die Hintergrundfarbe der Kopfzeile eines CiPump1-Symbols gesetzt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#eedd00` oder eine Textangabe wie `red`, `white` etc. sein.   
Wird keine Farbe gew&auml;hlt, ist die Kopfzeile transparent.   

### Farbe der Fu&szlig;zeile   
* Tritt ein Fehler auf, wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   
* Ist der Status "ALARM" wird die Hintergrundfarbe auf rot gesetzt. (Farbe "`colorAlarm`" aus der Datei `Geo.ts`)   
* Ist der Status "ERROR" wird die Hintergrundfarbe auf rot gesetzt. (Farbe "`colorError`" aus der Datei `Geo.ts`)   
* Falls mit `colorF=` ein Farbwert angegeben wurde, wird diese Farbe verwendet.   
* Ansonsten ist der Hintergrund transparent.   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiPump1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiPump1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiPump1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiPump1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiPump1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft         | Bedeutung                            |   
| ------------------- | ------------------------------------ |   
| `iPump1State`       | Status des CiPump1-Symbols. Daf&uuml;r sind in der Datei `CiPump1Controller.ts` die Konstanten `stateOn`, `stateOff`, `stateNotOk` und `stateNoState` definiert.   |   
| `type`              | Typ des CiPump1 Symbols. Sagt etwas dar&uuml;ber aus, welche Daten von der Pumpe erwartet werden.   |   
| `lastPump1Date`     | Datum, wann die letzte MQTT-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Date?` oder `--.--.--` gesetzt werden.   |   
| `lastPump1Time`     | Uhrzeit, wann die letzte MQTT-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Time?` oder `--:--` gesetzt werden.   |   

Folgende Attribute sind optional und k&ouml;nnen bei Bedarf verwendet werden:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `sSensor`      | Ein eventueller Sensorzustand. Wird in Zeile 2 angezeigt.   |   
| `sStatus`      | Statuszustand der Pumpe. Wird in Zeile 5 angezeigt.   |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiPump1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<CiPump1>` erg&auml;nzt werden.   
Im unten stehenden Beispiel ist ein CiPump1-Symbol definiert.   
In der Methode `public onMessage (message: Message): void` werden die Strings f&uuml;r "`lastPump1Date`" und "`lastPump1Time`" dazu genutzt, im Fehlerfall den Text "`ERROR json`" anzuzeigen.   

M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

```
// ______CiPump1Controller.ts____________________khartinger_____
// 2022-10-19: new
// 2023-02-15: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Pump1 extends IBase {
  // ---------mandatory-----------------------------------------
  iPump1State: number;
  type: string;
  lastPump1Date: string;
  lastPump1Time: string;
  // ---------optional------------------------------------------
  sSensor?: string;
  sStatus?: string;
  textFooter?: string;
}

export class CiPump1Controller extends CiBaseController {
  public pump1s: Array<Pump1> = reactive(
    [
      { // ---pump1---------------------------------------------
        id: 'pump1',
        name: 'Pump 1',
        type: 'ESP32',
        iPump1State: -99,
        sSensor: '--',
        sStatus: 'Status?',
        lastPump1Date: 'Date?',
        lastPump1Time: 'Time?',
        subTopic: 'z2m/t/pump/-1 z2m/t/pump/-1/time',
        pubTopic: ''
      }
    ]
  );

  // --------------state constants------------------------------
  //  public stateOk = -1;
  public stateOn = -1;
  public stateOff = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.pump1s.forEach(pump1 => {
      const aSubTopic = pump1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------pump1 topic found ---------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            pump1.lastPump1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            pump1.lastPump1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
            // pump1.iPump1State = this.stateOk
          } else {
            pump1.lastPump1Date = geo0f.noDate
            pump1.lastPump1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different pump types-----------------
          pump1.iPump1State = this.stateNotOk
          if (pump1.type === 'D1' || pump1.type === 'ESP32') {
            try {
              const aPayload = JSON.parse(message.payload)
              if (aPayload.pump === 0 || aPayload.pump === '0') pump1.iPump1State = this.stateOff
              if (aPayload.pump === 1 || aPayload.pump === '1') pump1.iPump1State = this.stateOn
              pump1.sSensor = `${aPayload.sensor}`
              pump1.sStatus = `${aPayload.status}`
            } catch (error) {
              pump1.iPump1State = this.stateNotOk
              pump1.lastPump1Date = 'ERROR'
              pump1.lastPump1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (pump1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different repeater types----------
      } // --------END pump1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiRepeaterController: ERROR:', e) })
  }
}

export const ciPump1Controller = new CiPump1Controller()
```

[Zum Seitenanfang](#up)
