Letzte &Auml;nderung: 28.1.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiButton2-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiButton2`-Symbols.   
Allgemein gilt:   
* Klickt man auf die obere H&auml;lfte des `CiButton2`-Symbols, so wird eine erste MQTT-Nachricht ausgesendet (zB "Ein").   
* Klickt man auf die untere H&auml;lfte des `CiButton2`-Symbols, so wird eine andere MQTT-Nachricht ausgesendet (zB "Aus").   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiButton2Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex120_ci_mqtt_button2` herunterladen.   
* Visual Studio Code (VSC) starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Dr&uuml;ckt man im Browser auf die obere H&auml;lfte des Buttons, so werden eine bzw. mehrere MQTT-Nachrichten gesendet, die in der Konsole zu sehen sind, zB.   
`ci/lamp/1/set/lamp 1`   
`ci/lamp/2/set/lamp 1`   
* Dr&uuml;ckt man im Browser auf die untere H&auml;lfte des Buttons, so werden eine bzw. mehrere MQTT-Nachrichten gesendet, die in der Konsole zu sehen sind, zB.   
`ci/lamp/1/set/lamp 0`   
`ci/lamp/2/set/lamp 0`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiButton2`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![Buttonvarianten](./images/vuex120_all_buttons2.png "Buttonvarianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiButton2-Darstellungen_   

## Button-Aufruf
Das Zeichnen eines Buttons erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiButton2>` und weiteren Attributen.   
_Beispiel_:   
```   
<CiButton2 :x="50" :y="80" sid="button1" :border="3" lines="0" :fx="2" :fy="1.5" shape="round_1right_2left"></CiButton2>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 300 150">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="60"`, dass der Button an den linken Rand grenzt und 10 Einheiten vom oberen Rand entfernt ist:   
![Ort eines Buttons](./images/vuex120_button2_location1.png "Ort eines Buttons")   
_Bild 2: Ort eines Buttons_   

_Anmerkung_:   
Restlicher Platz unten: 150 - 10 - 100 = 40 Einheiten   
Restlicher Platz rechts: 300 - 100 = 200 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")

## Randes eines Symbols (border)
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

## Textangaben im CiButton2-Symbol (lines)
Jedes `CiButton2`-Symbol kennt vier m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Obere Beschriftung   
* Untere Beschriftung   

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

Einen Sonderfall stellen die Beschriftungstexte oben und unten im Button-Symbols dar. Die Anzeige dieser Texte ist unabh&auml;ngig vom Wert des `lines`-Parameters. Sind Werte f&uuml;r die Parameter `textUpper` und `textUpper` in der Datei `CiButton2Controller.ts` definiert, so werden diese Text auf jeden Fall angezeigt.   

