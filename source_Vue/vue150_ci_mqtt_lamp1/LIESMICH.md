Letzte &Auml;nderung: 30.1.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiLamp1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiLamp1`-Symbols.   
Allgemein gilt:   
* Beim Klicken auf das `CiLamp1`-Symbol wird eine MQTT-Nachricht ausgesendet.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiLamp1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vue150_ci_mqtt_lamp1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Dr&uuml;ckt man im Browser einen Lamp1, so werden eine bzw. mehrere MQTT-Nachrichten gesendet, die in der Konsole zu sehen sind, zB.   
`ci/lamp/1/set/lamp 0`   
`ci/lamp/2/set/lamp 0`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiLamp1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![Lamp1varianten](./images/vue150_all_lamp1.png "Lamp1varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene Lamp1-Darstellungen_   

## Lamp1-Aufruf
Das Zeichnen eines Lamp1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiLamp1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiLamp1 :x="50" :y="80" sid="lamp1" :border="3" lines="0" :fx="2" :fy="1.5" shape="round"></CiLamp1>
```   

## Positionierung eines Symbols
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das Lamp1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines Lamp1-Symbols](./images/vue150_lamp1_location1.png "Ort eines Lamp1-Symbols")   
_Bild 2: Ort eines Lamp1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## Randes eines Symbols
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

## Textangaben im CiLamp1-Symbol
Jedes `CiLamp1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
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

Einen Sonderfall stellt der Text in der Mitte des Lamp1-Symbols dar. Die Anzeige dieses Textes ist unabh&auml;ngig vom Wert des `lines`-Parameters. Ist der Parameter `textCenter` definiert, so wird dieser Text auf jeden Fall in der Mitte des Symbols angezeigt.   

## Strecken eines Symbols
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Beispiel zeigt:   
![Gestrecktes Lamp1-Symbol](./images/vue150_lamp1_stretch1.png "Gestrecktes Lamp1-Symbol")   
_Bild 3: Gestrecktes Lamp1-Symbol_   

Die linken beiden CiLamp1-Symbole sind um den Faktor fx=2 gestreckt (ergibt 2*100 = 200 Einheiten), das dritte Symbol ist um den Faktor fy=2 gesteckt und das rechte Symbol ist um fx=2 und fy=2 gestreckt, d.h., es ist mit einer Gr&ouml;&szlig;e von 200x200 doppelt so gro&szlig; wie das Original.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiLamp1 :x="50" :y="70" sid="lamp1" :fx="2" :fy="1"></CiLamp1>
  <CiLamp1 :x="50" :y="170" sid="lamp2" :fx="2" :fy="1"></CiLamp1>
  <CiLamp1 :x="250" :y="70" sid="lamp1" :fx="1" :fy="2"></CiLamp1>
  <CiLamp1 :x="350" :y="70" sid="lamp2" :fx="2" :fy="2"></CiLamp1>
  </svg>
</template>
```   

## Verschiedene CiLamp1-Formen
CiLamp1-Symbole gibt es in vier Formen, die durch das Attribut `shape=` ausgew&auml;hlt werden:   
| Wert        | Bedeutung                                |   
| ----------- | ---------------------------------------- |   
| `"circle"`  | Kreisf&ouml;rmiges CiLamp1-Symbol, das an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird. |   
| `"round"`   | Ovales CiLamp1-Symbol, bei dem der Kreis so gedehnt wird, dass die Zeichenfl&auml;che ausgef&uuml;llt ist.   |   
| `"rect"`    | Rechteckiges CiLamp1-Symbol mit abgerundeten Ecken (1/10 eines Kreisradius)   |    
| `"square"`  | Quadratisches CiLamp1-Symbol mit abgerundeten Ecken, das an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird.    |   

Vorgabe ist `shape="circle"`   

## Farbe des CiLamp1-Symbols
Mit Hilfe des Attributs `color=` kann die Farbe eines CiLamp1-Symbols gew&auml;hlt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
Wird kein Wert angegeben, ist die Farbe hellblau (entsprechend der Konstanten `colorUnknown` aus der Datei `Geo.ts`).   
Die CiLamp1-Farbe kann auch &uuml;ber empfangene MQTT-Nachrichten ver&auml;ndert werden, 
was &uuml;ber die Variable `iLamp1State` gesteuert werden kann. Dazu muss die Codierung der Methoden `onMessage` (in der Datei `CiLamp1Controller.ts`) und `colorLamp1` (in der Datei `CiLamp1.vue`) angepasst werden.   

# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiLamp1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiLamp1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiLamp1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiLamp1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiLamp1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `iLamp1State` | Status des CiLamp1-Symbols. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `battery`      | Empfang des Ladezustands, falls ein CiLamp1-Symbol mit Batterie betrieben wird. Kann in der Fu&szlig;zeile angezeigt werden. |   
| `textCenter`   | Text in der Mitte des Symbols.     |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiLamp1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<Lamp1>` erg&auml;nzt werden.   
M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

Als Beispiel enth&auml;lt der unten stehende Code eine Reaktion auf das Topic `ci/lamp/2/set/lamp`. L&auml;uft ein MQTT-Server auf einem Rechner oder Raspberry Pi mit der IP 10.1.1.1, so bewirkt das Dr&uuml;cken eines `lamp1`-Sybmols unter anderem das Aussenden einer Nachricht mit dem Topic `ci/lamp/2/set/lamp` und der Payload `0`. Diese wird von Lamp1-Symbolen mit der ID `lamp2` empfangen und die `lamp2`-Symbole unten "grau" eingef&auml;rbt ("aus"). 
Das Dr&uuml;cken eines `lamp2`-Symbols sendet Nachrichten mit dem Topic `ci/lamp/2/set/lamp` und der Payload `1` aus, was zum Einf&auml;rben der unteren `lamp2`-Symbole auf "gelb" f&uuml;hrt ("ein").   

```
// ______CiLamp1Controller.ts____________________khartinger_____
// 2021-12-28: new
// 2023-01-30: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Lamp1 extends IBase {
  iLamp1State: number;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiLamp1Controller extends CiBaseController {
  public lamps1: Array<Lamp1> = reactive(
    [
      {
        id: 'lamp1',
        name: 'Lamp_1',
        iLamp1State: -99,
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'lamp2',
        name: 'Lamp_2',
        iLamp1State: -99,
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.lamps1.forEach(lamp1 => {
      const aSubTopic = lamp1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---lamp1 topic found --------------------------------
        // console.log('CiLamp1Controller.onMessage payload=', message.payload)
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') lamp1.iLamp1State = -2
          if (message.payload === '1') lamp1.iLamp1State = -1
        }
        if (message.topic.includes('voltage')) {
          lamp1.battery = `${message.payload}` + '%'
        }
        // ---end lamp1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiLamp1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiLamp1Controller: ERROR:', e) })
  }
}

export const ciLamp1Controller = new CiLamp1Controller()
```

[Zum Seitenanfang](#up)
