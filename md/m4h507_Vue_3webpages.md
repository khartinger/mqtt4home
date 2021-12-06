Letzte &Auml;nderung: 6.12.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Erstellen eines Vue Projekts mit mehreren Web-Seiten</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h507_Vue_2webpages_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
Dieses Vue-Projekt zeigt das Erstellen eines Web-Anwendung mit drei Seiten:   
1. Hauptseite ("Home")   
  Darstellung einer Lampe und eines Tasters, mit der die Lampe geschaltet werden kann.   
2. "History-Seite" ("Page2")   
  Darstellung der letzten f&uuml;nf empfangenen MQTT-Befehle.   
3. About-Seite (About")   
  Info &uuml;ber die Applikation.   

![Hauptseite Mehr-Webseiten-Projekt](./images/vue_mqtt3_3webpages_home.png "Hauptseite Zwei-Webseiten-Projekt")   
_Bild 1: Hauptseite des Mehr-Webseiten-Projektes (Home)_   

![History-Seite](./images/vue_mqtt3_3webpages_page2.png "History-Seite")   
_Bild 2: History-Seite (Page2)_   

![About-Seite](./images/vue_mqtt3_3webpages_about.png "History-Seite")   
_Bild 3: About-Seite (About)_   

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Hardware: Raspberry Pi (oder PC), auf dem ein MQTT-Broker l&auml;uft (zB Mosquitto)
* Software: Visual Studio Code ("VSC"), das f&uuml;r Vue-Anwendungen bereits vorbereitet ist.   
   (Dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.)   

Fertiges Projekt: [https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue_mqtt3_3webpages](https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue_mqtt3_3webpages)

# Erstellen des Projektes
## 1. &Uuml;bersicht &uuml;ber die beteiligten Dateien
![Uebersicht Dateien](./images/vue_mqtt3_3webpages_files.png "Uebersicht Dateien")   
_Bild 4: &Uuml;bersicht &uuml;ber die beteiligten Dateien_   

## 2. Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   
2. VSC: Terminal-Fenster &ouml;ffnen: Men&uuml; Terminal - New Terminal.   
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll:   
   `cd /g/github/mqtt4home/source_Vue`   
4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create vue_mqtt3_2webpages`  
   Mit Cursortasten, Leertaste und &lt;Enter&gt; Folgendes ausw&auml;hlen:   
   `> Manually select features` &nbsp; &lt;Enter&gt;   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `(*) Router`   
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
   `/github/mqtt4home/source_Vue/vue_mqtt3_2webpages` [Ordner ausw&auml;hlen]   
6. MQTT Bibliothek installieren:   
   VSC: Terminal-Fenster &ouml;ffnen: Men&uuml; Terminal - New Terminal.   
   `npm install mqtt --save`   
&nbsp;   
## 3. Anpassen der automatisch erstellten Dateien
### 3.1 Erg&auml;nzen der Vue-Konfiguration   
Erstellen der Datei `vue.config.js`: auf das Plus neben `VUE_MQTT3_2WEBPAGES` klicken, Namen eingeben.   
_Inhalt der Datei_:   
```   
module.exports = {
  lintOnSave: false,
  publicPath: './',
  configureWebpack: {
    devtool: 'source-map'
  }
}
```   

### 3.2 Linter-Warnung "Unexpected any" bei "(value: any)" abstellen    
  In der Datei `.eslintrc.js` unter "`rules: {`" erg&auml;nzen:   
  ```   
  '@typescript-eslint/no-explicit-any': 'off',
  '@typescript-eslint/explicit-module-boundary-types': 'off'
  ```   

### 3.3 Datei f&uuml;r die Seite `Page2` erstellen   
Mit der linken Maustaste auf `views/About.vou`klicken, Datei mit &lt;strg&gt; c und &lt;strg&gt; v kopieren und die Kopie auf `Page2.vue` umbenennen (zB mit dem Hilfsmen&uuml; mit der rechten Maustaste).   
In der Datei `Page2.vue` die Zeile `<h1>This is Page2</h1>` anpassen.   

### 3.4 Anpassen der "Routen" in der automatisch erstellten Datei `router/index.ts`:   
```   
// ___________router/index.ts___________________________________
import { createRouter, createWebHashHistory, RouteRecordRaw } from 'vue-router'
import Home from '../views/Home.vue'
import Page2 from '../views/Page2.vue'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'Home',
    component: Home,
    meta: {
      title: 'vue_mqtt3_2webpages-Home'
    }
  },
  {
    path: '/page2',
    name: 'Page2',
    component: Page2,
    meta: {
      title: 'vue_mqtt3_2webpages-Home'
    }
  },
  {
    path: '/about',
    name: 'About',
    // route level code-splitting
    // this generates a separate chunk (about.[hash].js) for this route
    // which is lazy-loaded when the route is visited.
    component: () => import(/* webpackChunkName: "about" */ '../views/About.vue'),
    meta: {
      title: 'vue_mqtt3_2webpages-About'
    }
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

router.beforeEach((to, from, next) => {
  document.title = <string> to.meta.title
  next()
})

export default router

```   

### 3.5 Datei `App.vue` anpassen   
Die Datei `App.vue` ist f&uuml;r folgende Punkte zust&auml;ndig:   
  * Darstellung einer Linkzeile mit Links auf die einzelnen Seiten.   
    Daher: Die Zeile `<router-link to="/page2">Page2</router-link> |` erg&auml;nzen.   
  * Herstellung der Verbindung zum Broker.   
    Dies sollte nicht in der Seite "Home" erfolgen, da sonst bei jedem Laden der Seite die Verbindung neu hergestellt wird.   
    Der Zustand der Verbindung wird in der Linkzeile angezeigt.   
  * Definition von einheitlichen Styles f&uuml;r alle Seiten.   
    Daher: Die Styles mit Punkt erg&auml;nzen.   

_Inhalt der Datei_:   
```   
<!--App.vue-->
<template>
  <div id="nav">
    <router-link to="/">Home</router-link> |
    <router-link to="/page2">Page2</router-link> |
    <router-link to="/about">About</router-link>
    &nbsp; &nbsp; &nbsp;
    <span v-if="isConnected">
      <b><font color="lightgreen">Connected to {{ getConnectUrl }}</font></b>
    </span>
    <span v-if="!isConnected && !isConnecting">
      <b><font color="red"> Not connected! &nbsp; </font></b>
      <button @click="connect()">Connect</button>
    </span>
    <span v-if=isConnecting>
      <b><font color="red"> Is connecting... &nbsp; </font></b>
      &nbsp; <button @click="cancel()">Cancel</button>
    </span>
  <router-view/>
</div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

// ------MQTT broker parameter----------------------------------
const hostip = '10.1.1.1'
const hostport = 1884
const subtopic = '#'

export default defineComponent({
  name: 'Home',
  components: {
  },
  mounted: async function (): Promise<void> {
    mqttClientInstance.connect(hostip, hostport, '')
    mqttClientInstance.subscribe(subtopic, 0)
  },
  computed: {
    isConnected: function (): boolean {
      return mqttClientInstance.mqttState.connected
    },
    isConnecting: function (): boolean {
      if (mqttClientInstance.mqttState.iConnMqttState === 2) return true
      return false
    },
    isSubscribed: function (): boolean {
      return mqttClientInstance.mqttSubscription.subscribed
    },
    getMqttState: function (): string {
      return mqttClientInstance.sConnMqttState()
    },
    getConnectUrl: function (): string {
      return mqttClientInstance.connectUrl()
    }
  },
  methods: {
    connect: async function (): Promise<void> {
      if (!this.isConnected) {
        mqttClientInstance.connect(hostip, hostport, '')
        mqttClientInstance.subscribe(subtopic, 0)
      }
    },
    end: async function (): Promise<void> {
      mqttClientInstance.disconnect()
    },
    cancel: async function (): Promise<void> {
      mqttClientInstance.disconnect()
    }
  }
})

</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

#nav {
  padding: 3px;
}

#nav a {
  font-weight: bold;
  color: #2c3e50;
}

