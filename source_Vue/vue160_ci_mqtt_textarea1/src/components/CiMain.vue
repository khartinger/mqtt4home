<!-- CiMain.vue (vue160_ci_mqtt_textarea1)--------khartinger -->
<!-- 2023-02-05: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTextarea1 :x="50" :y="70" sid="textarea1" :border="3" lines="2" :fx="1" :fy="1"></CiTextarea1>
-->
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTextarea1 :x="50" :y="70"  sid="textarea1" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="50" :y="170" sid="textarea2" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="250" :y="70" sid="textarea1" :fx="1" :fy="2"></CiTextarea1>
  <CiTextarea1 :x="350" :y="70" sid="textarea2" :fx="2" :fy="2"></CiTextarea1>
-->
<svg width="100%" viewBox="-10 -10 670 1520">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="1520" />
  <text x=0 y=10 class="ciFont1">&lt;CiTextarea1 :x="0" :y="0" sid="textarea1" :border="0" lines="0"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>
  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiTextarea1 :x="50"  :y="100" sid="textarea1" :border="0" lines="0"></CiTextarea1>
  <CiTextarea1 :x="160" :y="100" sid="textarea1" :border="1"  lines="0"></CiTextarea1>
  <CiTextarea1 :x="270" :y="100" sid="textarea1" :border="2" lines="0" ></CiTextarea1>
  <CiTextarea1 :x="380" :y="100" sid="textarea1" :border="3" lines="0" ></CiTextarea1>
  <text x="5"   y="165" class="ciFont1">:border="0"</text>
  <text x="115" y="165" class="ciFont1">:border="1"</text>
  <text x="225" y="165" class="ciFont1">:border="2"</text>
  <text x="335" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="2")</text>

  <CiTextarea1 :x="50"  :y="260" sid="textarea1" lines="0"></CiTextarea1>
  <CiTextarea1 :x="160" :y="260" sid="textarea1" lines="1"></CiTextarea1>
  <CiTextarea1 :x="270" :y="260" sid="textarea1" lines="2"></CiTextarea1>
  <text x="5"   y="325" class="ciFont1">lines="0"</text>
  <text x="115" y="325" class="ciFont1">lines="1"</text>
  <text x="225" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">Stretch factor fx, fy</text>
  <text x=0 y=385 class="ciFont1">:border="2" lines="0"</text>
  <CiTextarea1 :x="50"  :y="440" sid="textarea1" :fx=2  lines="0" :border="2"></CiTextarea1>
  <CiTextarea1 :x="270" :y="440" sid="textarea1" :fy=0.5  lines="0" :border="2"></CiTextarea1>
  <CiTextarea1 :x="380" :y="440" sid="textarea1" :fx=1.5 :fy=0.7 lines="0" :border="2"></CiTextarea1>
  <CiTextarea1 :x="540" :y="440" sid="textarea1" :fx=1.6 :fy=1.4 lines="0" :border="2"></CiTextarea1>

  <text x="5"   y="500" class="ciFont1">:fx=2</text>
  <text x="225" y="450" class="ciFont1">:fy=0.5</text>
  <text x="335" y="470" class="ciFont1">:fx=1.5 :fy=0.7</text>
  <text x="495" y="540" class="ciFont1">:fx=1.6 :fy=1.4</text>

  <text x=0 y=555 class="ciFont1">:border="3" lines="1"</text>
  <CiTextarea1 :x="50"  :y="610" sid="textarea1" :fx=2  lines="1" :border="3"></CiTextarea1>
  <CiTextarea1 :x="270" :y="610" sid="textarea1" :fy=0.5  lines="1" :border="3"></CiTextarea1>
  <CiTextarea1 :x="380" :y="610" sid="textarea1" :fx=1.5 :fy=0.7 lines="1" :border="3"></CiTextarea1>
  <CiTextarea1 :x="540" :y="610" sid="textarea1" :fx=1.6 :fy=1.4 lines="1" :border="3"></CiTextarea1>

  <text x="5"   y="670" class="ciFont1">:fx=2</text>
  <text x="225" y="620" class="ciFont1">:fy=0.5</text>
  <text x="335" y="640" class="ciFont1">:fx=1.5 :fy=0.7</text>
  <text x="495" y="710" class="ciFont1">:fx=1.6 :fy=1.4</text>

  <text x=0 y=725 class="ciFont1">:border="3" lines="2"</text>
  <CiTextarea1 :x="50"  :y="780" sid="textarea1" :fx=2  lines="2" :border="3"></CiTextarea1>
  <CiTextarea1 :x="270" :y="780" sid="textarea1" :fy=0.5  lines="2" :border="3"></CiTextarea1>
  <CiTextarea1 :x="380" :y="780" sid="textarea1" :fx=1.5 :fy=0.7 lines="2" :border="3"></CiTextarea1>
  <CiTextarea1 :x="540" :y="780" sid="textarea1" :fx=1.6 :fy=1.4 lines="2" :border="3"></CiTextarea1>

  <text x="5"   y="840" class="ciFont1">:fx=2</text>
  <text x="225" y="790" class="ciFont1">:fy=0.5</text>
  <text x="335" y="810" class="ciFont1">:fx=1.5 :fy=0.7</text>
  <text x="495" y="880" class="ciFont1">:fx=1.6 :fy=1.4</text>

  <text x=0 y=905 class="ciFont2">Various color=...</text>

  <CiTextarea1 :x="50"  :y="960" sid="textarea1" lines="0" color="white"></CiTextarea1>
  <CiTextarea1 :x="160" :y="960" sid="textarea1" color="red"></CiTextarea1>
  <CiTextarea1 :x="270" :y="960" sid="textarea1" lines="2" color="#EEDD00"></CiTextarea1>
  <CiTextarea1 :x="400" :y="960" sid="textarea1" lines="2" color="white" colorF="lightgreen" colorH="lightblue" :border="2" :fx="3" :fy="1.2"></CiTextarea1>
  <text x="0"   y="1025" class="ciFont1">color="white"</text>
  <text x="115" y="1025" class="ciFont1">color="red"</text>
  <text x="225" y="1025" class="ciFont1">color="#EEDD00"</text>
  <text x="355" y="1045" class="ciFont1">color="white" colorH="lightblue"</text>
  <text x="355" y="1060" class="ciFont1">colorF="lightgreen"</text>

  <line x1="0" y1="1070" x2="670" y2="1070" stroke="black" stroke-width="2"/>
  <text x="0" y="1090" class="ciFont1">Define this in CiTextarea1Controller.ts:</text>
  <text x=0 y=1120 class="ciFont2">clkWhenMouted</text>

  <CiTextarea1 :x="50"  :y="1180" sid="textarea1" lines="2" :fx="2"></CiTextarea1>
  <CiTextarea1 :x="250" :y="1180" sid="textarea2" lines="2" :fx="2"></CiTextarea1>
  <text x="5"   y="1240" class="ciFont1">clkWhenMounted: false</text>
  <text x="205" y="1240" class="ciFont1">clkWhenMounted: true</text>

  <text x=0 y=1275 class="ciFont2">useRawText</text>

  <CiTextarea1 :x="50"  :y="1330" sid="textarea3" lines="0" :fx="0.9" :fy="2.1"></CiTextarea1>
  <CiTextarea1 :x="250" :y="1330" sid="textarea4" lines="0" :fx="0.9" :fy="2.1"></CiTextarea1>
  <text x="5"   y="1500" class="ciFont1">useRawText: false</text>
  <text x="205" y="1500" class="ciFont1">useRawText: true</text>
  <text x="330" y="1300" class="ciFont1">Use ^~ for extra line breaks.</text>
  <text x="330" y="1340" class="ciFont1">Send text to this textareas by MQTT:</text>
  <text x="330" y="1360" class="ciFont1">mosquitto_pub -h 10.1.1.1</text>
  <text x="350" y="1380" class="ciFont1">-t ci/textarea1/3/set -m text</text>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiTextarea1 from './CiTextarea1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiTextarea1
  }
})
</script>

<style>
</style>
