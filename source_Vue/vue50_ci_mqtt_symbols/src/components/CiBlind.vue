<!-- CiBlind.vue ----------------------------2022-01-08----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <g id="blindgroup" :transform="rotateGroup">
    <path :d="blindMotor" :fill="colorMotor" stroke="black" stroke-width="1"/>
    <path :d="blindRect" :fill="colorBlind" :stroke="colorBlindline" :stroke-width="1"/>
    <!--write text------------------------------------------ -->
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
    <rect :x="geo.x1()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
  </g>
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Blind, ciBlindController } from '../controller/CiBlindController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiBlind',
  components: {
    CiBase
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
    dir: {
      type: String,
      required: true
    },
    f: {
      type: Number,
      required: false,
      default: 1
    },
    text5: {
      type: String,
      required: false,
      default: ''
    },
    type: {
      type: String,
      required: false,
      default: ''
    }
  },
  computed: {
    blind: function (): Blind | undefined {
      return ciBlindController.blinds.find(blind => blind.id === this.sid)
    },
    iBlindState: function (): number {
      return this.blind?.iBlindState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.blind?.name) {
        if (this.blind.name.includes('?')) {
          const i2 = this.blind.name.indexOf('?')
          const s1 = this.blind.name.substring(0, i2)
          let s2 = '?'
          if (this.iBlindState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iBlindState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iBlindState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.blind.name)
      }
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.text5.length > 0) return this.text5
      if (this.blind?.text5) {
        if (this.blind.text5.includes('?')) {
          const i2 = this.blind.text5.indexOf('?')
          const s1 = this.blind.text5.substring(0, i2)
          let s2 = '?'
          if (this.iBlindState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iBlindState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iBlindState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.blind.text5)
      }
      if (this.blind?.battery) return this.geo.center(this.blind.battery)
      return ''
      // return this.geo.center(this.sid)
    },
    // -------color wall----------------------------------------
    colorMotor: function (): string {
      try {
        if (this.blind?.iMotorState === undefined) return this.geo.colorWall
        if (this.blind) {
          if (this.blind.iMotorState === -1) return this.geo.colorOn2 // motor down
          if (this.blind.iMotorState === 0) return this.geo.colorOff2 // motor stopp
          if (this.blind.iMotorState === 1) return this.geo.colorOn // motor up
          return this.geo.colorUnknown
        }
      } catch (error) { console.error(error) }
      return this.geo.colorWall
    },
    // -------blind color----------------------------------------
    colorBlind: function (): string {
      if (this.iBlindState === 0) return this.geo.colorOff2 //    closed (light red)
      if (this.iBlindState === 1) return this.geo.colorBackground // open (light green)
      if (this.iBlindState === 2) return this.geo.colorLock //    locked (red)
      return this.geo.colorUnknown
    },
    // -------color of line around blind rectangle--------------
    colorBlindline: function (): string {
      if (this.iBlindState === 1) return this.geo.colorOff
      return 'black'
    },
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.blind?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.blind.battery).valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorOk
          if (batt > this.geo.batteryMin) return this.geo.colorBackground
          return this.geo.colorNotOk
        } catch (error) { console.error(error) }
      }
      return 'none'
    },
    // strokewidth: function (): string {
    //   return '2'
    // },
    blindMotor: function (): string {
      const dxof = this.geo.dxo * this.f
      const dyb01 = 3 * this.geo.dyl / 2
      const dmx = 2 * this.geo.dxm
      const dmy = dyb01 - this.geo.dw2
      const sDir = this.dir.toLowerCase()
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (sDir === 'h1' || sDir === 'h2' || sDir === 'v2' || sDir === 'v4' || sDir === 'd1y' || sDir === 'd2y' || sDir === 'd3x' || sDir === 'd4x') {
        s1 += 'm-' + dxof / 2 + ',-' + dyb01 + ' h' + dmx + ' v' + dmy + ' h-' + dmx + ' z'
        s1 += 'm' + dxof + ',0 h-' + dmx + ' v' + dmy + ' h' + dmx + ' z'
      }
      if (sDir === 'h3' || sDir === 'h4' || sDir === 'v1' || sDir === 'v3' || sDir === 'd1x' || sDir === 'd2x' || sDir === 'd3y' || sDir === 'd4y') {
        s1 += 'm-' + dxof / 2 + ',' + dyb01 + ' h' + dmx + ' v-' + dmy + ' h-' + dmx + ' z'
        s1 += 'm' + dxof + ',0 h-' + dmx + ' v-' + dmy + ' h' + dmx + ' z'
      }
      return s1
    },
    blindRect: function (): string {
      if (this.dir.length < 2) return ''
      const dxof = this.geo.dxo * this.f
      const dmx = 2 * this.geo.dxm
      const dxb = dxof - 2 * dmx
      const dyb01 = 3 * this.geo.dyl / 2
      const dyb0 = this.geo.dyl * 3 / 4
      const dyb1 = dyb01 - dyb0
      const dyb2 = this.geo.dyi / 2 - dyb0
      const sDir = this.dir.toLowerCase()
      let dybx = dyb1
      if (this.dir.charAt(0) === 'H' || this.dir.charAt(0) === 'V' || this.dir.charAt(0) === 'D') dybx = dyb2
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (sDir === 'h1' || sDir === 'h2' || sDir === 'v2' || sDir === 'v4' || sDir === 'd1y' || sDir === 'd2y' || sDir === 'd3x' || sDir === 'd4x') {
        s1 = s1 + ' m-' + (dxof / 2 - dmx) + ',-' + dyb0 + ' h' + dxb + 'v-' + dybx + ' h-' + dxb + ' z'
      }
      if (sDir === 'h3' || sDir === 'h4' || sDir === 'v1' || sDir === 'v3' || sDir === 'd1x' || sDir === 'd2x' || sDir === 'd3y' || sDir === 'd4y') {
        s1 = s1 + ' m-' + (dxof / 2 - dmx) + ',' + dyb0 + ' h' + dxb + 'v' + dybx + ' h-' + dxb + ' z'
      }
      return s1
    },
    rotateGroup: function (): string {
      let grad = 0
      const sDir = this.dir.toLowerCase()
      if (this.dir.length > 1) {
        if (sDir.charAt(0) === 'v') grad = -90
        if (sDir === 'd1x' || sDir === 'd1y' || sDir === 'd3x' || sDir === 'd3y') grad = -45
        if (sDir === 'd2x' || sDir === 'd2y' || sDir === 'd4x' || sDir === 'd4y') grad = 45
      }
      return 'rotate(' + grad + ',' + this.x + ',' + this.y + ')'
    }
  },

  methods: {
    onClk: function (): void {
      console.log(this.sid, 'Blind-Click')
      const topic = 'ci/error/blind'
      let payload = '-1'
      if (!this.blind) ciBlindController.publishCi(topic, payload)
      if (this.blind?.pubTopic) {
        const aPubTopic = this.blind.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.blind?.pubPayload) payload = this.blind.pubPayload
          ciBlindController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
