// ______MqttClient.ts__________________________________________
import { DeviceController } from '@/controller/DeviceController'
import mqtt, { QoS } from 'mqtt'
import { reactive, readonly } from 'vue'

// -------------------------------------------------------------
// interfaces
// -------------------------------------------------------------
export interface Message {
  topic: string,
  payload: string,
  retain: boolean,
  qos: QoS
}

export interface MqttState {
  connectOnStart: boolean;
  connected: boolean;
  iConnMqttState: number;
  message: Message;
}

export interface MqttConnection {
  host: string;
  port: number;
  endpoint: string;
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
  subscribed: boolean;
}

// -------------------------------------------------------------
// class MqttClient
// -------------------------------------------------------------
export class MqttClient {
  public client: mqtt.Client | null = null;

  public controller: Array<DeviceController> = [];

  private privateMqttState: MqttState = reactive<MqttState>({
    connectOnStart: true,
    connected: false,
    iConnMqttState: -1,
    message: { topic: '', payload: '', retain: false, qos: 0 }
  })

  public mqttState = readonly(this.privateMqttState)

  private privateMqttConnection: MqttConnection = reactive<MqttConnection>({
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

  public mqttConnection = readonly(this.privateMqttConnection)

  private privateMqttSubscription: MqttSubscription = reactive<MqttSubscription>({
    topic: '#',
    qos: 0,
    subscribed: false
  })

  public mqttSubscription = readonly(this.privateMqttSubscription)

  // ---------methods-------------------------------------------

  constructor () {
    if (this.mqttState.connectOnStart) {
      console.log('MqttClient-Constructor: connecting to ' + this.connectUrl())
      this.connect_()
      console.log('MqttClient-Constructor: subscribe ' + this.mqttSubscription.topic)
      this.subscribe_()
      console.log('MqttClient-Constructor: finished')
    }
  }

  public connectUrl (): string {
    return `ws://${this.mqttConnection.host}:${this.mqttConnection.port}${this.mqttConnection.endpoint}`
  }

  public connect (host: string, port: number, endpoint: string): Promise<void> {
    this.privateMqttConnection.host = host
    this.privateMqttConnection.port = port
    this.privateMqttConnection.endpoint = endpoint
    return this.connect_()
  }

  private connect_ (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (this.client) this.disconnect()
      this.privateMqttState.iConnMqttState = 2
      const options_ = {
        clean: this.mqttConnection.clean,
        connectTimeout: this.mqttConnection.connectTimeout,
        reconnectPeriod: this.mqttConnection.reconnectPeriod,
        clientId: this.mqttConnection.clientId,
        username: this.mqttConnection.username,
        password: this.mqttConnection.password
      }
      console.log('MqttCient.ts-connect: url=' + this.connectUrl())
      const client = mqtt.connect(this.connectUrl(), options_)
      this.client = client
      this.client.on('connect', () => {
        this.privateMqttState.connected = true
        this.privateMqttState.iConnMqttState = 1
        client.on('error', (err) => {
          console.error('MQTT Error', err)
          this.privateMqttState.iConnMqttState = 7
        })
        client.on('connecting', () => {
          console.error('MQTT Connecting')
          this.privateMqttState.iConnMqttState = 2
        })
        client.on('offline', (value: any) => {
          this.privateMqttState.connected = false
          this.privateMqttSubscription.subscribed = false
          console.error('MQTT Offline', value)
          this.privateMqttState.iConnMqttState = 3
        })
        client.on('disconnect', (value: any) => {
          console.error('MQTT Disconnect', value)
          this.privateMqttState.iConnMqttState = 0
        })
        client.on('end', (value: any) => {
          this.privateMqttState.connected = false
          this.privateMqttSubscription.subscribed = false
          console.error('MqttClient.ts-end: value=', value)
          this.privateMqttState.iConnMqttState = 9
        })
        client.on('message', (topic: string, payload: any, props1: any) => {
          let retain1 = false
          try {
            retain1 = props1.retain
          } catch (error) { console.error(error) }
          let qos1 = 0 as QoS
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
        resolve()
      })
      client.on('error', () => {
        console.error('Error on connecting...')
        this.privateMqttState.iConnMqttState = 8
        reject(Error('Error'))
      })
    })
  }

  public disconnect (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.privateMqttState.iConnMqttState = 9
      this.client.end(true, {}, (err) => {
        if (err) return reject(new Error('Could not disconnect'))
        resolve()
      })
    })
  }

  public subscribe (topic: string, qos: QoS): Promise<void> {
    this.privateMqttSubscription.topic = topic
    this.privateMqttSubscription.qos = qos
    return this.subscribe_()
  }

  private subscribe_ (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('subscribe: Not Connected'))
      if (this.mqttSubscription.subscribed) {
        this.unsubscribe()
        if (this.mqttSubscription.subscribed) return reject(new Error('subscribe: Could not unsubscribe'))
      }
      console.log('MqttClient.ts-subscribe: ' + this.mqttSubscription.topic)
      this.client.subscribe(this.mqttSubscription.topic, { qos: this.mqttSubscription.qos }, (err) => {
        if (err) return reject(new Error('Could not subscribe topic ' + this.mqttSubscription.topic))
        this.privateMqttSubscription.subscribed = true
        resolve()
      })
    })
  }

  public unsubscribe (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.unsubscribe(this.mqttSubscription.topic, {}, (err) => {
        if (err) return reject(new Error('Could not unsubscribe topic ' + this.mqttSubscription.topic))
        this.privateMqttSubscription.subscribed = false
        resolve()
      })
    })
  }

  public publish (topic: string, payload: string, retain: boolean, qos: QoS): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.publish(topic, payload, { qos: qos, retain: retain }, (err) => {
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

  public hostSubscribe (host: string, topicSubscribe: string): boolean {
    this.privateMqttConnection.host = host
    this.privateMqttSubscription.topic = topicSubscribe
    try {
      this.connect_()
      this.subscribe_()
    } catch (err) {
      console.error('hostSubscribe: Error ' + err)
      return false
    }
    return true
  }

  public registerController (controller: DeviceController): void {
    this.controller.push(controller)
    controller.registerClient(this)
  }
}
