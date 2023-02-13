Letzte &Auml;nderung: 9.2.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiMotion1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiMotion1`-Symbols.   
Allgemein gilt:   
* Das `CiMotion1`-Symbol wartet auf MQTT-Nachrichten und zeigt den Bewegungszustand an.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiMotion1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex180_ci_mqtt_motion1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Das CiMotion1-Symbol wartet auf MQTT-Nachrichten der folgenden Art, die zB von Zigbee-Sensoren gesendet werden oder - zur Simulation - mit dem Programm `mosquitto_pub` in einer Windows-Konsole eingegeben werden k&ouml;nnen:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/motion/-1 -m "{\"battery\":47,\"occupancy\":false}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/motion/-1/time -m "20230209 123344"`   
Die Werte werden entsprechend angezeigt.   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Ci-Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiMotion1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![CiMotion1-Varianten](./images/vuex180_all_motion1.png "CiMotion1-Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiMotion1-Darstellungen_   

## CiMotion1-Aufruf
Das Zeichnen eines CiMotion1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiMotion1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiMotion1 :x="50" :y="80" sid="motion1" :border="3" :fx="2" :fy="1.5" colorH="white"></CiMotion1>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiMotion1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiMotion1-Symbols](./images/vuex180_motion1_location1.png "Ort eines CiMotion1-Symbols")   
_Bild 2: Ort eines CiMotion1-Symbols_   

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

## Textangaben im CiMotion1-Symbol
Jedes `CiMotion1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Zeichenbereich (= Textbereich)   

### Kopfzeile   
Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`.   

### Fu&szlig;zeile
1. Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt.   
2. Enth&auml;lt die empfangene MQTT-Nachricht eine Angabe zum Batterieladezustand ("battery") und wird der Wert in der Methode `onMessage()` (Datei `CiMotion12Controller.ts`) dem Attribut zugewiesen, so wird dieser Wert angezeigt.   
3. Ist keiner der Punkte 1 und 2 erf&uuml;llt, wird die `id` angezeigt.   

### Zeichenbereich
Der Zeichenbereich enth&auml;lt drei Zeilen (Zeile 2 bis 4), die folgendes anzeigen:   
#### Zeile 2   
Links wird das Symbol für einen Bewegungsmelder `))` und rechts der Bewegungszustand angezeigt (`on`, `off` oder `?`)   
#### Zeile 3   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public motion1s: Array<Motion1>` (Datei `CiMotion12Controller.ts`), so wird "`unknown`" angezeigt.   
2. Gibt es einen Datumswert, so wird dieser angezeigt (Format `dd.mm.yy`).   
3. Ansonsten wird `Date?` angezeigt.   
#### Zeile 4   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public motion1s: Array<Motion1>` (Datei `CiMotion12Controller.ts`), so wird "`sid`" angezeigt.   
2. Gibt es einen Uhrzeit-Wert, wird dieser angezeigt (Format `HH:MM:SS`).   
3. Ansonsten wird `Time?` angezeigt.   

## Strecken eines Symbols (fx, fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Bild zeigt:   
![Gestrecktes CiMotion1-Symbol](./images/vuex180_motion1_stretch1.png "Gestrecktes CiMotion1-Symbol")   
_Bild 3: Gestrecktes CiMotion1-Symbol_   

Die linken beiden CiMotion1-Symbole sind um den Faktor fx=2 gestreckt (ergibt 2*100 = 200 Einheiten), das dritte Symbol ist um den Faktor fy=2 gesteckt und das rechte Symbol ist um fx=2 und fy=2 gestreckt, d.h., es ist mit einer Gr&ouml;&szlig;e von 200x200 doppelt so gro&szlig; wie das Original.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiMotion1 :x="50" :y="70"  sid="motion1" :fx="2" :fy="1"></CiMotion1>
  <CiMotion1 :x="50" :y="170" sid="motion3" :fx="2" :fy="1"></CiMotion1>
  <CiMotion1 :x="250" :y="70" sid="motion1" :fx="1" :fy="2"></CiMotion1>
  <CiMotion1 :x="350" :y="70" sid="motion1" :fx="2" :fy="2"></CiMotion1>
  </svg>
</template>
```   

## Farbe des CiMotion1-Symbols (colorH, colorF)
### Farbe des Zeichenbereichs   
Der Zeichenbereich wird abhängig vom Bewegungszustand eingefärbt:   
| Zustand | Farbbezeichnung     | Farbe        |   
| ------- | ------------------- | ------------ |   
| `on`    | `colorMotion`       | gelb         |   
| `off`   | `colorNoMotion`     | hellgrau     |   
| `?`     | `geof.colorUnknown` | mittelblau   |   

Tritt ein Fehler auf, wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   

### Farbe der Kopfzeile   
Mit Hilfe des Attributs `colorH=` kann die Hintergrundfarbe der Kopfzeile eines CiMotion1-Symbols gesetzt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#eedd00` oder eine Textangabe wie `red`, `white` etc. sein.   
Wird keine Farbe gewählt, ist die Kopfzeile transparent.   

