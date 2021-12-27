<!--CiMain.vue-->
<template>
<svg width="100%" viewBox="-10 -15 1620 360">
  <rect class="ciBackground" x="-10" y="-15" width="1630" height="375" />
  <text :x="0.5*dx" :y="-0.05*dy" class="ciFont1">0</text>
  <text :x="1.5*dx" :y="-0.05*dy" class="ciFont1">1</text>
  <text :x="2.5*dx" :y="-0.05*dy" class="ciFont1">2</text>
  <text :x="3.5*dx" :y="-0.05*dy" class="ciFont1">3</text>
  <text :x="4.5*dx" :y="-0.05*dy" class="ciFont1">4</text>
  <text :x="5.5*dx" :y="-0.05*dy" class="ciFont1">5</text>
  <text :x="6.5*dx" :y="-0.05*dy" class="ciFont1">6</text>
  <text :x="7.5*dx" :y="-0.05*dy" class="ciFont1">7</text>
  <text :x="8.5*dx" :y="-0.05*dy" class="ciFont1">8</text>
  <text :x="9.5*dx" :y="-0.05*dy" class="ciFont1">9</text>
  <text :x="10.5*dx" :y="-0.05*dy" class="ciFont1">10</text>
  <text :x="11.5*dx" :y="-0.05*dy" class="ciFont1">11</text>
  <text :x="12.5*dx" :y="-0.05*dy" class="ciFont1">12</text>
  <text :x="13.5*dx" :y="-0.05*dy" class="ciFont1">13</text>
  <text :x="14.5*dx" :y="-0.05*dy" class="ciFont1">14</text>
  <text :x="15.5*dx" :y="-0.05*dy" class="ciFont1">15</text>

  <CiWall :x="0.5*dx" :y="0.5*dy" sid="w01" :type="0" :border="1"></CiWall>
  <CiWall :x="1.5*dx" :y="0.5*dy" sid="w02" :type="1" :border="1"></CiWall>
  <CiWall :x="2.5*dx" :y="0.5*dy" sid="w03" :type="2" :border="1"></CiWall>
  <CiWall :x="3.5*dx" :y="0.5*dy" sid="w04" :type="3" :border="1"></CiWall>
  <CiWall :x="4.5*dx" :y="0.5*dy" sid="w05" :type="4" :border="1"></CiWall>
  <CiWall :x="5.5*dx" :y="0.5*dy" sid="w06" :type="5" :border="1"></CiWall>
  <CiWall :x="6.5*dx" :y="0.5*dy" sid="w07" :type="6" :border="1"></CiWall>
  <CiWall :x="7.5*dx" :y="0.5*dy" sid="w08" :type="7" :border="1"></CiWall>
  <CiWall :x="8.5*dx" :y="0.5*dy" sid="w09" :type="8"  :border="1"></CiWall>
  <CiWall :x="9.5*dx" :y="0.5*dy" sid="w10" :type="9" :border="1"></CiWall>
  <CiWall :x="10.5*dx" :y="0.5*dy" sid="w11" :type="10" :border="1"></CiWall>
  <CiWall :x="11.5*dx" :y="0.5*dy" sid="w12" :type="11" :border="1"></CiWall>
  <CiWall :x="12.5*dx" :y="0.5*dy" sid="w13" :type="12" :border="1"></CiWall>
  <CiWall :x="13.5*dx" :y="0.5*dy" sid="w14" :type="13" :border="1"></CiWall>
  <CiWall :x="14.5*dx" :y="0.5*dy" sid="w15" :type="14" :border="1"></CiWall>
  <CiWall :x="15.5*dx" :y="0.5*dy" sid="w15" :type="15" :border="1"></CiWall>

  <text :x="0.5*dx" :y="1.15*dy" class="ciFont1">16</text>
  <text :x="1.5*dx" :y="1.15*dy" class="ciFont1">17</text>
  <text :x="2.5*dx" :y="1.15*dy" class="ciFont1">18</text>
  <text :x="3.5*dx" :y="1.15*dy" class="ciFont1">19</text>
  <text :x="4.5*dx" :y="1.15*dy" class="ciFont1">20</text>
  <text :x="5.5*dx" :y="1.15*dy" class="ciFont1">21</text>
  <text :x="6.5*dx" :y="1.15*dy" class="ciFont1">22</text>
  <text :x="7.5*dx" :y="1.15*dy" class="ciFont1">23</text>
  <text :x="8.5*dx" :y="1.15*dy" class="ciFont1">24</text>
  <text :x="9.5*dx" :y="1.15*dy" class="ciFont1">25</text>
  <text :x="10.5*dx" :y="1.15*dy" class="ciFont1">26</text>
  <text :x="11.5*dx" :y="1.15*dy" class="ciFont1">27</text>
  <text :x="12.5*dx" :y="1.15*dy" class="ciFont1">28</text>
  <text :x="13.5*dx" :y="1.15*dy" class="ciFont1">29</text>
  <text :x="14.5*dx" :y="1.15*dy" class="ciFont1">30</text>
  <text :x="15.5*dx" :y="1.15*dy" class="ciFont1">31</text>

  <CiWall :x="0.5*dx" :y="1.7*dy" sid="w09" :type="16"  :border="1"></CiWall>
  <CiWall :x="1.5*dx" :y="1.7*dy" sid="w10" :type="17" :border="1"></CiWall>
  <CiWall :x="2.5*dx" :y="1.7*dy" sid="w11" :type="18" :border="1"></CiWall>
  <CiWall :x="3.5*dx" :y="1.7*dy" sid="w12" :type="19" :border="1"></CiWall>
  <CiWall :x="4.5*dx" :y="1.7*dy" sid="w13" :type="20" :border="1"></CiWall>
  <CiWall :x="5.5*dx" :y="1.7*dy" sid="w14" :type="21" :border="1"></CiWall>
  <CiWall :x="6.5*dx" :y="1.7*dy" sid="w15" :type="22" :border="1"></CiWall>
  <CiWall :x="7.5*dx" :y="1.7*dy" sid="w15" :type="23" :border="1"></CiWall>
  <CiWall :x="8.5*dx" :y="1.7*dy" sid="w09" :type="24"  :border="1"></CiWall>
  <CiWall :x="9.5*dx" :y="1.7*dy" sid="w10" :type="25" :border="1"></CiWall>
  <CiWall :x="10.5*dx" :y="1.7*dy" sid="w11" :type="26" :border="1"></CiWall>
  <CiWall :x="11.5*dx" :y="1.7*dy" sid="w12" :type="27" :border="1"></CiWall>
  <CiWall :x="12.5*dx" :y="1.7*dy" sid="w13" :type="28" :border="1"></CiWall>
  <CiWall :x="13.5*dx" :y="1.7*dy" sid="w14" :type="29" :border="1"></CiWall>
  <CiWall :x="14.5*dx" :y="1.7*dy" sid="w15" :type="30" :border="1"></CiWall>
  <CiWall :x="15.5*dx" :y="1.7*dy" sid="w15" :type="31" :border="1"></CiWall>

  <text   :x="0.5*dx" :y="2.35*dy" class="ciFont1">32</text>
  <CiWall :x="0.5*dx" :y="2.9*dy"   sid="w" :type="32"  :border="1"></CiWall>
  <text   :x="1.5*dx" :y="2.35*dy" class="ciFont1">64</text>
  <CiWall :x="1.5*dx" :y="2.9*dy"   sid="w" :type="64" :border="1"></CiWall>
  <text   :x="2.5*dx" :y="2.35*dy" class="ciFont1">128</text>
  <CiWall :x="2.5*dx" :y="2.9*dy"   sid="w" :type="128" :border="1"></CiWall>

  <text   :x="4.2*dx" :y="2.35*dy" class="ciFont1">16+32=48</text>
  <CiWall :x="4.5*dx" :y="2.9*dy"   sid="w" :type="48" :border="1"></CiWall>
  <text   :x="5.2*dx" :y="2.35*dy" class="ciFont1">32+64=96</text>
  <CiWall :x="5.5*dx" :y="2.9*dy"   sid="w" :type="96" :border="1"></CiWall>
  <text   :x="6.2*dx" :y="2.35*dy" class="ciFont1">16+64=80</text>
  <CiWall :x="6.5*dx" :y="2.9*dy"   sid="w" :type="80" :border="1"></CiWall>
  <text   :x="7.2*dx" :y="2.35*dy" class="ciFont1">32+128=160</text>
  <CiWall :x="7.5*dx" :y="2.9*dy"   sid="w" :type="160" :border="1"></CiWall>

  <text   :x="9.3*dx" :y="2.35*dy" class="ciFont1">16+12=28</text>
  <CiWall :x="9.5*dx" :y="2.9*dy"   sid="w" :type="28" :border="2"></CiWall>
  <text   :x="10.3*dx" :y="2.35*dy" class="ciFont1">64+3=67</text>
  <CiWall :x="10.5*dx" :y="2.9*dy"   sid="w" :type="67" :border="3"></CiWall>
  <text   :x="11.3*dx" :y="2.35*dy" class="ciFont1">32+64+1=97</text>
  <CiWall :x="11.5*dx" :y="2.9*dy"   sid="w" :type="97" :border="0"></CiWall>
  <text   :x="12.3*dx" :y="2.35*dy" class="ciFont1">128+4=132</text>
  <CiWall :x="12.5*dx" :y="2.9*dy"   sid="w" :type="132"></CiWall>

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
