<!-- MqttSubscribe.vue -->
<template>
  <div class="mqtt_title">Subscribe topic</div>
  <p>Subscribe topic: <input v-model="topicsub"></p>
  <button @click="subscribe()"   :disabled="(!isConnected) || (isSuccess)">Subscribe</button> &nbsp;
  <button @click="unsubscribe()" :disabled="(!isConnected) || (!isSuccess)">Unsubscribe</button> &nbsp;
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

export default defineComponent({
  data () {
    return {
      topicsub: '#'
    }
  },
  computed: {
    isConnected: function (): boolean {
      return mqttClientInstance.mqttState.connected
    },
    isSuccess: function (): boolean {
      return mqttClientInstance.mqttSubscription.success
    }
  },
  mounted: async function (): Promise<void> {
    // this.connect()
  },
  methods: {
    subscribe: async function (): Promise<void> {
      mqttClientInstance.subscribe()
      mqttClientInstance.mqttSubscription.success = true
    },
    unsubscribe: async function (): Promise<void> {
      mqttClientInstance.unsubscribe()
      mqttClientInstance.mqttSubscription.success = false
    }
  }
})
</script>

<style>
</style>
