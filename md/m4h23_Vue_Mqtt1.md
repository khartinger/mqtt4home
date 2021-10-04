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
   * `connect ...........` Verbinden mit dem Broker   
   * `disconnect ........` L&ouml;sen der Verbindung zum Broker   
   * `subscribe .........` Abonnieren eines Topics   
   * `unsubscribe .......` L&ouml;sen des Abonnieren eines Topics   
   * `publish ...........` Ver&ouml;ffentlichen einer Nachricht   
   * `sConnMqttState ....` Verbindungszustand als Text ("connected" etc.)   
   * `hostSubscribe .....` Verbinden mit dem angegebenen Broker und abonnieren des angegebenen Topics   
   * `registerController ` Anmeldung eines Controllers, damit er Nachrichten weitergeleitet bekommt (und in der Methode `onMessage` auswerten kann).   

   Codierung der Datei [`MqttClient.ts` siehe `https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt1/src/services/MqttClient.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue_mqtt1/src/services/MqttClient.ts)


2. Die Datei `MqttClientInstance.ts` exportiert das Verbindungsobjekt `mqttClientInstance`.   
Weiters m&uuml;ssen in dieser Datei alle MQTT-Controller registriert werden:   
```   
// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { mqttLastXController } from '@/controller/MqttLastXController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(mqttLastXController)
```   

## 2. Verbindungs-GUI `MqttConnect.vue`
...

## Anpassungen in `App.vue` und `main.ts`
...