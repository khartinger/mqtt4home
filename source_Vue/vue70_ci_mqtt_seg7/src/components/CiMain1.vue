<!--CiMain1.vue-->
<template>
<svg width="100%" viewBox="0 0 260 20">
  <rect class="ciBackground" x="0" y="0" width="260" height="20" />
  <CiSeg7x :x="130" :y="10" value="+120+121+109+376" digits="4" :height="10" sid="seg7_1" :border="0"> </CiSeg7x>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiSeg7x from './CiSeg7x.vue'

export default defineComponent({
  name: 'CiMain1',
  components: {
    CiSeg7x
  }
})
</script>

<style>
</style>
