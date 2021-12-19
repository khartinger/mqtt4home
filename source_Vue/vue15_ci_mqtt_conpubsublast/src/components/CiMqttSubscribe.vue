<!-- CiMqttSubscribe.vue -->
<template>
  <div class="mqtt_title">Subscribe topic</div>
  <p>Subscribe topic: <input v-model="topicsub"></p>
  <button @click="subscribe()"   :disabled="(!isConnected) || (isSubscribed)">Subscribe</button> &nbsp;
  <button @click="unsubscribe()" :disabled="(!isConnected) || (!isSubscribed)">Unsubscribe</button> &nbsp;
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'
import { clearMessage } from '@/store/CiMessageStore'

export default defineComponent({
  data () {
    return {
      topicsub: ciMqttClientInstance.mqttSubscription.topic
    }
  },
  computed: {
    subscribedTopic: function (): string {
      return ciMqttClientInstance.mqttSubscription.topic
    },
    isConnected: function (): boolean {
      return ciMqttClientInstance.mqttState.connected
    },
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    }
  },
  watch: {
    subscribedTopic: function (newValue: string): void {
      this.topicsub = newValue
    }
  },
  methods: {
    subscribe: async function (): Promise<void> {
      console.log('MqttSubscribe.vue-subscribe: topic=' + this.topicsub)
      await ciMqttClientInstance.subscribe(this.topicsub, 0)
    },
    unsubscribe: async function (): Promise<void> {
      ciMqttClientInstance.unsubscribe()
      clearMessage()
    }
  }
})
</script>

<style>
</style>
