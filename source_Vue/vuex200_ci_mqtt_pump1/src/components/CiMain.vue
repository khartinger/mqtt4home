<!-- CiMain.vue (vuex200_ci_mqtt_pump1)-----------khartinger -->
<!-- 2023-02-14: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiPump1 :x="50" :y="70" sid="pump1" :border="3" colorH="white"></CiPump1>
-->
<!--
<svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiPump1 :x="50" :y="70"  sid="pump1" :fx="2" :fy="1"></CiPump1>
  <CiPump1 :x="50" :y="170" sid="pumpX" :fx="2" :fy="1"></CiPump1>
  <CiPump1 :x="250" :y="70" sid="pump1" :fx="1" :fy="2"></CiPump1>
  <CiPump1 :x="350" :y="70" sid="pump1" :fx="2" :fy="2"></CiPump1>
-->

<svg width="100%" viewBox="-10 -10 670 570">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="570" />
  <text x=0 y=10 class="ciFont1">&lt;CiPump1 :x="0" :y="0" sid="pump1" :border="0" /&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>
  <text x=0 y=45 class="ciFont2">Various :border=...</text>

  <CiPump1 :x="50"  :y="100" sid="pump1" :border="0"></CiPump1>
  <CiPump1 :x="160" :y="100" sid="pump1" :border="1"></CiPump1>
  <CiPump1 :x="270" :y="100" sid="pump1" :border="2"></CiPump1>
  <CiPump1 :x="380" :y="100" sid="pump1" :border="3"></CiPump1>
  <text x="5"   y="165" class="ciFont1">:border="0"</text>
  <text x="115" y="165" class="ciFont1">:border="1"</text>
  <text x="225" y="165" class="ciFont1">:border="2"</text>
  <text x="335" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Stretch factor fx, fy</text>
  <text x=0 y=225 class="ciFont1">:border="2"</text>
  <CiPump1 :x="50"  :y="280" sid="pump1" :fx=2  :border="2"></CiPump1>
  <CiPump1 :x="270" :y="280" sid="pump1" :fy=0.5 :border="2"></CiPump1>
  <CiPump1 :x="380" :y="280" sid="pump1" :fx=1.5 :fy=0.7 :border="2"></CiPump1>
  <CiPump1 :x="540" :y="280" sid="pump1" :fx=1.6 :fy=1.4 :border="2"></CiPump1>

  <text x="5"   y="340" class="ciFont1">:fx=2</text>
  <text x="225" y="290" class="ciFont1">:fy=0.5</text>
  <text x="335" y="310" class="ciFont1">:fx=1.5 :fy=0.7</text>
  <text x="495" y="380" class="ciFont1">:fx=1.6 :fy=1.4</text>

  <text x=0 y=395 class="ciFont2">Various color=...</text>

  <CiPump1 :x="50"  :y="450" sid="pump1" colorH="white"></CiPump1>
  <CiPump1 :x="160" :y="450" sid="pumpX" colorH="yellow" colorF="gold"></CiPump1>
  <CiPump1 :x="270" :y="450" sid="pump1" colorH="yellow" colorF="gold" :border="2" :fx="3" :fy="1.2"></CiPump1>
  <text x="0" y="515" class="ciFont1">colorH="white"</text>
  <text x="225" y="535" class="ciFont1">colorH="yellow" colorF="gold"</text>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiPump1 from './CiPump1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiPump1
  }
})
</script>

<style>
</style>
