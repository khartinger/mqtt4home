<!--CiMain.vue-->
<template>
  <svg width="100%" viewBox="-50 -80 870 670">
  <rect class="ciBackground" x="-50" y="-80" width="920" height="750" />

  <text :x="-50" :y="-0.55*dy" class="ciFont2">Controller</text>
  <CiButton  :x="0*dx" :y="0*dy" sid="button_1" :border="0"></CiButton>
  <CiButton  :x="1*dx" :y="0*dy" sid="button_2" :border="0"></CiButton>
  <CiButton  :x="2*dx" :y="0*dy" sid="button_3" :border="0"></CiButton>
  <CiButton  :x="3*dx" :y="0*dy" sid="button_4" :border="0"></CiButton>
  <CiButton2 :x="4*dx" :y="0*dy" sid="button2_1" lines="2" :border="0"></CiButton2>
  <CiSocket  :x="5*dx" :y="0*dy" sid="socket_1" :border="0"></CiSocket>
  <CiSmsOut  :x="6*dx" :y="0*dy" sid="smsOut_1" :border="3"></CiSmsOut>

  <text :x="-50" :y="0.9*dy" class="ciFont2">Indicator</text>
  <CiLamp    :x="0*dx" :y="1.5*dy" sid="lamp_1"    :border="0"></CiLamp>
  <CiMotion  :x="1*dx" :y="1.5*dy" sid="motion_1"  :border="0"></CiMotion>
  <CiPump    :x="2*dx" :y="1.5*dy" sid="pump_1"    :border="0"></CiPump>
  <CiWeather :x="3*dx" :y="1.5*dy" sid="weather_1" :border="0"></CiWeather>
  <CiText5   :x="4*dx" :y="1.5*dy" sid="text5_1" :border="3"></CiText5>
  <CiText5L  :x="5*dx" :y="1.5*dy" sid="text5L_1" :border="3"></CiText5L>
  <CiSmsIn   :x="0*dx" :y="2.5*dy" sid="smsIn_1"  :border="3"></CiSmsIn>

  <CiDoor   :x="0.0*dx" :y="3.8*dy" sid="doorh1" dir="h1" :border="0"></CiDoor>
  <CiDoor   :x="1.5*dx" :y="3.8*dy" sid="doorD4x" dir="D4x" :border="0"></CiDoor>
  <CiWindow :x="3.0*dx" :y="3.8*dy" sid="window_1" dir="V4" :border="0"></CiWindow>
  <CiWindow :x="4.5*dx" :y="3.8*dy" sid="window_2" dir="d1x" :f="2.0" :border="1"></CiWindow>
  <CiBlind  :x="6.0*dx" :y="3.8*dy" sid="blind_1" dir="h1" :border="0"></CiBlind>
  <CiBlind  :x="7.5*dx" :y="3.8*dy" :f="1.8" sid="blind_2" dir="V1" :border="0"></CiBlind>

  <text :x="-50" :y="4.7*dy" class="ciFont2">Miscellaneous</text>
  <CiWall :x="0*dx" :y="5.3*dy" sid="w" type="9" title="    Wall" :border="0"></CiWall>
  <CiWall :x="1*dx" :y="5.3*dy" sid="w" type="7" text5="example: children's room" :fx="3" :border="0"></CiWall>
  <CiWall :x="3*dx" :y="5.3*dy" sid="w" type="21" :border="0"></CiWall>
  <CiWall :x="4*dx" :y="5.3*dy" sid="w" type="0x10C" :border="0"></CiWall>

  <CiTimeDate :x="5*dx" :y="5.3*dy" sid="timeDate_1" :border="2"></CiTimeDate>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geo } from './CiBase.vue'
import CiBlind from './CiBlind.vue'
import CiButton from './CiButton.vue'
import CiButton2 from './CiButton2.vue'
import CiDoor from './CiDoor.vue'
import CiLamp from './CiLamp.vue'
import CiMotion from './CiMotion.vue'
import CiPump from './CiPump.vue'
import CiSmsIn from './CiSmsIn.vue'
import CiSmsOut from './CiSmsOut.vue'
import CiSocket from './CiSocket.vue'
import CiText5 from './CiText5.vue'
import CiText5L from './CiText5L.vue'
import CiTimeDate from './CiTimeDate.vue'
import CiWeather from './CiWeather.vue'
import CiWall from './CiWall.vue'
import CiWindow from './CiWindow.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiBlind,
    CiButton,
    CiButton2,
    CiDoor,
    CiLamp,
    CiMotion,
    CiPump,
    CiSmsIn,
    CiSmsOut,
    CiSocket,
    CiText5,
    CiText5L,
    CiTimeDate,
    CiWeather,
    CiWall,
    CiWindow
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
