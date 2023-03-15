<!-- CiMain.vue (vuex530_ci_mqtt_smsstatus1)------khartinger -->
<!-- 2023-03-15: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiSmsStatus1 :x="50" :y="70" sid="smsstatus1" colorH="white" colorF="white"></CiSmsStatus1>
  -->
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <CiSmsStatus1 :x="50" :y="70"  sid="smsstatus1" :fx="1" :fy="1" colorH="white" colorF="white" :border="3"></CiSmsStatus1>
  <CiSmsStatus1 :x="50" :y="170" sid="smsstatus1" :fx="2" :fy="1" colorH="white" colorF="white" :border="3"></CiSmsStatus1>
  <CiSmsStatus1 :x="250" :y="70" sid="smsstatus1" :fx="1" :fy="2" colorH="white" colorF="white" :border="3"></CiSmsStatus1>
  <CiSmsStatus1 :x="350" :y="70" sid="smsstatus1" :fx="2" :fy="2" colorH="white" colorF="white" :border="3"></CiSmsStatus1>
-->

  <svg width="100%" viewBox="-10 -10 580 530">
  <rect class="ciBackground" x="-10" y="-10" width="580" height="530" />
  <text x=0 y=10 class="ciFont1">&lt;CiSmsStatus1 :x="0" :y="0" sid="smsstatus1" :fx="2" :border="0"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=...</text>

  <CiSmsStatus1 :x="50"  :y="100" sid="smsstatus1" :border="0"></CiSmsStatus1>
  <CiSmsStatus1 :x="160" :y="100" sid="smsstatus1" :border="1"></CiSmsStatus1>
  <CiSmsStatus1 :x="270" :y="100" sid="smsstatus1" :border="2"></CiSmsStatus1>
  <CiSmsStatus1 :x="380" :y="100" sid="smsstatus1" :border="3"></CiSmsStatus1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Stretch factor fx, fy</text>
  <CiSmsStatus1 :x="50"  :y="260" sid="smsstatus1" :fx=2></CiSmsStatus1>
  <CiSmsStatus1 :x="270" :y="260" sid="smsstatus1" :fy=0.5></CiSmsStatus1>
  <CiSmsStatus1 :x="380" :y="260" sid="smsstatus1" :fx=1.5 :fy=0.75></CiSmsStatus1>
  <text x="0"   y="325" class="ciFont1">:fx=2</text>
  <text x="220" y="275" class="ciFont1">:fy=0.5</text>
  <text x="330" y="300" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=365 class="ciFont2">Various colorH=... colorF=...</text>

  <CiSmsStatus1 :x="50"  :y="430" sid="smsstatus1" :fx="1.5" colorH="green" colorF="red"></CiSmsStatus1>
  <CiSmsStatus1 :x="210" :y="430" sid="smsstatus1" :fx="1.5" colorH="yellow" colorF="grey"></CiSmsStatus1>
  <CiSmsStatus1 :x="370" :y="430" sid="smsstatus1" :fx="1.5" colorH="#EEDD00"></CiSmsStatus1>
  <text x="0"   y="495" class="ciFont1">colorH="green"</text>
  <text x="0"   y="510" class="ciFont1">colorF="red"</text>
  <text x="160" y="495" class="ciFont1">colorH="yellow"</text>
  <text x="160" y="510" class="ciFont1">colorF="grey"</text>
  <text x="320" y="495" class="ciFont1">colorH="#EEDD00"</text>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiSmsStatus1 from './CiSmsStatus1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiSmsStatus1
  }
})
</script>

<style>
</style>
