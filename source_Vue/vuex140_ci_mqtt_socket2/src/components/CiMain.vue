<!-- CiMain.vue (vuex140_ci_mqtt_socket2)---------khartinger -->
<!-- 2022-01-27: new                                         -->
<template>
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiSocket2 :x="50" :y="70" sid="socket2" :fx="1" :fy="1"></CiSocket2>
  -->
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiSocket2 :x="50" :y="70" sid="socket1" :fx="2" :fy="1"></CiSocket2>
  <CiSocket2 :x="50" :y="170" sid="socket2" :fx="2" :fy="1"></CiSocket2>
  <CiSocket2 :x="250" :y="70" sid="socket1" :fx="1" :fy="2"></CiSocket2>
  <CiSocket2 :x="350" :y="70" sid="socket2" :fx="2" :fy="2"></CiSocket2>
  -->
<svg width="100%" viewBox="-10 -10 670 850">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="850" />
  <text x=0 y=10 class="ciFont1">&lt;CiSocket2 :x="0" :y="0" sid="socket1" :border="0" lines="0" /&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiSocket2 :x="50"  :y="100" sid="socket1" :border="0" lines="0"></CiSocket2>
  <CiSocket2 :x="160" :y="100" sid="socket1" :border="1"  lines="0"></CiSocket2>
  <CiSocket2 :x="270" :y="100" sid="socket1" :border="2" lines="0" ></CiSocket2>
  <CiSocket2 :x="380" :y="100" sid="socket1" :border="3" lines="0" ></CiSocket2>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="1")</text>

  <CiSocket2 :x="50"  :y="260" sid="socket1" lines="0"></CiSocket2>
  <CiSocket2 :x="160" :y="260" sid="socket1" lines="1"></CiSocket2>
  <CiSocket2 :x="270" :y="260" sid="socket1" lines="2"></CiSocket2>
  <text x="0"   y="325" class="ciFont1">lines="0"</text>
  <text x="110" y="325" class="ciFont1">lines="1"</text>
  <text x="220" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">Stretch factor fx, fy</text>

  <CiSocket2 :x="50"  :y="420" sid="socket1" :fx=2  lines="0"></CiSocket2>
  <CiSocket2 :x="270" :y="420" sid="socket1" :fy=0.5  lines="0"></CiSocket2>
  <CiSocket2 :x="380" :y="420" sid="socket1" :fx=1.5 :fy=0.75  lines="0"></CiSocket2>
  <text x="0"   y="485" class="ciFont1">:fx=2</text>
  <text x="220" y="435" class="ciFont1">:fy=0.5</text>
  <text x="330" y="460" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=525 class="ciFont2">Various colorup=... colordn=...</text>

  <CiSocket2 :x="50"  :y="580" sid="socket1" lines="0" colorup="green" colordn="red"></CiSocket2>
  <CiSocket2 :x="210" :y="580" sid="socket1" colorup="lightblue" colordn="blue"></CiSocket2>
  <CiSocket2 :x="370" :y="580" sid="socket1" lines="2" colorup="#EEDD00"></CiSocket2>
  <text x="0"   y="645" class="ciFont1">colorup="green"</text>
  <text x="0"   y="660" class="ciFont1">colordn="red"</text>
  <text x="160" y="645" class="ciFont1">colorup="lightblue"</text>
  <text x="160" y="660" class="ciFont1">colordn="blue"</text>
  <text x="320" y="645" class="ciFont1">colorup="#EEDD00"</text>

  <line x1="0" y1="675" x2="670" y2="675" stroke="black" stroke-width="2"/>
  <text x=0 y=705 class="ciFont2">"textUpper" and "textLower"</text>
  <text x="0" y="725" class="ciFont1">defined in CiSocket2Controller.ts</text>
  <CiSocket2 :x="50" :y="780" sid="socket2"></CiSocket2>
  <CiSocket2 :x="160" :y="780" sid="socket2" lines="2"></CiSocket2>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiSocket2 from './CiSocket2.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiSocket2
  }
})
</script>

<style>
</style>
