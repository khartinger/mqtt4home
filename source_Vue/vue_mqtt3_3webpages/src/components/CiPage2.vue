<!--CiPage2.vue-->
<template>
  <svg width="100%" viewBox="-10 -10 370 120">
  <rect class="ciBackground" x="-10" y="-10" width="380" height="120" />
  <CiLamp    :x="50" :y="50" sid="lamp1" :border="0"></CiLamp>
  <CiButton2 :x="160" :y="50" sid="button2" lines="0" :border="0"></CiButton2>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'
import CiLamp from './CiLamp.vue'
import CiButton2 from './CiButton2.vue'

// ------MQTT broker parameter----------------------------------
const host = '10.1.1.1'
const port = 1884
const subtopic = '#'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiLamp,
    CiButton2
  },
  mounted: async function (): Promise<void> {
    mqttClientInstance.connect(host, port, '')
    mqttClientInstance.subscribe(subtopic, 0)
  },
  computed: {
  }
})
</script>

<style>
</style>
