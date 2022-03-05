<!--CiMain1.vue-->
<template>
<svg width="100%" viewBox="0 0 240 160">
  <rect class="ciBackground" x="0" y="0" width="240" height="160" />
  <text :x="5" :y="57" class="ciFont1">ci/seg7/1/set/value</text>
  <text :x="5" :y="122" class="ciFont1">ci/seg7x/1/set/value</text>
  <text :x="5" :y="147" class="ciFont1">ci/seg7x/2/set/value</text>
  <CiSeg7 :x="180" :y="55" value="8" :height="48" sid= "ciSeg7_1" lines="0" :border="3"></CiSeg7>
  <CiSeg7x :x="180" :y="120" digits="4" value="----" :height="20" sid= "ciSeg7x_1"></CiSeg7x>
  <CiSeg7x :x="180" :y="145" digits="4" value="----" :height="20" sid= "ciSeg7x_2"></CiSeg7x>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiSeg7 from './CiSeg7.vue'
import CiSeg7x from './CiSeg7x.vue'

export default defineComponent({
  name: 'CiMain1',
  components: {
    CiSeg7,
    CiSeg7x
  }
})
</script>

<style>
</style>
