<!-- CiMain.vue (vuex120_ci_mqtt_button2)---------khartinger -->
<!-- 2022-01-27: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 400 210">
  <rect class="ciBackground" x="0" y="0" width="400" height="210" />
  <CiButton2 :x="50" :y="60" sid="button1" shape="round_1up_2down" :fx="2" :fy="1"></CiButton2>
  <CiButton2 :x="50" :y="160" sid="button1" shape="rect_1left_2right" :fx="2" :fy="1"></CiButton2>
  <CiButton2 :x="250" :y="60" sid="button1" shape="round_1up_2down" :fx="1" :fy="2"></CiButton2>
  <CiButton2 :x="350" :y="60" sid="button1" shape="rect_1left_2right" :fx="1" :fy="2"></CiButton2>
-->
<svg width="100%" viewBox="-10 -10 670 1410">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="1410" />
  <text x=0 y=10 class="ciFont1">&lt;CiButton2 :x="0" :y="0" sid="button1" :border="0" lines="0" shape="round"/&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>

  <text x=0 y=45 class="ciFont2">Various :border=... (lines="0")</text>

  <CiButton2 :x="50"  :y="100" sid="button1" :border="0" lines="0"></CiButton2>
  <CiButton2 :x="160" :y="100" sid="button1" :border="1"  lines="0"></CiButton2>
  <CiButton2 :x="270" :y="100" sid="button1" :border="2" lines="0" ></CiButton2>
  <CiButton2 :x="380" :y="100" sid="button1" :border="3" lines="0" ></CiButton2>
  <text x="0"   y="165" class="ciFont1">:border="0"</text>
  <text x="110" y="165" class="ciFont1">:border="1"</text>
  <text x="220" y="165" class="ciFont1">:border="2"</text>
  <text x="330" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Various lines=... (:border="1")</text>

  <CiButton2 :x="50"  :y="260" sid="button1" lines="0"></CiButton2>
  <CiButton2 :x="160" :y="260" sid="button1" lines="1"></CiButton2>
  <CiButton2 :x="270" :y="260" sid="button1" lines="2"></CiButton2>
  <text x="0"   y="325" class="ciFont1">lines="0"</text>
  <text x="110" y="325" class="ciFont1">lines="1"</text>
  <text x="220" y="325" class="ciFont1">lines="2"</text>

  <text x=0 y=365 class="ciFont2">Stretch factor fx, fy</text>
  <CiButton2 :x="50"  :y="420" sid="button1" :fx=2  lines="0"></CiButton2>
  <CiButton2 :x="270" :y="420" sid="button1" :fy=0.5  lines="0"></CiButton2>
  <CiButton2 :x="380" :y="420" sid="button1" :fx=1.5 :fy=0.75  lines="0"></CiButton2>
  <text x="0"   y="485" class="ciFont1">:fx=2</text>
  <text x="220" y="435" class="ciFont1">:fy=0.5</text>
  <text x="330" y="460" class="ciFont1">:fx=1.5 :fy=0.75</text>

  <text x=0 y=525 class="ciFont2">Various shape=...</text>
  <text x="0" y="545" class="ciFont1">with :fy="1.5"</text>
  <CiButton2 :x="50"  :y="600" sid="button1" lines="0" :fy="1.5" shape="circle"></CiButton2>
  <CiButton2 :x="160" :y="600" sid="button1" lines="0" :fy="1.5" shape="round"></CiButton2>
  <CiButton2 :x="270" :y="600" sid="button1" lines="0" :fy="1.5" shape="rect"></CiButton2>
  <CiButton2 :x="380" :y="600" sid="button1" lines="0" :fy="1.5" shape="square"></CiButton2>
  <CiButton2 :x="490" :y="600" sid="button1" lines="1" :fy="1.5" shape="round"></CiButton2>
  <CiButton2 :x="600" :y="600" sid="button1" lines="2" :fy="1.5" shape="round"></CiButton2>
  <text x="0"   y="715" class="ciFont1">"circle"</text>
  <text x="110" y="715" class="ciFont1">"round"</text>
  <text x="220" y="715" class="ciFont1">"rect"</text>
  <text x="330" y="715" class="ciFont1">"square"</text>
  <text x="440" y="715" class="ciFont1">round:lines="1"|lines="2"</text>

  <text x="0" y="735" class="ciFont1">with :fx="1.5"</text>
  <CiButton2 :x="50"  :y="790" sid="button1"  lines="0" :fx="1.5" shape="circle"></CiButton2>
  <CiButton2 :x="210" :y="790" sid="button1"  lines="0" :fx="1.5" shape="round"></CiButton2>
  <CiButton2 :x="370" :y="790" sid="button1"  lines="0" :fx="1.5" shape="rect"></CiButton2>
  <CiButton2 :x="530" :y="790" sid="button1"  lines="0" :fx="1.5" shape="square"></CiButton2>
  <text x="0"   y="855" class="ciFont1">"circle"</text>
  <text x="160" y="855" class="ciFont1">"round"</text>
  <text x="320" y="855" class="ciFont1">"rect"</text>
  <text x="480" y="855" class="ciFont1">"square"</text>

  <text x=0 y=895 class="ciFont2">Additional shape drawings</text>

  <CiButton2 :x="50"  :y="950" sid="button1" lines="0" shape="1up_2down"></CiButton2>
  <CiButton2 :x="160" :y="950" sid="button1" lines="0" shape="1left_2right"></CiButton2>
  <CiButton2 :x="270" :y="950" sid="button1" lines="0" shape="rect_1up_2down"></CiButton2>
  <CiButton2 :x="380" :y="950" sid="button1" lines="0" :fx=2 shape="round_1right_2left"></CiButton2>
  <CiButton2 :x="600" :y="950" sid="button1" lines="2" shape="square_1up_2down"></CiButton2>

  <text x="0"   y="1015" class="ciFont1">"1up_2down"</text>
  <text x="105" y="1030" class="ciFont1">"1left_2right"</text>
  <text x="200" y="1015" class="ciFont1">"rect_1up_2down"</text>
  <text x="330" y="1030" class="ciFont1">"round_1right_2left" (:fx=2)</text>
  <text x="510" y="1015" class="ciFont1">"square_1up_2down"</text>

  <text x=0 y=1075 class="ciFont2">Various colorup=... colordn=...</text>

  <CiButton2 :x="50"  :y="1140" sid="button1" lines="0" colorup="green" colordn="red"></CiButton2>
  <CiButton2 :x="210" :y="1140" sid="button1" colorup="yellow" colordn="grey"></CiButton2>
  <CiButton2 :x="370" :y="1140" sid="button1" lines="2" colorup="#EEDD00"></CiButton2>
  <text x="0"   y="1205" class="ciFont1">colorup="green"</text>
  <text x="0"   y="1220" class="ciFont1">colordn="red"</text>
  <text x="160" y="1205" class="ciFont1">colorup="yellow"</text>
  <text x="160" y="1220" class="ciFont1">colordn="grey"</text>
  <text x="320" y="1205" class="ciFont1">colorup="#EEDD00"</text>

  <line x1="0" y1="1235" x2="670" y2="1235" stroke="black" stroke-width="2"/>
  <text x=0 y=1265 class="ciFont2">"textUpper" and "textLower"</text>
  <text x="0" y="1285" class="ciFont1">defined in CiButton2Controller.ts</text>
  <CiButton2 :x="50" :y="1340" sid="button2"></CiButton2>
  <CiButton2 :x="160" :y="1340" sid="button2" lines="2"></CiButton2>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiButton2 from './CiButton2.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiButton2
  }
})
</script>

<style>
</style>
