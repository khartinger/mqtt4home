<!--CiMain.vue-->
<template>
  <svg width="100%" viewBox="0 0 1200 1570">
  <rect class="ciBackground" x="0" y="0" width="1200" height="1570" />
<!--
<CiDoor   :x="8.0*dx" :y="6.8*dy" sid="door_h1" dir="h1" :border="0"></CiDoor>
-->
<CiWindow :x="6.5*dx" :y="6.8*dy" sid="window_h1" dir="h1" :border="0"></CiWindow>
<CiWindow :x="8.0*dx" :y="6.8*dy" sid="window_h1" dir="h1" shape="rect" :border="0"></CiWindow>
<CiWindow :x="9.5*dx" :y="6.8*dy" sid="window_H1" dir="H1" :border="0"></CiWindow>
<CiWindow :x="11.0*dx" :y="6.8*dy" sid="window_H1" dir="H1" shape="rect" :border="0"></CiWindow>

  <text :x="0.0*dx" :y="0.3*dy" class="ciFont2">CiDoor</text>
  <CiDoor   :x="0.5*dx" :y="1.0*dy" sid="door_h1" dir="h1" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="1.0*dy" sid="door_H1" dir="H1" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="1.0*dy" sid="door_h2" dir="h2" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="1.0*dy" sid="door_H2" dir="H2" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="1.0*dy" sid="door_h3" dir="h3" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="1.0*dy" sid="door_H3" dir="H3" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="1.0*dy" sid="door_h4" dir="h4" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="1.0*dy" sid="door_H4" dir="H4" :border="0"></CiDoor>

  <CiDoor   :x="0.5*dx" :y="2.5*dy" sid="door_v1" dir="v1" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="2.5*dy" sid="door_V1" dir="V1" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="2.5*dy" sid="door_v2" dir="v2" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="2.5*dy" sid="door_V2" dir="V2" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="2.5*dy" sid="door_v3" dir="v3" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="2.5*dy" sid="door_V3" dir="V3" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="2.5*dy" sid="door_v4" dir="v4" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="2.5*dy" sid="door_V4" dir="V4" :border="0"></CiDoor>

  <CiDoor   :x="0.5*dx" :y="4.0*dy" sid="door_d1x" dir="d1x" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="4.0*dy" sid="door_D1x" dir="D1x" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="4.0*dy" sid="door_d1y" dir="d1y" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="4.0*dy" sid="door_D1y" dir="D1y" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="4.0*dy" sid="door_d2x" dir="d2x" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="4.0*dy" sid="door_D2x" dir="D2x" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="4.0*dy" sid="door_d2y" dir="d2y" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="4.0*dy" sid="door_D2y" dir="D2y" :border="0"></CiDoor>

  <CiDoor   :x="0.5*dx" :y="5.5*dy" sid="door_d3x" dir="d3x" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="5.5*dy" sid="door_D3x" dir="D3x" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="5.5*dy" sid="door_d3y" dir="d3y" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="5.5*dy" sid="door_D3y" dir="D3y" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="5.5*dy" sid="door_d4x" dir="d4x" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="5.5*dy" sid="door_D4x" dir="D4x" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="5.5*dy" sid="door_d4y" dir="d4y" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="5.5*dy" sid="door_D4y" dir="D4y" :border="0"></CiDoor>

  <CiDoor   :x="1.0*dx" :y="6.8*dy" sid="door_h1" dir="h1" :f="2" text5=":f=&quot;2&quot;" :border="1"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="6.8*dy" sid="door_d1x" dir="d1x" :f="2" text5=":f=&quot;2&quot;" :border="1"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="6.8*dy" sid="door_batt" dir="h1" :border="0"></CiDoor>

  <text :x="0.0*dx" :y="7.8*dy" class="ciFont2">CiWindow</text>
  <CiWindow :x="0.5*dx" :y="8.5*dy" sid="window_h1" dir="h1" :border="0"></CiWindow>
  <CiWindow :x="2.0*dx" :y="8.5*dy" sid="window_H1" dir="H1" :border="0"></CiWindow>
  <CiWindow :x="3.5*dx" :y="8.5*dy" sid="window_h2" dir="h2" :border="0"></CiWindow>
  <CiWindow :x="5.0*dx" :y="8.5*dy" sid="window_H2" dir="H2" :border="0"></CiWindow>
  <CiWindow :x="6.5*dx" :y="8.5*dy" sid="window_h3" dir="h3" :border="0"></CiWindow>
  <CiWindow :x="8.0*dx" :y="8.5*dy" sid="window_H3" dir="H3" :border="0"></CiWindow>
  <CiWindow :x="9.5*dx" :y="8.5*dy" sid="window_h4" dir="h4" :border="0"></CiWindow>
  <CiWindow :x="11.0*dx" :y="8.5*dy" sid="window_H4" dir="H4" :border="0"></CiWindow>

  <CiWindow :x="0.5*dx" :y="10.0*dy" sid="window_v1"  dir="v1" :border="0"></CiWindow>
  <CiWindow :x="2.0*dx" :y="10.0*dy" sid="window_V1"  dir="V1" :border="0"></CiWindow>
  <CiWindow :x="3.5*dx" :y="10.0*dy" sid="window_d1x" dir="d1x" :f="2" text5=":f=&quot;2&quot;" :border="0"></CiWindow>
  <CiWindow :x="5.0*dx" :y="10.0*dy" sid="window_D1x" dir="D1x" :border="0"></CiWindow>
  <CiWindow :x="6.5*dx" :y="10.0*dy" sid="window_v1"  dir="v1"  shape="rect" text5="shape=&quot;rect&quot;" :border="0"></CiWindow>
  <CiWindow :x="8.0*dx" :y="10.0*dy" sid="window_V1"  dir="V1"  shape="rect" text5="shape=&quot;rect&quot;" :border="0"></CiWindow>
  <CiWindow :x="9.5*dx" :y="10.0*dy" sid="window_d1x" dir="d1x" shape="rect" text5="shape=&quot;rect&quot;" :border="0"></CiWindow>
  <CiWindow :x="11.0*dx" :y="10.0*dy" sid="window_D1x" dir="D1x" shape="rect" text5="shape=&quot;rect&quot;" :border="0"></CiWindow>

  <text :x="0.0*dx" :y="11.3*dy" class="ciFont2">CiBlind</text>
  <CiWindow :x="3.5*dx" :y="12.0*dy" sid=""  dir="h1" :border="0"></CiWindow>
  <CiWindow :x="5.0*dx" :y="12.0*dy" sid=""  dir="h1" :border="0"></CiWindow>
  <CiWindow :x="9.5*dx" :y="12.0*dy" sid=""  dir="h1" :border="0"></CiWindow>
  <CiWindow :x="11.0*dx" :y="12.0*dy" sid="" dir="h1" :border="0"></CiWindow>
  <CiBlind  :x="0.5*dx" :y="12.0*dy" sid="blind_h1" dir="h1" :border="0"></CiBlind>
  <CiBlind  :x="2.0*dx" :y="12.0*dy" sid="blind_H1" dir="H1" :border="0"></CiBlind>
  <CiBlind  :x="3.5*dx" :y="12.0*dy" sid="blind_h2" dir="h2" :border="1"></CiBlind>
  <CiBlind  :x="5.0*dx" :y="12.0*dy" sid="blind_H2" dir="H2" :border="1"></CiBlind>
  <CiBlind  :x="6.5*dx" :y="12.0*dy" sid="blind_h3" dir="h3" :border="0"></CiBlind>
  <CiBlind  :x="8.0*dx" :y="12.0*dy" sid="blind_H3" dir="H3" :border="0"></CiBlind>
  <CiBlind  :x="9.5*dx" :y="12.0*dy" sid="blind_h4" dir="h4" :border="1"></CiBlind>
  <CiBlind  :x="11.0*dx" :y="12.0*dy" sid="blind_H4" dir="H4" :border="1"></CiBlind>

  <CiBlind  :x="0.5*dx" :y="13.5*dy" sid="blind_v1" dir="v1" :border="1"></CiBlind>
  <CiBlind  :x="2.0*dx" :y="13.5*dy" sid="blind_V1" dir="V1" :border="1"></CiBlind>
  <CiBlind  :x="3.5*dx" :y="13.5*dy" sid="blind_v2" dir="v2" :border="1"></CiBlind>
  <CiBlind  :x="5.0*dx" :y="13.5*dy" sid="blind_V2" dir="V2" :border="1"></CiBlind>
  <CiBlind  :x="6.5*dx" :y="13.5*dy" sid="blind_v3" dir="v3" :border="1"></CiBlind>
  <CiBlind  :x="8.0*dx" :y="13.5*dy" sid="blind_V3" dir="V3" :border="1"></CiBlind>
  <CiBlind  :x="9.5*dx" :y="13.5*dy" sid="blind_v4" dir="v4" :border="1"></CiBlind>
  <CiBlind  :x="11.0*dx" :y="13.5*dy" sid="blind_V4" dir="V4" :border="1"></CiBlind>

  <CiWindow :x="0.5*dx" :y="15.0*dy" sid=""  dir="d1x" :border="0"></CiWindow>
  <CiWindow :x="2.0*dx" :y="15.0*dy" sid=""  dir="d1y" :border="0"></CiWindow>
  <CiWindow :x="3.5*dx" :y="15.0*dy" sid=""  dir="d2x" :border="0"></CiWindow>
  <CiWindow :x="5.0*dx" :y="15.0*dy" sid="" dir="d2y" :border="0"></CiWindow>
  <CiBlind  :x="0.5*dx" :y="15.0*dy" sid="blind_d1x" dir="d1x" :border="1"></CiBlind>
  <CiBlind  :x="2.0*dx" :y="15.0*dy" sid="blind_d1y" dir="d1y" :border="1"></CiBlind>
  <CiBlind  :x="3.5*dx" :y="15.0*dy" sid="blind_d2x" dir="d2x" :border="1"></CiBlind>
  <CiBlind  :x="5.0*dx" :y="15.0*dy" sid="blind_d2y" dir="d2y" :border="1"></CiBlind>

  <CiWindow :x="6.5*dx" :y="15.0*dy" sid=""  dir="d3x" :border="0"></CiWindow>
  <CiWindow :x="8.0*dx" :y="15.0*dy" sid=""  dir="d3y" :border="0"></CiWindow>
  <CiWindow :x="9.5*dx" :y="15.0*dy" sid=""  dir="d4x" :border="0"></CiWindow>
  <CiWindow :x="11.0*dx" :y="15.0*dy" sid="" dir="d4y" :border="0"></CiWindow>
  <CiBlind  :x="6.5*dx" :y="15.0*dy" sid="blind_D3x" dir="D3x" :border="1"></CiBlind>
  <CiBlind  :x="8.0*dx" :y="15.0*dy" sid="blind_D3y" dir="D3y" :border="1"></CiBlind>
  <CiBlind  :x="9.5*dx" :y="15.0*dy" sid="blind_D4x" dir="D4x" :border="1"></CiBlind>
  <CiBlind  :x="11.0*dx" :y="15.0*dy" sid="blind_D4y" dir="D4y" :border="1"></CiBlind>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geo } from './CiBase.vue'
import CiBlind from './CiBlind.vue'
import CiDoor from './CiDoor.vue'
import CiWindow from './CiWindow.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiBlind,
    CiDoor,
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
