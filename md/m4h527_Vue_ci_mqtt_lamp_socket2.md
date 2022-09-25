Letzte &Auml;nderung: 25.09.2022 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Vue: Schalten einer Lampe mit Steckdosen-Symbol</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="./m4h527_Vue_ci_mqtt_lamp_socket2_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue27_ci_mqtt_lamp_socket2">==> Code @ GitHub</a><hr>
  
# Ziel
Erstellen einer Vue-Anwendung, mit der der Zustand einer Lampe angezeigt wird und die &uuml;ber ein Steckdosen-Symbol ein- und ausgeschaltet werden kann:   

![MQTT-Lampendarstellung](./images/vue27_ci_mqtt_lamp_socket2_view_1.png "MQTT-Lampendarstellung")   
_Bild 1: Browser-Darstellung der Lampe und der Schalt-Steckdose_   

Das _Bild 1_ zeigt einige M&ouml;glichkeiten des Programms:   
* Das gesamte GUI ist eine svg-Grafik.
* Steuer- oder Anzeigesymbole sind quadratisch und gleich gro&szlig;.
* Symbole k&ouml;nnen mit oder ohne Rand dargestellt werden.
* Symbole k&ouml;nnen mit oder ohne Text dargestellt werden.
* Klickt man auf die Lampe, wird ihr Zustand umgeschaltet und es werden folgende MQTT-Nachrichten ausgesendet:   
   `ci/lamp/1/ret/lamp -1`   
   `return_lamp_state... -1`   
   Dabei bedeutet die Payload -1 den Zustand "unbekannt", 0 "aus" und 1 "ein".   
* Eine gelbe Lampe bedeutet, die Lampe ist eingeschaltet, grau bedeutet ausgeschaltet und blau bedeutet, der Zustand ist nicht bekannt.
* Durch Dr&uuml;cken des Steckdosensymbols (rechts) kann die Lampe ein- oder ausgeschaltet werden. Dazu wird folgende Nachricht versendet:   
   `ci/lamp/1/set/lamp 1`   
   Die Lampe antwortet dann mit   
   `ci/lamp/1/ret/lamp 1`   
   `return_lamp_state... 1`   

# Bestandteile des Programms
Damit das Programm &uuml;bersichtlich und leicht erweiterbar ist, wird es aus zahlreichen Einzelteilen zusammengesetzt.   
Das folgende Bild gibt einen &Uuml;berblick &uuml;ber die beteiligten Dateien:   
![Dateien des Projekts MQTT-Lampen](./images/vue27_ci_mqtt_lamp_socket2_files_1.png "Dateien des Projekts MQTT-Lampen")   
_Bild 2: Dateien des Projekts MQTT-Lampen_   

## Bedeutung der einzelnen Dateien   
Der rechte Zweig mit (Browser -->) `index.html` --> `main.ts` --> `App.vue` wird automatisch erstellt und muss etwas angepasst werden.   
   * Die Datei `main.ts` erzeugt das App-Element, das in `index.html` angezeigt wird.   
   * `App.vue` stellt `<CiMain>` dar und enth&auml;lt css-Elemente, die im gesamten Projekt verwendet werden k&ouml;nnen.   
---   
Der linke Teil der Darstellung stellt das eigentliche Programm dar:   
* Die Datei `CiMqttClient.ts` enth&auml;lt die Klasse `CiMqttClient`, die alle Methoden zur Kommunikation mit dem MQTT-Broker enth&auml;lt.   
* Die Datei `CiMqttClientInstance.ts` erzeugt das Objekt `ciMqttClientInstance`, bei dem sich jeder Controller registrieren muss, der MQTT-Nachrichten erhalten will.   
Symbolisch ist dies im _Bild 2_ durch die Schalter dargestellt: Keine Registrierung bedeutet, keine Zusendung von MQTT-Nacchrichten.   
* F&uuml;r jeden Symbol-Typ (hier CiLamp und CiSocket2) gibt es einen eigenen Controller, der von "`CiBaseController.ts`" abgeleitet ist.   
* Jeder Controller enth&auml;lt eine Liste (genauer "`Array`") seiner Symbole sowie die Methoden zum Empfangen und Senden von MQTT-Nachrichten (`onMessage` und `publishCi`)
* Die grafische Gestaltung jedes Symbols erfolgt in einer vue-Datei, die die Basis-Vue `CiBase.vue` einbindet (`CiLamp.vue` und `CiSocket2.vue`).   
* Alle angezeigten Symbol-Vues werden in `CiMain.vue` positioniert.   

# Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Hardware: Raspberry Pi (oder PC), auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)
* Software: Visual Studio Code ("VSC"), das f&uuml;r Vue-Anwendungen bereits vorbereitet ist.   
   (Dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.)   
   incl. node.js, npm

# Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   
2. VSC: Terminal-Fenster &ouml;ffnen: Men&uuml; Terminal - New Terminal.   
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll:   
   `cd /github/mqtt4home/source_Vue`   
4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create vue27_ci_mqtt_lamp_socket2`  
   Mit Cursortasten, Leertaste und &lt;Enter&gt; Folgendes ausw&auml;hlen:   
   `> Manually select features` &nbsp; &lt;Enter&gt;   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `( ) Router`   
   `(*) Linter / Formatter`   
   &lt;Enter&gt;   
   _`> 3.x`_ &nbsp; &lt;Enter&gt;      
   _`? Use class-style component syntax?`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)?`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Use history mode for router? (Requires proper server setup for index fallback in production)`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Pick a linter / formatter config:`_ &nbsp; __`ESLint + Standard config`__ &lt;Enter&gt;   
   _`? Pick additional lint features: `_ &nbsp; __`Lint on save`__ &lt;Enter&gt;   
   _`? Where do you prefer placing config for Babel, ESLint, etc.?`_  &nbsp; __`In dedicated config file`__ &lt;Enter&gt;   
   _`? Save this as a preset for future projects? (y/N)`_ &nbsp; __`N`__ &lt;Enter&gt;   
5. In den Projektordner wechseln: _VSC Men&uuml; Datei - Ordner &ouml;ffnen_..
6. MQTT Bibliothek installieren:   
   VSC: Terminal-Fenster &ouml;ffnen: Men&uuml; Terminal - New Terminal.   
   `npm install mqtt --save`   

## 1. Allgemeine Anpassungen im Projekt
1. Der Inhalt der automatisch erstellten Datei `main.ts` sollte durch folgende Zeilen ersetzt werden:   
```   
// ______main.ts________________________________________________
import { createApp, h } from 'vue'
import App from './App.vue'

const app = createApp({
  render: () => h(App)
})

app.mount('#app')

```   

2. In der Datei `App.vue` wird die Komponente `ciMain` aufgerufen und verschiedene Stile definiert.   
Der Inhalt f&uuml;r diese Datei findet sich auf [https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/App.vue](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/App.vue)   

3. Erstellen der Vue-Konfig-Datei   
   Men&uuml; Anzeigen - Explorer. Mit der Maus auf in die Zeile `vue27_ci_mqtt_lamp_socket2` gehen, das Symbol "+ Neue Datei" anklicken und  `vue.config.js` eingeben.   
   Inhalt der Datei:   
   ```   
   module.exports = {
     lintOnSave: false,
     publicPath: './',
     configureWebpack: {
       devtool: 'source-map'
     }
   }

   ```   
4. Erg&auml;nzung in der Datei `.eslintrc.js`   
Im Abschnitt `rules: {` einf&uuml;gen:
```   
    '@typescript-eslint/no-explicit-any': 'off',
    '@typescript-eslint/explicit-module-boundary-types': 'off',
    '@typescript-eslint/no-multi-spaces': 'off',
```   

