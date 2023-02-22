<!-- CiMain.vue (vuex320_ci_mqtt_door1)-----------khartinger -->
<!-- 2022-02-18: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiDoor1 :x="50" :y="70" sid="door_o" dir="h1"></CiDoor1>
-->
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiDoor1 :x="100" :y="70"  sid="door_o" dir="h1" :border="3"></CiDoor1>
  <CiDoor1 :x="100" :y="170" sid="door_o" dir="h1" :f="2"></CiDoor1>
  <CiDoor1 :x="300" :y="110" sid="door_o" dir="v2"></CiDoor1>
  <CiDoor1 :x="400" :y="110" sid="door_o" dir="v2" :f="2"></CiDoor1>
  -->
  <!--
  <svg width="100%" viewBox="0 0 500 220">
    <rect class="ciBackground" x="0" y="0" width="500" height="220" />
    <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
    <CiDoor1 :x="50"  :y="70" sid="doorX" dir="h1"></CiDoor1>
    <CiDoor1 :x="150" :y="70" sid="door_o" dir="h1"></CiDoor1>
    <CiDoor1 :x="350" :y="70" sid="door_o" dir="h1" lines="0"></CiDoor1>
    <text x="215" y="25" class="ciFont1">No text</text>
    <text x="215" y="40" class="ciFont1">output if</text>
    <text x="215" y="55" class="ciFont1">lines="0":</text>
    <CiDoor1 :x="50"  :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="150" :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="250" :y="170" sid="doorF" dir="h1"></CiDoor1>
    <CiDoor1 :x="350" :y="170" sid="doorF" dir="h1" textF="textF :)"></CiDoor1>
