Letzte &Auml;nderung: 25.2.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiTextarea1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiTextarea1`-Symbols.   
Allgemein gilt:   
* Beim Klicken auf das `CiTextarea1`-Symbol wird eine MQTT-Nachricht ausgesendet.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiTextarea1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex160_ci_mqtt_textarea1` herunterladen.   
* Visual Studio Code (VSC) mit installierter TypeScript-Unterst&uuml;tzung starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Dr&uuml;ckt man im Browser ein CiTextarea1-Symbol, so werden eine bzw. mehrere MQTT-Nachrichten gesendet, die in der Konsole zu sehen sind, zB.   
`ci/textarea1/1/get standard text published by textarea1 :)`   
`ci/textarea1/2/set standard text published by textarea1 :)`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Ci-Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit den verschiedenen `CiTextarea1`-Darstellungen. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![CiTextarea1-Varianten](./images/vuex160_all_textarea1_1.png "CiTextarea1-Varianten")   
![CiTextarea1-Varianten](./images/vuex160_all_textarea1_2.png "CiTextarea1-Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiTextarea1-Darstellungen_   

## CiTextarea1-Aufruf
Das Zeichnen eines CiTextarea1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiTextarea1>` und die Angabe weiterer Attribute.   
_Beispiel_:   
```   
<CiTextarea1 :x="50" :y="80" sid="textarea1" :border="3" lines="0" :fx="2" :fy="1.5" color="white"></CiTextarea1>
```   

## Positionierung eines Symbols (:x, :y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiTextarea1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiTextarea1-Symbols](./images/vuex160_textarea1_location1.png "Ort eines CiTextarea1-Symbols")   
_Bild 2: Ort eines CiTextarea1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")

## Rand eines Symbols (:border)
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

## Textangaben im CiTextarea1-Symbol (lines)
Jedes `CiTextarea1`-Symbol kennt drei m&ouml;gliche Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Zeichenbereich (= Textbereich)   

Welche Texte des Kopf- und Fu&szlig;bereichs im Symbol wirklich angezeigt werden, h&auml;ngt vom Wert des Attributs `lines=` ab:    
| Parameter     | Bedeutung                      |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Weder Kopfzeile noch Fu&szlig;zeile werden angezeigt. |   
| `lines="1"`   | Die Kopfzeile wird angezeigt. Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`. |   
| `lines="2"`   | Kopf- und Fu&szlig;zeile werden angezeigt.<br>Kopfzeile: Ist im Controller der Parameter `name` definiert, so wird dieser Text angezeigt, ansonsten die `id`.<br>Fu&szlig;zeile: Ist im Controller der Parameter `textFooter` definiert, so wird dieser Text angezeigt, ansonsten die `id`. |   

Vorgabe ist `lines="1"`   

Je nach Anzahl angezeigter Zeilen ver&auml;ndert sich die Gr&ouml;&szlig;e des Zeichenbereichs f&uuml;r das CI-Symbol. F&uuml;r das Standard-Symbol mit der Gesamtgr&ouml;&szlig;e 100x100 gilt:   
* `lines="0"`: Zeichenbereich 90x90 Einheiten   
* `lines="1"`: Zeichenbereich 90x72 Einheiten   
* `lines="2"`: Zeichenbereich 90x54 Einheiten   

## Strecken eines Symbols (:fx, :fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Bild zeigt:   
![Gestrecktes CiTextarea1-Symbol](./images/vuex160_textarea1_stretch1.png "Gestrecktes CiTextarea1-Symbol")   
_Bild 3: Gestrecktes CiTextarea1-Symbol_   

Die linken beiden CiTextarea1-Symbole sind um den Faktor fx=2 gestreckt (ergibt 2*100 = 200 Einheiten), das dritte Symbol ist um den Faktor fy=2 gesteckt und das rechte Symbol ist um fx=2 und fy=2 gestreckt, d.h., es ist mit einer Gr&ouml;&szlig;e von 200x200 doppelt so gro&szlig; wie das Original.   

__Codierungsbeispiel__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTextarea1 :x="50" :y="70"  sid="textarea1" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="50" :y="170" sid="textarea2" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="250" :y="70" sid="textarea1" :fx="1" :fy="2"></CiTextarea1>
  <CiTextarea1 :x="350" :y="70" sid="textarea2" :fx="2" :fy="2"></CiTextarea1>
  </svg>
</template>
```   

## Farbe des CiTextarea1-Symbols (color, colorH, colorF)
Mit Hilfe des Attributs `color=` kann die Hintergrundfarbe eines CiTextarea1-Symbols gew&auml;hlt werden. Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#eedd00` oder eine Textangabe wie `red`, `white` etc. sein.   
Wird kein Wert angegeben, so ist das Symbol transparent, das hei&szlig;t, es hat die Hintergrundfarbe.   
Weiters kann mit Hilfe des Attributs `colorH=` die Hintergrundfarbe der Kopfzeile ("header") bzw. mit `colorF=` die Hintergrundfarbe der Fu&szlig;zeile ("footer") festgelegt werden.   

## Textgr&ouml;&szlig;e (:textsize)
Die Standard-Schriftgr&ouml;&szlig;e ist so gew&auml;hlt, dass im 100x100 gro&szlig;en Standardsymbol f&uuml;nf Zeilen Platz haben [Zeilenh&ouml;he = (100 - 2x5 Rand) / 5 = 18 Einheiten). Mit dem Parameter `textsize` kann die Schriftgr&ouml;&szlig;e ver&auml;ndert werden:   
| Parameter           | Bedeutung                                |   
| ------------------- | ---------------------------------------- |   
| `:textsize="1.5"`   | Schrifth&ouml;he ist 1,5 Zeilen hoch. Das ergibt z.B. zwei Zeilen im Zeichenbereich bei `lines="2"`   |   
| `:textsize="2"`   | Schrifth&ouml;he ist 2 Zeilen hoch. Das ergibt z.B. zwei Zeilen bei `lines="1"`   |   
| `:textsize="2.5"`   | Schrifth&ouml;he ist 2,5 Zeilen hoch. Das ergibt z.B. zwei Zeilen im Zeichenbereich bei `lines="0"`   |   
| `:textsize="3"`   | Schrifth&ouml;he ist 3 Zeilen hoch. Das ergibt z.B. eine Zeile bei `lines="2"`   |   
| `:textsize="4"`   | Schrifth&ouml;he ist 4 Zeilen hoch. Das ergibt z.B. eine Zeile bei `lines="1"`   |   
| `:textsize="5"`   | Schrifth&ouml;he ist 5 Zeilen hoch. Das ergibt z.B. eine Zeile bei `lines="0"`   |   

![Schriftgr&ouml;&szlig;en](./images/vuex160_textarea1_textsize1.png "Schriftgr&ouml;&szlig;en")   
_Bild 4: Verschiedene Schriftgr&ouml;&szlig;en im Zeichenbereich_   


<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiTextarea1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiTextarea1-Symbol "h&ouml;rt" (kann auch leer '' sein). |   
| `pubTopic`   | MQTT-Topics, die das CiTextarea1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiTextarea1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Senden einer Nachricht verwendet werden soll. |   


## In `CiTextarea1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft       | Bedeutung                            |   
| ----------------- | ------------------------------------ |   
| `iTextarea1State` | Status des CiTextarea1-Symbols. Gibt an, was in der Kopfzeile angezeigt werden soll: 0=Kein Titel, 1=Name, 2=Id, 4=Erste Zeile der Nachricht.<br>Ist der Wert positiv, wird dies als Farbe f&uuml;r den Hintergrund verwendet.<br>Ben&ouml;tigt man keine dieser M&ouml;glichkeiten, so sollte man den Wert auf zB -99 setzen. |   
| `subPayload`     | String-Variable f&uuml;r den Inhalt des Textbereichs. |   
| `clkWhenMounted` | `true` bedeutet, dass beim Mounten des Symbols ein Klick auf das Symbol simuliert wird. Dadurch wird zB eine MQTT-Nachricht ausgesendet, die einen Text etc. anfordern kann. |   

Folgende Attribute sind optional und k&ouml;nnen bei Bedarf verwendet werden:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `useRawText` | Wird dieser Wert auf `true` gesetzt, so wird der Inhalt der Variablen `subPayload` bei der Anzeige nicht ver&auml;ndert. <br>Ansonsten werden Worte mit zwei, drei oder vier Buchstaben m&ouml;glichst nicht getrennt und Leerzeichen am Beginn und Ende einer Zeile entfernt. |   
| `textFooter` | Angabe eines fixen Textes in der Fu&szlig;zeile. |   

## Beispiel: Codierung des `CiTextarea1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<CiTextarea1>` erg&auml;nzt werden.   
Im unten stehenden Beispiel sind vier CiTextarea1-Symbole definiert, wobei sich die Symbole `textarea3` und `textarea4` nur dadurch unterscheiden, wie der Text angezeigt werden soll (mit oder ohne Textanpassungen entsprechend dem Wert des Attributs `useRawText`).

M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

Als Beispiel enth&auml;lt der unten stehende Code ein spezielles Verhalten von Symbolen mit der id `textarea1`:   
* Bei Empfang eines Topics `ci/textarea1/2/get` sendet das Symbol eine Nachricht mit dem Topic `ci/textarea1/2/set` und der Payload `special: requested text from id=textarea1` aus.   

In allen anderen F&auml;llen wird die empfangene Payload entsprechend den Einstellungen im Symbol angezeigt.   

```
// ______CiTextarea1Controller.ts________________khartinger_____
// 2022-08-14: New
// 2023-02-05: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Textarea1 extends IBase {
  iTextarea1State: number; // 0=no title, 1=name, 2=id, 4=message
  subPayload: string; //      text displayed in textarea
  clkWhenMounted: boolean; // text request on mounting?
  useRawText?: boolean; //    do not wrap text
  textFooter?: string; //     footer text
}

export class CiTextarea1Controller extends CiBaseController {
  public textarea1s: Array<Textarea1> = reactive(
    [
      {
        id: 'textarea1',
        name: 'Textarea1_1',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/1/set ci/textarea1/2/get',
        subPayload: 'click: publish a text by textarea1 - this is just to make the text longer and longer 1234567890 and once more longer...',
        pubTopic: 'ci/textarea1/1/get ci/textarea1/2/set',
        pubPayload: 'standard text published by textarea1 :)',
        textFooter: '==Footer=='
      },
      {
        id: 'textarea2',
        name: 'Textarea1_2',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: true,
        subTopic: 'ci/textarea1/2/set',
        subPayload: 'click sends request ci/textarea1/2/get',
        pubTopic: 'ci/textarea1/2/get',
        pubPayload: 'text'
      },
      {
        id: 'textarea3',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      },
      {
        id: 'textarea4',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: true,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      }

    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for topic textarea1------------------------
    this.textarea1s.forEach(textarea1 => {
      const aSubTopic = textarea1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea1 topic found ----------------------------
        if (textarea1.id === 'textarea1') {
          // -message for id=textarea1--------------------------
          if (message.topic === 'ci/textarea1/2/get') {
            this.publishCi('ci/textarea1/2/set', 'special: requested text from id=textarea1')
          } else textarea1.subPayload = message.payload
        } else {
        // ---message is not for id=textarea1-------------------
          textarea1.subPayload = message.payload
        }
      } // ---END textarea1 topic found-------------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTextarea1Controller: ERROR:', e) })
  }
}

export const ciTextarea1Controller = new CiTextarea1Controller()
```

[Zum Seitenanfang](#up)
