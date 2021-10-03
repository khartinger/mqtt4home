<template>
<MqttConnect></MqttConnect>
<hr>
<MqttPublish></MqttPublish>
<hr>
<MqttSubscribe></MqttSubscribe>
<br><br>
<MqttLastX></MqttLastX>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import MqttConnect from './components/MqttConnect.vue'
import MqttPublish from './components/MqttPublish.vue'
import MqttSubscribe from './components/MqttSubscribe.vue'
import MqttLastX from './components/MqttLastX.vue'

export default defineComponent({
  name: 'App',
  components: {
    MqttConnect,
    MqttPublish,
    MqttSubscribe,
    MqttLastX
  }
})
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #2c3e50;
  margin-top: 0px;
}
.mqtt_title {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  font-weight: bold;
  text-align: left;
  color: #FF0000;
  background-color: #FFFFBB;
  margin-top: 6px;
  margin-bottom: 6px;
}
</style>