### Farbe der Fu&szlig;zeile   
* Bei einem Fehler wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   
* Ist ein "`battery`"-Wert vorhanden, so wird dieser Wert mit dem `batteryMin`-Wert (=15) verglichen. Ist der "`battery`"-Wert gr&ouml;&szlig;er, wird die Fu&szlig;zeile gr&uuml;n eingef&auml;rbt (`colorOk`) ansonsten hellrot (`colorNotOk`). Tritt ein Fehler auf, wird die Farbe Mittelblau gew&auml;hlt.(`colorUnknown`).   
* Falls mit `colorF=` ein Farbwert angegeben wurde, wird diese Farbe verwendet.   
* Ansonsten ist der Hintergrund transparent.   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiMotion1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiMotion1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiMotion1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiMotion1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiMotion1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft         | Bedeutung                            |   
| ------------------- | ------------------------------------ |   
| `iMotion1State`    | Status des CiMotion1-Symbols. Dafür sind in der Datei `CiMotion1Controller.ts` die Konstanten `stateOn`, `stateOff`, `stateNotOk` und `stateNoState` definiert.   |   
| `type`              | Typ des CiMotion1 Symbols. Sagt etwas dar&uuml;ber aus, welche Daten vom Bewegungsmelder erwartet werden.   |   
| `lastMotion1Date` | Datum, wann die letzte Wetter-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Date?` oder `--.--.--` gesetzt werden.   |   
| `lastMotion1Time` | Uhrzeit, wann die letzte Wetter-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Time?` oder `--:--` gesetzt werden.   |   

Folgende Attribute sind optional und k&ouml;nnen bei Bedarf verwendet werden:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `battery`      | String mit dem Ladezustand einer Batterie. Wird in Zeile 5 angezeigt.   |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiMotion1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<CiMotion1>` erg&auml;nzt werden.   
Im unten stehenden Beispiel sind drei CiMotion1-Symbole definiert, wobei das dritte Symbol einen eventuellen `battery`-Wert nicht auswertet. Das Symbol dient nur dazu, die Farbe der Fu&szlig;zeile zu zeigen.   
In der Methode `public onMessage (message: Message): void` werden die Strings f&uuml;r "`lastMotion1Date`" und "`lastMotion1Time`" dazu genutzt, im Fehlerfall den Text "`ERROR json`" anzuzeigen.   

M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

```
// ______CiMotion1Controller.ts_________________khartinger_____
// 2022-01-01: new
// 2023-02-09: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Motion1 extends IBase {
  // ---------mandatory-----------------------------------------
  iMotion1State: number;
  type: string;
  lastMotion1Date: string;
  lastMotion1Time: string;
  // ---------optional------------------------------------------
  battery?: string;
  textFooter?: string;
}

export class CiMotion1Controller extends CiBaseController {
  public motion1s: Array<Motion1> = reactive(
    [
      { // ---motion1: IKEA-------------------------------------
        id: 'motion1',
        name: 'Motion1_1',
        type: 'E1745', // IKEA (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-1 z2m/t/motion/-1/time',
        pubTopic: ''
      },
      { // ---motion2: Aqara------------------------------------
        id: 'motion2',
        name: 'Motion1_2',
        type: 'RTCGQ11LM', // Aquara (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-2 z2m/t/motion/-2/time',
        pubTopic: ''
      },
      { // ---motion3: just for test...-------------------------
        id: 'motion3',
        name: 'No_batt',
        type: 'noBattery',
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        textFooter: 'show colorF',
        subTopic: 'z2m/t/motion/-3 z2m/t/motion/-3/time',
        pubTopic: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOn = -1;
  public stateOff = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.motion1s.forEach(motion1 => {
      const aSubTopic = motion1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------motion1 topic found ------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            motion1.lastMotion1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            motion1.lastMotion1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            motion1.lastMotion1Date = geo0f.noDate
            motion1.lastMotion1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different motion types---------------
          motion1.iMotion1State = this.stateNotOk
          if (motion1.type === 'RTCGQ11LM' || motion1.type === 'E1745') {
            try {
              const aPayload = JSON.parse(message.payload)
              motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'noBattery') {
            try {
              const aPayload = JSON.parse(message.payload)
              // motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different motion types----------
      } // --------END motion1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiMotionController: ERROR:', e) })
  }
}

export const ciMotion1Controller = new CiMotion1Controller()
```

[Zum Seitenanfang](#up)
