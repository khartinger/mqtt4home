Letzte &Auml;nderung: 29.1.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiSocket1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiSocket1`-Symbols.   
Allgemein gilt:   
* Beim Klicken auf das `CiSocket1`-Symbol wird eine MQTT-Nachricht ausgesendet.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiSocket1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex130_ci_mqtt_socket1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Dr&uuml;ckt man im Browser auf das Symbol, so wird eine MQTT-Nachricht gesendet, die in der Konsole zu sehen sind, zB.   
`ci/socket/1/set {"state": "TOGGLE"}`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines CI-Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiSocket1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![socket1-Symbolvarianten](./images/vuex130_all_socket1.png "socket1-Symbolvarianten")   
_Bild 1: Beispiele f&uuml;r verschiedene socket1-Symbol-Darstellungen_   

## Aufruf des socket1-Symbols
Das Zeichnen eines socket1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiSocket1>` und weiteren Attributen.   
_Beispiel_:   
```   
<CiSocket1 :x="50" :y="80" sid="socket1" :border="3" lines="0" :fx="2" :fy="1.5"></CiSocket1>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 300 150">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das socket1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines socket1-Symbols](./images/vuex130_socket_location1.png "Ort eines socket1-Symbols")   
_Bild 2: Ort eines socket1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 150 - 20 - 100 = 30 Einheiten   
Restlicher Platz rechts: 300 - 100 = 200 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")

## Rand eines CI-Symbols (border)
Alle CI-Symbole sind im Normalfall au&szlig;en 100x100 Einheiten gro&szlig; und haben nach innen einen fixen Rand von 5 Einheiten. Die individuelle "Arbeitsfl&auml;che" eines Symbols ist also 90 x 90 Einheiten gro&szlig;.   
Durch den Rand ist es m&ouml;glich, Symbole direkt nebeneinander in einem 100er-Raster anzuordnen, ohne dass sich die Symbol-Zeichnungen optisch ber&uuml;hren.   
Will man den Rand eines CI-Symbols darstellen, so gibt es dazu vier M&ouml;glichkeiten, die &uuml;ber das Attribut `:border=` eingestellt werden:   

| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `:border="0"` | Es wird kein Rand dargestellt. |   
| `:border="1"` | Es wird nur der &auml;u&szlig;ere Rand des Symbols dargestellt (= gesamter Platzbedarf des Symbols). |   
| `:border="2"` | Es wird nur der innere Rand des Symbols dargestellt (= der Zeichenbereich f&uuml;r das Symbol). |   
| `:border="3"` | Es wird der Rand des Symbols gelb angezeigt. |   

Vorgabe ist `:border="1"`   

## Textangaben im CiSocket1-Symbol (lines)
Jedes `CiSocket1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Mittelzeile   

Welche Texte im Symbol wirklich angezeigt werden, h&auml;ngt vom Wert des Attributs `lines=` ab:    
| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Weder Kopfzeile noch Fu&szlig;zeile werden angezeigt. |   
| `lines="1"`   | Die Kopfzeile wird angezeigt. Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`. |   
| `lines="2"`   | Kopf- und Fu&szlig;zeile werden angezeigt.<br>Kopfzeile: Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`.<br>Fu&szlig;zeile: Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt, ansonsten der Wert von `battery` oder, falls `battery` nicht definiert ist, die `id`. |   

Vorgabe ist `lines="1"`   

Je nach Anzahl angezeigter Zeilen ver&auml;ndert sich die Gr&ouml;&szlig;e des Zeichenbereichs f&uuml;r das CI-Symbol. F&uuml;r das Standard-Symbol mit der Gesamtgr&ouml;&szlig;e 100x100 gilt:   
* `lines="0"`: Zeichenbereich 90x90 Einheiten   
* `lines="1"`: Zeichenbereich 90x72 Einheiten   
* `lines="2"`: Zeichenbereich 90x54 Einheiten   

Einen Sonderfall stellt der Text in der Mitte des socket1-Symbols dar. Die Anzeige dieses Textes ist unabh&auml;ngig vom Wert des `lines`-Parameters. Ist der Parameter `textCenter` definiert, so wird dieser Text auf jeden Fall etwas oberhalb der Mitte des Symbols angezeigt.   

