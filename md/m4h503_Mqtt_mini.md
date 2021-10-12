Letzte &Auml;nderung: 16.9.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Vue: Erste MQTT Anwendung</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h503_Vue_Mqtt_mini_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
* Erstellen einer MQTT-Anwendung, mit der ein MQTT-Befehl gesendet und eine empfangene MQTT-Nachricht angezeigt werden können.   
   

## Voraussetzungen
* Etwas Wissen &uuml;ber HTML, CSS und JavaScript bzw. TypeScript :)   
* Visual Studio Code ist für Vue-Anwendungen bereits vorbereitet, dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.   

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Software: Visual Studio Code, node.js, npm

## Aufgabenstellung im Detail
Es soll eine MQTT-Anwendung erstellt werden, die 
* sich über WebSocket mit dem MQTT-Broker 10.1.1.1 auf Port 1883 und 1884 verbindet,   
* alle Topics abonniert,   
* empfangene Topics anzeigt und   
* auf Knopfdruck die Nachricht `-t test/vue/1 -m "Hello from mqtt_mini"` verschickt.   


## Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   

2. VSC: Terminal-Fenster &ouml;ffnen: `Men&uuml; Terminal - New Terminal`.   

3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll: `cd /g/github/mqtt4home/source_Vue`   

4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create mqtt_mini`  
   Mit Cursortasten, Leertaste und &lt;Enter&gt; Folgendes auswählen:   
   `> Manually select features`   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `( ) Router`   
   `(*) Linter / Formatter`   
   `> 3.x`   
   `? Use class-style component syntax? No`   
   `? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)? No`   
   `? Use history mode for router? (Requires proper server setup for index fallback in production) No`   
   `? Pick a linter / formatter config: ESLint + Standard config`   
   `? Pick additional lint features: Lint on save`   
   `? Where do you prefer placing config for Babel, ESLint, etc.? In dedicated config file`   
   `? Save this as a preset for future projects? (y/N) N`   

5. In den Projektordner wechseln: `VSC Menü Datei - Ordner öffnen..`
   `G:\github\mqtt4home\source_Vue\vue_mini`   

6. MQTT Bibliothek installieren:   
   `npm install mqtt --save`   

## Dateien für die MQTT Verbindung in VSC
MQTT-Anwendungsprogramme bestehen meist aus Komponenten, die über MQTT angesteuert werden (zB Schalter, Lampen usw.), sowie dem eigentlichen MQTT-Zugriff.   
Aus diesem Grund werden im VSC-MQTT-Projekt zwei Verzeichnisse erstellt: `controller` für die Steuerdateien und `services` für die Dateien zum eigentlichen MQTT-Zugriff. Das Erstellen der Verzeichnisse kann direkt in VSC erfolgen:   
__`VSC Menü: View – Explorer`__   
*   Im Explorer `src` markieren, mit der rechten Maustaste auf `src` klicken, New Folder wählen und `controller` eingeben.   
*   Im Explorer `src` markieren, mit der rechten Maustaste auf `src` klicken, New Folder wählen und `services` eingeben.   
   
---   
Damit der MQTT-Zugriff für verschiedene Komponenten einheitlich ist, wird eine abstrakte Basisklasse `DeviceController` definiert, die drei Methoden vorgibt:   
* `public registerClient(mqttClient: MqttClient)` ... zum Registrieren des Controllers beim MqttClientInstance-Objekt, damit die onMessage-Methode aufgerufen wird.   
* `protected async publish(topic: string, payload: string)` zum Versenden einer Nachricht.   
* `public abstract onMessage(message: Message)` zum Verarbeiten eintreffender Nachrichten.   
   
Das Erstellen der Datei `src/controller/DeviceController.ts` erfolgt so:   
* Im Explorer das Verzeichnis `controller` markieren, mit der rechten Maustaste auf `controller` klicken, New File wählen und `DeviceController.ts` eingeben.   
* In die Datei folgenden Code eingeben:   
```   
//_____DeviceController.ts______________________________________
import { Message, MqttClient } from "@/services/MqttClient";
export abstract class DeviceController {
  protected client: MqttClient | null = null;

  public registerClient(mqttClient: MqttClient) {
    this.client = mqttClient;
  }

  protected async publish(topic: string, payload: string): Promise<void> {
    return this.client?.publish(topic, payload);
  }

