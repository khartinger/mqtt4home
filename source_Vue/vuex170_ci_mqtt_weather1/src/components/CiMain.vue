<!-- CiMain.vue (vuex170_ci_mqtt_weather1)---------khartinger -->
<!-- 2023-02-08: new                                         -->

<template>
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiWeather1 :x="50" :y="70" sid="weather1" :border="3"></CiWeather1>
  -->
<!--
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiWeather1 :x="50" :y="70"  sid="weather1" :fx="2" :fy="1"></CiWeather1>
  <CiWeather1 :x="50" :y="170" sid="weather3" :fx="2" :fy="1"></CiWeather1>
  <CiWeather1 :x="250" :y="70" sid="weather1" :fx="1" :fy="2"></CiWeather1>
  <CiWeather1 :x="350" :y="70" sid="weather1" :fx="2" :fy="2"></CiWeather1>
-->

<svg width="100%" viewBox="-10 -10 670 570">
  <rect class="ciBackground" x="-10" y="-10" width="670" height="570" />
  <text x=0 y=10 class="ciFont1">&lt;CiWeather1 :x="0" :y="0" sid="weather1" :border="0" /&gt;</text>
  <line x1="-10" y1="15" x2="670" y2="15" stroke="black" stroke-width="2"/>
  <text x=0 y=45 class="ciFont2">Various :border=...</text>

  <CiWeather1 :x="50"  :y="100" sid="weather1" :border="0"></CiWeather1>
  <CiWeather1 :x="160" :y="100" sid="weather1" :border="1"></CiWeather1>
  <CiWeather1 :x="270" :y="100" sid="weather1" :border="2"></CiWeather1>
  <CiWeather1 :x="380" :y="100" sid="weather1" :border="3"></CiWeather1>
  <text x="5"   y="165" class="ciFont1">:border="0"</text>
  <text x="115" y="165" class="ciFont1">:border="1"</text>
  <text x="225" y="165" class="ciFont1">:border="2"</text>
  <text x="335" y="165" class="ciFont1">:border="3"</text>

  <text x=0 y=205 class="ciFont2">Stretch factor fx, fy</text>
  <text x=0 y=225 class="ciFont1">:border="2"</text>
  <CiWeather1 :x="50"  :y="280" sid="weather1" :fx=2  :border="2"></CiWeather1>
  <CiWeather1 :x="270" :y="280" sid="weather1" :fy=0.5 :border="2"></CiWeather1>
  <CiWeather1 :x="380" :y="280" sid="weather1" :fx=1.5 :fy=0.7 :border="2"></CiWeather1>
  <CiWeather1 :x="540" :y="280" sid="weather1" :fx=1.6 :fy=1.4 :border="2"></CiWeather1>

  <text x="5"   y="340" class="ciFont1">:fx=2</text>
  <text x="225" y="290" class="ciFont1">:fy=0.5</text>
  <text x="335" y="310" class="ciFont1">:fx=1.5 :fy=0.7</text>
  <text x="495" y="380" class="ciFont1">:fx=1.6 :fy=1.4</text>

  <text x=0 y=395 class="ciFont2">Various color=...</text>

  <CiWeather1 :x="50"  :y="450" sid="weather1" color="white"></CiWeather1>
  <CiWeather1 :x="160" :y="450" sid="weather1" color="red"></CiWeather1>
  <CiWeather1 :x="270" :y="450" sid="weather1" color="#EEDD00" colorH="white"></CiWeather1>
  <CiWeather1 :x="400" :y="450" sid="weather2" color="white" colorF="yellow" colorH="lightblue" :border="2" :fx="3" :fy="1.2"></CiWeather1>
  <text x="0"   y="515" class="ciFont1">color="white"</text>
  <text x="115" y="515" class="ciFont1">color="red"</text>
  <text x="225" y="515" class="ciFont1">color="#EEDD00"</text>
  <text x="225" y="535" class="ciFont1">colorH="white"</text>
  <text x="355" y="535" class="ciFont1">color="white" colorH="lightblue"</text>
  <text x="355" y="555" class="ciFont1">colorF="yellow"</text>

</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiWeather1 from './CiWeather1.vue'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiWeather1
  }
})
</script>

<style>
</style>
