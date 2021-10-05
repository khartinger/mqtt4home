Letzte &Auml;nderung: 3.10.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Vue: Senden und Empfangen von MQTT-Nachrichten</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h23_Vue_Mqtt1_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
* Erstellen einer Vue-Anwendung, mit der MQTT-Nachrichten gesendet und abonniert bzw. angezeigt werden k&ouml;nnen:   
![MQTT-Sende-/Empfangsprogramm](./images/211003_vue_mqtt1_start.png "MQTT-Sende-/Empfangsprogramm")   
_Bild 1: Startseite des MQTT-Sende- und Empfangsprogramms_   

__Das Programm soll aus f&uuml;nf Teilen bestehen:__   
1. Ein MQTT Client, der die MQTT Funktionen zur Verf&uuml;gung stellt.   
2. Eine GUI-Komponente, mit der man sich mit einem Broker verbinden kann.   
   Dabei kann der Host, auf dem der Broker l&auml;uft (zB IP 10.1.1.1) und der Port f&uuml;r die WebSocket-Verbindung (zB 1884) in Eingabefeldern eingegeben werden.   
3. Eine GUI-Komponente, mit deren Hilfe man eine Nachricht versenden kann.   
   Es ist die Eingabe von Topic, Payload, ob die Nachricht gespeichert werden soll ("retain") und vom QoS-Level (Quality of Service) m&ouml;glich.   
4. Eine GUI-Komponente, mit der man ein Topic abonnieren ("subscribe") kann.   
5. Eine GUI-Komponente, die f&uuml;r das abonnierte Topic die letzten 5 Nachrichten anzeigt.   

_F&uuml;r Ungeduldige_: [Link zum fertigen Programm](https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue_mqtt1)

## Voraussetzungen
* Etwas Wissen &uuml;ber HTML, CSS und JavaScript/Typescript :)   
* Visual Studio Code, das f&uuml;r Vue-Anwendungen bereits vorbereitet ist.   
   (Dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.)   

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Software: Visual Studio Code, node.js, npm

## Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   
2. VSC: Terminal-Fenster &ouml;ffnen: `Men&uuml; Terminal - New Terminal`.   
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll: `cd /g/github/mqtt4home/source_Vue`   
4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create mqtt1`  
   Mit Cursortasten, Leertaste und &lt;Enter&gt; Folgendes ausw&auml;hlen:   
   `> Manually select features`   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `(*) Router`   
   `(*) Linter / Formatter`   
   `> 3.x`   
   `? Use class-style component syntax? No`   
   `? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)? No`   
   `? Use history mode for router? (Requires proper server setup for index fallback in production) No`   
   `? Pick a linter / formatter config: ESLint + Standard config`   
   `? Pick additional lint features: Lint on save`   
   `? Where do you prefer placing config for Babel, ESLint, etc.? In dedicated config file`   
   `? Save this as a preset for future projects? (y/N) N`   
5. In den Projektordner wechseln: VSC Men&uuml; Datei - Ordner &ouml;ffnen..
6. MQTT Bibliothek installieren: `npm install mqtt --save`   

## 1. Erstellung des MQTT Clients
Der MQTT Client besteht aus zwei Dateien, die sich im Verzeichnis `services` befinden.   
1. Die Datei `MqttClient.ts` enth&auml;lt die vier Interfaces `Message`, `MqttState`, `MqttConnection`, `MqttSubscription` sowie die Klasse `MqttClient`.   
Die Interfaces werden in der Klasse `MqttClient` dazu verwendet, Informationen zum Verbindungs- und Abonnierungszustand zu speichern. Weiters werden Methoden f&uuml;r folgende Zwecke zur Verf&uuml;gung gestellt:   
   * `connectUrl ........` Zusammenbau der Verbindungs-URL aus Host, Port und MQTT-Endpunkt   
   * `connect ...........` Aufbau der Verbindung zum Broker   
   * `disconnect ........` L&ouml;sen der Verbindung zum Broker   
   * `subscribe .........` Abonnieren eines Topics   
   * `unsubscribe .......` L&ouml;sen des Abonnieren eines Topics   
   * `publish ...........` Ver&ouml;ffentlichen einer Nachricht   
   * `sConnMqttState ....` Verbindungszustand als Text ("connected" etc.)   
   * `hostSubscribe .....` Verbinden mit dem angegebenen Broker und abonnieren des angegebenen Topics   
   * `registerController ` Anmeldung eines Controllers, damit er Nachrichten weitergeleitet bekommt (und in der Methode `onMessage` auswerten kann).   

   Codierung der Datei [`MqttClient.ts` siehe `https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt1/src/services/MqttClient.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt1/src/services/MqttClient.ts)