5. Weiters k&ouml;nnen  die automatisch erstellten Dateien `components/HelloWord.vue` und `assets/logo.png` gel&ouml;scht werden.

<a name="mqtt-function"></a>

# MQTT-Funktionalit&auml;t einbauen
## Einbinden der erforderlichen Dateien
* Erstellen des Verzeichnisses "controller"   
  Mit der rechten Maustaste auf das Verzeichnis `src` klicken, "Neuer Ordner" w&auml;hlen und den Namen `controller` eingeben.   
* Erstellen des Verzeichnisses "services"   
  Mit der rechten Maustaste auf das Verzeichnis `src` klicken, "Neuer Ordner" w&auml;hlen und den Namen `services` eingeben.   
* Erstellen der Datei `CiMqttClient.ts`   
  * Mit der rechten Maustaste auf das Verzeichnis `services` klicken, "Neue Datei" w&auml;hlen und den Namen `CiMqttClient.ts` eingeben.   
  * Inhalt der Datei zB von [GitHub holen](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue10_ci_mqtt_mini/src/services/CiMqttClient.ts), hineinkopieren und Datei speichern.   
* Erstellen der Datei `CiBaseController.ts`   
  * Mit der rechten Maustaste auf das Verzeichnis `controller` klicken, "Neue Datei" w&auml;hlen und den Namen `CiBaseController.ts` eingeben.   
  * Inhalt der Datei zB von [GitHub holen](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue10_ci_mqtt_mini/src/controller/CiBaseController.ts), hineinkopieren und Datei speichern.   
* Erstellen der Datei `CiBase.vue`   
  * Mit der rechten Maustaste auf das Verzeichnis `components` klicken, "Neue Datei" w&auml;hlen und den Namen `CiBase.vue` eingeben.   
  * Inhalt der Datei zB von [GitHub holen](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue10_ci_mqtt_mini/src/components/CiBase.vue), hineinkopieren und Datei speichern.   

## Erstellen der Datei "services/CiMqttClientInstance"
* Mit der rechten Maustaste auf das Verzeichnis `services` klicken, "Neue Datei" w&auml;hlen und den Namen `CiMqttClientInstance.ts` eingeben.   
* Festlegen, dass beim Start der App eine Verbindung zum Broker hergestellt werden soll (Konstruktor-Wert `true`).   
* Da lediglich die Komponente zum Empfangen (und Speichern) Zugriff auf die MQTT-Nachrichten ben&ouml;tigt, braucht auch nur diese Komponente registriert werden.   

_Ergebnis:_   
```   
// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciSocket2Controller } from '@/controller/CiSocket2Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciSocket2Controller)
```   

# Das CiBase-Element
Das CiBase-Element ist das Basiselement f&uuml;r alle Steuer-/Anzeige-Symbole (kurz "CI-Symbole") und besteht aus einem Anzeigeteil `CiBase.vue` und einem Steuerteil `CiBaseController.ts`.

## components/CiBase.vue
Die Basis-Ansicht `CiBase.vue` ist f&uuml;r die Darstellung von Symbolen zust&auml;ndig:   
1. Zeichnen eines Rahmens um das Symbol oder nicht (`border = 0 | 1 | 2 | 3`)   
   0 ... kein Rahmen   
   1 ... &auml;u&szlig;erer blauer Rahmen   
   2 ... innerer blauer Rahmen   
   3 ... innerer blauer Rahmen mit gelbem &auml;u&szlig;eren Rand   
2. Bereitstellung von Geometriedaten zum Zeichnen eines Symbols. Diese werden von der Klasse `Geo` zur Verf&uuml;gung gestellt.    

![Geometrie-Daten](./images/geo_CI_Symbol_1.png "Geometriedaten")   
_Bild 3: Geometriedaten eines CI-Symbols_   

_Codierung_: [https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/components/CiBase.vue](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/components/CiBase.vue)   

