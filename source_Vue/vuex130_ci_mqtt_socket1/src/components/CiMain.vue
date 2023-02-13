<!-- CiMain.vue (vuex130_ci_mqtt_socket1)---------khartinger -->
<!-- 2022-01-25: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <text x="0" y="15" class="ciFont1">viewbox 300x150</text>
  <CiSocket1 :x="50" :y="70" sid="socket1" :fx="2" :fy="1.3"></CiSocket1>
  <CiSocket1 :x="250" :y="70" sid="socket2" :border="3" color="#CCCC00"></CiSocket1>
  -->
  <svg width="100%" viewBox="-10 -10 670 850">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="850" />
  <text x=0 y=10 class="ciFont1">&lt;CiSocket1 :x="0" :y="0" sid="socket1" :border="0" lines="0"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiSocket1 :x="50"  :y="100" sid="socket1" :border="0" lines="0"></CiSocket1>
  <CiSocket1 :x="160" :y="100" sid="socket1" :border="1"  lines="0"></CiSocket1>
  <CiSocket1 :x="270" :y="100" sid="socket1" :border="2" lines="0" ></CiSocket1>
  <CiSocket1 :x="380" :y="100" sid="socket1" :border="3" lines="0" ></CiSocket1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="1")</text>

  <CiSocket1 :x="50"  :y="260" sid="socket1" lines="0"></CiSocket1>
  <CiSocket1 :x="160" :y="260" sid="socket1" lines="1"></CiSocket1>
  <CiSocket1 :x="270" :y="260" sid="socket1" lines="2"></CiSocket1>
  <text x="0"   y="325" class="ciFont1">lines="0"</text>
  <text x="110" y="325" class="ciFont1">lines="1"</text>
  <text x="220" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">Stretch factor fx, fy</text>

  <CiSocket1 :x="50"  :y="420" sid="socket1" :fx=2  lines="0"></CiSocket1>
  <CiSocket1 :x="270" :y="420" sid="socket1" :fy=0.5  lines="0"></CiSocket1>
  <CiSocket1 :x="380" :y="420" sid="socket1" :fx=1.5 :fy=0.75  lines="0"></CiSocket1>
  <text x="0"   y="485" class="ciFont1">:fx=2</text>
  <text x="220" y="435" class="ciFont1">:fy=0.5</text>
  <text x="330" y="460" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=525 class="ciFont2">Various color=...</text>

  <CiSocket1 :x="50"  :y="580" sid="socket1" lines="0" color="red"></CiSocket1>
  <CiSocket1 :x="160" :y="580" sid="socket1" color="blue"></CiSocket1>
  <CiSocket1 :x="270" :y="580" sid="socket1" lines="2" color="#EEDD00"></CiSocket1>
  <text x="0"   y="645" class="ciFont1">color="red"</text>
  <text x="110" y="645" class="ciFont1">color="blue"</text>
  <text x="220" y="645" class="ciFont1">color="#EEDD00"</text>

  <line x1="0" y1="663" x2="670" y2="663" stroke="black" stroke-width="2"/>
  <text x=0 y=695 class="ciFont2">"textCenter"</text>
  <text x="0" y="715" class="ciFont1">Text is defined in CiSocket1Controller.ts</text>

  <CiSocket1 :x="50"  :y="770" sid="socket2" :border="1" lines="0"></CiSocket1>
  <CiSocket1 :x="160" :y="770" sid="socket2" :border="1" lines="1"></CiSocket1>
  <CiSocket1 :x="270" :y="770" sid="socket2" :border="1" lines="2" ></CiSocket1>
  <text x="0"   y="835" class="ciFont1">lines="0"</text>
  <text x="110" y="835" class="ciFont1">lines="1"</text>
  <text x="220" y="835" class="ciFont1">lines="2"</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
// import CiLamp from './CiLamp.vue'
import CiSocket1 from './CiSocket1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    // CiLamp,
    CiSocket1
  }
})
</script>

<style>
</style>
