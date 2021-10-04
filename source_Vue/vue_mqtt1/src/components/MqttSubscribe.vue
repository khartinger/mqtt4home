<!-- MqttSubscribe.vue -->
<template>
  <div class="mqtt_title">Subscribe topic</div>
  <p>Subscribe topic: <input v-model="topicsub"></p>
  <button @click="subscribe()"   :disabled="(!isConnected) || (isSubscribed)">Subscribe</button> &nbsp;
  <button @click="unsubscribe()" :disabled="(!isConnected) || (!isSubscribed)">Unsubscribe</button> &nbsp;
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mqttClientInstance } from '@/services/MqttClientInstance'

export default defineComponent({
  data () {
    return {
      topicsub: mqttClientInstance.mqttSubscription.topic
    }
  },
  computed: {
    subscribedTopic: function (): string {
      return mqttClientInstance.mqttSubscription.topic
    },
    isConnected: function (): boolean {
      return mqttClientInstance.mqttState.connected
    },
    isSubscribed: function (): boolean {
      return mqttClientInstance.mqttSubscription.subscribed
    }
  },
  mounted: async function (): Promise<void> {
    // this.connect()
  },
  watch: {
    subscribedTopic: function (newValue: string): void {
      this.topicsub = newValue
    }
  },
  methods: {
    subscribe: async function (): Promise<void> {
      mqttClientInstance.mqttSubscription.topic = this.topicsub
      console.log('MqttSubscribe.vue-subscribe: topic=' + mqttClientInstance.mqttSubscription.topic)
      await mqttClientInstance.subscribe()
    },
    unsubscribe: async function (): Promise<void> {
      mqttClientInstance.unsubscribe()
    }
  }
})
</script>

<style>
</style>
