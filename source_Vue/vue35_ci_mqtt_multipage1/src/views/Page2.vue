<!--views/Page2.vue-->
<template>
  <div class="about">
    <h2>Page2: Last messages received</h2>
    <CiMqttLastX></CiMqttLastX>
    <hr>
  </div>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import CiMqttLastX from '@/components/CiMqttLastX.vue'
export default defineComponent({
  name: 'Home',
  components: {
    CiMqttLastX
  }
})
</script>
