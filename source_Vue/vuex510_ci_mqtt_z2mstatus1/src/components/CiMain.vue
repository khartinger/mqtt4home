<!-- CiMain.vue (vuex510_ci_mqtt_z2mstatus1)------khartinger -->
<!-- 2023-02-28: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiZ2Mstatus1 :x="50" :y="70" sid="z2mstatus1" ></CiZ2Mstatus1>
-->
<!--
  <CiZ2Mstatus1 :x="50" :y="70"  sid="z2mstatus1" :fx="1" :fy="1" colorH="white" colorF="white" :border="3"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="50" :y="170" sid="z2mstatus1" :fx="2" :fy="1" colorH="white" colorF="white" :border="3"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="250" :y="70" sid="z2mstatus1" :fx="1" :fy="2" colorH="white" colorF="white" :border="3"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="350" :y="70" sid="z2mstatus1" :fx="2" :fy="2" colorH="white" colorF="white" :border="3"></CiZ2Mstatus1>
-->

  <svg width="100%" viewBox="-10 -10 580 530">
  <rect class="ciBackground" x="-10" y="-10" width="580" height="530" />
  <text x=0 y=10 class="ciFont1">&lt;CiZ2Mstatus1 :x="0" :y="0" sid="z2mstatus1" :fx="2" :border="0"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=...</text>

  <CiZ2Mstatus1 :x="50"  :y="100" sid="z2mstatus1" :border="0"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="160" :y="100" sid="z2mstatus1" :border="1"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="270" :y="100" sid="z2mstatus1" :border="2"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="380" :y="100" sid="z2mstatus1" :border="3"></CiZ2Mstatus1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Stretch factor fx, fy</text>
  <CiZ2Mstatus1 :x="50"  :y="260" sid="z2mstatus1" :fx=2></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="270" :y="260" sid="z2mstatus1" :fy=0.5></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="380" :y="260" sid="z2mstatus1" :fx=1.5 :fy=0.75></CiZ2Mstatus1>
  <text x="0"   y="325" class="ciFont1">:fx=2</text>
  <text x="220" y="275" class="ciFont1">:fy=0.5</text>
  <text x="330" y="300" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=365 class="ciFont2">Various colorH=... colorF=...</text>

  <CiZ2Mstatus1 :x="50"  :y="430" sid="z2mstatus1" :fx="1.5" colorH="green" colorF="red"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="210" :y="430" sid="z2mstatus1" :fx="1.5" colorH="yellow" colorF="grey"></CiZ2Mstatus1>
  <CiZ2Mstatus1 :x="370" :y="430" sid="z2mstatus1" :fx="1.5" colorH="#EEDD00"></CiZ2Mstatus1>
  <text x="0"   y="495" class="ciFont1">colorH="green"</text>
  <text x="0"   y="510" class="ciFont1">colorF="red"</text>
  <text x="160" y="495" class="ciFont1">colorH="yellow"</text>
  <text x="160" y="510" class="ciFont1">colorF="grey"</text>
  <text x="320" y="495" class="ciFont1">colorH="#EEDD00"</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiZ2Mstatus1 from './CiZ2Mstatus1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiZ2Mstatus1
  }
})
</script>

<style>
</style>