  public abstract onMessage(message: Message): void;
}
```   
---   
Für den bequemen MQTT-Zugriff werden zwei Interfaces und die Klasse MqttClient erstellt:   
* `export interface Message` ... stellt eine Message mit topic, payload und retain-Flag dar.   
* `interface State` ... stellt den Status der MQTT-Verbindung (connected, WebSocket-Daten, empfpangene Nachrichten) dar.   
* `export class MqttClient` ... Eigenschaften und Methoden für den Zugriff auf MQTT sowie eine Liste der registrierten Controller.   
Das Erstellen der Datei `src/services/MqttClient.ts` erfolgt so:   
* Im Explorer das Verzeichnis `services` markieren, mit der rechten Maustaste auf `services` klicken, New File wählen und `MqttClient.ts` eingeben.   
* In die Datei folgenden Code eingeben:   
```   
// ______MqttClient.ts__________________________________________
import { DeviceController } from '@/controller/DeviceController'
import mqtt from 'mqtt'
import { reactive } from 'vue'

export interface Message {
  topic: string,
  payload: string,
  retain: boolean
}

export interface State {
  wsconn: string;
  connected: boolean;
  message1: string;
  messages: Array<Message>;
}

export class MqttClient {
  public client: mqtt.Client | null = null;

  public controller: Array<DeviceController> = [];

  public mqttState: State = reactive<State>({
    wsconn: 'ws://192.168.0.8:1884',
    connected: false,
    message1: '',
    messages: []
  })

  public connect (): Promise<void> {
    return new Promise((resolve, reject) => {
      // const client = mqtt.connect('ws://10.1.1.1:1884');
      const client = mqtt.connect(this.mqttState.wsconn)
      this.client = client
      client.on('connect', () => {
        this.mqttState.connected = true
        client.on('error', (err) => {
          console.error('MQTT Error', err)
        })
        client.on('connecting', () => {
          console.error('MQTT Connecting')
        })
        client.on('offline', (value: any) => {
          this.mqttState.connected = false
          console.error('MQTT Offline', value)
        })
        client.on('disconnect', (value: any) => {
          console.error('MQTT Disconnect', value)
        })
        client.on('end', (value: any) => {
          this.mqttState.connected = false
          console.error('end: value=', value)
        })
        client.on('message', (topic: string, message: any) => {
          if (topic !== 'zb/bridge/info' && topic !== 'zb/bridge/devices') {
            this.mqttState.messages.push({
              topic: topic,
              payload: message.toString(),
              retain: false
            })
            this.mqttState.message1.topic=topic
            this.mqttState.message1.payload=message.toString()
            this.mqttState.message1.retain: false
            console.error('PUSH:', topic, message.toString())
          }
          while (this.mqttState.messages.length > 10) {
          if (this.mqttState.messages.length > 10) {
            this.mqttState.messages.splice(0, 1)
          }
          this.controller.forEach(controller => controller.onMessage({
            topic: topic,
            payload: message.toString()
          }))
          // console.error('MESSAGE:', topic, message.toString());
          // topic = ''
        })
        client.subscribe('#')
        resolve()
      })
      client.on('error', () => {
        reject(Error('Error'))
      })
    })
  }

  public publish (topic: string, message: string): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.publish(topic, message, (err) => {
        if (err) return reject(new Error('Could not publish topic'))
        resolve()
      })
    })
  }

  public end () {
    return this.client?.end()
  }

  public registerController (controller: DeviceController) {
    this.controller.push(controller)
    controller.registerClient(this)
  }
}
```   

Das Registrieren der Controller erfolgt in der Datei `src/services/MqttClientInstance.ts` so:   
* Im Explorer das Verzeichnis `services` markieren, mit der rechten Maustaste auf `services` klicken, New File wählen und `MqttClientInstance.ts` eingeben.   
* In die Datei folgenden Code eingeben:   
```   
// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
// import { mqttGuiController } from '@/controller/MqttGuiController';

export const mqttClientInstance = new MqttClient()
// mqttClientInstance.registerController(mqttGuiController);
```   

## Erstellung der GUI in VSC
Zum Testen der Verbindung wird eine Datei `MqttGui.vue` erstellt, die auf Knopfdruck die Nachricht `-t test/vue/1 -m "Hello from mqtt_mini"` verschickt und eintreffende MQTT-Nachrichten anzeigt.   

1. Erstellen der Datei `src/components/MqttGui1.vue`:   
* Im Explorer das Verzeichnis `components` markieren, mit der rechten Maustaste auf `components` klicken, New File wählen und `MqttGui1.vue` eingeben.   
* In die Datei folgenden Code eingeben:   
```   

```   

2. Anzeige in `App.vue` ermöglichen   
   Anzeige von `MqttGui.vue1` ergänzen, HelloWorld-Anzeige auskommentieren (oder löschen).
```   

```   
