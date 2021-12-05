<!--App.vue-->
<template>
  <div id="nav">
    <router-link to="/">Home</router-link> |
    <router-link to="/page2">Page2</router-link> |
    <router-link to="/about">About</router-link> 
  </div>
  <router-view/>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

// ------MQTT broker parameter----------------------------------
const host = '10.1.1.1'
const port = 1884
const subtopic = '#'

export default defineComponent({
  name: 'Home',
  components: {
  },
  mounted: async function (): Promise<void> {
    mqttClientInstance.connect(host, port, '')
    mqttClientInstance.subscribe(subtopic, 0)
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
