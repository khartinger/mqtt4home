<!--CiMain.vue-->
<template>
<svg width="100%" viewBox="-5 -5 210 110">
  <rect class="ciBackground" x="-5" y="-5" width="210" height="110" />
  <CiLamp :x="50" :y="50" sid="lamp1" :border="0"></CiLamp>
  <CiSocket2 :x="150" :y="50" sid="socket2" :border="0"></CiSocket2>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiLamp from './CiLamp.vue'
import CiSocket2 from './CiSocket2.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiLamp,
    CiSocket2
  }
})
</script>

<style>
</style>
