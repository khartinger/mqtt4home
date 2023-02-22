<!-- CiMain.vue (vuex340_ci_mqtt_blind1)----------khartinger -->
<!-- 2022-02-20: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiBlind1 :x="50" :y="70" sid="blind_o" dir="h1"></CiBlind1>
  -->
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiBlind1 :x="100" :y="70"  sid="blind_o" dir="h1" :border="3"></CiBlind1>
  <CiBlind1 :x="100" :y="170" sid="blind_o" dir="h1" :f="2"></CiBlind1>
  <CiBlind1 :x="300" :y="110" sid="blind_o" dir="v2"></CiBlind1>
  <CiBlind1 :x="400" :y="110" sid="blind_o" dir="v2" :f="2"></CiBlind1>
-->
<!--
  <svg width="100%" viewBox="0 0 500 220">
    <rect class="ciBackground" x="0" y="0" width="500" height="220" />
    <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
    <CiBlind1 :x="50"  :y="70" sid="blindX" dir="h1"></CiBlind1>
    <CiBlind1 :x="150" :y="70" sid="blind_o" dir="h1"></CiBlind1>
    <CiBlind1 :x="350" :y="70" sid="blind_o" dir="h1" lines="0"></CiBlind1>
    <text x="215" y="25" class="ciFont1">No text</text>
    <text x="215" y="40" class="ciFont1">output if</text>
    <text x="215" y="55" class="ciFont1">lines="0":</text>
    <CiBlind1 :x="50"  :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="150" :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="250" :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="350" :y="170" sid="blindF" dir="h1" textF="textF :)"></CiBlind1>
