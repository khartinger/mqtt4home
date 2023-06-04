<!-- CiMain.vue (vue66_ci_mqtt_door1)-------------khartinger -->
<!-- 2023-06-04 new                                          -->
<template>
  <svg width="100%" viewBox="0 0 140 100">
  <rect class="ciBackground" x="0" y="0" width="140" height="100" />
  <CiDoor1 :x="70" :y="50" sid="h6T1" dir="h4" :border="0"></CiDoor1>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiDoor1 from './CiDoor1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiDoor1
  }
})
</script>

<style>
</style>
