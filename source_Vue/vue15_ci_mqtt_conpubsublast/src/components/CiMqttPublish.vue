<!-- CiMqttPublish.vue -->
<template>
  <div class="mqtt_title">Publish message</div>
  <table>
   <tr><td>Topic: </td><td><input v-model="msg.intopic"></td></tr>
   <tr><td>Payload: </td><td><input v-model="msg.inpayload"></td></tr>
   <tr><td>Retain: </td><td><input type="checkbox" v-model="msg.inretain">yes</td></tr>
   <tr><td>QoS: </td><td>
     <select v-model="msg.inqos">
       <option>0</option>
       <option>1</option>
       <option>2</option>
     </select>
    </td></tr>
</table>
  <button @click="publish()" :disabled="!isConnected">Publish</button> &nbsp;
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'
import { QoS } from 'Mqtt'

export default defineComponent({
  data () {
    return {
      msg: {
        intopic: 'test/vue/1',
        inpayload: '?',
        inretain: false,
        inqos: 0 as QoS
      }
    }
  },
  computed: {
    isConnected: function (): boolean {
      return ciMqttClientInstance.mqttState.connected
    }
  },
  methods: {
    publish: async function (): Promise<void> {
      try {
        await ciMqttClientInstance.publish(
          this.msg.intopic, this.msg.inpayload, this.msg.inretain, this.msg.inqos)
      } catch (e) {
        console.error('NO CONNECTION')
      }
    }
  }
})
</script>

<style>
</style>