## Strecken eines Symbols (fx, fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Beispiel zeigt:   
![Gestreckter Button](./images/vuex120_button2_stretch1.png "Gestreckter Button")   
_Bild 3: Gestreckter Button_   
Der Button ist in x-Richtung um den Faktor 2 gestreckt (ergibt 2*100 =  200 Einheiten) und in y-Richtung um 1,4 (ergibt 140 Einheiten).   
__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiButton2 :x="50" :y="60" sid="button1" :fx="2" :fy="1.4"></CiButton2>
</svg>
</template>
```   

## Verschiedene CiButton2-Formen (shape)
CiButton2-Symbole gibt es in vier Formen, die durch das Attribut `shape=` ausgew&auml;hlt werden:   
| Wert        | Bedeutung                                |   
| ----------- | ---------------------------------------- |   
| `"circle"`  | Kreisf&ouml;rmiger Button, der an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird. |   
| `"round"`   | Ovaler Button, bei dem der Kreis so gedehnt wird, dass die Zeichenfl&auml;che ausgef&uuml;llt ist.   |   
| `"rect"`    | Rechteckiger Button mit abgerundeten Ecken (1/10 eines Kreisradius)   |    
| `"square"`  | Quadratischer Button mit abgerundeten Ecken, der an die kleinere Seitenl&auml;nge des Zeichenbereichs angepasst wird.    |   

Vorgabe ist `shape="circle"`   

## Zus&auml;tzliche Symbole im Button
Das Symbol `CiButton2` kann im oberen und unteren Bereich je ein gleichseitiges Dreieck enthalten, dessen Spitze nach oben, unten, links oder rechts zeigt. Erreicht wird dies, indem man beim `shape`-Attribut folgendes angibt:   
* Oberes Symbol:  "1up", "1down", "1left" oder "1right"   
* Unteres Symbol: "2up", "2down", "2left" oder "2right"   
Zur Trennung der Werte sollte ein Unterstrich verwendet werden.   
_Beispiele_:   
* `shape="1up_2down"` erzeugt einen kreisf&ouml;rmigen Button (= Vorgabe) mit einem "Pfeil" (Dreieck) nach oben im oberen Bereich und einem "Pfeil" nach unten im unteren Bereich.   
* `shape="square_1left_2right"` erzeugt einen quadratischen Button mit einem "Pfeil" nach links im oberen Bereich und einem "Pfeil" nach rechts im unteren Bereich.   
![Zusatzsymbole](./images/vuex120_button2_additional1.png "Zusatzsymbole")   
_Bild 4: Zus&auml;tzliche Symbole im Button_   

## Farbe des Buttons (colorup, colordn)
Mit Hilfe der Attribute `colorup=` und `colordn=` kann die Farbe des oberen bzw. unteren `CiButton2`-Symbols gew&auml;hlt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
Wird kein Wert angegeben, ist die Farbe hellblau (entsprechend der Konstanten `colorUnknown` aus der Datei `Geo.ts`).   
Die `CiButton2`-Farben k&ouml;nnen auch &uuml;ber empfangene MQTT-Nachrichten ver&auml;ndert werden, was &uuml;ber die Variable `iButtonState` gesteuert werden kann. Dazu muss die Codierung der Methoden `onMessage` (in der Datei `CiButton1Controller.ts`) und `colorUpper` bzw. `colorLower` (in der Datei `CiButton2.vue`) angepasst werden.   

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
| `pubPayload` | Payload, die beim Dr&uuml;cken der oberen H&auml;lfte des Buttons versendet wird. |   


## In `CiButton2Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft    | Bedeutung                            |   
| -------------- | ------------------------------------ |   
| `iButtonState` | Status des Buttons. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `battery`      | Empfang des Ladezustands, falls ein Taster mit Batterie betrieben wird. Kann in der Fu&szlig;zeile angezeigt werden. |   
| `textUpper`   | Text in der oberen Symbolh&auml;lfte, zB "auf", "ab", "links", "rechts" usw. |   
| `textLower`   | Text in der unteren Symbolh&auml;lfte, zB "auf", "ab", "links", "rechts" usw. |   
| `textFooter`   | Angabe eines fixen Textes in der Fu&szlig;zeile. |   
| `pubPayloadLower` | Payload, die beim Dr&uuml;cken der unteren H&auml;lfte des Buttons versendet wird. |   

## Beispiel: Codierung des `CiButton2Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<Button2>` erg&auml;nzt werden.   
M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

Als Beispiel enth&auml;lt der unten stehende Code eine Reaktion auf das Topic `ci/lamp/2/set/lamp`. L&auml;uft ein MQTT-Server auf einem Rechner oder Raspberry Pi mit der IP 10.1.1.1, so bewirkt das Dr&uuml;cken
* der oberen H&auml;lfte eines Buttons mit der id `button1` ein Aussenden der Nachrichten mit dem Topic `ci/lamp/1/set/lamp` sowie `ci/lamp/2/set/lamp` und der Payload `1`. Die zweite Nachricht wird von Buttons mit der ID `button2` empfangen und die obere Button-H&auml;lfte "gelb" sowie die untere H&auml;lfte "grau" eingef&auml;rbt. 
* der unteren H&auml;lfte eines Buttons mit der id `button1` ein Aussenden einer Nachricht mit dem Topic `ci/lamp/1/set/lamp` sowie `ci/lamp/2/set/lamp` und der Payload `0`. Die zweite Nachricht wird von Buttons mit der ID `button2` empfangen und die obere Button-H&auml;lfte "grau" sowie die untere H&auml;lfte "gelb" eingef&auml;rbt.   

```
// ______CiButton2Controller.ts__________________khartinger_____
// 2023-01-28: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button2 extends IBase {
  iButtonState: number;
  battery?: string;
  textUpper?: string;
  textLower?: string;
  textFooter?: string;
  pubPayloadLower?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiButton2Controller extends CiBaseController {
  public buttons2: Array<Button2> = reactive(
    [
      {
        id: 'button1',
        name: 'Button2_1',
        iButtonState: -99,
        // textUpper: 'ON',
        // textLower: 'OFF',
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1',
        pubPayloadLower: '0'
      },
      {
        id: 'button2',
        name: 'Button2_2',
        iButtonState: -99,
        textUpper: 'ON',
        textLower: 'OFF',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: 'ON',
        pubPayloadLower: 'OFF'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons2.forEach(button2 => {
      const aSubTopic = button2.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button2.iButtonState = -2
          if (message.payload === '1') button2.iButtonState = -1
        }
        if (message.topic.includes('voltage')) {
          button2.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton2Controller = new CiButton2Controller()
```

[Zum Seitenanfang](#up)