## controller/CiBaseController.ts
Die Datei `CiBaseController.ts` muss im Normalfall nicht ver&auml;ndert werden. Sie definiert im Interface `IBase` einige Eigenschaften, die alle (abgeleiteten) `CiXxxController` haben sollten. Die wichtigste Eigenschaft ist dabei die `id`, die die Verbindung zwischen einer Darstellung und den Daten im Controller darstellt.   

In der (abstrakten) Klasse `CiBaseController` werden dann die Methoden `registerClient`, `publish` und `onMessage(message: Message)` definiert.   

_Codierung_: [https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/controller/CiBaseController.ts](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue27_ci_mqtt_lamp_socket2/src/controller/CiBaseController.ts)   

# Lampen

## Schritt 1: Definition der Eigenschaften des Lampensymbols
Jede Lampe muss im Lampencontroller (Datei `controller/CiLampController.ts`) definiert werden:   

```   
public lamps: Array<Lamp> = reactive(
    [
      {
        id: 'lamp1',
        name: 'Lamp_1',
        iLampState: -1,
        text5: 'Text_5',
        // subTopic: 'ci/lamp/1/ret/lamp',
        subTopic: 'ci/lamp/1/set/lamp',
        pubTopic: 'ci/lamp/1/ret/lamp return_lamp_state...',
        pubPayload: '1'
      }
    ]
  );
```   

## Schritt 2: Darstellung des Lampensymbols durch die Vue-Datei `components/CiMain.vue`
Die Darstellung des Lampensymbols erfolgt im `<template>`-Bereich der Vue-Datei:   
`<CiLamp :x="50" :y="50" sid="lamp1" :border="0"></CiLamp>`   

Weiters sind im `<script>`-Bereich folgende zwei Zeilen erforderlich:   
`import CiLamp from './CiLamp.vue'`   
Im Bereich "defineComponent -  components: {":   
`    CiLamp,`   

## Schritt 3: Registierung des Lampen-Controllers zum Empfang von MQTT-Nachrichten
In der Datei `services/CiMqttClientInstance.ts` folgende zwei Zeilen erg&auml;nzen:   
`import { ciLampController } from '@/controller/CiLampController'`   
`ciMqttClientInstance.registerController(ciLampController)`   

# Steckdosen-Symbol (Socket2)
## Schritt 1: Definition der Eigenschaften des Steckdosen-Symbols
Jedes Steckdosen-Symbol muss im Steckdosen-Controller (Datei `controller/CiSocket2Controller.ts`) definiert werden:   

```   
  public sockets: Array<Socket> = reactive(
    [
      {
        type: 'test', // delock11826, e1603=IKEA
        id: 'socket2',
        name: 's2',
        iSocketState: -9, // 0x9999FF,
        battery: '-',
        text5: 'text5',
        subTopic: 'ci/lamp/1/ret/lamp',
        pubTopic: 'ci/lamp/1/set/lamp',
        pubPayload: '0',
        pubPayloadOn: '1'
      }
    ]
  );

```   

## Schritt 2: Darstellung des Steckdosen-Symbols durch die Vue-Datei `components/CiMain.vue`
Die Darstellung des Steckdosen-Symbols erfolgt im `<template>`-Bereich der Vue-Datei:   
`<CiSocket2 :x="150" :y="50" sid="socket2" :border="0"></CiSocket2>`   

Weiters sind im `<script>`-Bereich folgende zwei Zeilen erforderlich:   
`import CiButton from './CiSocket2.vue'`   
Im Bereich "defineComponent -  components: {":   
`    CiSocket2`   

## Schritt 3: Registierung des Controllers zum Empfang von MQTT-Nachrichten
In der Datei `services/CiMqttClientInstance.ts` folgende zwei Zeilen erg&auml;nzen:   
`import { ciSocket2Controller } from '@/controller/CiSocket2Controller'`   
`ciMqttClientInstance.registerController(ciSocket2Controller)`   

[Zum Seitenanfang](#up)
