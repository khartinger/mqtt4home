Letzte &Auml;nderung: 27.1.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiButton1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiButton1`-Symbols.   
Allgemein gilt:   
* Beim Klicken auf das `CiButton1`-Symbol wird eine MQTT-Nachricht ausgesendet.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiButton1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex110_ci_mqtt_button1` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Dr&uuml;ckt man im Browser einen Button, so werden eine bzw. mehrere MQTT-Nachrichten gesendet, die in der Konsole zu sehen sind, zB.   
`ci/lamp/1/set/lamp 0`   
`ci/lamp/2/set/lamp 0`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiButton1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![Buttonvarianten](./images/vuex110_all_buttons1.png "Buttonvarianten")   
_Bild 1: Beispiele f&uuml;r verschiedene Button-Darstellungen_   

## Button-Aufruf
Das Zeichnen eines Buttons erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiButton1>` und weiteren Attributen.   
_Beispiel_:   
```   
<CiButton1 :x="50" :y="80" sid="button1" :border="3" lines="0" :fx="2" :fy="1.5" shape="round_right"></CiButton1>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 300 150">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="60"`, dass der Button an den linken Rand grenzt und 10 Einheiten vom oberen Rand entfernt ist:   
![Ort eines Buttons](./images/vuex110_button_location1.png "Ort eines Buttons")   
_Bild 2: Ort eines Buttons_   

_Anmerkung_:   
Restlicher Platz unten: 150 - 10 - 100 = 40 Einheiten   
Restlicher Platz rechts: 300 - 100 = 200 Einheiten   

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

Vorgabe ist `:border="1"`   

## Textangaben im CiButton1-Symbol (lines)
Jedes `CiButton1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
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

Einen Sonderfall stellt der Text in der Mitte des Button-Symbols dar. Die Anzeige dieses Textes ist unabh&auml;ngig vom Wert des `lines`-Parameters. Ist der Parameter `textCenter` definiert, so wird dieser Text auf jeden Fall in der Mitte des Symbols angezeigt.   

## Strecken eines Symbols (fx, fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Beispiel zeigt:   
![Gestreckter Button](./images/vuex110_button_stretch1.png "Gestreckter Button")   
_Bild 3: Gestreckter Button_   
Der Button ist in x-Richtung um den Faktor 2 gestreckt (ergibt 2*100 =  200 Einheiten) und in y-Richtung um 1,4 (ergibt 140 Einheiten).   
__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiButton1 :x="50" :y="60" sid="button1" :fx="2" :fy="1.4"></CiButton1>
</svg>
</template>
```   

## Verschiedene CiButton1-Formen (shape)
CiButton1-Symbole gibt es in vier Formen, die durch das Attribut `shape=` ausgew&auml;hlt werden:   
| Wert        | Bedeutung                                |   
| ----------- | ---------------------------------------- |   
| `"circle"`  | Kreisf&ouml;rmiger Button, der an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird. |   
| `"round"`   | Ovaler Button, bei dem der Kreis so gedehnt wird, dass die Zeichenfl&auml;che ausgef&uuml;llt ist.   |   
| `"rect"`    | Rechteckiger Button mit abgerundeten Ecken (1/10 eines Kreisradius)   |    
| `"square"`  | Quadratischer Button mit abgerundeten Ecken, der an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird.    |   

Vorgabe ist `shape="circle"`   

## Zus&auml;tzliche Symbole im Button
CiButtons k&ouml;nnen im Inneren ein gleichseitiges Dreieck enthalten, dessen Spitze nach oben, unten, links oder rechts zeigt. Erreicht wird dies, indem man beim `shape`-Attribut "up", "down", "left" oder "right" angibt. Bei Kombination mit einer Button-Formangabe sollte ein Unterstrich verwendet werden.   
_Beispiele_:   
* `shape="up"` erzeugt einen kreisf&ouml;rmigen Button (= Vorgabe) mit "Pfeil" (Dreieck) nach oben.   
* `shape="square_down"` erzeugt einen quadratischen Button mit "Pfeil" (Dreieck) nach unten.   

## Farbe des Buttons (color)
Mit Hilfe des Attributs `color=` kann die Farbe eines CiButton1 gew&auml;hlt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
Wird kein Wert angegeben, ist die Farbe hellblau (entsprechend der Konstanten `colorUnknown` aus der Datei `Geo.ts`).   
Die CiButton1-Farbe kann auch &uuml;ber empfangene MQTT-Nachrichten ver&auml;ndert werden, was &uuml;ber die Variable `iButtonState` gesteuert werden kann. Dazu muss die Codierung der Methoden `onMessage` (in der Datei `CiButton1Controller.ts`) und `colorButton` (in der Datei `CiButton1.vue`) angepasst werden.   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines Buttons sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die der Button "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die der Button beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines Buttons. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiButton1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `iButton1State` | Status des Buttons. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `battery`      | Empfang des Ladezustands, falls ein Taster mit Batterie betrieben wird. Kann in der Fu&szlig;zeile angezeigt werden. |   
| `textCenter`   | Text in der Mitte des Symbols, zB "auf", "ab", "links", "rechts" usw. |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiButton1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<Button1>` erg&auml;nzt werden.   
M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   
Als Beispiel enth&auml;lt der unten stehende Code eine Reaktion auf das Topic `ci/lamp/2/set/lamp`. L&auml;uft ein MQTT-Server auf einem Rechner oder Raspberry Pi mit der IP 10.1.1.1, so bewirkt das Dr&uuml;cken eines Buttons rechts unten (bei "Centertext") unter anderem das Aussenden einer Nachricht mit dem Topic `ci/lamp/2/set/lamp` und der Payload `1`. Diese wird von Buttons mit der ID `button2` empfangen und der Button "gelb" eingef&auml;rbt ("ein"). 
Das Dr&uuml;cken der anderen Buttons mit ID `button1` senden eine Nachricht mit dem Topic `ci/lamp/2/set/lamp` und der Payload `0` aus, was zum Einf&auml;rben der unteren Buttons auf "grau" f&uuml;hrt ("aus").   

```
// ______CiButtonController.ts___________________khartinger_____
// 2022-09-14: add .catch((e) ..) to this.publish
// 2023-01-25: add textCenter, textFooter | remove shape, color
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button1 extends IBase {
  iButtonState: number;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiButton1Controller extends CiBaseController {
  public buttons1: Array<Button1> = reactive(
    [
      {
        id: 'button1',
        name: 'Button_1',
        iButtonState: -99,
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'button2',
        name: 'Button_2',
        iButtonState: -99,
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons1.forEach(button1 => {
      const aSubTopic = button1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (button1.id.substring(0, 7) === 'lamp1') {
          const aPayload = JSON.parse(message.payload)
          button1.iButtonState = 10 + aPayload.motor
        }
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button1.iButtonState = -2
          if (message.payload === '1') button1.iButtonState = -1
        }
        if (message.topic.includes('voltage')) {
          button1.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton1Controller = new CiButton1Controller()
```

[Zum Seitenanfang](#up)