-->
  <svg width="100%" viewBox="-10 -10 1000 1510">
  <rect class="ciBackground" x="-10" y="-10" width="1000" height="1510" />
  <text x=0 y=5 class="ciFont1">&lt;CiBlind1 :x="0" :y="0" sid="blind1" dir="h1" :border="0" shape="arc"/&gt;</text>
  <line x1="-10" y1="10" x2="1000" y2="10" stroke="black" stroke-width="2"/>

  <text x=0 y=40 class="ciFont2">Various :border=... (dir="h1")</text>
  <CiBlind1 :x="50"  :y="100" sid="blind_o" dir="h1" :border="0"></CiBlind1>
  <CiBlind1 :x="160" :y="100" sid="blind_o" dir="h1" :border="1"></CiBlind1>
  <CiBlind1 :x="270" :y="100" sid="blind_o" dir="h1" :border="2"></CiBlind1>
  <CiBlind1 :x="380" :y="100" sid="blind_o" dir="h1" :border="3"></CiBlind1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=200 class="ciFont2">Different blind directions</text>
  <CiBlind1 :x="50"  :y="260" sid="blind_c" dir="h1"></CiBlind1>
  <CiBlind1 :x="160" :y="260" sid="blind_c" dir="v1"></CiBlind1>
  <CiBlind1 :x="270" :y="260" sid="blind_c" dir="d1x"></CiBlind1>
  <CiBlind1 :x="380" :y="260" sid="blind_c" dir="d2x"></CiBlind1>
  <text x="15"  y="325" class="ciFont1">dir="h1"</text>
  <text x="125" y="325" class="ciFont1">dir="v1"</text>
  <text x="235" y="325" class="ciFont1">dir="d1x"</text>
  <text x="345" y="325" class="ciFont1">dir="d2x"</text>

  <text x=0 y=360 class="ciFont2">Different blind shapes</text>
  <CiBlind1 :x="50"  :y="420" sid="no_sid" dir="h1"></CiBlind1>
  <CiBlind1 :x="160" :y="420" sid="blind_l" dir="h1"></CiBlind1>
  <CiBlind1 :x="270" :y="420" sid="blind_l" dir="H1"></CiBlind1>
  <CiBlind1 :x="380" :y="420" sid="blind_c" dir="h1"></CiBlind1>
  <CiBlind1 :x="490" :y="420" sid="blind_c" dir="H1"></CiBlind1>
  <CiBlind1 :x="600" :y="420" sid="blind_o" dir="h1"></CiBlind1>
  <CiBlind1 :x="710" :y="420" sid="blind_o" dir="H1"></CiBlind1>
  <text x="0"  y="485" class="ciFont1">sid not in</text>
  <text x="125" y="485" class="ciFont1">dir="h1"</text>
  <text x="235" y="485" class="ciFont1">dir="H1"</text>
  <text x="345" y="485" class="ciFont1">dir="h1"</text>
  <text x="455" y="485" class="ciFont1">dir="H1"</text>
  <text x="565" y="485" class="ciFont1">dir="h1"</text>
  <text x="675" y="485" class="ciFont1">dir="H1"</text>
  <text x="0"   y="500" class="ciFont1">Controller</text>
  <text x="110" y="500" class="ciFont1">iBlind1State=-3|stateLock</text>
  <text x="330" y="500" class="ciFont1">iBlind1State=-2|stateClose</text>
  <text x="555" y="500" class="ciFont1">iBlind1State=-1|stateOpen</text>

  <text x=0 y=540 class="ciFont2">Different roller blind mounting directions...</text>
  <rect x="100" y="591" width="10"  height="18" fill="gray"/>
  <rect x="210" y="591" width="10"  height="18" fill="gray"/>
  <rect x="320" y="591" width="10"  height="18" fill="gray"/>
  <rect x="430" y="591" width="100" height="18" fill="gray"/>
  <rect x="591" y="540" width="18" height="10" fill="gray"/>
  <rect x="701" y="540" width="18" height="10" fill="gray"/>
  <rect x="811" y="540" width="18" height="10" fill="gray"/>
  <rect x="921" y="540" width="18" height="10" fill="gray"/>
  <rect x="591" y="650" width="18" height="10" fill="gray"/>
  <rect x="701" y="650" width="18" height="10" fill="gray"/>
  <rect x="811" y="650" width="18" height="10" fill="gray"/>
  <rect x="921" y="650" width="18" height="10" fill="gray"/>
  <text x="435" y="586" class="ciFont1">normal wall</text>
  <text x="435" y="624" class="ciFont1">position</text>
  <text x="435" y="586" class="ciFont1">normal wall</text>
  <text x="435" y="624" class="ciFont1">position</text>
  <CiBlind1 :x="50"  :y="600" sid="blind_o" dir="h1"></CiBlind1>
  <CiBlind1 :x="160" :y="600" sid="blind_o" dir="h2"></CiBlind1>
  <CiBlind1 :x="270" :y="600" sid="blind_o" dir="h3"></CiBlind1>
  <CiBlind1 :x="380" :y="600" sid="blind_o" dir="h4"></CiBlind1>
  <CiBlind1 :x="600" :y="600" sid="blind_o" dir="v1"></CiBlind1>
  <CiBlind1 :x="710" :y="600" sid="blind_o" dir="v2"></CiBlind1>
  <CiBlind1 :x="820" :y="600" sid="blind_o" dir="v3"></CiBlind1>
  <CiBlind1 :x="930" :y="600" sid="blind_o" dir="v4"></CiBlind1>
  <text x="15"  y="665" class="ciFont1">dir="h1"</text>
  <text x="125" y="665" class="ciFont1">dir="h2"</text>
  <text x="235" y="665" class="ciFont1">dir="h3"</text>
  <text x="345" y="665" class="ciFont1">dir="h4"</text>
  <text x="575" y="674" class="ciFont1">dir="v1"</text>
  <text x="685" y="674" class="ciFont1">dir="v2"</text>
  <text x="795" y="674" class="ciFont1">dir="v3"</text>
  <text x="905" y="674" class="ciFont1">dir="v4"</text>

  <CiBlind1 :x="50"  :y="740" sid="blind_o" dir="d1x"></CiBlind1>
  <CiBlind1 :x="160" :y="740" sid="blind_o" dir="d1y"></CiBlind1>
  <CiBlind1 :x="270" :y="740" sid="blind_o" dir="d3x"></CiBlind1>
  <CiBlind1 :x="380" :y="740" sid="blind_o" dir="d3y"></CiBlind1>
  <CiBlind1 :x="600" :y="740" sid="blind_o" dir="d2x"></CiBlind1>
  <CiBlind1 :x="710" :y="740" sid="blind_o" dir="d2y"></CiBlind1>
  <CiBlind1 :x="820" :y="740" sid="blind_o" dir="d4x"></CiBlind1>
  <CiBlind1 :x="930" :y="740" sid="blind_o" dir="d4y"></CiBlind1>
  <text x="15"  y="805" class="ciFont1">dir="d1x"</text>
  <text x="125" y="805" class="ciFont1">dir="d1y"</text>
  <text x="235" y="805" class="ciFont1">dir="d3x"</text>
  <text x="345" y="805" class="ciFont1">dir="d3y"</text>
  <text x="565" y="805" class="ciFont1">dir="d2x"</text>
  <text x="675" y="805" class="ciFont1">dir="d2y"</text>
  <text x="785" y="805" class="ciFont1">dir="d4x"</text>
  <text x="895" y="805" class="ciFont1">dir="d4y"</text>

  <text x="5" y="830" class="ciFont1">The same with large blind opening...</text>
  <CiBlind1 :x="50"  :y="900" sid="blind_o" dir="H1"></CiBlind1>
  <CiBlind1 :x="160" :y="900" sid="blind_o" dir="H2"></CiBlind1>
  <CiBlind1 :x="270" :y="900" sid="blind_o" dir="H3"></CiBlind1>
  <CiBlind1 :x="380" :y="900" sid="blind_o" dir="H4"></CiBlind1>
  <CiBlind1 :x="530" :y="900" sid="blind_o" dir="V1"></CiBlind1>
  <CiBlind1 :x="710" :y="900" sid="blind_o" dir="V2"></CiBlind1>
  <CiBlind1 :x="820" :y="900" sid="blind_o" dir="V3"></CiBlind1>
  <CiBlind1 :x="930" :y="900" sid="blind_o" dir="V4"></CiBlind1>
  <text x="15"  y="965" class="ciFont1">dir="H1"</text>
  <text x="125" y="965" class="ciFont1">dir="H2"</text>
  <text x="235" y="965" class="ciFont1">dir="H3"</text>
  <text x="345" y="965" class="ciFont1">dir="H4"</text>
  <text x="495" y="965" class="ciFont1">dir="V1"</text>
  <text x="675" y="965" class="ciFont1">dir="V2"</text>
  <text x="785" y="965" class="ciFont1">dir="V3"</text>
  <text x="895" y="965" class="ciFont1">dir="V4"</text>

  <CiBlind1 :x="50"  :y="1060" sid="blind_o" dir="D1x"></CiBlind1>
  <CiBlind1 :x="160" :y="1060" sid="blind_o" dir="D1y"></CiBlind1>
  <CiBlind1 :x="270" :y="1060" sid="blind_o" dir="D3x"></CiBlind1>
  <CiBlind1 :x="380" :y="1060" sid="blind_o" dir="D3y"></CiBlind1>
  <CiBlind1 :x="600" :y="1060" sid="blind_o" dir="D2x"></CiBlind1>
  <CiBlind1 :x="710" :y="1060" sid="blind_o" dir="D2y"></CiBlind1>
  <CiBlind1 :x="820" :y="1060" sid="blind_o" dir="D4x"></CiBlind1>
  <CiBlind1 :x="930" :y="1060" sid="blind_o" dir="D4y"></CiBlind1>
  <text x="15"  y="1125" class="ciFont1">dir="D1x"</text>
  <text x="125" y="1125" class="ciFont1">dir="D1y"</text>
  <text x="235" y="1125" class="ciFont1">dir="D3x"</text>
  <text x="345" y="1125" class="ciFont1">dir="D3y"</text>
  <text x="565" y="1125" class="ciFont1">dir="D2x"</text>
  <text x="675" y="1125" class="ciFont1">dir="D2y"</text>
  <text x="785" y="1125" class="ciFont1">dir="D4x"</text>
  <text x="895" y="1125" class="ciFont1">dir="D4y"</text>

  <text x=0 y=1170 class="ciFont2">Stretch factor :f</text>
  <CiBlind1 :x="160" :y="1230" sid="blind_o" dir="h1" :f="2"></CiBlind1>
  <CiBlind1 :x="380" :y="1230" sid="blind_o" dir="V1" :f="1.5"></CiBlind1>
  <CiBlind1 :x="600" :y="1230" sid="blind_o" dir="D1x" :f="1.4142"></CiBlind1>
  <CiBlind1 :x="875" :y="1230" sid="blind_o" dir="d2x" :f="2"></CiBlind1>
  <text x="100" y="1295" class="ciFont1">dir="h1" :f="2"</text>
  <text x="320" y="1320" class="ciFont1">dir="V1" :f="1.5"</text>
  <text x="510" y="1320" class="ciFont1">dir="D1x" :f="1.4142"</text>
  <text x="770" y="1320" class="ciFont1">dir="d2x" :f="2"</text>

  <text x=0 y=1360 class="ciFont2">Various colors</text>
  <CiBlind1 :x="50"  :y="1420" sid="blind_bat10" dir="h1" colorH="yellow"></CiBlind1>
  <CiBlind1 :x="160" :y="1420" sid="blind_bat20" dir="h1" colorH="yellow"></CiBlind1>
  <CiBlind1 :x="325" :y="1420" sid="blindF" dir="h1" colorF="#DDBBFF"></CiBlind1>
  <CiBlind1 :x="490" :y="1420" sid="blindF" dir="h1" colorF="#DDBBFF" colorH="yellow" :f="2"></CiBlind1>
  <CiBlind1 :x="710" :y="1420" sid="blind_bat20" dir="h1" colorMotor="blue"></CiBlind1>

  <text x="40"  y="1485" class="ciFont1">colorH="yellow"</text>
  <text x="280" y="1485" class="ciFont1">colorF="#DDBBFF" + colorH="yellow"</text>
  <text x="650" y="1485" class="ciFont1">colorMotor="blue"</text>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiBlind1 from './CiBlind1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiBlind1
  }
})
</script>

<style>
</style>
