<!-- MqttConnect.vue -->
<template>
  <div class="mqtt_title">Connect to broker</div>
  <p>
    Host: <input v-model="hostip">
    Port: <input type="number" v-model.number="hostport">
  </p>
  <button @click="connect()" :disabled="isConnected || isConnecting">Connect</button> &nbsp;
  <button @click="end()"     :disabled="!isConnected">Disconnect</button> &nbsp;
  <p>
  MqttState: {{ getMqttState }}
  <span v-if=isConnected> &nbsp; (Broker at {{ getConnectUrl }})</span>
  <span v-if=isConnecting>
    &nbsp;
    <button @click="cancel()">Cancel</button>
  </span>
  </p>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

export default defineComponent({
  data () {
    return {
      hostip: '192.168.0.8',
      hostport: 1884
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
        mqttClientInstance.mqttConnection.host = this.hostip
        mqttClientInstance.mqttConnection.port = this.hostport
        mqttClientInstance.mqttState.iConnMqttState = 2
        console.log('MqttConnect.vue: Connecting to ' + this.getConnectUrl + '...')
        await mqttClientInstance.connect()
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
