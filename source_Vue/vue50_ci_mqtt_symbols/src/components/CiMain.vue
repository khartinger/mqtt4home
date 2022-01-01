<!--CiMain.vue-->
<template>
  <svg width="100%" viewBox="-50 -70 870 240">
  <rect class="ciBackground" x="-50" y="-70" width="920" height="310" />

  <text :x="-50" :y="-0.55*dy" class="ciFont1">Indicator</text>
  <CiLamp :x="0*dx" :y="0*dy" sid="lamp1" :border="0"></CiLamp>
  <!--
  <CiLamp :x="1*dx" :y="0*dy" sid="lamp1" :border="1"></CiLamp>
  <CiLamp :x="2*dx" :y="0*dy" sid="lamp1" :border="2"></CiLamp>
  <CiLamp :x="3*dx" :y="0*dy" sid="lamp1" :border="3"></CiLamp>
  <CiLamp :x="4*dx" :y="0*dy" sid="lamp1"></CiLamp>
  -->

  <text :x="-50" :y="0.7*dy" class="ciFont1">Controller</text>
  <CiButton :x="0*dx" :y="1.2*dy" sid="button_1" :border="0"></CiButton>
  <CiButton :x="1*dx" :y="1.2*dy" sid="button_2" :border="0"></CiButton>
  <CiButton :x="2*dx" :y="1.2*dy" sid="button_3" :border="0"></CiButton>
  <CiButton :x="3*dx" :y="1.2*dy" sid="button_4" :border="0"></CiButton>
  <CiButton2 :x="4*dx" :y="1.2*dy" sid="button2_1" :border="0"></CiButton2>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geo } from './CiBase.vue'
import CiLamp from './CiLamp.vue'
import CiButton from './CiButton.vue'
import CiButton2 from './CiButton2.vue'

// import CiWeather from './CiWeather.vue'
// import CiMotion from './CiMotion.vue'
// import CiText5 from './CiText5.vue'
// import CiText5L from './CiText5L.vue'
// import CiPump from './CiPump.vue'
// import CiSocket from './CiSocket.vue'
// import CiDoor from './CiDoor.vue'
// import CiBlind from './CiBlind.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiButton,
    CiButton2,
    // CiWeather,
    // CiMotion,
    // CiText5,
    // CiText5L,
    // CiPump,
    // CiSocket,
    // CiDoor,
    // CiBlind,

    CiLamp
  },
  computed: {
    geo: function (): Geo {
      const geo1 = new Geo(0, 0)
      return geo1
    },
    dx: function (): number { return (this.geo.dxo) },
    dy: function (): number { return (this.geo.dyo) }
  }
})
</script>

<style>
</style>