2. Die Datei `MqttClientInstance.ts` erzeugt und exportiert das Verbindungsobjekt `mqttClientInstance`.   
In dieser Datei können alle MQTT-Controller registriert werden, die MQTT-Nachrichten weitergeleitet haben wollen:   
```   
// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { mqttLastXController } from '@/controller/MqttLastXController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(mqttLastXController)
```   
Im obigen Beispiel wird nur der Controller `mqttLastXController` registriert.

## 2. Verbindungs-GUI `MqttConnect.vue`
Die Datei `MqttConnect.vue` enthält eine grafische Oberfläche zur Eingabe der Verbindungsdaten für Host und Port sowie die Tasten [Connect] und [Disconnect] zum Verbinden und Trennen der Verbindung zum Host. Mit Hilfe der Eigenschaft `connected` und dem Status der Verbindung werden die Tasten ein- bzw. ausgeblendet und der Verbindungsstatus angezeigt.    
```   
<!-- MqttConnect.vue -->
<template>
  <div class="mqtt_title">Connect to broker</div>
  <p>                                            <!--1-->
    Host: <input v-model="hostip">
    Port: <input type="number" v-model.number="hostport">
  </p>
                                                 <!--2-->
  <button @click="connect()" :disabled="isConnected || isConnecting">Connect</button> &nbsp;
  <button @click="end()"     :disabled="!isConnected">Disconnect</button> &nbsp;
  <p>
    MqttState: {{ getMqttState }}                <!--3-->
    <span v-if=isConnected>
       &nbsp; (Broker at {{ getConnectUrl }})    <!--4-->
    </span>
    <span v-if=isConnecting>
      &nbsp;
      <button @click="cancel()">Cancel</button>  <!--5-->
    </span>
  </p>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

export default defineComponent({
  data () {
    return {
      hostip: mqttClientInstance.mqttConnection.host,
      hostport: mqttClientInstance.mqttConnection.port
    }
  },
  computed: {
    isConnected: function (): boolean {
      return mqttClientInstance.mqttState.connected
    },
    isConnecting: function (): boolean {
      if (mqttClientInstance.mqttState.iConnMqttState === 2) return true
      return false
    },
    getMqttState: function (): string {
      return mqttClientInstance.sConnMqttState()
    },
    getConnectUrl: function (): string {
      return mqttClientInstance.connectUrl()
    }
  },
  mounted: async function (): Promise<void> {
    // this.connect()
  },
  methods: {
    connect: async function (): Promise<void> {
      if (!this.isConnected) {
        console.log('MqttConnect.vue: Connecting to ' + this.hostip + ':' + this.hostport + '...')
        await mqttClientInstance.connect(this.hostip, this.hostport, '')
        console.log('MqttConnect.vue: Connected to ' + this.getConnectUrl)
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
</style>
```   
Man erkennt die Aufteilung des Codes in den Grafik-Teil `<template>` und den Programm-Teil `<script>`.   
__Grafik-Teil:__
* `<--1-->` Eingabefelder für Host und Port.   
   Mit `v-model` wird der Inhalt der Textfelder mit den Variablen `hostip` bzw. `hostport` verknüpft. Die Vorgabewerte stammen aus `mqttClientInstance.mqttConnection`   
* `<--2-->` Eingabetaster [Connect] und [Disconnect], die je nach Programmzustand deaktiviert werden.   
* `<--3-->` Anzeige des Verbindungszustandes (als Text).   
* `<--4-->` Besteht eine Verbindung mit dem Broker, so wird zusätzlich die URL angezeigt.   
* `<--5-->` Während des Verbindungszustandes wird ein Taster [Cancel] angezeigt, mit dem der Verbindungsaufbau abgebrochen werden kann (zB bei falscher IP-Adresse).   
Der Stil für die Überschrift (`class="mqtt_title"` mit roter Schrift auf gelbem Grund) ist in der Datei `App.vue` definiert, damit er für alle Vue-Komponenten zur Verfügung steht.   

__Skript-Teil:__
Im Skript-Teil werden die Daten des Verbindungszustandes (aus der `mqttClientInstance`) für den Grafik-Teil sowie die Funktionen `connect()`, `end()` und `cancel()` zur Verfügung gestellt.

## Anpassungen in `App.vue` und `main.ts`
...