## Strecken eines Symbols (fx, fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Beispiel zeigt:   
![Gestrecktes socket1-Symbol](./images/vuex130_socket1_stretch1.png "Gestrecktes socket1-Symbol")   
_Bild 3: Gestrecktes socket1-Symbol_   
Der linke Symbol ist in x-Richtung um den Faktor 2 gestreckt (ergibt 2*100 =  200 Einheiten) und in y-Richtung um 1,3 (ergibt 130 Einheiten).   
__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiSocket1 :x="50" :y="70" sid="socket1" :fx="2" :fy="1.3"></CiSocket1>
</svg>
</template>
```   

## Farbe des socket1-Symbols (color)
Mit Hilfe des Attributs `color=` kann die Farbe eines CiSocket1-Symbols gew&auml;hlt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
Wird kein Wert angegeben, ist die Farbe hellblau (entsprechend der Konstanten `colorUnknown` aus der Datei `Geo.ts`).   
Die CiSocket1-Farbe kann auch &uuml;ber empfangene MQTT-Nachrichten ver&auml;ndert werden, was &uuml;ber die Variable `iSocket1State` gesteuert werden kann. Dazu muss die Codierung der Methoden `onMessage` (in der Datei `CiSocket1Controller.ts`) und `colorSocket1` (in der Datei `CiSocket1.vue`) angepasst werden.   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines socket1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das socket1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das socket1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines socket1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   

## In `CiSocket1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `iSocket1State` | Status des socket1-Symbols. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   
| `type`          | Typ des socket2-Elements. Je nach Hardware-Type kann auf empfangene MQTT-Nachrichten entsprechend reagiert werden.  |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `battery`      | Empfang des Ladezustands, falls ein socket1 symbol mit Batterie betrieben wird. Kann in der Fu&szlig;zeile angezeigt werden. |   
| `textCenter`   | Text in der Mitte des Symbols. |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiSocket1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<Socket1>` erg&auml;nzt werden.   
M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

Als Beispiel enth&auml;lt der unten stehende Code eine Reaktion auf Antworten einer (Zigbee-)Funksteckdose vom Typ e1603 (IKEA). Die Payload ist JSON-formatiert und der Schl&uuml;ssel "`state`" enth&auml;lt den Schaltzustand der Steckdose ("ON" oder "OFF"). Diese Information setzt den Wert von `iSocketState` auf -1 oder -2 und das socket1-Symbol wird entsprechend eingef&auml;rbt (siehe Methode `colorSocket1` in der Datei CiSocket1.vue).   

```
// ______CiSocket1Controller.ts__________________khartinger_____
// 2022-09-14: new
// 2023-01-29: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Socket1 extends IBase {
  iSocket1State: number;
  type: string;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiSocket1Controller extends CiBaseController {
  public sockets1: Array<Socket1> = reactive(
    [
      {
        id: 'socket1',
        name: 'Socket1_1',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textFooter: '--Footer1--',
        subTopic: 'ci/socket/1',
        pubTopic: 'ci/socket/1/set',
        pubPayload: '{"state": "TOGGLE"}'
      },
      {
        id: 'socket2',
        name: 'Socket1_2',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/socket/2',
        pubTopic: 'ci/socket/2/set',
        pubPayload: '{"state": "TOGGLE"}'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.sockets1.forEach(socket1 => {
      const aSubTopic = socket1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---socket found -------------------------------------
        // console.log('CiSocket1Controller.onMessage payload=', message.payload)
        if (socket1.type === 'e1603') {
          const aPayload = JSON.parse(message.payload)
          if ((aPayload.state === 'ON') || (aPayload.state === 'on')) socket1.iSocket1State = -1
          else {
            if ((aPayload.state === 'OFF') || (aPayload.state === 'off')) socket1.iSocket1State = -2
            else { socket1.iSocket1State = -9 }
          }
        }
        // ---end socket found----------------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSocket1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSocketController: ERROR:', e) })
  }
}

export const ciSocket1Controller = new CiSocket1Controller()
```

[Zum Seitenanfang](#up)
