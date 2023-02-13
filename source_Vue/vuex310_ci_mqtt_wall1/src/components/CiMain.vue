<!-- CiMain.vue (vuex310_ci_mqtt_wall1)-----------khartinger -->
<!-- 2023-02-12: new                                         -->
<template>
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiWall1 :x="50" :y="70" sid="wall1" dir="h-h3" :border="1"></CiWall1>
  <CiWall1 :x="50" :y="170" sid="wall1" dir="w3h-h3" :border="1"></CiWall1>
  <CiWall1 :x="300" :y="70" sid="wall1" dir="c3" :border="1"></CiWall1>
  <CiWall1 :x="300" :y="170" sid="wall1" dir="h-1.5d2t-.5" :border="1"></CiWall1>
  -->
  <!--
  <CiWall1 :x="50" :y="70" sid="wall1" dir="h+v+q" :border="3"></CiWall1>
  <CiWall1 :x="50" :y="70" sid="wall1" dir="h+v+q" :border="3"></CiWall1>
  -->
  <!--
  <CiWall1 :x="300" :y="120" sid="textarea1" dir="h-d-v-t-" :border="1"></CiWall1>
  <CiWall1 :x="100" :y="300" sid="textarea1" dir="w2h+2d+2v2t2" :border="1"></CiWall1>
  <CiWall1 :x="300" :y="300" sid="textarea1" dir="h-1.5d-1.5v-1.5t-1.5" :border="1"></CiWall1>

  <CiWall1 :x="420" :y="100" sid="textarea1" dir="h+" :border="1"></CiWall1>
  <CiWall1 :x="420" :y="210" sid="textarea1" dir="q" :border="1"></CiWall1>
  <CiWall1 :x="420" :y="320" sid="textarea1" dir="k" :border="1"></CiWall1>
