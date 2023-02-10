Letzte &Auml;nderung: 10.2.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiRepeater1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiRepeater1`-Symbols.   
Allgemein gilt:   
* Das `CiRepeater1`-Symbol wartet auf MQTT-Nachrichten von Repeatern und zeigt diese an.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiRepeater1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vue190_ci_mqtt_repeater1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Das CiRepeater1-Symbol wartet auf MQTT-Nachrichten der folgenden Art, die zB von Zigbee-Sensoren gesendet werden oder - zur Simulation - mit dem Programm `mosquitto_pub` in einer Windows-Konsole eingegeben werden k&ouml;nnen:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/repeater/-1 -m "{\"last_seen\":\"2023-02-10T17:03:00+02:00\",\"linkquality\":72,\"update\":{\"state\":\"idle\"},\"update_available\":false}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/repeater/-1/time -m "20230209 123344"`   
Die Werte werden entsprechend angezeigt.   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Ci-Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiRepeater1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![CiRepeater1-Varianten](./images/vue190_all_repeater1.png "CiRepeater1-Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiRepeater1-Darstellungen_   

## CiRepeater1-Aufruf
Das Zeichnen eines CiRepeater1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiRepeater1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiRepeater1 :x="50" :y="80" sid="repeater1" :border="3" :fx="2" :fy="1.5" colorH="white"></CiRepeater1>
```   

## Positionierung eines Symbols
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiRepeater1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiRepeater1-Symbols](./images/vue190_repeater1_location1.png "Ort eines CiRepeater1-Symbols")   
_Bild 2: Ort eines CiRepeater1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## Rand eines Symbols
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

## Textangaben im CiRepeater1-Symbol
Jedes `CiRepeater1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Zeichenbereich (= Textbereich)   

### Kopfzeile   
Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`.   

### Fu&szlig;zeile
1. Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt.   
2. Enth&auml;lt die empfangene MQTT-Nachricht eine Angabe zur Verbindungsqualität ("linkquality") und wird der Wert in der Methode `onMessage()` (Datei `CiRepeater12Controller.ts`) dem Attribut zugewiesen, so wird dieser Wert angezeigt.   
3. Ist keiner der Punkte 1 und 2 erf&uuml;llt, wird nichts angezeigt (Leerzeile).   

### Zeichenbereich
Der Zeichenbereich enth&auml;lt drei Zeilen (Zeile 2 bis 4), die folgendes anzeigen:   
#### Zeile 2   
Links wird der Text "`Rep`" (für Repeater) und rechts der Zustand angezeigt (`ok`, `OK` oder `?`)   
#### Zeile 3   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public repeater1s: Array<Repeater1>` (Datei `CiRepeater12Controller.ts`), so wird "`unknown`" angezeigt.   
2. Gibt es einen Datumswert, so wird dieser angezeigt (Format `dd.mm.yy`).   
3. Ansonsten wird `Date?` angezeigt.   
#### Zeile 4   
1. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public repeater1s: Array<Repeater1>` (Datei `CiRepeater12Controller.ts`), so wird "`sid`" angezeigt.   
2. Gibt es einen Uhrzeit-Wert, wird dieser angezeigt (Format `HH:MM:SS`).   
3. Ansonsten wird `Time?` angezeigt.   

## Strecken eines Symbols
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Bild zeigt:   
![Gestrecktes CiRepeater1-Symbol](./images/vue190_repeater1_stretch1.png "Gestrecktes CiRepeater1-Symbol")   
_Bild 3: Gestrecktes CiRepeater1-Symbol_   

Die linken beiden CiRepeater1-Symbole sind um den Faktor fx=2 gestreckt (ergibt 2*100 = 200 Einheiten), das dritte Symbol ist um den Faktor fy=2 gesteckt und das rechte Symbol ist um fx=2 und fy=2 gestreckt, d.h., es ist mit einer Gr&ouml;&szlig;e von 200x200 doppelt so gro&szlig; wie das Original.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiRepeater1 :x="50" :y="70"  sid="repeater1" :fx="2" :fy="1"></CiRepeater1>
  <CiRepeater1 :x="50" :y="170" sid="repeaterX" :fx="2" :fy="1"></CiRepeater1>
  <CiRepeater1 :x="250" :y="70" sid="repeater1" :fx="1" :fy="2"></CiRepeater1>
  <CiRepeater1 :x="350" :y="70" sid="repeater1" :fx="2" :fy="2"></CiRepeater1>
  </svg>
</template>
```   

## Farbe des CiRepeater1-Symbols   
### Farbe des Zeichenbereichs   
Die Hintergrundfarbe des Zeichenbereichs wird automatisch gesetzt:   
* Tritt ein Fehler auf, wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   
* Ist der Repeater-Zustand ok, wird der Hintergrund hellgrün (`colorRepeater`) gesetzt.   
* In allen anderen Fällen wird der Hintergrund hellgrau (`colorNoRepeater`) gesetzt.   

### Farbe der Kopfzeile   
Mit Hilfe des Attributs `colorH=` kann die Hintergrundfarbe der Kopfzeile eines CiRepeater1-Symbols gesetzt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#eedd00` oder eine Textangabe wie `red`, `white` etc. sein.   
Wird keine Farbe gewählt, ist die Kopfzeile transparent.   

### Farbe der Fu&szlig;zeile   
* Bei einem Fehler wird die Hintergrundfarbe auf hellrot gesetzt. (Farbe "`colorNotOK`" aus der Datei `Geo.ts`)   
* Ist der Repeater-Zustand ok, wird der Hintergrund hellgrün (`colorRepeater`) gesetzt.   
* Falls mit `colorF=` ein Farbwert angegeben wurde, wird diese Farbe verwendet.   
* In allen anderen Fällen wird der Hintergrund transparent gesetzt (`none`).   

# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiRepeater1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiRepeater1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiRepeater1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiRepeater1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiRepeater1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft         | Bedeutung                            |   
| ------------------- | ------------------------------------ |   
| `iRepeater1State`    | Status des CiRepeater1-Symbols. Dafür sind in der Datei `CiRepeater1Controller.ts` die Konstanten `stateOk`, `stateLastSeen`, `stateNotOk` und `stateNoState` definiert.   |   
| `type`              | Typ des CiRepeater1 Symbols. Sagt etwas dar&uuml;ber aus, welche Daten vom Repeater erwartet werden.   |   
| `lastRepeater1Date` | Datum, wann die letzte Repeater-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Date?` oder `--.--.--` gesetzt werden.   |   
| `lastRepeater1Time` | Uhrzeit, wann die letzte Repeater-Nachricht eingetroffen ist.<br>Kann beim Ausf&uuml;llen f&uuml;r ein bestimmtes Symbol zB auf `Time?` oder `--:--` gesetzt werden.   |   

Folgende Attribute sind optional und k&ouml;nnen bei Bedarf verwendet werden:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `linkquality`  | Angabe zur Verbindungsqualität (bei IKEA zB ein Wert von 0 bis 255). Wird in Zeile 5 angezeigt.   |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiRepeater1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<CiRepeater1>` erg&auml;nzt werden.   
Im unten stehenden Beispiel ist ein CiRepeater1-Symbol definiert.    
In der Methode `public onMessage (message: Message): void` werden die Strings f&uuml;r "`lastRepeater1Date`" und "`lastRepeater1Time`" dazu genutzt, im Fehlerfall den Text "`ERROR json`" anzuzeigen.   

M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

```
// ______CiRepeater1Controller.ts________________khartinger_____
// 2022-10-19: new
// 2023-02-10: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Repeater1 extends IBase {
  // ---------mandatory-----------------------------------------
  iRepeater1State: number;
  type: string;
  lastRepeater1Date: string;
  lastRepeater1Time: string;
  // ---------optional------------------------------------------
  linkquality?: string;
  textFooter?: string;
}

export class CiRepeater1Controller extends CiBaseController {
  public repeater1s: Array<Repeater1> = reactive(
    [
      { // ---repeater1: IKEA-------------------------------------
        id: 'repeater1',
        name: 'Repeat1 IKEA',
        type: 'TRADFRI', // IKEA
        iRepeater1State: -99,
        lastRepeater1Date: 'Date?',
        lastRepeater1Time: 'Time?',
        subTopic: 'z2m/t/repeater/-1 z2m/t/repeater/-1/time',
        pubTopic: ''
      }
    ]
  );

  // --------------state constants------------------------------
  public stateOk = -1;
  public stateLastSeen = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.repeater1s.forEach(repeater1 => {
      const aSubTopic = repeater1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------repeater1 topic found ------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            repeater1.lastRepeater1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            repeater1.lastRepeater1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
            repeater1.iRepeater1State = this.stateOk
          } else {
            repeater1.lastRepeater1Date = geo0f.noDate
            repeater1.lastRepeater1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different repeater types-------------
          repeater1.iRepeater1State = this.stateNotOk
          repeater1.lastRepeater1Date = geo0f.noDate
          repeater1.lastRepeater1Time = geo0f.noTime
          if (repeater1.type === 'TRADFRI') {
            try {
              const aPayload = JSON.parse(message.payload)
              // {"last_seen":"2022-10-19T17:03:00+02:00","linkquality":72,"update":{"state":"idle"},"update_available":false}
              if (aPayload.last_seen.length > 24) {
                const dt = aPayload.last_seen // "last_seen":"2022-10-19T17:03:00+02:00"
                const date1 = dt.substring(8, 10) + '.' + dt.substring(5, 7) + '.' + dt.substring(2, 4)
                repeater1.lastRepeater1Date = date1
                repeater1.lastRepeater1Time = dt.substring(11, 19)
                const lqiPercent = Math.round(aPayload.linkquality * 100 / 255)
                repeater1.linkquality = ' link ' + `${lqiPercent}` + '% = ' + `${aPayload.linkquality}` + ' lqi'
                repeater1.iRepeater1State = this.stateLastSeen
                // console.log('Repeater:', aPayload.last_seen + ' = ' + date1 + ' ' + dt.substring(11, 19) + ' state=' + repeater1.iRepeater1State)
              }
            } catch (error) {
              repeater1.lastRepeater1Date = 'ERROR'
              repeater1.lastRepeater1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (repeater1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different repeater types----------
      } // --------END repeater1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiRepeaterController: ERROR:', e) })
  }
}

export const ciRepeater1Controller = new CiRepeater1Controller()
```

[Zum Seitenanfang](#up)
