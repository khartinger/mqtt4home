<!-- CiMain.vue (vuex410_ci_mqtt_timedate1)-------khartinger -->
<!-- 2023-02-25 new                                          -->
<template>
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTimeDate1 :x="50" :y="70" sid="timedate1" :border="3" lines="2" :fx="1" :fy="1" :textsize="1"></CiTimeDate1>
  -->
  <!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTimeDate1 :x="50" :y="70"  sid="timedate1" :fx="2" :fy="1"></CiTimeDate1>
  <CiTimeDate1 :x="50" :y="170" sid="timedate1" :fx="2" :fy="1"></CiTimeDate1>
  <CiTimeDate1 :x="250" :y="70" sid="timedate1" :fx="1" :fy="2"></CiTimeDate1>
  <CiTimeDate1 :x="350" :y="70" sid="timedate1" :fx="2" :fy="2"></CiTimeDate1>
  -->
  <svg width="100%" viewBox="0 0 500 710">
    <rect class="ciBackground" x="0" y="0" width="500" height="710" />
    <text x="0" y="15" class="ciFont1">viewbox 500x710</text>
    <CiTimeDate1 :x="50"  :y="70"  sid="timedate1" lines="0" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="160" :y="70"  sid="timedate1" :fx=1.5 :textsize=1.5 lines="2" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="325" :y="70" sid="timedate1" :fx=1.5 :textsize=2 lines="1" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="50"  :y="200" sid="timedate1" :fx=2 :textsize=2.5 lines="0" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="270" :y="200" sid="timedate1" :fx=2.5 :fy=1.2 :textsize=3 lines="0" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="50"  :y="350" sid="timedate1" :fx=3 :fy=1.6 :textsize=4 lines="0" :border="3" :textcenter="true"></CiTimeDate1>
    <CiTimeDate1 :x="50"  :y="540" sid="timedate1" :fx=4 :fy=2 :textsize=5 lines="0" :border="3" :textcenter="true"></CiTimeDate1>
    <text x=0   y=135 class="ciFont1">:textsize=1</text>
    <text x=110 y=135 class="ciFont1">:textsize=1.5</text>
    <text x=275 y=135 class="ciFont1">:textsize=2 (1.5 x 1)</text>
    <text x=0   y=265 class="ciFont1">:textsize=2.5 (2 x 1)</text>
    <text x=220 y=285 class="ciFont1">:textsize=3 (2.5 x 1.2)</text>
    <text x=0   y=475 class="ciFont1">:textsize=4 (3 x 1.6)</text>
    <text x=0   y=705 class="ciFont1">:textsize=5 (4 x 2)</text>
    <!--
  <svg width="100%" viewBox="-10 -10 660 1670">
  <rect class="ciBackground" x="-10" y="-10" width="660" height="1670" />
  <text x=0 y=10 class="ciFont1">&lt;CiTimeDate1 :x="0" :y="0" sid="timedate1" :border="0" lines="0"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiTimeDate1 :x="50"  :y="100" sid="timedate1" :border="0" lines="0"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="100" sid="timedate1" :border="1"  lines="0"></CiTimeDate1>
  <CiTimeDate1 :x="270" :y="100" sid="timedate1" :border="2" lines="0" ></CiTimeDate1>
  <CiTimeDate1 :x="380" :y="100" sid="timedate1" :border="3" lines="0" ></CiTimeDate1>
  <text x="5"   y="165" class="ciFont1">:border="0"</text>
  <text x="115" y="165" class="ciFont1">:border="1"</text>
  <text x="225" y="165" class="ciFont1">:border="2"</text>
  <text x="335" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="2")</text>

  <CiTimeDate1 :x="50"  :y="260" sid="timedate1" lines="0"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="260" sid="timedate1" lines="1"></CiTimeDate1>
  <CiTimeDate1 :x="270" :y="260" sid="timedate1" lines="2"></CiTimeDate1>
  <text x="5"   y="325" class="ciFont1">lines="0"</text>
  <text x="115" y="325" class="ciFont1">lines="1"</text>
  <text x="225" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">:textsize and stretch factor :fx, :fy</text>

  <text x=0 y=385 class="ciFont1k"> lines="0" :border="2"</text>
  <CiTimeDate1 :x="50"  :y="440" sid="timedate1" :fx=0.85 :fy=0.5 lines="0" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="440" sid="timedate1" :fx=1.15 :fy=0.65 :textsize=1.5 lines="0" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="295" :y="440" sid="timedate1" :fx=1.5 :fy=0.85 :textsize=2 lines="0" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="470" :y="440" sid="timedate1" :fx=2.2 :fy=1.2 :textsize=3 lines="0" :border="2"></CiTimeDate1>
  <text x="0"   y="490" class="ciFont1">:textsize=1</text>
  <text x="0"   y="505" class="ciFont1">:fx=0.85</text>
  <text x="0"   y="520" class="ciFont1">:fy=0.5</text>
  <text x="115" y="490" class="ciFont1">:textsize=1.5</text>
  <text x="115" y="505" class="ciFont1">:fx=1.15</text>
  <text x="115" y="520" class="ciFont1">:fy=0.65</text>
  <text x="250" y="490" class="ciFont1">:textsize=2</text>
  <text x="250" y="505" class="ciFont1">:fx=1.5</text>
  <text x="250" y="520" class="ciFont1">:fy=0.85</text>
  <text x="410" y="520" class="ciFont1">:textsize=3 :fx=2.2 :fy=1.2</text>

  <text x=0 y=555 class="ciFont1k"> lines="1" :border="2"</text>
  <CiTimeDate1 :x="50"  :y="610" sid="timedate1" :fx=0.85 :fy=0.65 lines="1" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="610" sid="timedate1" :fx=1.15 :fy=0.85 :textsize=1.5 lines="1" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="295" :y="610" sid="timedate1" :fx=1.5 :fy=1    :textsize=2 lines="1" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="470" :y="610" sid="timedate1" :fx=2.2 :fy=1.4 :textsize=3 lines="1" :border="2"></CiTimeDate1>
  <text x="0"   y="680" class="ciFont1">:textsize=1</text>
  <text x="0"   y="695" class="ciFont1">:fx=0.85</text>
  <text x="0"   y="710" class="ciFont1">:fy=0.65</text>
  <text x="115" y="680" class="ciFont1">:textsize=1.5</text>
  <text x="115" y="695" class="ciFont1">:fx=1.15</text>
  <text x="115" y="710" class="ciFont1">:fy=0.85</text>
  <text x="250" y="680" class="ciFont1">:textsize=2</text>
  <text x="250" y="695" class="ciFont1">:fx=1.5</text>
  <text x="250" y="710" class="ciFont1">:fy=1</text>
  <text x="410" y="710" class="ciFont1">:textsize=3 :fx=2.2 :fy=1.4</text>

  <text x=0 y=745 class="ciFont1k"> lines="2" :border="2"</text>

  <CiTimeDate1 :x="50"  :y="800" sid="timedate1" :fx=0.85 :fy=0.85 lines="2" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="800" sid="timedate1" :fx=1.15 :fy=1 :textsize=1.5 lines="2" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="295" :y="800" sid="timedate1" :fx=1.5 :fy=1.2 :textsize=2 lines="2" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="470" :y="800" sid="timedate1" :fx=2.2 :fy=1.55 :textsize=3 lines="2" :border="2"></CiTimeDate1>
  <text x="0"   y="885" class="ciFont1">:textsize=1</text>
  <text x="0"   y="900" class="ciFont1">:fx=0.85</text>
  <text x="0"   y="915" class="ciFont1">:fy=0.85</text>
  <text x="115" y="885" class="ciFont1">:textsize=1.5</text>
  <text x="115" y="900" class="ciFont1">:fx=1.15</text>
  <text x="115" y="915" class="ciFont1">:fy=1</text>
  <text x="250" y="885" class="ciFont1">:textsize=2</text>
  <text x="250" y="900" class="ciFont1">:fx=1.5</text>
  <text x="250" y="915" class="ciFont1">:fy=1.2</text>
  <text x="410" y="915" class="ciFont1">:textsize=3 :fx=2.2 :fy=1.55</text>

  <text x=0 y=950 class="ciFont1k">:textsize=2.5 | 4 | 5</text>

  <CiTimeDate1 :x="50"  :y="1005" sid="timedate1" :textsize=2.5 :fx=1.85 :fy=1 lines="0" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="400" :y="1005" sid="timedate1" :textsize=4 :fx=2.9 :fy=1.75 lines="1" :border="2"></CiTimeDate1>
  <CiTimeDate1 :x="50"  :y="1120" sid="timedate1" :textsize=5 :fx=3.5 :fy=1.9  lines="0" :border="2"></CiTimeDate1>
  <text x="0"   y="1065" class="ciFont1">:textsize=2.5 :fx=1.85 :fy=1</text>
  <text x="370" y="1140" class="ciFont1">:textsize=4 :fx=2.9 :fy=1.75</text>
  <text x="0"   y="1270" class="ciFont1">:textsize=5 :fx=3.5 :fy=1.9</text>

  <text x=0 y=1310 class="ciFont2">textcenter="true"</text>
  <CiTimeDate1 :x="50"  :y="1365" sid="timedate1" :border="2" lines="1" :fx=2.4 :textsize=2 :textcenter="true"></CiTimeDate1>
  <CiTimeDate1 :x="295" :y="1365" sid="timedate1" :border="2" lines="1" :fx=4 :fy=1.4 :textsize=3 :textcenter="true"></CiTimeDate1>
  <text x="0"   y="1425" class="ciFont1">:fx=2.4 :textsize=2</text>
  <text x="0"   y="1440" class="ciFont1">:textcenter="true"</text>
  <text x="245" y="1465" class="ciFont1">:fx=4 :fy=1.4 :textsize=3 :textcenter="true"</text>

  <text x=0 y=1510 class="ciFont2">Various color=...</text>
  <CiTimeDate1 :x="50"  :y="1565" sid="timedate1" :border="2" lines="0" color="white"></CiTimeDate1>
  <CiTimeDate1 :x="160" :y="1565" sid="timedate1" :border="2" lines="1" color="white"></CiTimeDate1>
  <CiTimeDate1 :x="270" :y="1565" sid="timedate1" :border="2" lines="2" color="white"></CiTimeDate1>
  <CiTimeDate1 :x="380"  :y="1565" sid="timedate1" :border="2" lines="2" color="gold" colorH="lightgreen" colorF="green"></CiTimeDate1>
  <CiTimeDate1 :x="490" :y="1565" sid="timedate1" :border="2" lines="2" :fx=2 :textsize=1.5 color="#C0C0FF" colorH="#E0E0FF" colorF="#A0A0FF"></CiTimeDate1>

  <text x="0"   y="1625" class="ciFont1">color="white"</text>
  <text x="110" y="1640" class="ciFont1">color="white"</text>
  <text x="215" y="1625" class="ciFont1">color="white"</text>
  <text x="330" y="1625" class="ciFont1">color="gold"</text>
  <text x="300" y="1640" class="ciFont1">colorH="lightgreen"</text>
  <text x="330" y="1655" class="ciFont1">colorF="green"</text>
  <text x="480" y="1625" class="ciFont1">color="#C0C0FF"</text>
  <text x="480" y="1640" class="ciFont1">colorH="#E0E0FF"</text>
  <text x="480" y="1655" class="ciFont1">colorF="#A0A0FF"</text>
-->
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiTimeDate1 from './CiTimeDate1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiTimeDate1
  }
})
</script>

<style>
</style>
