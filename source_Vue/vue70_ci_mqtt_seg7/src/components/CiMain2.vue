<!--CiMain2.vue-->
<template>
<div></div>

<svg width="100%" viewBox="0 0 220 50">
  <rect class="ciBackground" x="0" y="0" width="220" height="50" />
  <CiSeg7x :x="110" :y="25" :value="value2" digits="6" :height="36" sid="ciSeg7x_3" :border="0"> </CiSeg7x>
</svg>

</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiSeg7x from './CiSeg7x.vue'

const TESTVALUE = ['0.123', '1.23', '12.3', '123.', '3-2=1', '12:34:56', '12.03.22', '+1+64+8', '+128+128+128']

export default defineComponent({
  name: 'CiMain2',
  components: {
    CiSeg7x
  },
  data () {
    return {
      cnt2: 0 as number,
      value2: '11.11' as string,
      intervalHandle: null as any
    }
  },
  computed: {
    value1: function (): string {
      return TESTVALUE[2]
    }
  },
  mounted: function (): void {
    this.intervalHandle = setInterval(() => {
      this.value2 = TESTVALUE[this.cnt2]
      this.cnt2++
      if (this.cnt2 > 8) this.cnt2 = 0
    }, 2000)
  },
  unmounted: function (): void {
    clearInterval(this.intervalHandle)
  }
})
</script>

<style>
</style>
