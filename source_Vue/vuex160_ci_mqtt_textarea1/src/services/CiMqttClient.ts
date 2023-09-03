// ______CiMqttClient.ts_________________________khartinger_____
// 2021-12-26: new
// 2023-01-22: add classes, update CiBase.vue, CiBaseController.ts
import mqtt, { MqttClient } from 'mqtt'
import { CiBaseController } from '@/controller/CiBaseController'
import { reactive, readonly } from 'vue'

export type QoS = 0 | 1 | 2

// *************************************************************
// interfaces
// *************************************************************

// -----------message properties--------------------------------
export interface Message {
  topic: string,
  payload: string,
  retain: boolean,
  qos: QoS
}

// -----------state of the MQTT connection----------------------
export interface MqttState {
  connectOnStart: boolean;
  connected: boolean;
  iConnMqttState: number;
  message: Message;
}

// -----------properties for the MQTT connection----------------
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

// -----------info about the subscribed messages----------------
export interface MqttSubscription {
  topic: string;
  qos: QoS;
  subscribed: boolean;
}

// *************************************************************
// class CiMqttClient
// *************************************************************

export class CiMqttClient {
  // ---------basic properties----------------------------------
  public client: mqtt.MqttClient | null = null;
  public controller: Array<CiBaseController> = [];

  private subTopic = '#';
  private subQos: QoS = 0;

