<!--CiMain.vue-->
<template>
<svg width="100%" viewBox="-10 -10 370 120">
  <rect class="ciBackground" x="-10" y="-10" width="380" height="120" />
  <CiLamp :x="50" :y="50" sid="lamp1" :border="0"></CiLamp>
  <CiLamp :x="160" :y="50" sid="lamp2" lines="2"></CiLamp>
  <line x1 = 230 y1 = -10 x2 = 230 y2 = 120 stroke = "black" />
  <CiButton :x="300" :y="50" sid="button1" lines="2" :border="0"></CiButton>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'
import CiLamp from './CiLamp.vue'
import CiButton from './CiButton.vue'

// ------MQTT broker parameter----------------------------------
const host = '10.1.1.1'
const port = 1884
const subtopic = '#'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiLamp,
    CiButton
  },
  mounted: async function (): Promise<void> {
    mqttClientInstance.connect(host, port, '')
    mqttClientInstance.subscribe(subtopic, 0)
  }
})
</script>

<style>
</style>