-->
  <svg width="100%" viewBox="-60 0 670 1130">
  <rect class="ciBackground" x="-60" y="0" width="670" height="1130" />
  <text x="-49"  y="20" class="ciFont1">&lt;CiWall1 :x="0" :y="0" sid="wall1" dir="h+" :border="1" /&gt;</text>
  <line x1="-60" y1="28" x2="720" y2="28" stroke="black" stroke-width="2"/>

  <text x="-49"  :y="0.7*dy" class="ciFont2">Basic Elements: dir=</text>

  <CiWall1 :x="0.0*dx" :y="1.3*dy" sid="wall1" dir="h+" :border="1"></CiWall1>
  <CiWall1 :x="1.1*dx" :y="1.3*dy" sid="wall1" dir="v+" :border="1"></CiWall1>
  <CiWall1 :x="2.2*dx" :y="1.3*dy" sid="wall1" dir="d+" :border="1"></CiWall1>
  <CiWall1 :x="3.3*dx" :y="1.3*dy" sid="wall1" dir="t+" :border="1"></CiWall1>
  <CiWall1 :x="4.4*dx" :y="1.3*dy" sid="wall1" dir=""   :border="1"></CiWall1>
  <CiWall1 :x="5.5*dx" :y="1.3*dy" sid="wall1" dir="q"  :border="1"></CiWall1>
  <text :x="-0.3*dx"   :y="2.0*dy" class="ciFont1">dir="h+"</text>
  <text :x="0.8*dx"    :y="2.0*dy" class="ciFont1">dir="v+"</text>
  <text :x="1.9*dx"    :y="2.0*dy" class="ciFont1">dir="d+"</text>
  <text :x="3.0*dx"    :y="2.0*dy" class="ciFont1">dir="t+"</text>
  <text :x="4.1*dx"    :y="2.0*dy" class="ciFont1">dir=""</text>
  <text :x="5.2*dx"    :y="2.0*dy" class="ciFont1">dir="q"</text>

  <CiWall1 :x="0.0*dx" :y="2.6*dy" sid="wall1" dir="h-" :border="1"></CiWall1>
  <CiWall1 :x="1.1*dx" :y="2.6*dy" sid="wall1" dir="v-" :border="1"></CiWall1>
  <CiWall1 :x="2.2*dx" :y="2.6*dy" sid="wall1" dir="d-" :border="1"></CiWall1>
  <CiWall1 :x="3.3*dx" :y="2.6*dy" sid="wall1" dir="t-" :border="1"></CiWall1>
  <CiWall1 :x="4.4*dx" :y="2.6*dy" sid="wall1" dir="k"  :border="1"></CiWall1>
  <CiWall1 :x="5.5*dx" :y="2.6*dy" sid="wall1" dir="c"  :border="1"></CiWall1>

  <text :x="-0.3*dx"   :y="3.3*dy" class="ciFont1">dir="h-"</text>
  <text :x="0.8*dx"    :y="3.3*dy" class="ciFont1">dir="v-"</text>
  <text :x="1.9*dx"    :y="3.3*dy" class="ciFont1">dir="d-"</text>
  <text :x="3.0*dx"    :y="3.3*dy" class="ciFont1">dir="t-"</text>
  <text :x="4.1*dx"    :y="3.3*dy" class="ciFont1">dir="k"</text>
  <text :x="5.2*dx"    :y="3.3*dy" class="ciFont1">dir="c"</text>

  <text x="-49"        :y="3.7*dy" class="ciFont2">Composite Elements: dir=</text>

  <CiWall1 :x="0.0*dx" :y="4.3*dy" sid="wall1" dir="h" :border="1"></CiWall1>
  <CiWall1 :x="1.1*dx" :y="4.3*dy" sid="wall1" dir="v" :border="1"></CiWall1>
  <CiWall1 :x="2.2*dx" :y="4.3*dy" sid="wall1" dir="d" :border="1"></CiWall1>
  <CiWall1 :x="3.3*dx" :y="4.3*dy" sid="wall1" dir="t" :border="1"></CiWall1>
  <CiWall1 :x="4.4*dx" :y="4.3*dy" sid="wall1" dir="h+q" :border="1"></CiWall1>
  <CiWall1 :x="5.5*dx" :y="4.3*dy" sid="wall1" dir="d+k" :border="1"></CiWall1>
  <text :x="-0.3*dx"   :y="5.0*dy" class="ciFont1">dir="h"</text>
  <text :x="0.8*dx"    :y="5.0*dy" class="ciFont1">dir="v"</text>
  <text :x="1.9*dx"    :y="5.0*dy" class="ciFont1">dir="d"</text>
  <text :x="3.0*dx"    :y="5.0*dy" class="ciFont1">dir="t"</text>
  <text :x="4.1*dx"    :y="5.0*dy" class="ciFont1">dir="h+q"</text>
  <text :x="5.2*dx"    :y="5.0*dy" class="ciFont1">dir="d+k"</text>

  <CiWall1 :x="0.0*dx" :y="5.6*dy" sid="wall1" dir="h+v+" :border="1"></CiWall1>
  <CiWall1 :x="1.1*dx" :y="5.6*dy" sid="wall1" dir="h+v+q" :border="1"></CiWall1>
  <CiWall1 :x="2.2*dx" :y="5.6*dy" sid="wall1" dir="h+v-q" :border="1"></CiWall1>
  <CiWall1 :x="3.3*dx" :y="5.6*dy" sid="wall1" dir="hv+" :border="1"></CiWall1>
  <CiWall1 :x="4.4*dx" :y="5.6*dy" sid="wall1" dir="hv-" :border="1"></CiWall1>
  <CiWall1 :x="5.5*dx" :y="5.6*dy" sid="wall1" dir="h+v" :border="1"></CiWall1>
  <text :x="-0.45*dx"  :y="6.3*dy" class="ciFont1">dir="h+v+"</text>
  <text :x="0.65*dx"   :y="6.3*dy" class="ciFont1">dir="h+v+q"</text>
  <text :x="1.8*dx"    :y="6.3*dy" class="ciFont1">dir="h+v-q"</text>
  <text :x="2.9*dx"    :y="6.3*dy" class="ciFont1">dir="hv+"</text>
  <text :x="4.0*dx"    :y="6.3*dy" class="ciFont1">dir="hv-"</text>
  <text :x="5.1*dx"    :y="6.3*dy" class="ciFont1">dir="h+v"</text>

  <text x="-49"        :y="6.7*dy" class="ciFont2">Stretched Elements: dir=</text>

  <CiWall1 :x="0.0*dx" :y="7.3*dy" sid="wall1" dir="h2" :border="1"></CiWall1>
  <CiWall1 :x="1.6*dx" :y="7.3*dy" sid="wall1" dir="v-1.1d.5" :border="1"></CiWall1>
  <CiWall1 :x="3.2*dx" :y="7.3*dy" sid="wall1" dir="h-2v+q" :border="1"></CiWall1>
  <CiWall1 :x="4.8*dx" :y="7.3*dy" sid="wall1" dir="h2v2t2d-2" :border="1"></CiWall1>
  <text :x="-0.3*dx"   :y="8.0*dy" class="ciFont1">dir="h2"</text>
  <text :x="1.1*dx"    :y="8.0*dy" class="ciFont1">dir="v-1.1d.5"</text>
  <text :x="2.7*dx"    :y="8.0*dy" class="ciFont1">dir="h-2v+q"</text>
  <text :x="4.4*dx"    :y="8.0*dy" class="ciFont1">dir="h2v2t2d-2"</text>

  <CiWall1 :x="0.0*dx" :y="8.6*dy" sid="wall1" dir="w2h2" :border="1"></CiWall1>
  <CiWall1 :x="1.6*dx" :y="8.6*dy" sid="wall1" dir="v+d-.8w2h2" :border="1"></CiWall1>

  <text :x="-0.3*dx"   :y="9.3*dy" class="ciFont1">dir="w2h2"</text>
  <text :x="1.1*dx"    :y="9.3*dy" class="ciFont1">dir="v+d-.8w2h2"</text>

  <text x="-49"        :y="9.7*dy" class="ciFont2">Colored Elements: color=</text>

  <CiWall1 :x="0.0*dx" :y="10.3*dy" sid="wall1" dir="h2q" color="red" :border="1"></CiWall1>
  <CiWall1 :x="1.6*dx" :y="10.3*dy" sid="wall1" dir="c3"  color="#8888FF" :border="1"></CiWall1>
  <CiWall1 :x="3.2*dx" :y="10.3*dy" sid="wall1" dir="h-h2" color="gold" header="header..." footer="... and footer" :border="1"></CiWall1>
  <CiWall1 :x="4.8*dx" :y="10.3*dy" sid="wall1" dir="hv" color="green" line2="line2..." line4="... and line4" :border="1"></CiWall1>

  <text :x="-0.40*dx"  :y="11.0*dy" class="ciFont1">dir="h2q"</text>
  <text :x="-0.45*dx"  :y="11.2*dy" class="ciFont1">color="red"</text>
  <text :x="1.25*dx"   :y="11.0*dy" class="ciFont1">dir="c3"</text>
  <text :x="1.0*dx"    :y="11.2*dy" class="ciFont1">color="#8888FF"</text>
  <text :x="2.85*dx"   :y="11.0*dy" class="ciFont1">dir="h-h2"</text>
  <text :x="2.7*dx"    :y="11.2*dy" class="ciFont1">color="gold"</text>
  <text :x="4.45*dx"   :y="11.0*dy" class="ciFont1">dir="hv"</text>
  <text :x="4.3*dx"    :y="11.2*dy" class="ciFont1">color="green"</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geof } from '../classes/Geo'

// import { Geo } from './CiBase.vue'
import CiWall1 from './CiWall1.vue'
// import WallRoom1 from './WallRoom1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    // WallRoom1,
    CiWall1
  },
  computed: {
    // =======standard methods==================================
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(0, 0, 1, 1)
      return geo1
    },
    dx: function (): number { return (this.geof.dxo()) },
    dy: function (): number { return (this.geof.dyo()) }
  }
})
</script>

<style>
</style>
