<!--mqttMini-->
<!-- ----[1] GUI of mqttMini ------------------------------- -->
<template>
<span v-if="!isConnected">
    <h2>NOT connected to {{ getWsbroker }}</h2>
  </span>
  <span v-else>
    <h2>Connected to {{ getWsbroker }} :)</h2>
    <button @click="publish()">Publish</button>
    <p>
      Topic: {{ getTopic }}<br>
      Payload: {{ getPayload }}
    </p>
</span>
</template>

<script lang="ts">
// ======[2] import some utilities==============================
import { defineComponent, reactive } from 'vue'
import mqtt from 'mqtt'

// ======[3] script part of component mqttMini==================
export default defineComponent({
  // ____[4] computed functions (values from mqttClientInstance)
  computed: {
    isConnected: function (): boolean {
      return mqttClientInstance.state.connected
    },
    getWsbroker: function (): string {
      return mqttClientInstance.state.wsbroker
    },
    getTopic: function (): string {
      return mqttClientInstance.state.topic
    },
    getPayload: function (): string {
      return mqttClientInstance.state.payload
    }
  },
  // ____[5] what should be done during the mounting..._________
  mounted: async function (): Promise<void> {
    this.connect()
  },
  // ____[6] functions of mqttMini______________________________
  methods: {
    connect: async function (): Promise<void> {
      if (!this.isConnected) {
        await mqttClientInstance.connect()
      }
    },
    publish: async function (): Promise<void> {
      try {
        await mqttClientInstance.publish('test/vue', 'Hello from vue05_mqtt_mini')
      } catch (e) {
        console.error('NO CONNECTION')
      }
    }
  }
})

// ======[7] Class MqttClient===================================
export class MqttClient {
  // ____[8] properties_________________________________________
  public client: mqtt.Client | null = null;
  public state = reactive({
    wsbroker: 'ws://10.1.1.1:1884',
    connected: false,
    topic: '-',
    payload: '-'
  });

  // ____[9] connect method_____________________________________
  public connect (): Promise<void> {
    return new Promise((resolve, reject) => {
      const client = mqtt.connect(this.state.wsbroker)
      this.client = client
      client.on('connect', () => {
        client.on('error', (err) => {
          console.error('MQTT Error', err)
        })
        client.on('connecting', () => {
          console.log('MQTT Connecting')
        })
        client.on('offline', (value: any) => {
          console.error('MQTT Offline', value)
        })
        client.on('disconnect', (value: any) => {
          this.state.connected = false
          console.log('MQTT Disconnect', value)
        })
        client.on('message', (topic: string, message: any) => {
          this.state.topic = topic
          this.state.payload = message.toString()
        })
        client.subscribe('#')
        this.state.connected = true
        resolve()
      })
      client.on('error', () => {
        reject(new Error('Error!'))
      })
    })
  }

  // ____[10] publish method____________________________________
  public publish (topic: string, message: string): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new Error('Not Connected'))
      this.client.publish(topic, message, (err) => {
        if (err) return reject(new Error('Could not publish topic'))
        resolve()
      })
    })
  }
}

// ======[11] Instance to work with the class MqttClient========
export const mqttClientInstance = new MqttClient()
</script>

<style>
</style>