  // ---------some more properties (interfaces)-----------------
  // To prevent values from being changed by mistake,
  // there is always a private and a public variant.
  private privateMqttState: MqttState = reactive<MqttState>({
    connectOnStart: false,
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

  // =========methods===========================================
  // _________constructor_______________________________________
  constructor (connectOnStart: boolean) {
    this.privateMqttState.connectOnStart = connectOnStart
    console.log('MqttClient-Constructor: privateMqttState.connectOnStart = ', this.privateMqttState.connectOnStart)
    if (this.privateMqttState.connectOnStart) {
      console.log('MqttClient-Constructor: connecting to ' + this.connectUrl())
      this.connect_().then(() => {
        this.subTopic = this.privateMqttSubscription.topic
        this.subQos = this.privateMqttSubscription.qos
        console.log('MqttClient-Constructor: subscribe ' + this.privateMqttSubscription.topic)
        this.subscribe_().catch((e) => { console.error('MqttClient-constructor: Subcribe ERROR:', e) })
      }).catch((e) => { console.error('MqttClient-constructor: Connect ERROR:', e) })
      console.log('MqttClient-Constructor: finished')
    }
  }

  // _________connect url like 10.1.1.1:1884____________________
  public connectUrl (): string {
    return `ws://${this.mqttConnection.host}:${this.mqttConnection.port}${this.mqttConnection.endpoint}`
  }

  // _________public variant: connect to server (broker)________
  public connect (host: string, port: number, endpoint: string): Promise<void> {
    this.privateMqttConnection.host = host
    this.privateMqttConnection.port = port
    this.privateMqttConnection.endpoint = endpoint
    return this.connect_()
  }

  // _________private variant: connect to server (broker)_______
  private connect_ (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (this.client) this.disconnect().catch((e) => { console.error('MqttClient-connect_: ERROR while disconnecting:', e) })
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
        // client.on('connecting', () => {
        //   console.error('MQTT Connecting')
        //   this.privateMqttState.iConnMqttState = 2
        // })
        // client.on('offline', (value) => {
        //   this.privateMqttState.connected = false
        //   this.privateMqttSubscription.subscribed = false
        //   console.error('MQTT Offline', value)
        //   this.privateMqttState.iConnMqttState = 3
        // })
        client.on('disconnect', (value: any) => {
          console.error('MQTT Disconnect', value)
          this.privateMqttState.iConnMqttState = 0
        })
        // client.on('end', (value: any) => {
        //   this.privateMqttState.connected = false
        //   this.privateMqttSubscription.subscribed = false
        //   console.error('MqttClient.ts-end: value=', value)
        //   this.privateMqttState.iConnMqttState = 9
        // })
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

  // _________disconnect from broker____________________________
  public disconnect (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.privateMqttState.iConnMqttState = 9
      this.client.end(true, {}, (err: any) => {
        if (err) {
          // console.log('CiMqttClient:disconnect: ', 'failed!')
          return reject(new Error('Could not disconnect'))
        }
        // console.log('CiMqttClient:disconnect: ', 'success!')
        resolve()
      })
    })
  }

  // _________public variant: subscribe topic___________________
  public subscribe (topic: string, qos: QoS): Promise<void> {
    this.subTopic = topic
    this.subQos = qos
    return this.subscribe_()
  }

  // _________private variant: subscribe topic__________________
  private subscribe_ (): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) {
        // console.log('CiMqttClient:subscribe: failed - not connected! ' + this.privateMqttSubscription.topic)
        return reject(new Error('subscribe: Not Connected'))
      }
      this.unsubscribe().catch((e) => { console.error('MqttClient-subscribe_: ERROR:', e) })
      this.client.subscribe(this.subTopic, { qos: this.subQos }, (err: any) => {
        if (err) {
          // console.log('CiMqttClient:subscribe: failed! ' + this.subTopic)
          return reject(new Error('Could not subscribe topic ' + this.subTopic))
        }
        this.privateMqttSubscription.subscribed = true
        this.privateMqttSubscription.topic = this.subTopic
        this.privateMqttSubscription.qos = this.subQos
        // console.log('CiMqttClient:subscribe: success! ' + this.privateMqttSubscription.topic)
        resolve()
      })
    })
  }

  // _________unsubscribe topic_________________________________
  public unsubscribe (): Promise<void> {
    return new Promise((resolve, reject) => {
      const subscribedOld = this.privateMqttSubscription.subscribed
      this.privateMqttSubscription.subscribed = false
      if (!this.client) {
        // console.log('CiMqttClient:unsubscribe: failed - not connected! ', this.privateMqttSubscription.topic)
        return reject(new Error('Not Connected'))
      }
      if (subscribedOld) {
        this.client.unsubscribe(this.privateMqttSubscription.topic, {
          qos: 1
        }, (err: any) => {
          if (err) {
            // console.log('CiMqttClient:unsubscribe: failed! ', this.privateMqttSubscription.topic)
            return reject(new Error('Could not unsubscribe topic ' + this.privateMqttSubscription.topic))
          }
          // console.log('CiMqttClient:unsubscribe: success! ', this.privateMqttSubscription.topic)
          resolve()
        })
      }
    })
  }

  // _________publish a message_________________________________
  public publish (topic: string, payload: string, retain: boolean, qos: QoS): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.publish(topic, payload, { qos: qos, retain: retain }, (err: any) => {
        if (err) return reject(new Error('Could not publish topic ' + topic))
        resolve()
      })
    })
  }

  // _________return mqtt connection state as string____________
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

  // _________connect to a broker and subscribe a topic_________
  public hostSubscribe (host: string, topicSubscribe: string): boolean {
    this.privateMqttConnection.host = host
    this.privateMqttSubscription.topic = topicSubscribe
    try {
      this.connect_()
      this.subscribe_()
    } catch (err) {
      console.error('CiMqttClient.hostSubscribe: Error ' + err)
      return false
    }
    return true
  }

  // _________method to register controller_____________________
  public registerController (controller: CiBaseController): void {
    this.controller.push(controller)
    controller.registerClient(this)
  }

  // _________reconnect to broker with default values___________
  public reconnectBroker (): boolean {
    try {
      this.connect_()
      this.subscribe_()
    } catch (err) {
      console.error('CiMqttClient.reconnectBroker: Error ' + err)
      return false
    }
    return true
  }

  // _________nothing to do...__________________________________
  public init (): void {
    console.log('CiMqttClient.init')
  }
}
