<!-- ---------Seg7MainMqtt.vue---------------2022-03-04----- -->
<!-- Show a value received via MQTT on the 7-segment-display -->
<!-- mosquitto_pub -h 10.1.1.1 -t seg7/1/set/value -m 12.34  -->
<template>
  <!-- show area with display ------------------------------ -->
  <svg width="100%" viewBox="0 0 260 80">
  <rect x="0" y="0" width="260" height="80" fill="#ddFFdd"/>
  <Seg7x :x="130" :y="40" :value="value7x" digits="4" :height="40" sid="sid1"> </Seg7x>
  <!-- define click area ----------------------------------- -->
  <rect @click="onClick_()" :x="(132-2*38)" :y="20" :width="(4*38)" :height="40" fill-opacity="0.0"/>
  </svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import Seg7x from './Seg7x.vue'
import { Seg7Mqtt, seg7MainMqttController } from '@/controller/Seg7MainMqttController'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'

export default defineComponent({
  name: 'Seg7MainMqtt',
  components: {
    Seg7x
  },
  data () {
    return {
      sid1: 'seg7MainMqtt_1'
    }
  },
  props: {
  },
  computed: {
    // -------ID of 7-segment-display as string-----------------
    seg7Mqtt: function (): Seg7Mqtt | undefined {
      return seg7MainMqttController.seg7Mqtts.find(seg7Mqtt => seg7Mqtt.id === this.sid1)
    },
    // -------value to be displayed (on error '----')-----------
    value7x: function (): string {
      if (this.seg7Mqtt?.value7x) return this.seg7Mqtt.value7x
      return '+64+64+64+64'
    }
  },
  // =========only needed if MQTT connection is required========
  mounted: function (): void {
    ciMqttClientInstance.init()
  },
  methods: {
    // -------On Click: publish the displayed value by MQTT-----
    onClick_: function (): void {
      console.log(this.sid1, '!!Click!!')
      const topic = this.seg7Mqtt?.pubTopic ?? 'seg7/ret/error'
      seg7MainMqttController.publishCi(topic, this.value7x)
    }
  }
})
</script>

<style>
</style>
