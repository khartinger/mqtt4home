<!--CiMain.vue-->
<template>
  <svg width="100%" viewBox="0 0 2100 800">
  <rect class="ciBackground" x="0" y="0" width="2100" height="800" />
  <text x="5" :y="0.1*dy" class="ciFont1">Sensors</text>
  <CiLamp :x="0.5*dx" :y="0.6*dy" sid="lamp1" :border="0"></CiLamp>
  <CiLamp :x="1.5*dx" :y="0.6*dy" sid="lamp2" lines="2"></CiLamp>
  <!--
  <line x1 = 230 y1 = -10 x2 = 230 y2 = 120 stroke = "black" />
  -->
  <CiWeather :x="3.5*dx" :y="0.6*dy" sid="weather1"></CiWeather>
  <CiWeather :x="4.5*dx" :y="0.6*dy" sid="weather2"></CiWeather>
  <CiMotion  :x="5.5*dx" :y="0.6*dy" sid="motion1"></CiMotion>
  <CiPump    :x="6.5*dx" :y="0.6*dy" sid="pump1"></CiPump>

  <CiText5   :x="7.5*dx" :y="0.6*dy" sid="text5" :border="2"></CiText5>
  <CiText5L  :x="8.5*dx" :y="0.6*dy" sid="text5L" :border="2"></CiText5L>

  <text x="5" :y="1.2*dy" class="ciFont1">Actors</text>
  <CiButton  :x="0.5*dx" :y="1.7*dy" sid="button_1" lines="2" :border="0"></CiButton>
  <CiButton  :x="1.5*dx" :y="1.7*dy" sid="button_2" lines="1" :border="0"></CiButton>
  <CiButton  :x="2.5*dx" :y="1.7*dy" sid="button_3" lines="1" :border="2"></CiButton>
  <CiButton  :x="3.5*dx" :y="1.7*dy" sid="button_up" lines="2" :border="1"></CiButton>
  <CiButton  :x="4.5*dx" :y="1.7*dy" sid="button_down" lines="0" :border="0"></CiButton>
  <CiButton  :x="5.5*dx" :y="1.7*dy" sid="button_left" lines="1" :border="0"></CiButton>
  <CiButton  :x="6.5*dx" :y="1.7*dy" sid="button_right" lines="1" :border="0"></CiButton>

  <CiButton2  :x="7.5*dx" :y="1.7*dy" sid="button2_1" lines="0" :border="0"></CiButton2>
  <CiButton2  :x="8.5*dx" :y="1.7*dy" sid="button2_1" lines="1" :border="0"></CiButton2>
  <CiButton2  :x="9.5*dx" :y="1.7*dy" sid="button2_2" lines="2" :border="0"></CiButton2>
  <CiSocket   :x="10.5*dx" :y="1.7*dy" sid="socket1" :border="0"></CiSocket>

  <CiDoor   :x="0.5*dx" :y="3.0*dy" sid="doorH1" door-dir="H1" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="3.0*dy" sid="doorh1" door-dir="h1" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="3.0*dy" sid="doorH2" door-dir="H2" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="3.0*dy" sid="doorh2" door-dir="h2" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="3.0*dy" sid="doorH3" door-dir="H3" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="3.0*dy" sid="doorh3" door-dir="h3" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="3.0*dy" sid="doorH4" door-dir="H4" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="3.0*dy" sid="doorh4" door-dir="h4" :border="0"></CiDoor>

  <CiDoor   :x="0.5*dx" :y="4.5*dy" sid="doorV1" door-dir="V1" :border="0"></CiDoor>
  <CiDoor   :x="2.0*dx" :y="4.5*dy" sid="doorv1" door-dir="v1" :border="0"></CiDoor>
  <CiDoor   :x="3.5*dx" :y="4.5*dy" sid="doorV2" door-dir="V2" :border="0"></CiDoor>
  <CiDoor   :x="5.0*dx" :y="4.5*dy" sid="doorv2" door-dir="v2" :border="0"></CiDoor>
  <CiDoor   :x="6.5*dx" :y="4.5*dy" sid="doorV3" door-dir="V3" :border="0"></CiDoor>
  <CiDoor   :x="8.0*dx" :y="4.5*dy" sid="doorv3" door-dir="v3" :border="0"></CiDoor>
  <CiDoor   :x="9.5*dx" :y="4.5*dy" sid="doorV4" door-dir="V4" :border="0"></CiDoor>
  <CiDoor   :x="11.0*dx" :y="4.5*dy" sid="doorv4" door-dir="v4" :border="0"></CiDoor>

  <CiBlind  :x="0.5*dx" :y="6.0*dy" sid="blindH" blind-dir="H" :border="0"></CiBlind>
  <CiBlind  :x="2.0*dx" :y="6.0*dy" sid="blindV" blind-dir="V" :border="0"></CiBlind>
  <CiBlind  :x="4.0*dx" :y="6.0*dy" sid="blindH" blind-dir="H" :border="0" :f="2"></CiBlind>

  <CiDoor   :x="6.0*dx" :y="6.0*dy" sid="doorH1" door-dir="h1" :border="0" :f=2></CiDoor>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
// import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'
import CiLamp from './CiLamp.vue'
import CiButton from './CiButton.vue'
import CiWeather from './CiWeather.vue'
import CiMotion from './CiMotion.vue'
import CiText5 from './CiText5.vue'
import CiText5L from './CiText5L.vue'
import CiPump from './CiPump.vue'
import CiSocket from './CiSocket.vue'
import CiDoor from './CiDoor.vue'
import CiBlind from './CiBlind.vue'
import CiButton2 from './CiButton2.vue'
import { Geo } from './CiBase.vue'

// ------MQTT broker parameter----------------------------------
// const host = '10.1.1.1'
// const port = 1884
// const subtopic = '#'

export default defineComponent({
  name: 'CiMain',
  components: {
    CiLamp,
    CiButton,
    CiWeather,
    CiMotion,
    CiText5,
    CiText5L,
    CiPump,
    CiSocket,
    CiDoor,
    CiBlind,
    CiButton2
  },
  // mounted: async function (): Promise<void> {
  //   ciMqttClientInstance.connect(host, port, '')
  //   ciMqttClientInstance.subscribe(subtopic, 0)
  // },
  computed: {
    geo: function (): Geo {
      const geo1 = new Geo(0, 0)
      return geo1
    },
    dx: function (): number { return (10 + this.geo.dxo) },
    dy: function (): number { return (10 + this.geo.dyo) }
  }
})
</script>

<style>
</style>