-->
  <svg width="100%" viewBox="-10 -10 1000 1510">
  <rect class="ciBackground" x="-10" y="-10" width="1000" height="1510" />
  <text x=0 y=5 class="ciFont1">&lt;CiDoor1 :x="0" :y="0" sid="door1" dir="h1" :border="0" shape="arc"/&gt;</text>
  <line x1="-10" y1="10" x2="1000" y2="10" stroke="black" stroke-width="2"/>

  <text x=0 y=40 class="ciFont2">Various :border=... (dir="h1")</text>
  <CiDoor1 :x="50"  :y="100" sid="door_o" dir="h1" :border="0"></CiDoor1>
  <CiDoor1 :x="160" :y="100" sid="door_o" dir="h1" :border="1"></CiDoor1>
  <CiDoor1 :x="270" :y="100" sid="door_o" dir="h1" :border="2"></CiDoor1>
  <CiDoor1 :x="380" :y="100" sid="door_o" dir="h1" :border="3"></CiDoor1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=200 class="ciFont2">Different door directions</text>
  <CiDoor1 :x="50"  :y="260" sid="door_c" dir="h1"></CiDoor1>
  <CiDoor1 :x="160" :y="260" sid="door_c" dir="v1"></CiDoor1>
  <CiDoor1 :x="270" :y="260" sid="door_c" dir="d1x"></CiDoor1>
  <CiDoor1 :x="380" :y="260" sid="door_c" dir="d2x"></CiDoor1>
  <text x="15"  y="325" class="ciFont1">dir="h1"</text>
  <text x="125" y="325" class="ciFont1">dir="v1"</text>
  <text x="235" y="325" class="ciFont1">dir="d1x"</text>
  <text x="345" y="325" class="ciFont1">dir="d2x"</text>

  <text x=0 y=360 class="ciFont2">Different door shapes</text>
  <CiDoor1 :x="50"  :y="420" sid="no_sid" dir="h1"></CiDoor1>
  <CiDoor1 :x="160"  :y="420" sid="door_l" dir="h1"></CiDoor1>
  <CiDoor1 :x="270" :y="420" sid="door_c" dir="h1"></CiDoor1>
  <CiDoor1 :x="380" :y="420" sid="door_o" dir="h1"></CiDoor1>
  <CiDoor1 :x="490" :y="420" sid="door_o" dir="H1"></CiDoor1>
  <CiDoor1 :x="600" :y="420" sid="door_o" dir="h1" shape="tri"></CiDoor1>
  <CiDoor1 :x="710" :y="420" sid="door_o" dir="H1" shape="tri"></CiDoor1>
  <text x="0"  y="485" class="ciFont1">sid not in</text>
  <text x="125" y="485" class="ciFont1">dir="h1"</text>
  <text x="235" y="485" class="ciFont1">dir="h1"</text>
  <text x="345" y="485" class="ciFont1">dir="h1"</text>
  <text x="455" y="485" class="ciFont1">dir="H1"</text>
  <text x="565" y="485" class="ciFont1">dir="h1"</text>
  <text x="675" y="485" class="ciFont1">dir="H1"</text>
  <text x="0"   y="500" class="ciFont1">Controller</text>
  <text x="115" y="500" class="ciFont1">iDoorState=-3</text>
  <text x="240" y="500" class="ciFont1">... -2</text>
  <text x="350" y="500" class="ciFont1">... -1</text>
  <text x="550" y="500" class="ciFont1">shape="tri"</text>
  <text x="660" y="500" class="ciFont1">shape="tri"</text>

  <text x=0 y=540 class="ciFont2">Different door opening directions...</text>
  <CiDoor1 :x="50"  :y="600" sid="door_o" dir="h1"></CiDoor1>
  <CiDoor1 :x="160" :y="600" sid="door_o" dir="h2"></CiDoor1>
  <CiDoor1 :x="270" :y="600" sid="door_o" dir="h3"></CiDoor1>
  <CiDoor1 :x="380" :y="600" sid="door_o" dir="h4"></CiDoor1>
  <CiDoor1 :x="600" :y="600" sid="door_o" dir="v1"></CiDoor1>
  <CiDoor1 :x="710" :y="600" sid="door_o" dir="v2"></CiDoor1>
  <CiDoor1 :x="820" :y="600" sid="door_o" dir="v3"></CiDoor1>
  <CiDoor1 :x="930" :y="600" sid="door_o" dir="v4"></CiDoor1>
  <text x="15"  y="665" class="ciFont1">dir="h1"</text>
  <text x="125" y="665" class="ciFont1">dir="h2"</text>
  <text x="235" y="665" class="ciFont1">dir="h3"</text>
  <text x="345" y="665" class="ciFont1">dir="h4"</text>
  <text x="565" y="665" class="ciFont1">dir="v1"</text>
  <text x="675" y="665" class="ciFont1">dir="v2"</text>
  <text x="785" y="665" class="ciFont1">dir="v3"</text>
  <text x="895" y="665" class="ciFont1">dir="v4"</text>

  <CiDoor1 :x="50"  :y="740" sid="door_o" dir="d1x"></CiDoor1>
  <CiDoor1 :x="160" :y="740" sid="door_o" dir="d1y"></CiDoor1>
  <CiDoor1 :x="270" :y="740" sid="door_o" dir="d3x"></CiDoor1>
  <CiDoor1 :x="380" :y="740" sid="door_o" dir="d3y"></CiDoor1>
  <CiDoor1 :x="600" :y="740" sid="door_o" dir="d2x"></CiDoor1>
  <CiDoor1 :x="710" :y="740" sid="door_o" dir="d2y"></CiDoor1>
  <CiDoor1 :x="820" :y="740" sid="door_o" dir="d4x"></CiDoor1>
  <CiDoor1 :x="930" :y="740" sid="door_o" dir="d4y"></CiDoor1>
  <text x="15"  y="805" class="ciFont1">dir="d1x"</text>
  <text x="125" y="805" class="ciFont1">dir="d1y"</text>
  <text x="235" y="805" class="ciFont1">dir="d3x"</text>
  <text x="345" y="805" class="ciFont1">dir="d3y"</text>
  <text x="565" y="805" class="ciFont1">dir="d2x"</text>
  <text x="675" y="805" class="ciFont1">dir="d2y"</text>
  <text x="785" y="805" class="ciFont1">dir="d4x"</text>
  <text x="895" y="805" class="ciFont1">dir="d4y"</text>

  <text x="5" y="830" class="ciFont1">The same with large door opening...</text>
  <CiDoor1 :x="50"  :y="900" sid="door_o" dir="H1"></CiDoor1>
  <CiDoor1 :x="160" :y="900" sid="door_o" dir="H2"></CiDoor1>
  <CiDoor1 :x="270" :y="900" sid="door_o" dir="H3"></CiDoor1>
  <CiDoor1 :x="380" :y="900" sid="door_o" dir="H4"></CiDoor1>
  <CiDoor1 :x="530" :y="900" sid="door_o" dir="V1"></CiDoor1>
  <CiDoor1 :x="710" :y="900" sid="door_o" dir="V2"></CiDoor1>
  <CiDoor1 :x="820" :y="900" sid="door_o" dir="V3"></CiDoor1>
  <CiDoor1 :x="930" :y="900" sid="door_o" dir="V4"></CiDoor1>
  <text x="15"  y="965" class="ciFont1">dir="H1"</text>
  <text x="125" y="965" class="ciFont1">dir="H2"</text>
  <text x="235" y="965" class="ciFont1">dir="H3"</text>
  <text x="345" y="965" class="ciFont1">dir="H4"</text>
  <text x="495" y="965" class="ciFont1">dir="V1"</text>
  <text x="675" y="965" class="ciFont1">dir="V2"</text>
  <text x="785" y="965" class="ciFont1">dir="V3"</text>
  <text x="895" y="965" class="ciFont1">dir="V4"</text>

  <CiDoor1 :x="50"  :y="1060" sid="door_o" dir="D1x"></CiDoor1>
  <CiDoor1 :x="160" :y="1060" sid="door_o" dir="D1y"></CiDoor1>
  <CiDoor1 :x="270" :y="1060" sid="door_o" dir="D3x"></CiDoor1>
  <CiDoor1 :x="380" :y="1060" sid="door_o" dir="D3y"></CiDoor1>
  <CiDoor1 :x="600" :y="1060" sid="door_o" dir="D2x"></CiDoor1>
  <CiDoor1 :x="710" :y="1060" sid="door_o" dir="D2y"></CiDoor1>
  <CiDoor1 :x="820" :y="1060" sid="door_o" dir="D4x"></CiDoor1>
  <CiDoor1 :x="930" :y="1060" sid="door_o" dir="D4y"></CiDoor1>
  <text x="15"  y="1125" class="ciFont1">dir="D1x"</text>
  <text x="125" y="1125" class="ciFont1">dir="D1y"</text>
  <text x="235" y="1125" class="ciFont1">dir="D3x"</text>
  <text x="345" y="1125" class="ciFont1">dir="D3y"</text>
  <text x="565" y="1125" class="ciFont1">dir="D2x"</text>
  <text x="675" y="1125" class="ciFont1">dir="D2y"</text>
  <text x="785" y="1125" class="ciFont1">dir="D4x"</text>
  <text x="895" y="1125" class="ciFont1">dir="D4y"</text>

  <text x=0 y=1170 class="ciFont2">Stretch factor :f</text>
  <CiDoor1 :x="160" :y="1230" sid="door_o" dir="h1" :f="2"></CiDoor1>
  <CiDoor1 :x="380" :y="1230" sid="door_o" dir="V1" :f="1.5"></CiDoor1>
  <CiDoor1 :x="600" :y="1230" sid="door_o" dir="D1x" :f="1.4142" shape="tri"></CiDoor1>
  <CiDoor1 :x="875" :y="1230" sid="door_o" dir="d2x" :f="2"></CiDoor1>
  <text x="100" y="1295" class="ciFont1">dir="h1" :f="2"</text>
  <text x="320" y="1320" class="ciFont1">dir="V1" :f="1.5"</text>
  <text x="510" y="1300" class="ciFont1">dir="D1x" :f="1.4142" shape="tri"</text>
  <text x="800" y="1320" class="ciFont1">dir="d2x" :f="2"</text>

  <text x=0 y=1360 class="ciFont2">Various colors</text>
  <CiDoor1 :x="50"  :y="1420" sid="door_bat10" dir="h1" colorH="yellow"></CiDoor1>
  <CiDoor1 :x="160" :y="1420" sid="door_bat20" dir="h1" colorH="yellow"></CiDoor1>
  <CiDoor1 :x="325" :y="1420" sid="doorF" dir="h1" colorF="#DDBBFF"></CiDoor1>
  <CiDoor1 :x="490" :y="1420" sid="doorF" dir="h1" colorF="#DDBBFF" colorH="yellow" :f="2"></CiDoor1>
  <CiDoor1 :x="710" :y="1420" sid="door_bat20" dir="h1" colorWall="blue"></CiDoor1>

  <text x="40"  y="1485" class="ciFont1">colorH="yellow"</text>
  <text x="280" y="1485" class="ciFont1">colorF="#DDBBFF" + colorH="yellow"</text>
  <text x="650" y="1485" class="ciFont1">colorWall="blue"</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiDoor1 from './CiDoor1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiDoor1
  }
})
</script>

<style>
</style>
