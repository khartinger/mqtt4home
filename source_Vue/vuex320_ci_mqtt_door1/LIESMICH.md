Letzte &Auml;nderung: 18.2.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiDoor1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiDoor1`-Symbols.   
Allgemein gilt:   
* Das `CiDoor1`-Symbol wartet auf MQTT-Nachrichten, um den Zustand der T&uuml;re darzustellen (offen, geschlossen, versperrt).   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiDoor1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex320_ci_mqtt_door1` herunterladen.   
* Visual Studio Code (VSC) mit installierter TypeScript-Unterst&uuml;tzung starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Das CiDoor1-Symbol wartet auf MQTT-Nachrichten, die Informationen zum T&uuml;rzustand und eventuell den Batteriezustand enthalten. Zur Simulation k&ouml;nnen diese Werte auch mit dem Programm `mosquitto_pub` in einer Windows-Konsole eingegeben werden:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/door/-1 -m "{\"contact\":false,\"battery\":90}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/door/-1 -m "{\"contact\":true,\"battery\":10}"`   
  * `mosquitto_pub -h 10.1.1.1 -t wifi/t/door/-5/ret/status -m 1`   

Die Werte werden entsprechend angezeigt.   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiDoor1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![Door1 Varianten](./images/vuex320_all_door1.png "Door1 Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene Door1-Darstellungen_   

## CiDoor1-Aufruf
Das Zeichnen eines CiDoor1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiDoor1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiDoor1 :x="50" :y="70" sid="door_o" dir="h1"></CiDoor1>
```   

## Positionierung eines Symbols (:x, :y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiDoor1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiDoor1-Symbols](./images/vuex320_door1_location1.png "Ort eines CiDoor1-Symbols")   
_Bild 2: Ort eines CiDoor1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")   
Wird `sid=""` gesetzt, werden keine Texte und Hintergründe angezeigt.   

## Rand eines Symbols (:border)
Normalerweise sind CI-Symbole au&szlig;en 100x100 Einheiten gro&szlig; und haben nach innen einen fixen Rand von 5 Einheiten. Beim CiWall1-Symbol ist dies anders:   
Damit nebeneinander liegende Symbole in einem 100er Raster eine kontinuerliche Darstellung bewirken, gehen die `CiDoor1`-Symbole bis an die Symbolgrenze, sind also ausnahmsweise 100x100 Einheiten gro&szlig;.   
Im Normalfall wird der Rand der `CiDoor1`-Symbole nicht dargestellt. Will man den Rand ausnahmsweise darstellen, so gibt es dazu vier M&ouml;glichkeiten, die &uuml;ber das Attribut `:border=` eingestellt werden:   

| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `:border="0"` | Es wird kein Rand dargestellt. |   
| `:border="1"` | Es wird nur der &auml;u&szlig;ere Rand des Symbols dargestellt (= gesamter Platzbedarf des Symbols). |   
| `:border="2"` | Es wird nur der innere Rand des Symbols dargestellt (= der Zeichenbereich f&uuml;r das Symbol). |   
| `:border="3"` | Es wird der Rand des Symbols gelb angezeigt. |   

Vorgabe ist `:border="0"`   

## Richtung des CiDoor1-Symbols (dir)
Die Richtung eines Symboles wird mit der Eigenschaft `dir` ("Direction") festgelegt.   

Der __erste Buchstabe__ gibt die Basis-Richtung an:   
* `h` oder `H`: Horizontal (waagrecht, x-Richtung)   
* `v` oder `V`: Vertikal   (senkrecht, y-Richtung)   
* `d` oder `D`: Diagonal (unter +/- 45° geneigt)   

Bei Verwendung eines Kleinbuchstabens wird die T&uuml;r&ouml;ffnungsrichtung klein gezeichnet, bei einem Gro&szlig;buchstaben gro&szlig; (siehe _Bild 1_)   

Der __zweite Buchstabe__ gibt die &Ouml;ffnungsrichtung an, das ist der Quadrant, in den sich die T&uuml;re hinein &ouml;ffnet. Entsprechend den 4 Quadranten sind die Ziffern 1 bis 4 m&ouml;glich.   

![door_dir_axes](./images/vuex320_door1_dir_axes1.png "door_dir_axes")   
_Bild 3: Richtungsbezeichnungen bei Symbolen in Achsenrichtung_   

Bei diagonalen Elementen ist ein __dritter Buchstabe__ erforderlich, um die "Ausbreitungs"-Richtung genauer anzugeben. Der erste Buchstabe ist ein `d` (f&uuml;r Diagonale), der zweite Buchstabe gibt die Diagonalenrichtung f&uuml;r das Symbol an (`1` bis `4`) und der dritte Buchstabe bezeichnet die Achse, zu der sich ein Element hin &ouml;ffnet (`x` oder `y`).   

![door_dir_dia](./images/vuex320_door1_dir_dia1.png  "door_dir_dia")   
_Bild 4: Richtungsbezeichnungen bei Symbolen in Diagonalrichtung_   

__Beispiele f&uuml;r Bezeichnungen:__   
`dir="h1" ....` Waagrechtes Symbol, dessen Ausbreitung (T&uuml;r&ouml;ffnung) in den ersten Quadranten erfolgt (nach "oben").   
`dir="v1" ....` Senkrechtes Symbol, dessen Ausbreitung (T&uuml;r&ouml;ffnung) in den ersten Quadranten erfolgt (nach "rechts").   
`dir="d1x" ...` Diagonales Symbol nach rechts-oben, dessen Ausbreitung (T&uuml;r&ouml;ffnung) in Richtung der x-Richtung erfolgt (nach "unten").   

## Textangaben im CiDoor1-Symbol (lines)
Jedes `CiDoor1`-Symbol kennt drei m&ouml;gliche Zeilen zur Textausgabe:   
* Kopfzeile   
* Zeile 4   
* Fu&szlig;zeile   

Welche Texte im Symbol wirklich angezeigt werden, h&auml;ngt vom Wert des Attributs `lines=` ab:    
| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Weder Kopfzeile noch Fu&szlig;zeile werden angezeigt. |   
| `lines="1"`   | Die Kopfzeile wird angezeigt. Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`. |   
| `lines="2"`   | Kopf- und Fu&szlig;zeile werden angezeigt.   |   

Vorgabe ist `lines="2"`   

### Kopfzeile
1. Wird im Symbol das Attribut `lines="0"` gesetzt, wird die Kopfzeile nicht angezeigt.   
2. Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt.   
3. Enth&auml;lt der Parameter `name` ein Fragezeichen, so wird dieses je nach T&uuml;rstatus durch den Text `OPEN`, `CLOSE`  oder `LOCK` ersetzt.   
4. Ansonsten wird die `id` angezeigt.   

### Zeile 4
1. Wird im Symbol das Attribut `lines="0"` gesetzt, wird die Zeile 4 nicht angezeigt.   
2. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public door1s: Array<Door1>` (Datei `CiDoor1Controller.ts`), so wird "`unknown`" angezeigt.   
3. Gibt es einen Wert f&uuml;r den MQTT-Key `battery`, so wird dieser Wert angezeigt.   
4. Ansonsten wird eine leere Zeile ausgegeben.   

### Fu&szlig;zeile
1. Wird im Symbol das Attribut `lines="0"` oder `lines="1"` gesetzt, wird die Fu&szlig;zeile nicht angezeigt.   
2. Passt die Id des Symbols (`sid`) zu keinem Eintrag in der Liste `public door1s: Array<Door1>` (Datei `CiDoor1Controller.ts`), so wird "`sid`" angezeigt.   
3. Wird im Symbol das Attribut `textF=` verwendet, so wird dieser Text angezeigt.   
4. Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt.   
5. Enth&auml;lt der Parameter `textFooter` ein Fragezeichen, so wird dieses je nach T&uuml;rstatus durch den Text `OPEN`, `CLOSE`  oder `LOCK` ersetzt.   
6. Ansonsten wird eine leere Zeile ausgegeben.   

![Verschiedene Fu&szlig;zeilen](./images/vuex320_door1_footer1.png "Verschiedene Fu&szlig;zeilen")   
_Bild 5: Verschiedene Fu&szlig;zeilen_   

__Codierungsbeispiel__:   
```   
    <CiDoor1 :x="50"  :y="70" sid="doorX" dir="h1"></CiDoor1>
    <CiDoor1 :x="150" :y="70" sid="door_o" dir="h1"></CiDoor1>
    <CiDoor1 :x="350" :y="70" sid="door_o" dir="h1" lines="0"></CiDoor1>
    <CiDoor1 :x="50"  :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="150" :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="250" :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="350" :y="170" sid="doorF" dir="h1" textF="textF :)"></CiDoor1>
```   

## Strecken eines Symbols (:f)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, das Symbol (beidseitig) zu strecken, sodass der Platzierungspunkt gleich bleibt.   
Das Strecken erfolgt durch Angabe des Parameters `f`, wie das folgende Beispiel zeigt:   
![Gestrecktes CiDoor1-Symbol](./images/vuex320_door1_stretch1.png "Gestrecktes CiDoor1-Symbol")   
_Bild 6: Gestrecktes CiDoor1-Symbol_   

Das Symbol links unten und ganz rechts ist jeweils um den Faktor 2 gestreckt.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiDoor1 :x="100" :y="70"  sid="door_o" dir="h1"></CiDoor1>
  <CiDoor1 :x="100" :y="170" sid="door_o" dir="h1" :f="2"></CiDoor1>
  <CiDoor1 :x="300" :y="110" sid="door_o" dir="v2"></CiDoor1>
  <CiDoor1 :x="400" :y="110" sid="door_o" dir="v2" :f="2"></CiDoor1>
  </svg>
</template>
```   

## Verschiedene CiDoor1-Formen (shape)
CiDoor1-Symbole gibt es in vier Formen, die durch die Attribute `dir=` und `shape=` ausgew&auml;hlt werden:   
### `dir`
* ein Kleinbuchstabe ergibt einen kleinen Bogen oder ein kleines Dreieck, das an der Grenze der Zeichenfl&auml;che endet.   
* ein Gro&szlig;buchstabe ergibt einen Bogen oder ein Dreieck, das einem Viertelkreis entspricht.   
### `shape`
* Setzt man `shape= "tri"`, so wird statt des Bogens ein Dreieck gezeichnet.   
Vorgabe ist `shape="arc"`.   

## Farben im CiDoor1-Symbol (colorH, colorF, colorWall)
Die Farben im CiDoor1-Symbol k&ouml;nnen teilweise gew&auml;hlt werden oder werden automatisch vergeben.   

### Automatisch gesetzte Farben
* Fehlerhafte "`sid`", d.h. ein Objekt mit dieser "`sid`" existiert nicht im Controller:   
Der T&uuml;rbereich wird hellblau (Farbe "`colorUnknown`" aus der Datei `Geo.ts`) und die Zeilen 4 und 5 werden hellrot eingef&auml;rbt (Farbe "`colorNotOK`" aus der Datei `Geo.ts`).   
* Der T&uuml;rbereich wird je nach T&uuml;r-Status eingef&auml;rbt (Variable `iDoor1State`):   
  * T&uuml;re offen: hellgr&uuml;n (`colorOpen`)   
  * T&uuml;re geschlossen: hellrot (`colorClose`)   
  * T&uuml;re versperrt: rot (`colorLock`)   
* Ist ein Batterie-Wert kleiner als `batteryMin`, so wird die Zeile 4 hellrot eingef&auml;rbt (Farbe "`colorNotOK`" aus der Datei `Geo.ts`).   

### W&auml;hlbare Farben
* Mit dem Attribut `colorH=` kann die Hintergrundfarbe der Kopfzeile gesetzt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
* Mit dem Attribut `colorF=` kann die Hintergrundfarbe der Fu&szlig;zeile gesetzt werden (Farbwert siehe oben).   
* Mit dem Attribut `colorWall=` kann die Farbe der Wandelemente (Rechtecke links und rechts vom T&uuml;rbereich) gesetzt werden  (Farbwert siehe oben).   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiDoor1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiDoor1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiDoor1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiDoor1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiDoor1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `iDoor1State`  | Status des CiDoor1-Symbols. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   
| `type`         | Sensortyp, der den T&uuml;rstatus feststellt und eine entsprechende MQTT-Nachricht sendet. |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `battery`      | Empfang des Ladezustands, falls ein CiDoor1-Sensor mit Batterie betrieben wird. Kann in der Fu&szlig;zeile angezeigt werden. |   
| `textHeader`   | Text f&uuml;r die Kopfzeile.     |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung eines `CiDoor1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<Door1>` erg&auml;nzt werden.   
M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

Im untenstehenden Beispiel sind sechs T&uuml;robjekte definiert:   
Die ersten drei Objekte `door_o`, `door_c` und `door_l` dienen dazu, offene, geschlossene und versperrte T&uuml;ren darzustellen.   
Die Objekte `door_bat10` und `door_bat20` dienen dazu, einen Batterie-Wert darzustellen und `doorF` zeigt die Anzeige eines Fu&szlig;zeilenwertes.   

```
// ______CiDoor1Controller.ts____________________khartinger_____
// 2022-01-08: new
// 2023-02-18: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Door1 extends IBase {
  // ---------mandatory-----------------------------------------
  iDoor1State: number;
  type: string;
  // ---------optional------------------------------------------
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiDoor1Controller extends CiBaseController {
  public door1s: Array<Door1> = reactive(
    [
      {
        id: 'door_o',
        type: 'zb',
        iDoor1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/door/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_c',
        type: 'zb',
        iDoor1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/door/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_l',
        type: 'zb',
        iDoor1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/door/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_bat10',
        type: 'D1',
        iDoor1State: -1,
        battery: '10',
        subTopic: 'z2m/t/door/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_bat20',
        type: 'D1',
        iDoor1State: -1,
        battery: '20',
        subTopic: 'wifi/t/door/-5/ret/status wifi/t/door/-5/ret/voltage',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'doorF',
        name: 'Door1_2D1',
        type: 'D1',
        iDoor1State: -99,
        textFooter: '--Footer1--',
        subTopic: 'wifi/t/door/-6/ret/status wifi/t/door/-6/ret/voltage',
        pubTopic: '',
        pubPayload: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOpen = -1;
  public stateClose = -2;
  public stateLock = -3;
  public stateNotOk = -998;
  public stateNoState = -999;

  public onMessage (message: Message): void {
    this.door1s.forEach(door1 => {
      const aSubTopic = door1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---door1 topic found --------------------------------
        if (door1.type === 'zb') {
        // ---door type 1---------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            door1.battery = `${aPayload.battery}`
            const doorstate_ = String(aPayload.contact)
            if (doorstate_ === 'true') door1.iDoor1State = this.stateClose
            else {
              if (doorstate_ === 'false') door1.iDoor1State = this.stateOpen
              else door1.iDoor1State = this.stateNotOk
            }
          } catch (error) { }
        }
        if (door1.type === 'D1') {
        // ---door type 2---------------------------------------
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'on')) door1.iDoor1State = this.stateOpen
            else {
              if ((message.payload === '0') || (message.payload === 'off')) door1.iDoor1State = this.stateClose
              else {
                if (message.payload === '-1') door1.iDoor1State = this.stateLock
                else door1.iDoor1State = this.stateNotOk
              }
            }
          }
          if (message.topic.includes('voltage')) {
            door1.battery = `${message.payload}`
          }
        }
        // ---door type 3---------------------------------------
        if (door1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different door1 types------------
        // ---END door1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiDoor1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiDoor1Controller: ERROR:', e) })
  }
}

export const ciDoor1Controller = new CiDoor1Controller()
```

[Zum Seitenanfang](#up)
