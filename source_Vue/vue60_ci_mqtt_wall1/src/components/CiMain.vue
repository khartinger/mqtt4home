<!--CiMain.vue-->
<template>
<svg width="100%" viewBox="0 0 1600 510">
  <rect class="ciBackground" x="0" y="0" width="1600" height="510" />

  <text :x="1" :y="0.25*dy" class="ciFont2">Basic Elements</text>
  <text   :x="0.2*dx" :y="0.45*dy" class="ciFont1">0 = 0x00</text>
  <CiWall :x="0.5*dx" :y="1.0*dy"  sid="w01" type="0" :border="1"></CiWall>
  <text   :x="2.0*dx" :y="0.45*dy" class="ciFont1">1</text>
  <CiWall :x="2.0*dx" :y="1.0*dy"  sid="w01" type="1" :border="1"></CiWall>
  <text   :x="3.5*dx" :y="0.45*dy" class="ciFont1">2</text>
  <CiWall :x="3.5*dx" :y="1.0*dy"  sid="w01" type="2" :border="1"></CiWall>
  <text   :x="5.0*dx" :y="0.45*dy" class="ciFont1">4</text>
  <CiWall :x="5.0*dx" :y="1.0*dy"  sid="w01" type="4" :border="1"></CiWall>
  <text   :x="6.5*dx" :y="0.45*dy" class="ciFont1">8</text>
  <CiWall :x="6.5*dx" :y="1.0*dy"  sid="w01" type="8" :border="1"></CiWall>
  <text   :x="8.0*dx" :y="0.45*dy" class="ciFont1">16</text>
  <CiWall :x="8.0*dx" :y="1.0*dy"  sid="w01" type="16" :border="1"></CiWall>
  <text   :x="9.5*dx" :y="0.45*dy" class="ciFont1">32</text>
  <CiWall :x="9.5*dx" :y="1.0*dy"  sid="w01" type="32" :border="1"></CiWall>
  <text   :x="11.0*dx" :y="0.45*dy" class="ciFont1">64</text>
  <CiWall :x="11.0*dx" :y="1.0*dy"  sid="w01" type="64" :border="1"></CiWall>
  <text   :x="12.1*dx" :y="0.45*dy" class="ciFont1">128 = 0x80</text>
  <CiWall :x="12.5*dx" :y="1.0*dy"  sid="w01" type="128" :border="1"></CiWall>
  <text   :x="13.5*dx" :y="0.45*dy" class="ciFont1">256 = 0x100</text>
  <CiWall :x="14.0*dx" :y="1.0*dy"  sid="w01" type="256" :border="1"></CiWall>
  <text   :x="15.0*dx" :y="0.45*dy" class="ciFont1">512 = 0x200</text>
  <CiWall :x="15.5*dx" :y="1.0*dy"  sid="w01" type="0x200" :border="1"></CiWall>

  <text :x="1" :y="1.85*dy" class="ciFont2">Composite Elements</text>
  <text   :x="0.05*dx" :y="2.05*dy" class="ciFont1">257 = 0x101</text>
  <CiWall :x="0.5*dx" :y="2.6*dy"  sid="w01" type="0x101" :border="1"></CiWall>
  <text   :x="1.2*dx" :y="2.05*dy" class="ciFont1">0x210 = 16+512 = 528</text>
  <CiWall :x="2.0*dx" :y="2.6*dy"  sid="w01" type="0x210" :border="1"></CiWall>
  <text   :x="3.1*dx" :y="2.05*dy" class="ciFont1">1 + 2 = 3</text>
  <CiWall :x="3.5*dx" :y="2.6*dy"  sid="w01" type="3" :border="1"></CiWall>
  <text   :x="4.8*dx" :y="2.05*dy" class="ciFont1">0x103</text>
  <CiWall :x="5.0*dx" :y="2.6*dy"  sid="w01" type="0x103" :border="1"></CiWall>
  <text   :x="6.1*dx" :y="2.05*dy" class="ciFont1">1 + 4 = 5</text>
  <CiWall :x="6.5*dx" :y="2.6*dy"  sid="w01" type="5" :border="1"></CiWall>
  <text   :x="7.6*dx" :y="2.05*dy" class="ciFont1">2 + 8 = 10</text>
  <CiWall :x="8.0*dx" :y="2.6*dy"  sid="w01" type="10" :border="1"></CiWall>

  <text   :x="9.1*dx" :y="2.05*dy" class="ciFont1">16 + 32 = 48</text>
  <CiWall :x="9.5*dx" :y="2.6*dy"  sid="w01" type="48" :border="1"></CiWall>
  <text   :x="10.8*dx" :y="2.05*dy" class="ciFont1">0x230</text>
  <CiWall :x="11.0*dx" :y="2.6*dy"  sid="w01" type="0x230" :border="1"></CiWall>

  <text   :x="12.1*dx" :y="2.05*dy" class="ciFont1">1+2+8 = 11</text>
  <CiWall :x="12.5*dx" :y="2.6*dy"  sid="w01" type="11" :border="1"></CiWall>
  <text   :x="13.1*dx" :y="2.05*dy" class="ciFont1">1+4+8 = 13</text>
  <CiWall :x="13.5*dx" :y="2.6*dy"  sid="w01" type="13" :border="1"></CiWall>
  <text   :x="14.1*dx" :y="2.05*dy" class="ciFont1">1+4+16 = 21</text>
  <CiWall :x="14.5*dx" :y="2.6*dy"  sid="w01" type="21" :border="1"></CiWall>
  <text   :x="15.2*dx" :y="2.05*dy" class="ciFont1">4+8 = 12</text>
  <CiWall :x="15.5*dx" :y="2.6*dy"  sid="w01" type="12" :border="1"></CiWall>

  <text :x="1" :y="3.45*dy" class="ciFont2">Stretched Elements</text>
  <text   :x="0.2*dx" :y="3.65*dy" class="ciFont1">1 fx=2</text>
  <CiWall :x="0.5*dx" :y="4.2*dy"   sid="w" type="1" :fx="2" :border="1"></CiWall>
  <text   :x="1.7*dx" :y="3.65*dy" class="ciFont1">2 &nbsp; &nbsp; fy=-1.5</text>
  <CiWall :x="2.0*dx" :y="4.2*dy"   sid="w" type="2" :fy="-1.25" :border="1"></CiWall>
  <text   :x="3.2*dx" :y="3.65*dy" class="ciFont1">4 fx=-1.25</text>
  <CiWall :x="3.5*dx" :y="4.2*dy"   sid="w" type="4" :fx="-1.5" :border="1"></CiWall>
  <text   :x="4.7*dx" :y="3.65*dy" class="ciFont1">8 fy=1.5</text>
  <CiWall :x="5.0*dx" :y="4.2*dy"   sid="w" type="8" :fy="1.5" :border="1"></CiWall>

  <text   :x="6.1*dx" :y="3.65*dy" class="ciFont1">16 fx=1.5</text>
  <CiWall :x="6.5*dx" :y="4.2*dy"   sid="w" type="16" :fx="1.5" :border="1"></CiWall>
  <text   :x="7.7*dx" :y="3.65*dy" class="ciFont1">32 fy=-1.5</text>
  <CiWall :x="8.0*dx" :y="4.2*dy"   sid="w" type="32" :fy="-1.5" :border="1"></CiWall>
  <text   :x="9.1*dx" :y="3.65*dy" class="ciFont1">64 fx=-1.5</text>
  <CiWall :x="9.5*dx" :y="4.2*dy"   sid="w" type="64" :fx="-1.5" :border="1"></CiWall>
  <text   :x="10.6*dx" :y="3.65*dy" class="ciFont1">128 fy=1.5</text>
  <CiWall :x="11.0*dx" :y="4.2*dy"   sid="w" type="128" :fy="1.5" :border="1"></CiWall>

  <text   :x="12.0*dx" :y="3.65*dy" class="ciFont1">9 fx=5 fy=0.5</text>
  <CiWall :x="12.5*dx" :y="4.2*dy"   sid="w" type="9" :fx="5" :fy="0.5" :border="1"></CiWall>
  <text   :x="13.5*dx" :y="3.65*dy" class="ciFont1">0</text>
  <CiWall :x="13.5*dx" :y="4.2*dy"   sid="w" type="0" :border="1"></CiWall>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geo } from './CiBase.vue'
import CiWall from './CiWall.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiWall
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
