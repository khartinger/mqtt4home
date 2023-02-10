<!-- CiWeather1.vue -------------------------khartinger----- -->
<!-- 2022-02-19: new                                         -->
<!-- 2023-01-02: import { Geo } changed                      -->
<!-- 2023-02-09: change at CiBase (add Geo.ts)               -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <!--color background-------------------------------------- -->
  <rect class="ciLine" :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="geof.dyi()-2*geof.dyl"/>
  <rect class="ciLine" :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect class="ciLine" :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />

  <line v-if="drawLine34" class="ciLine" :x1="geof.x1()" :y1="geof.yr(4)" :x2="geof.x2()" :y2="geof.yr(4)" />

  <!--write text-------------------------------------------- -->
  <text :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt()" :y="geof.yt(2)" class="ciFont1">{{line2}}</text>
  <text v-if="geof.calcLinemax()>3" :x="geof.xt()" :y="geof.yt(3)" class="ciFont1">{{line3}}</text>
  <text v-if="geof.calcLinemax()>4" :x="geof.xt()" :y="geof.yt(4)" class="ciFont1">{{line4}}</text>
  <text v-if="geof.calcLinemax()>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>

  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Weather1, ciWeather1Controller } from '../controller/CiWeather1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiWeather1',
  components: {
    CiBase
  },
  data () {
    return {
      batteryMin: 15 as number
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
    fx: {
      type: Number,
      required: false,
      default: 1
    },
    fy: {
      type: Number,
      required: false,
      default: 1
    },
    color: {
      type: String,
      required: false,
      default: 'none'
    },
    colorH: {
      type: String,
      required: false,
      default: 'none'
    },
    colorF: {
      type: String,
      required: false,
      default: 'none'
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find weather1 object to given sid_________________
    weather1: function (): Weather1 | undefined {
      return ciWeather1Controller.weather1s.find(weather1 => weather1.id === this.sid)
    },
    // _______get iWeather1State________________________________
    iWeather1State: function (): number {
      // console.log('this.weather1?.iWeather1State=', this.weather1?.iWeather1State)
      if (this.weather1?.iWeather1State) {
        return this.weather1?.iWeather1State
      }
      return ciWeather1Controller.stateNoState // no state
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // =======date/time yyyymmdd HHMMSS methods=================
    lastWeather1Date: function (): string {
      return this.weather1?.lastWeather1Date ?? this.geof.noDate
    },
    lastWeather1Time: function (): string {
      return this.weather1?.lastWeather1Time ?? this.geof.noTime
    },
    // =======text in line ...==================================
    lineHeader: function (): string {
      if (this.weather1?.name) return this.geof.center(this.weather1.name)
      return this.geof.center(this.sid)
    },
    line2: function (): string {
      if (!this.weather1) return this.geof.center('ERROR')
      if (this.weather1?.temperature) return this.geof.center(this.weather1.temperature)
      return ''
    },
    line3: function (): string {
      if (!this.weather1) return this.geof.center('unknown')
      if (this.weather1?.humidity) return this.geof.center(this.weather1.humidity)
      return ''
    },
    line4: function (): string {
      // console.log('line4:', this.iWeather1State)
      if (!this.weather1) return this.geof.center('sid')
      if (this.weather1?.pressure) return this.geof.center(this.weather1.pressure)
      if (this.weather1?.brightness) return this.geof.center(this.weather1.brightness)
      if (this.weather1?.lastWeather1Date) {
        return this.geof.center(this.weather1.lastWeather1Date)
      }
      return ''
    },
    lineFooter: function (): string {
      if (this.weather1?.textFooter) return this.geof.center(this.weather1.textFooter)
      if (this.weather1?.battery) return this.geof.center(this.weather1.lastWeather1Time.substring(0, 5) + ' ' + this.weather1.battery.toString())
      if (this.weather1?.lastWeather1Time) return this.geof.center(this.weather1.lastWeather1Time.substring(0, 8))
      return this.geof.center(this.sid)
    },
    // =======background colors=================================
    // _______color of text area (line 2 to 4)__________________
    // depending on iWeather1State
    colorA_: function (): string {
      if (!this.weather1) return this.geof.colorNotOk
      if (this.iWeather1State !== ciWeather1Controller.stateOk) return this.geof.colorNotOk
      if (this.color !== 'none') return this.color
      return 'none'
    },
    // _______color of header___________________________________
    colorH_: function (): string {
      if (this.colorH !== 'none') return this.colorH
      return 'none'
    },
    // _______color of footer depending on battery value________
    colorF_: function (): string {
      if (!this.weather1) return this.geof.colorNotOk
      if (this.iWeather1State !== ciWeather1Controller.stateOk) return this.geof.colorNotOk
      if (this.weather1?.battery) {
        try {
          const batt = parseInt(this.geof.center(this.weather1.battery).valueOf())
          if (batt > this.batteryMin) return this.geof.colorOk
          return this.geof.colorNotOk
        } catch (error) { return this.geof.colorUnknown }
      }
      if (this.colorF !== 'none') return this.colorF
      return 'none'
    },

    // =======aditional drawings================================
    // _______draw a horizontal line if line 4 = date___________
    drawLine34: function (): boolean {
      if (this.geof.calcLinemax() < 4) return false
      let s1 = ''
      if (this.weather1?.lastWeather1Date) {
        s1 = this.geof.center(this.weather1.lastWeather1Date)
      }
      if (s1 === '') return false
      if (s1 === this.line4) return true
      return false
    }

  },

  methods: {
    // _______react on click: publish a message (if any exists)_
    onClk: function (): void {
      // console.log(this.sid, 'Weather1-Click')
      if (this.weather1) {
        if (this.weather1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.weather1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.weather1?.pubPayload) payload = this.weather1.pubPayload
            ciWeather1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
