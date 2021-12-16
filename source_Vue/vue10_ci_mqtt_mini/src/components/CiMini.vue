<!--CiMini.vue-->
<!-- =========[1] GUI of vue10_ci_mqtt_mini=================== -->
<template>
  <h1>vue10_ci_mqtt_mini</h1>
  <!-- -------[2] show connection info---------------------- -->
  Broker-URL: {{ brokerurl }} <br>
  MQTT-State: {{ mqttstate }} <br>
  <!-- -------[3] show button only if connected to broker--- -->
  <button v-if="mqttstate==='connected'" @click="clkPublish()">Publish message</button>
  <hr>
  <!-- -------[4] show last received message---------------- -->
  Last received Topic: {{ mini.topicIn }} <br>
  Last received Payload: {{ mini.payloadIn }}
</template>

<!-- =========[5] script part of vue10_ci_mqtt_mini=========== -->
<script lang="ts">
// -----------[6] import required components--------------------
import { defineComponent } from '@vue/runtime-core'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'
import { Mini, ciMiniController } from '@/controller/CiMiniController'

// -----------[7] define a new (typescript) file component------
export default defineComponent({
  name: 'CiMini',
  computed: {
    // _______[8] get public properties of CiMiniController_____
    mini: function (): Mini { return ciMiniController.mini },
    // -------[9] get mqtt connection infos---------------------
    // _______[10] ip and port of broker________________________
    brokerurl: function (): string { return ciMqttClientInstance.connectUrl() },
    // _______[11] connection state as string___________________
    mqttstate: function (): string { return ciMqttClientInstance.sConnMqttState() }
  },
  methods: {
    // -------[12] functions of CiMini--------------------------
    // _______[13] Publish the default message__________________
    clkPublish: async function (): Promise<void> {
      try {
        // await ciMqttClientInstance.publish('test/vue', 'Hello from vue10_ci_mqtt_mini', false, 0)
        ciMiniController.publishCi('test/vue', 'Hello from vue10_ci_mqtt_mini')
      } catch (e) { console.error('NOT PUBLISHED: ', e) }
    }
  }
})
</script>

<!-- =========[14] styles of vue10_ci_mqtt_mini=============== -->
<style scoped>
</style>
