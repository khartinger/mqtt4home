Letzte &Auml;nderung: 5.11.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Vue: Schalten von Lampen mit MQTT</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h506_Vue_Mqtt2_Lamp_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
Erstellen einer Vue-Anwendung, mit der der Zustand zweier Lampen angezeigt wird und die &uuml;ber ein Taster-Symbol ausgeschaltet werden k&ouml;nnen:   
![MQTT-Lampendarstellung](./images/211105_vue_mqtt2_start.png "MQTT-Lampendarstellung")   
_Bild 1: Browser-Darstellung der beiden Lampen und des Tasters_   

Das _Bild 1_ zeigt einige M&ouml;glichkeiten des Programms:   
* Das gesamte GUI ist eine svg-Grafik.
* Steuer- oder Anzeigesymbole sind quadratisch und gleich gro&szlig;.
* Symbole k&ouml;nnen mit oder ohne Rand dargestellt werden.
* Symbole k&ouml;nnen mit oder ohne Text dargestellt werden.
* Eine gelbe Lampe bedeutet, die Lampe ist eingeschaltet, grau bedeutet ausgeschaltet und blau bedeutet, der Zustand ist nicht bekannt.
* Durch Dr&uuml;cken des Tasters rechts werden beide Lampen ausgeschaltet.

## Bestandteile des Programms
Damit das Programm &uuml;bersichtlich und leicht erweiterbar ist, wird es aus zahlreichen Einzelteilen zusammengesetzt.   
Das folgende Bild gibt einen &Uuml;berblick &uuml;ber die beteiligten Dateien:
![Dateien des Projekts MQTT-Lampen](./images/vue_mqtt2_lamp1_files.png "Dateien des Projekts MQTT-Lampen")   
_Bild 2: Dateien des Projekts MQTT-Lampen_   

### Bedeutung der einzelnen Dateien   
Der rechte Zweig mit (Browser -->) `index.html` --> `main.ts` --> `App.vue` wird automatisch erstellt und muss etwas angepasst werden.   
   * Die Datei `main.ts` erzeugt das App-Element, das in `index.html` angezeigt wird.   
   * `App.vue` stellt `<CiMain>` dar und enth&auml;lt css-Elemente, die im gesamten Projekt verwendet werden k&ouml;nnen.   
---   
Der linke Teil der Darstellung stellt das eigentliche Programm dar:   
* Die Datei `MqttClient.ts` enth&auml;lt die Klasse `MqttClient`, die alle Methoden zur Kommunikation mit dem MQTT-Broker enth&auml;lt.   
* Die Datei `MqttClientInstance.ts` erzeugt das Objekt `mqttClientInstance`, bei dem sich jeder Controller registrieren muss, der MQTT-Nachrichten erhalten will.   
Symbolisch ist dies im _Bild 2_ durch die Schalter dargestellt: Keine Registrierung bedeutet, keine Zusendung von MQTT-Nacchrichten.   
* F&uuml;r jeden Symbol-Typ (hier Lamp und Button) gibt es einen eigenen Controller, der von "`BaseController.ts`" abgeleitet ist.   
* Jeder Controller enth&auml;lt eine Liste (genauer "`Array`") seiner Symbole sowie die Methoden zum Empfangen und Senden von MQTT-Nachrichten (`onMessage` und `publishCi`)
* Die grafische Gestaltung jedes Symbols erfolgt in einer vue-Datei, die die Basis-Vue `Base.vue` einbindet (`CiLamp.vue` und `CiButton.vue`).   
* Alle angezeigten Symbol-Vues werden in `CiMain.vue` positioniert.   
* In `CiMain.vue` wird mit Hilfe des Objekts `mqttClientInstance` die Verbindung zum Broker hergestellt (zB Host 10.1.1.1, Port 1884, Subscribe-Topics -t "#" (=alle)).

[Link zum Projekt](https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue_mqtt2_lamp)   

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Hardware: Raspberry Pi (oder PC), auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)
* Software: Visual Studio Code ("VSC"), das f&uuml;r Vue-Anwendungen bereits vorbereitet ist.   
   (Dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.)   
   incl. node.js, npm

## Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   
2. VSC: Terminal-Fenster &ouml;ffnen: Men&uuml; Terminal - New Terminal.   
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll:   
   `cd /g/github/mqtt4home/source_Vue`   
4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create vue_mqtt2_lamp`  
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
Der Inhalt f&uuml;r diese Datei findet sich auf [https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt2_lamp/src/App.vue](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt2_lamp/src/App.vue)   

3. Erstellen der Vue-Konfig-Datei   
   Men&uuml; Anzeigen - Explorer. Mit der Maus auf in die Zeile `VUE_MQTT2_LAMP` gehen, das Symbol "+ Neue Datei" anklicken und  `vue.config.js` eingeben.   
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
## 2. MQTT Client hinzuf&uuml;gen
Der MQTT Client ist der gleiche wie im Beispiel
"[Vue: Senden und Empfangen von MQTT-Nachrichten](m4h505_Vue_Mqtt1.md)":   
1. Anlegen des Verzeichnisses `src/services`:   
   Men&uuml; Anzeigen - Explorer. Mit der rechten Maustaste auf `src` klicken - "Neuer Ordner" w&auml;hlen und `services` eingeben.   
2. Die Datei `MqttClient.ts` anlegen:   
   Men&uuml; Anzeigen - Explorer. Mit der rechten Maustaste auf `services` klicken - "Neue Datei" w&auml;hlen und `MqttClient.ts` eingeben.   
   Der Inhalt f&uuml;r diese Datei findet sich auf auf [`https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_pubsub2/src/services/MqttClient.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_pubsub2/src/services/MqttClient.ts)   
3. Die Datei `MqttClientInstance.ts` anlegen:   
   Men&uuml; Anzeigen - Explorer. Mit der rechten Maustaste auf `services` klicken - "Neue Datei" w&auml;hlen und `MqttClientInstance.ts` eingeben.   
   Der Inhalt f&uuml;r diese Datei findet sich auf auf [`https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_pubsub2/src/services/MqttClientInstance.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_pubsub2/src/services/MqttClientInstance.ts)   

## 3. Erstellen des Basis-Controllers
* Anlegen des Verzeichnisses `controller`:   
   Men&uuml; Anzeigen - Explorer. Mit der rechten Maustaste auf `src` klicken - "Neuer Ordner" w&auml;hlen und `controller` eingeben.   
* Die Datei `BaseController.ts` anlegen:   
   Men&uuml; Anzeigen - Explorer. Mit der rechten Maustaste auf `controller` klicken - "Neue Datei" w&auml;hlen und `BaseController.ts` eingeben.   
   Inhalt der Datei: siehe [https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt2_lamp/src/controller/CiBaseController.ts](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt2_lamp/src/controller/CiBaseController.ts)

..ToDo..
