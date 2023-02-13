<!-- CiMain.vue (vuex150_ci_mqtt_lamp1)-----------khartinger -->
<!-- 2022-01-25: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiLamp1 :x="50" :y="70" sid="lamp2" :fx="1" :fy="1"></CiLamp1>
-->
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiLamp1 :x="50" :y="70" sid="lamp1" :fx="2" :fy="1"></CiLamp1>
  <CiLamp1 :x="50" :y="170" sid="lamp2" :fx="2" :fy="1"></CiLamp1>
  <CiLamp1 :x="250" :y="70" sid="lamp1" :fx="1" :fy="2"></CiLamp1>
  <CiLamp1 :x="350" :y="70" sid="lamp2" :fx="2" :fy="2"></CiLamp1>
-->
<svg width="100%" viewBox="-10 -10 670 1210">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="1210" />
  <text x=0 y=10 class="ciFont1">&lt;CiLamp1 :x="0" :y="0" sid="lamp1" :border="0" lines="0" shape="round"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>
  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiLamp1 :x="50"  :y="100" sid="lamp1" :border="0" lines="0"></CiLamp1>
  <CiLamp1 :x="160" :y="100" sid="lamp1" :border="1"  lines="0"></CiLamp1>
  <CiLamp1 :x="270" :y="100" sid="lamp1" :border="2" lines="0" ></CiLamp1>
  <CiLamp1 :x="380" :y="100" sid="lamp1" :border="3" lines="0" ></CiLamp1>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="1")</text>

  <CiLamp1 :x="50"  :y="260" sid="lamp1" lines="0"></CiLamp1>
  <CiLamp1 :x="160" :y="260" sid="lamp1" lines="1"></CiLamp1>
  <CiLamp1 :x="270" :y="260" sid="lamp1" lines="2"></CiLamp1>
  <text x="0"   y="325" class="ciFont1">lines="0"</text>
  <text x="110" y="325" class="ciFont1">lines="1"</text>
  <text x="220" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">Stretch factor fx, fy</text>
  <CiLamp1 :x="50"  :y="420" sid="lamp1" :fx=2  lines="0"></CiLamp1>
  <CiLamp1 :x="270" :y="420" sid="lamp1" :fy=0.5  lines="0"></CiLamp1>
  <CiLamp1 :x="380" :y="420" sid="lamp1" :fx=1.5 :fy=0.75  lines="0"></CiLamp1>
  <text x="0"   y="485" class="ciFont1">:fx=2</text>
  <text x="220" y="435" class="ciFont1">:fy=0.5</text>
  <text x="330" y="460" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=525 class="ciFont2">Various shape=...</text>
  <text x="0" y="545" class="ciFont1">with :fy="1.5"</text>
  <CiLamp1 :x="50"  :y="600" sid="lamp1" lines="0" :fy="1.5" shape="circle"></CiLamp1>
  <CiLamp1 :x="160" :y="600" sid="lamp1" lines="0" :fy="1.5" shape="round"></CiLamp1>
  <CiLamp1 :x="270" :y="600" sid="lamp1" lines="0" :fy="1.5" shape="rect"></CiLamp1>
  <CiLamp1 :x="380" :y="600" sid="lamp1" lines="0" :fy="1.5" shape="square"></CiLamp1>
  <CiLamp1 :x="490" :y="600" sid="lamp1" lines="1" :fy="1.5" shape="round"></CiLamp1>
  <CiLamp1 :x="600" :y="600" sid="lamp1" lines="2" :fy="1.5" shape="round"></CiLamp1>
  <text x="0"   y="715" class="ciFont1">"circle"</text>
  <text x="110" y="715" class="ciFont1">"round"</text>
  <text x="220" y="715" class="ciFont1">"rect"</text>
  <text x="330" y="715" class="ciFont1">"square"</text>
  <text x="440" y="715" class="ciFont1">round:lines="1"|lines="2"</text>

  <text x="0" y="735" class="ciFont1">with :fx="1.5"</text>
  <CiLamp1 :x="50"  :y="790" sid="lamp1"  lines="0" :fx="1.5" shape="circle"></CiLamp1>
  <CiLamp1 :x="210" :y="790" sid="lamp1"  lines="0" :fx="1.5" shape="round"></CiLamp1>
  <CiLamp1 :x="370" :y="790" sid="lamp1"  lines="0" :fx="1.5" shape="rect"></CiLamp1>
  <CiLamp1 :x="530" :y="790" sid="lamp1"  lines="0" :fx="1.5" shape="square"></CiLamp1>
  <text x="0"   y="855" class="ciFont1">"circle"</text>
  <text x="160" y="855" class="ciFont1">"round"</text>
  <text x="320" y="855" class="ciFont1">"rect"</text>
  <text x="480" y="855" class="ciFont1">"square"</text>

  <text x=0 y=895 class="ciFont2">Various color=...</text>

  <CiLamp1 :x="50"  :y="950" sid="lamp1" lines="0" color="red"></CiLamp1>
  <CiLamp1 :x="160" :y="950" sid="lamp1" color="blue"></CiLamp1>
  <CiLamp1 :x="270" :y="950" sid="lamp1" lines="2" color="#EEDD00"></CiLamp1>
  <text x="0"   y="1015" class="ciFont1">color="red"</text>
  <text x="110" y="1015" class="ciFont1">color="blue"</text>
  <text x="220" y="1015" class="ciFont1">color="#EEDD00"</text>

  <line x1="0" y1="1030" x2="670" y2="1030" stroke="black" stroke-width="2"/>
  <text x=0 y=1055 class="ciFont2">Centertext</text>
  <text x="0" y="1075" class="ciFont1">defined in CiLamp2Controller.ts</text>
  <CiLamp1 :x="50" :y="1130" sid="lamp2"></CiLamp1>
  <CiLamp1 :x="160" :y="1130" sid="lamp2" lines="2"></CiLamp1>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
// import CiLamp from './CiLamp.vue'
import CiLamp1 from './CiLamp1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    // CiLamp,
    CiLamp1
  }
})
</script>

<style>
</style>
