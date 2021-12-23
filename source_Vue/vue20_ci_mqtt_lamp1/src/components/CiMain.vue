<!--CiMain.vue-->
<template>
<svg width="100%" viewBox="0 0 320 120">
  <rect class="ciBackground" x="0" y="0" width="320" height="120" />
  <CiLamp :x= "60" :y="60" sid="lamp1"></CiLamp>
  <CiLamp :x="160" :y="60" sid="lamp2" :border="1" lines="2"></CiLamp>
  <CiLamp :x="260" :y="60" sid="lamp2" :border="0" lines="0"></CiLamp>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiLamp from './CiLamp.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiLamp
  }
})
</script>

<style>
</style>
