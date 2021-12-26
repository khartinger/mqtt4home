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
