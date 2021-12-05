<template>
  <div class="about">
    <h2>Page2: Last messages received</h2>
    <MqttLastX></MqttLastX>
    <hr>
  </div>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import MqttLastX from '@/components/MqttLastX.vue'
export default defineComponent({
  name: 'Home',
  components: {
    MqttLastX
  }
})
</script>
