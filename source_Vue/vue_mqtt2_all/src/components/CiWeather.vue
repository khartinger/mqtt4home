<!--CiWeather.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <rect class="ciRect" :x="geo.xr()" :y="geo.yr(1)" :width="geo.dxi" :height="geo.dyl" />
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(2)" :width="geo.dxi" :height="3 * geo.dyl" :fill="colorState"/>
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
  <!--write text-------------------------------------------- -->
  <text :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text :x="geo.xt()" :y="geo.yt(2)" class="ciFont1">{{line2}}</text>
  <text :x="geo.xt()" :y="geo.yt(3)" class="ciFont1">{{line3}}</text>
  <text :x="geo.xt()" :y="geo.yt(4)" class="ciFont1">{{line4}}</text>
  <text :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Weather, ciWeatherController } from '../controller/CiWeatherController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiWeather',
  components: {
    CiBase
  },
  data () {
    return {
      batteryMin: 15
    }
  },
  props: {
    x: {
      type: Number,
      required: true
    },
    y: {
      type: Number,
      required: true
    },
    border: {
      type: Number,
      required: false,
      default: 2
    },
    sid: {
      type: String,
      required: true
    },
    lines: {
      type: String,
      required: false,
      default: '1'
    }
  },
  computed: {
    weather: function (): Weather | undefined {
      return ciWeatherController.weathers.find(weather => weather.id === this.sid)
    },
    iWeatherState: function (): number {
      return this.weather?.iWeatherState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.weather?.name) return this.geo.center(this.weather.name)
      return this.geo.center(this.sid)
    },
    line2: function (): string {
      if (this.iWeatherState !== 0) return ''
      if (this.weather?.temperature) return this.geo.center(this.weather.temperature.toString())
      return ''
    },
    line3: function (): string {
      if (this.iWeatherState !== 0) return this.geo.center('unknown')
      if (this.weather?.humidity) return this.geo.center(this.weather.humidity.toString())
      return ''
    },
    line4: function (): string {
      if (this.iWeatherState !== 0) return ''
      // if (this.weather?.type === 'zb') return ''
      if (this.weather?.brightness) return this.geo.center(this.weather.brightness.toString())
      return ''
    },
    line5: function (): string {
      if (this.weather?.battery) return this.geo.center(this.weather.battery.toString())
      return ''
    },

    // -------color line 2 to 4 depending on iWeatherState------
    colorState: function (): string {
      // console.log('colorState: ', 'iWeatherState=' + this.iWeatherState)
      if (this.iWeatherState === 0) return this.geo.colorOk
      if (this.iWeatherState === 1) return this.geo.colorNotOk
      return this.geo.colorUnknown
      // return 'none'
    },

    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.weather?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.weather.battery).valueOf())
          if (batt > this.batteryMin) return this.geo.colorOk
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return this.geo.colorUnknown
      // return 'none'
    }
  },

  methods: {
    /*
    // -------calculate y start value of rectangle 1...5--------
    calcYr: function (linenum: number): number {
      if (linenum < 1 || linenum > 5) linenum = 1
      return (this.y - geo.dyi2 + (linenum - 1) * geo.dyl)
    },
    // -------calculate y start value of text 1...5-------------
    calcYt: function (linenum: number): number {
      if (linenum < 1 || linenum > 5) linenum = 1
      return (this.y + geo.yt1 + (linenum - 1) * geo.dyl)
    },
    */
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'Weather-Click')
      if (this.weather) {
        if (this.weather.pubTopic) {
          let payload = '?'
          const aPubTopic = this.weather.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.weather?.pubPayload) payload = this.weather.pubPayload
            ciWeatherController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
