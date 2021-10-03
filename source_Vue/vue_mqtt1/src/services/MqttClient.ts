// ______MqttClient.ts__________________________________________
import { DeviceController } from '@/controller/DeviceController'
import mqtt from 'mqtt'
import type { QoS } from 'mqtt'
import { reactive } from 'vue'

export interface Message {
  topic: string,
  payload: string,
  retain: boolean,
  qos: number
}

export interface MqttState {
  connected: boolean;
  iConnMqttState: number;
  message: Message;
}

export interface MqttConnection {
  host: string;
  port: number;
  endpoint: string;
  // ---Currently not in use:---
  clean: boolean;
  connectTimeout: number;
  reconnectPeriod: number;
  clientId: string;
  username: string;
  password: string;
}

export interface MqttSubscription {
  topic: string;
  qos: QoS;
  success: boolean;
}

export class MqttClient {
  public client: mqtt.Client | null = null;

  public controller: Array<DeviceController> = [];

  public mqttState: MqttState = reactive<MqttState>({
    connected: false,
    iConnMqttState: -1,
    message: { topic: '', payload: '', retain: false, qos: 0 },
  })

  public mqttConnection: MqttConnection = reactive<MqttConnection>({
    host: '10.1.1.1',
    port: 1884,
    endpoint: '',
    clean: true,
    connectTimeout: 4000,
    reconnectPeriod: 4000,
    clientId: '',
    username: '',
    password: ''
  })

  public mqttSubscription: MqttSubscription = reactive<MqttSubscription>({
    topic: '#',
    qos: 0,
    success: false
  })

  public connectUrl (): string {
    return `ws://${this.mqttConnection.host}:${this.mqttConnection.port}${this.mqttConnection.endpoint}`
  }

  public connect (): Promise<void> {
    return new Promise((resolve, reject) => {
      console.log('MqttCient.ts-connect: url=' + this.connectUrl())
      const client = mqtt.connect(this.connectUrl())
      this.client = client
      this.client.on('connect', () => {
        this.mqttState.connected = true
        this.mqttState.iConnMqttState = 1
        client.on('error', (err) => {
          console.error('MQTT Error', err)
          this.mqttState.iConnMqttState = 7
        })
        client.on('connecting', () => {
          console.error('MQTT Connecting')
          this.mqttState.iConnMqttState = 2
        })
        client.on('offline', (value: any) => {
          this.mqttState.connected = false
          this.mqttSubscription.success = false
          console.error('MQTT Offline', value)
          this.mqttState.iConnMqttState = 3
        })
        client.on('disconnect', (value: any) => {
          console.error('MQTT Disconnect', value)
          this.mqttState.iConnMqttState = 0
        })
        client.on('end', (value: any) => {
          this.mqttState.connected = false
          this.mqttSubscription.success = false
          console.error('MqttClient.ts-end: value=', value)
          this.mqttState.iConnMqttState = 9
        })
        client.on('message', (topic: string, payload: any, props1: any) => {
          // console.log('MqttClient.ts connect()', 'props1=', props1)
          let retain1 = false
          try {
            retain1 = props1.retain
          } catch (error) { console.error(error) }
          let qos1 = 0
          try {
            qos1 = props1.qos
          } catch (error) { console.error(error) }
          this.controller.forEach(controller => controller.onMessage({
            topic: topic,
            payload: payload.toString(),
            retain: retain1,
            qos: qos1
          }))
        })
        // Only if subscribing is also required when connecting
        // client.subscribe(this.mqttSubscription.topic)
        resolve()
      })
      client.on('error', () => {
        console.error('Error on connecting...')
        this.mqttState.iConnMqttState = 8
        reject(Error('Error'))
      })
    })
  }

  public disconnect (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.mqttState.iConnMqttState = 9
      this.client.end(true, {}, (err) => {
        if (err) return reject(new Error('Could not disconnect'))
        resolve()
      })
    })
  }

  public subscribe (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.subscribe(this.mqttSubscription.topic, { qos: this.mqttSubscription.qos }, (err) => {
        if (err) return reject(new Error('Could not subscribe topic ' + this.mqttSubscription.topic))
        this.mqttSubscription.success = true
        resolve()
      })
    })
  }

  public unsubscribe (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.unsubscribe(this.mqttSubscription.topic, {}, (err) => {
        if (err) return reject(new Error('Could not unsubscribe topic ' + this.mqttSubscription.topic))
        this.mqttSubscription.success = false
        resolve()
      })
    })
  }

  public publish (topic: string, payload: string, retain: boolean): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.publish(topic, payload, { qos: 1, retain: retain }, (err) => {
        if (err) return reject(new Error('Could not publish topic ' + topic))
        resolve()
      })
    })
  }

  public sConnMqttState (): string {
    switch (this.mqttState.iConnMqttState) {
      case -1: return 'not connected'
      case 0: return 'disconnected'
      case 1: return 'connected'
      case 2: return 'connecting to ' + this.connectUrl()
      case 3: return 'offline'
      case 7: return 'mqtt error'
      case 8: return 'timeout'
      case 9: return 'end connected'
      default: break
    }
    return 'undefined'
  }

  public registerController (controller: DeviceController): void {
    this.controller.push(controller)
    controller.registerClient(this)
  }
}