#nav a.router-link-exact-active {
  color: #42b983;
}

  .fontOK    { font-weight: bold; fill: lightgreen; }
  .fontNOK   { font-weight: bold; fill: red; }

  .ciFont1   { font-size: 11px; font-weight: bold; font-family: monospace; fill: black; white-space: pre; }
  .cursor    { cursor: pointer; }
  .ciBackground {fill: #ddFFdd; }
  .ciOut     { fill: yellow; stroke: yellow; stroke-width: 1; }
  .ciInColor { fill: #F4F4F4; }
  .ciIn      { fill: #F4F4F4; stroke:blue; stroke-width: 1; }
  .ciLine    { stroke: blue; stroke-width: 1; }
  .ciRect    { fill: none; stroke: blue; stroke-width: 1; }
  .ciFillIt  { stroke: blue; stroke-width: 1; }
  .ciClick   { fill-opacity: 0.0; stroke-width: 1; }
  .ciBorder  { fill-opacity: 0.0; stroke: blue; stroke-width: 1; }
</style>
```   

### 3.6 Nicht ben&ouml;tigte Dateien und Verzeichnisse l&ouml;schen   
  * Datei `components/HelloWorld.vue` l&ouml;schen   
  * Verzeichnis `assets` l&ouml;schen   

## 4. MQTT Client erg&auml;nzen
Verzeichnis `src/services` erstellen, die Dateien `MqttClient.ts` und `MqttClientInstance.ts` erg&auml;nzen.   
Erkl&auml;rungen siehe "Teil 1: Erstellung des MQTT Clients" in [m4h504_Vue_PubSub2.md](m4h504_Vue_PubSub2.md)

## 
## 6. Home-Seite erstellen
Die Startseite ("Home") enth&auml;lt die beiden Steuerungs-/Anzeige-Elemente (CI-Elemente) "Lamp" und "Button2", die mit Hilfe folgender Dateien erzeugt werden:   
* [`components/CiBase.vue ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/components/CiBase.vue) Definition von Geometrie-Daten (Klasse Geo) sowie Darstellung des Randes der CI-Elemente.   
* [`controller/CiBaseController.ts ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/controller/CiBaseController.ts) Definition der Basis-Eigenschaften f&uuml;r alle CI-Elemente (Interface IBase) sowie eines abstrakten Basis-Controllers (CiBaseController) mit der Definition der f&uuml;r alle Controller erforderlichen Methoden `registerClient`, `publish` und `onMessage`.   
* [`components/CiLamp.vue ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/components/CiLamp.vue) Darstellung des Lampensymbols und Realisierung der Funktion `onClk`, die beim Klicken auf das Symbol ausgef&uuml;hrt wird.   
* [`controller/CiLampController.ts ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/controller/CiLampController.ts) Definition der Lampen-Eigenschaften, Realisierung der Methoden `onMessage` und `publish` (`publishCi`) sowie Erstellen eines Objekts `ciLampController`.   
* [`components/CiButton2.vue ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/components/CiButton2.vue) Darstellung des Schaltersymbols und Realisierung der Funktionen `onClkON` und `onClkOff`, die beim Klicken auf die Bereiche "ON" und "OFF" des Symbols ausgef&uuml;hrt werden.   
* [`controller/CiButton2Controller.ts ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/controller/CiButton2Controller.ts) Definition der Schalter-Eigenschaften, Realisierung der Methoden `onMessage` und `publish` (`publishCi`) sowie Erstellen eines Objekts `ciButton2Controller`.   

Diese Elemente werden auf folgende Weise in die Seite `views/Home.vue` eingebaut:   
```   
<!--src/views/Home.vue-->
<template>
    <svg viewBox="0 0 230 120" width="100%">
    <rect class="ciBackground" x="0" y="0" width="230" height="120" />
    <CiLamp    :x="60" :y="60" sid="lamp_1" :border="0"></CiLamp>
    <CiButton2 :x="170" :y="60" sid="button2_1" lines="0" :border="0"></CiButton2>
    </svg>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import CiLamp from '@/components/CiLamp.vue'
import CiButton2 from '@/components/CiButton2.vue'

export default defineComponent({
  name: 'Home',
  components: {
    CiLamp,
    CiButton2
  }
})
</script>
```   
Die Lampe ben&ouml;tigt als Parameter den Mittelpunkt x und y und die ID. Mit `:border="0"` wird das Zeichnen eines Randes verhindert. Da die Anzahl der Zeilen (`lines=`) nicht angegeben ist, wird die Vorgabe `lines="1"` verwendet und der Name der Lampe angezeigt.   
Der Schalter ben&ouml;tigt als Parameter den Mittelpunkt x und y und die ID. Mit `:border="0"` wird das Zeichnen eines Randes und mit `lines="0"` wird die Anzeige weiterer Informationen verhindert.   

## 7. Seite "Page2" erstellen
Die Seite "Page2" nutzt die Komponente `MqttLastX` zum Anzeigen der letzten f&uuml;nf empfangenen MQTT-Nachrichten:   
* [`components/MqttLastX.vue ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/components/MqttLastX.vue) Darstellung der Tabelle der letzten Nachrichten.   
* [`controller/MqttLastXController.ts ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/controller/MqttLastXController.ts) Definition der Klasse MqttLastXController, die mit Hilfe der Methode `onMessage` die empfangene Nachricht in den Speicher MessageStore schreibt.   
* [`store/MessageStore.ts ... `](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt3_3webpages/src/store/MessageStore.ts) Kodierung und Export des Speichers `messages` sowie der Methoden `addMessage` und `clearMessage`.   
Diese Elemente werden auf folgende Weise in die Seite `views/Page2.vue` eingebaut:   
```   
<!--views/Page2.vue-->
<template>
  <div class="about">
    <h2>Page2: Last messages received</h2>
    <MqttLastX></MqttLastX>
    <hr>
  </div>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import MqttLastX from '@/components/MqttLastX.vue'
export default defineComponent({
  name: 'Home',
  components: {
    MqttLastX
  }
})
</script>
```   

## 8. Seite "About" erstellen
Die "About"-Seite gibt Text aus und zeigt, wie man innerhalb einer SVG-Grafik einen Link auf eine andere Seite erzeugt.   
```   
<template>
  <div class="about">
    <h2>About: vue_mqtt3_3webpages</h2>
    Purpose: This MQTT sample project shows how a Vue app with multiple
    pages is built.<br>
    Version: 2021-12-05<br><br>
  </div>
  <svg viewBox="0 0 230 36" width="100%">
  <rect class="ciBackground" x="0" y="0" width="230" height="36" />
  <rect @click="onClk()" x="65" y="3" rx=6 ry=6 width="100" height="30" fill="#FFF831" stroke="gold" stroke-width="4"/>
  <text @click="onClk()" x="100" y="27" textLength="30" lengthAdjust="spacingAndGlyphs" class="fontAbout">OK</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
export default defineComponent({
  components: {
  },
  methods: {
    onClk: function () {
      console.log('CLICK - About: onClk!', '')
      this.$router.push({name: 'Home'})
    }
  }
})
</script>

<style scoped>
 .fontAbout { font: bold 24px sans-serif; fill: red; cursor: pointer; }
</style>
```   
