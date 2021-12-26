<!--App.vue-->
<template>
  <div id="nav">
    <router-link to="/">Home</router-link> |
    <router-link to="/page2">Page2</router-link> |
    <router-link to="/about">About</router-link>
    &nbsp; &nbsp; &nbsp;
    <span v-if="isConnected">
      <text class="fontOK">Connected to {{ getConnectUrl }}</text>
    </span>
    <span v-if="!isConnected && !isConnecting">
      <text class="fontNOK"> Not connected! &nbsp;</text>
      <button @click="reconnect()">Reconnect</button>
    </span>
    <span v-if=isConnecting>
      <text class="fontNOK"> Is connecting... &nbsp;</text>
      &nbsp; <button @click="cancel()">Cancel</button>
    </span>
  <router-view/>
</div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'

export default defineComponent({
  name: 'Home',
  components: {
  },
  computed: {
    isConnected: function (): boolean {
      return ciMqttClientInstance.mqttState.connected
    },
    isConnecting: function (): boolean {
      if (ciMqttClientInstance.mqttState.iConnMqttState === 2) return true
      return false
    },
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    },
    getMqttState: function (): string {
      return ciMqttClientInstance.sConnMqttState()
    },
    getConnectUrl: function (): string {
      return ciMqttClientInstance.connectUrl()
    }
  },
  methods: {
    reconnect: async function (): Promise<void> {
      ciMqttClientInstance.reconnectBroker()
    },
    end: async function (): Promise<void> {
      ciMqttClientInstance.disconnect()
    },
    cancel: async function (): Promise<void> {
      ciMqttClientInstance.disconnect()
    }
  }
})

</script>

<style>
#app {
  font-family: Helvetica, Arial, sans-serif;
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

  .ciFont1   { font-size: 11px; font-weight: bold; font-family: monospace; color: black; white-space: pre; }
  .fontOK    { font-weight: bold; color: lightgreen; }
  .fontNOK   { font-weight: bold; color: red; }
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
