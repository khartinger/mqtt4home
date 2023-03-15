Letzte &Auml;nderung: 15.3.2023 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Verwendung des CiSmsStatus1-Symbols</h1>
<a href="../../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./README.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
# Ziel
Kennenlernen aller M&ouml;glichkeiten zur Darstellung und Verwendung eines `CiSmsStatus1`-Symbols.   
Allgemein gilt:   
* Das `CiSmsStatus1`-Symbol dient zum Abfragen des Zustandes eines GSM-Moduls (zB SIM808 Modul GSM mit GPS Antenne f&uuml;r 3G 4G SIM Karte zB von [AliExpress](https://de.aliexpress.com/item/1005002384541464.html?spm=a2g0s.9042311.0.0.5c824c4dqUu43E) oder [Amazon](https://www.amazon.de/dp/B09CM8TSX9/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B09CM8TSX9&pd_rd_w=yNPjf&pf_rd_p=4f2ceb27-95e9-46ab-8808-db390b56ec01&pd_rd_wg=Muvvr&pf_rd_r=3NTH9ZQRZNE3VBZKD1YV&pd_rd_r=58ca39f9-b9f0-40b5-9b14-7f5d8a37ce1d&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzN1A5OTA0NUNBTTA2JmVuY3J5cHRlZElkPUEwMzcxNzc3MlkxMFpaUTBTRjhYMSZlbmNyeXB0ZWRBZElkPUEwNzU2MTYzMjRJSlNTREJMTjVHSiZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)).   
* Klickt man auf den oberen Button des `CiSmsStatus1`-Symbols, so wird der Status des GSM-Moduls und Produktname abgefragt.   
* Klickt man auf den mittleren Button des `CiSmsStatus1`-Symbols, so wird der Status der SIM-Karte (Provider) abgefragt.   
* Klickt man auf den unteren Button des `CiSmsStatus1`-Symbols, so wird das SMS-Guthaben beim Provider abgefragt.   
* Alle Eigenschaften, die die grafische Darstellung des Symbols betreffen, werden im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) festgelegt.   
* Alle nicht grafischen Eigenschaften, wie ID, Name, Beschriftungen und MQTT-Funktionalit&auml;t, werden in der Datei `CiSmsStatus1Controller.ts` definiert.   

# Kurzanleitung
* Verzeichnis `vuex530_ci_mqtt_smsstatus1` herunterladen.   
* Visual Studio Code (VSC) mit Vue 3 + TypeScript Unterst&uuml;tzung starten und den Ordner dieser Vue-Anwendung &ouml;ffnen.   
* In VSC das Terminal &ouml;ffnen und folgendes eingeben:   
`npm run serve`   
* Den Browser (zB Google Chrome) starten und die Seite `localhost:8080` aufrufen.   

Im Browser werden die im _Bild 1_ (siehe unten) dargestellten Symbole angezeigt.   

Will man die MQTT-Funktionalit&auml;t ohne GSM-Modem testen, so muss auf einem Rechner (bzw. Raspberry Pi) mit der IP-Adresse `10.1.1.1` ein MQTT-Broker laufen (zB Mosquitto).   
* Entwicklungsrechner mit dem Netzwerk des Broker-Servers verbinden. (Bei WLAN zB SSID `Raspi11`, Passwort `12345678`)   
* Windows-Konsole (`cmd.exe`) starten, ins mosquitto-Verzeichnis wechseln und folgendes eingeben:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* Das CiSmsStatus1-Symbol wartet auf MQTT-Nachrichten, die Informationen zum GSM-Modem enthalten. Zur Simulation k&ouml;nnen verschiedene Antwort-Werte auch mit dem Programm `mosquitto_pub` in einer Windows-Konsole eingegeben werden:   
* `mosquitto_pub -h 10.1.1.1 -t sms/cmd/ret/module -m "{\"device\":\"/dev/ttyUSB0\",\"module\":true,\"product\":\"SIM808 R14.18\"}"`   
* `mosquitto_pub -h 10.1.1.1 -t sms/cmd/ret/sim -m "{\"provider\":\"T-Mobile A\",\"sim\":true,\"result\":\"^~+COPS: 0,0,'T-Mobile A'^~^~OK^~\"}"`   
* `mosquitto_pub -h 10.1.1.1 -t sms/cmd/ret/credit -m "{\"euro\":33.25}"`   

Anmerkung: Eine detaillierte Beschreibung zu einzelnen Schritten befindet sich in anderen Kapiteln des Projektes MQTT4HOME.

# Grafische Darstellung eines Symbols
## Allgemeines
Die Codierung der grafischen Darstellung eines CI-Symbols `CiXxx` erfolgt in einer Datei mit dem Namen wie `CiXxx.vue`. Diese Dateien werden dann von der eigentlichen Projektdatei (zB. `CiMain.vue`) verwendet, um die Web-Seite darzustellen.   
Das folgende Bild zeigt die Ausgabe dieser Vue-Anwendung mit verschiedenen `CiSmsStatus1`-Gr&ouml;&szlig;en. Man sieht, dass Streckfaktoren kleiner 1 für das `CiSmsStatus1`-Symbol nicht sinnvoll sind. Welche Attribute welche Darstellung bewirken, wird anschlie&szlig;end beschrieben.     
![CiSmsStatus1-Varianten](./images/vuex530_all_smsstatus1.png "CiSmsStatus1-Varianten")   
_Bild 1: Beispiele f&uuml;r verschiedene CiSmsStatus1-Darstellungen_   

## Erzeugen des CiSmsStatus1-Symbols
Das Zeichnen eines CiSmsStatus1-Symbols erfolgt im `<template>`-Bereich der Darstellungsdatei (zB in `CiMain.vue`) durch das Tag `<CiSmsStatus1>` und weiteren Attributen.   
_Beispiel_:   
```   
<CiSmsStatus1 :x="50" :y="70"  sid="smsstatus1" :fx="1.5" :fy="1" colorH="white" colorF="white" :border="3"></CiSmsStatus1>
```   

## Positionierung eines Symbols (x, y)
Im Normalfall sind CI-Symbole 100x100 Einheiten gro&szlig; und der Mittelpunkt (50/50) dient zur Positionsangabe (Platzierungspunkt). Ist der Zeichenbereich zB durch   
`<svg width="100%" viewBox="0 0 500 220">`   
definiert, so bewirkt die Ortsangabe `:x="50" :y="70"`, dass das CiSmsStatus1-Symbol an den linken Rand grenzt und 20 Einheiten vom oberen Rand entfernt ist:   
![Ort eines CiSmsStatus1-Symbols](./images/vuex530_smsstatus1_location1.png "Ort eines CiSmsStatus1-Symbols")   
_Bild 2: Ort eines CiSmsStatus1-Symbols_   

_Anmerkung_:   
Restlicher Platz unten: 220 - 20 - 100 = 100 Einheiten   
Restlicher Platz rechts: 500 - 100 = 400 Einheiten   

## ID eines Symbols (sid)
Die Symbol-ID (sid) stellt die Verbindung zwischen der grafischen Darstellung und dem Controller dar (siehe Eigenschaft [id](#id) im Kapitel "[Im Basis-Controller definierte Attribute](#id)")

## Randes eines Symbols (:border)
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

## Textangaben im CiSmsStatus1-Symbol
Jedes `CiSmsStatus1`-Symbol kennt drei Bereiche zur Textausgabe:   
* Kopfzeile   
* Fu&szlig;zeile   
* Zeichenfläche mit den Beschriftungen des   
  * oberen Buttons   
  * mittleres Buttons   
  * unteren Buttons.   

Es gilt:   
* Die Kopfzeile enth&auml;lt den im Controller definierten Namen des Symbols (oder die `sid`).   
* Die Fu&szlig;zeile enth&auml;lt die R&uuml;ckmeldungen des GSM-Modems.   
* Die Beschriftungen der Buttons werden im Controller definiert.   

## Strecken eines Symbols (:fx, :fy)
Standardm&auml;&szlig;ig sind alle Symbole 100x100 Einheiten gro&szlig;. Es ist jedoch m&ouml;glich, die Symbole in x- und in y-Richtung zu strecken. Der Platzierungspunkt bleibt dabei gleich.   
Das Strecken erfolgt durch Angabe der Parameter `fx` und `fy`, wie das folgende Beispiel zeigt:   
![Gestreckte Symbole](./images/vuex530_smsstatus1_stretch1.png "Gestreckte Symbole")   
_Bild 3: Gestrecktes `CiSmsStatus1`-Symbol_   

## Farben im CiSmsStatus1-Symbol (colorH, colorF, colorA1, colorA2, colorA3)
Mit Hilfe der Attribute `colorH=` und `colorF=` kann die Hintergrundfarbe der Kopf- und Fu&szlig;zeile gew&auml;hlt werden (Vorgabe: keine = transparent). Der Wert kann entweder ein RGB-Wert (rot-gr&uuml;n-blau-Wert), wie zB `#ffee00` oder eine Textangabe wie `red`, `green` etc. sein.   
Mit Hilfe der Attribute `colorA1=`, `colorA2=` und `colorA3=` kann die Farbe der Buttons gew&auml;hlt werden. Setzt man keine Farbe, zeigen sie den Zustand der Daten&uuml;bertragung vom GSM-Modem an.   

<a name="id"></a>   
# Nicht-grafische Eigenschaften
## Im Basis-Controller definierte Attribute
Eigenschaften von CI-Symbolen, die nichts mit der grafischen Darstellung des Symbols zu tun haben, werden in "`xxxController.ts`"-Dateien definiert.   
Folgende Attribute sind im Basis-Controller (Datei `CiBaseController.ts`) definiert und m&uuml;ssen in allen abgeleiteten Controllern zwingend angegeben werden:   
| Eigenschaft  | Bedeutung                                |   
| ------------ | ---------------------------------------- |   
| `id`         | Sollte im Normalfall eine eindeutige Bezeichnung eines CiSmsStatus1-Symbols sein.    |   
| `subTopic`   | Nachrichten bzw. MQTT-Topics, auf die das CiSmsStatus1-Symbol "h&ouml;rt". |   
| `pubTopic`   | MQTT-Topics, die das CiSmsStatus1-Symbol beim Klicken sendet. |   

Folgende Attribute sind optional:   
| Eigenschaft  | Bedeutung                            |   
| ------------ | ------------------------------------ |   
| `name`       | Name eines CiSmsStatus1-Symbols. Kann in der Kopfzeile des Symbols angezeigt werden. |   
| `pubPayload` | Payload, die beim Dr&uuml;cken des oberen Buttons des CiSmsStatus1-Symbols versendet wird. |   

## In `CiSmsStatus1Controller.ts` definierte Eigenschaften
Folgende Attribute sind zwingend anzugeben:   
| Eigenschaft       | Bedeutung                            |   
| ----------------- | ------------------------------------ |   
| `iSmsStatusState` | Status des CiSmsStatus1-Symbols. Als Vorgabe f&uuml;r den Start zB -99 verwenden. |   

Folgende Attribute sind optional und k&ouml;nnen verwendet werden:   
| Eigenschaft   | Bedeutung                            |   
| ------------- | ------------------------------------ |   
| `textUpper`   | Beschriftung des oberen Buttons.     |   
| `textMiddle`  | Beschriftung des mittleren Buttons.  |   
| `textLower`   | Beschriftung des unteren Buttons.    |   
| `pubTopic2`   | Topic, das beim Dr&uuml;cken des mittleren Buttons des CiSmsStatus1-Symbols versendet wird. |   
| `pubPayload2` | Payload, die beim Dr&uuml;cken des mittleren Buttons des CiSmsStatus1-Symbols versendet wird. |   
| `pubTopic3`   | Topic, das beim Dr&uuml;cken des unteren Buttons des CiSmsStatus1-Symbols versendet wird. |   
| `pubPayload3` | Payload, die beim Dr&uuml;cken des unteren Buttons des CiSmsStatus1-Symbols versendet wird. |   

## Beispiel: Codierung des `CiSmsStatus1Controller.ts`   
Im Normalfall m&uuml;ssen nur die Werte innerhalb des Bereichs `Array<SmsStatus1>` erg&auml;nzt werden.   

M&ouml;chte man zus&auml;tzliche MQTT-Funktionalit&auml;ten beim Empfang von Nachrichten implementieren, so kann der Code in der Methode `public onMessage (message: Message): void` entsprechend erg&auml;nzt werden.   

```
// ______CiSmsStatus1Controller.ts_______________khartinger_____
// 2023-03-12: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface SmsStatus1 extends IBase {
  // ---------mandatory-----------------------------------------
  iSmsStatusState: number;
  // ---------optional------------------------------------------
  textUpper?: string;
  textMiddle?: string;
  textLower?: string;
  textFooter?: string;
  pubTopic2?: string;
  pubPayload2?: string;
  pubTopic3?: string;
  pubPayload3?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiSmsStatus1Controller extends CiBaseController {
  public z2Mstatus1s: Array<SmsStatus1> = reactive(
    [
      {
        id: 'smsstatus1',
        name: 'Status GSM Modem',
        iSmsStatusState: -99,
        textUpper: 'Modem Status',
        textMiddle: 'SIM Status',
        textLower: 'GUTHABEN',
        subTopic: 'sms/cmd/ret/module sms/cmd/ret/sim sms/cmd/ret/credit',
        pubTopic: 'sms/cmd/get',
        pubPayload: 'module',
        pubTopic2: 'sms/cmd/get',
        pubPayload2: 'sim',
        pubTopic3: 'sms/cmd/get',
        pubPayload3: 'credit'
      }
    ]
  );

  // _________state constants___________________________________
  public maskSmsModule = 0x01;
  public maskSmsSim = 0x02;
  public maskSmsCredit = 0x04;
  public maskSmsValidModule = 0x10;
  public maskSmsValidSim = 0x20;
  public maskSmsValidCredit = 0x40;
  public maskSmsValidAny = 0x70;

  // _________react on incomming MQTT messages__________________
  public onMessage (message: Message): void {
    this.z2Mstatus1s.forEach(smsstatus1 => {
      const aSubTopic = smsstatus1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ========smsstatus1 found=============================
        if (smsstatus1.iSmsStatusState < 0) smsstatus1.iSmsStatusState = 0
        if (message.topic.includes('/ret/module')) {
          // ------module state---------------------------------
          try {
            // payload: {"device":"/dev/ttyUSB_Modem","module":true,"product":"SIM808 R14.18"}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.module) {
              smsstatus1.textFooter = aPayload.device
              smsstatus1.iSmsStatusState |= this.maskSmsModule
            } else {
              smsstatus1.textFooter = 'No device'
              smsstatus1.iSmsStatusState &= ~this.maskSmsModule
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidModule
            console.log('i_=', smsstatus1.iSmsStatusState, ' -t ', message.topic, 'pay: ', aPayload)
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidModule
          }
        }
        if (message.topic.includes('/ret/sim')) {
          // ------sim state------------------------------------
          try {
            // payload: {"provider":"T-Mobile A","sim":true,"result":"^~+COPS: 0,0,'T-Mobile A'^~^~OK^~"}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.sim) {
              smsstatus1.textFooter = aPayload.provider
              smsstatus1.iSmsStatusState |= this.maskSmsSim
            } else {
              smsstatus1.textFooter = 'No provider'
              smsstatus1.iSmsStatusState &= ~this.maskSmsSim
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidSim
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidSim
          }
        }
        if (message.topic.includes('/ret/credit')) {
          // ------sim state------------------------------------
          try {
            // payload: {"euro":33.25}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.euro >= 0) {
              smsstatus1.textFooter = `${aPayload.euro}`
              smsstatus1.iSmsStatusState |= this.maskSmsCredit
            } else {
              smsstatus1.textFooter = 'No Info'
              smsstatus1.iSmsStatusState &= ~this.maskSmsCredit
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidCredit
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidCredit
          }
        }
      } // ===END of smsstatus1 found===========================
    })
  }

  // _________publish a MQTT message____________________________
  public publishCi (topic: string, payload: string): void {
    // console.log('CiSmsStatus11Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSmsStatus1Controller: ERROR:', e) })
  }
}

export const ciSmsStatus1Controller = new CiSmsStatus1Controller()
```

[Zum Seitenanfang](#up)
