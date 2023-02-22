<!-- CiBlind1.vue ---------------------------khartinger----- -->
<!-- 2022-01-08: new                                         -->
<!-- 2023-02-20: change at CiBase (add Geo.ts)               -->
<!-- 2023-02-22: add <template v-if..                        -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="1" :fy="1"></CiBase>
  <g id="blindgroup" :transform="rotateGroup">
    <!--draw symbol and color rectangles-------------------- -->
    <path :d="drawMotor" :fill="colorMotor_" stroke="black" stroke-width="1"/>
    <path :d="drawRect" :fill="colorBlind_" :stroke="colorBlindline" :stroke-width="1"/>
    <template v-if="sid.length > 0">
      <rect v-if="iLines>0" :fill="colorH_" :x="rx_" :y="geo.y1()" :width="rw_" :height="geo.dyl" stroke="colorH_"/>
      <rect v-if="iLines>0 && !isLine4" :fill="color4_" :x="rx_" :y="geo.yr(2)" :width="rw_" :height="geo.dyl" stroke="color4_"/>
      <rect v-if="iLines>0 && isLine4"  :fill="color4_" :x="rx_" :y="geo.yr(4)" :width="rw_" :height="geo.dyl" stroke="color4_"/>
      <rect v-if="iLines>1" :x="rx_"  :y="geo.yr(5)" :width="rw_" :height="geo.dyl" :fill="colorF_"/>
      <!--write text---------------------------------------- -->
      <text v-if="iLines>0" :x="xt_" :y="geo.ytHeader()" class="ciFont1">{{lineHeader}}</text>
      <text v-if="iLines>0 && !isLine4" :x="xt_" :y="geo.yt(2)" class="ciFont1">{{line4}}</text>
      <text v-if="iLines>0 && isLine4"  :x="xt_" :y="geo.yt(4)" class="ciFont1">{{line4}}</text>
      <text v-if="iLines>1" :x="xt_" :y="geo.ytFooter()" class="ciFont1">{{lineFooter}}</text>
    </template>
  </g>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo()" :height="geo.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Blind1, ciBlind1Controller } from '../controller/CiBlind1Controller'
import CiBase from './CiBase.vue'
import { Geo } from '../classes/Geo'

export default defineComponent({
  name: 'CiBlind1',
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
    sid: {
      type: String,
      required: true
    },
    border: {
      type: Number,
      required: false,
      default: 1 // 0
    },
    dir: {
      type: String,
      required: true
    },
    lines: {
      type: String,
      required: false,
      default: '2'
    },
    f: {
      type: Number,
      required: false,
      default: 1
    },
    textF: {
      type: String,
      required: false,
      default: ''
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
    },
    colorMotor: {
      type: String,
      required: false,
      default: 'brown'
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find blind1 object to given sid___________________
    blind1: function (): Blind1 | undefined {
      return ciBlind1Controller.blind1s.find(blind1 => blind1.id === this.sid)
    },
    // _______get iBlind1State__________________________________
    iBlind1State: function (): number {
      // console.log('iBlind1State=', this.blind1?.iBlind1State)
      if (this.blind1?.iBlind1State) {
        return this.blind1?.iBlind1State
      }
      return ciBlind1Controller.stateNoState
    },
    // _______provide geometric data____________________________
    geo: function (): Geo {
      // const geo1f = new Geof(this.x, this.y, this.fx, this.fy)
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    // _______number of displayed lines_________________________
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // =======text methods======================================
    // ____________write text in line 4 (if not: line 2)________
    isLine4: function (): boolean {
      const sDir = this.dir.toLowerCase()
      if (sDir === 'h1' || sDir === 'h2' || sDir === 'v2' || sDir === 'v4' ||
        sDir === 'd1y' || sDir === 'd3x' || sDir === 'd2y' || sDir === 'd4x') return true
      return false
    },
    // _______x start point text________________________________
    xt_: function (): number {
      if (this.f > 1) {
        return this.geo.xt() - (this.f - 1) * this.geo.dxo2()
      } return this.geo.xt()
    },
    // _______text in line 1____________________________________
    lineHeader: function (): string {
      if (this.blind1?.name) {
        if (this.blind1.name.includes('?')) {
          const i2 = this.blind1.name.indexOf('?')
          const s1 = this.blind1.name.substring(0, i2)
          let s2 = '?'
          if (this.iBlind1State === ciBlind1Controller.stateClose) s2 = this.geo.textClose // CLOSED
          if (this.iBlind1State === ciBlind1Controller.stateOpen) s2 = this.geo.textOpen // OPEN
          if (this.iBlind1State === ciBlind1Controller.stateLock) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2, this.f)
        }
        return this.geo.center(this.blind1.name, this.f)
      }
      return this.geo.center(this.sid, this.f)
    },
    // ______text in line 4_____________________________________
    line4: function (): string {
      if (!this.blind1) return this.geo.center('unknown', this.f)
      if (this.blind1?.battery) return this.geo.center(`${this.blind1.battery}` + '%', this.f)
      return ''
    },
    // _______text in line 5____________________________________
    lineFooter: function (): string {
      if (!this.blind1) return this.geo.center('sid', this.f)
      if (this.textF) return this.geo.center(this.textF, this.f)
      if (this.blind1?.textFooter) {
        if (this.blind1.textFooter.includes('?')) {
          const i2 = this.blind1.textFooter.indexOf('?')
          const s1 = this.blind1.textFooter.substring(0, i2)
          let s2 = '?'
          if (this.iBlind1State === ciBlind1Controller.stateClose) s2 = this.geo.textClose // CLOSED
          if (this.iBlind1State === ciBlind1Controller.stateOpen) s2 = this.geo.textOpen // OPEN
          if (this.iBlind1State === ciBlind1Controller.stateLock) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2, this.f)
        }
        return this.geo.center(this.blind1.textFooter, this.f)
      }
      return ''
    },
    // ======color methods======================================
    // _______color rectangle start value x_____________________
    rx_: function (): number {
      return this.x - this.f * this.geo.dxo() / 2 // this.geo.x1()
    },
    // _______color rectangle width value x_____________________
    rw_: function (): number {
      return this.f * this.geo.dxo() // this.geo.dxi()
    },
    // _______background color of header________________________
    colorH_: function (): string {
      if (this.colorH !== 'none') return this.colorH
      return 'none'
    },
    // _______background color of line 4________________________
    // also depending on battery value
    color4_: function (): string {
      if (!this.blind1) return this.geo.colorNotOk
      if (this.blind1?.battery) {
        try {
          const batt = parseInt(this.blind1.battery.valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorBackground
          if (batt > this.geo.batteryMin) return 'none'
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    // _______background color of footer________________________
    colorF_: function (): string {
      if (!this.blind1) return this.geo.colorNotOk
      if (this.colorF !== 'none') return this.colorF
      return 'none'
    },
    // _______blind color_______________________________________
    colorBlind_: function (): string {
      // console.log('this.iBlind1State', this.iBlind1State)
      if (this.iBlind1State === ciBlind1Controller.stateClose) return this.geo.colorClose // closed (light red)
      if (this.iBlind1State === ciBlind1Controller.stateOpen) return this.geo.colorOpen // open (light green)
      if (this.iBlind1State === ciBlind1Controller.stateLock) return this.geo.colorLock // locked (red)
      return this.geo.colorUnknown
    },
    // _______motor color_______________________________________
    colorMotor_: function (): string {
      try {
        // console.log('colorMotor_:', this.blind1?.iMotorState)
        if (this.blind1?.iMotorState === undefined) return this.geo.colorWall
        if (this.blind1) {
          if (this.blind1.iMotorState === ciBlind1Controller.stateMotorDown) return this.geo.colorOn2 // motor down
          if (this.blind1.iMotorState === ciBlind1Controller.stateMotorStop) return this.geo.colorOff2 // motor stopp
          if (this.blind1.iMotorState === ciBlind1Controller.stateMotorUp) return this.geo.colorOn // motor up
          return this.geo.colorUnknown
        }
        if (this.colorMotor) return this.colorMotor
      } catch (error) { console.error(error) }
      return this.geo.colorNotOk
    },
    // _______color of line around blind ("motor") rectangle____
    colorBlindline: function (): string {
      if (this.iBlind1State === 1) return this.geo.colorOff
      return 'black'
    },
    // =======drawing methods===================================
    // _______two rectangles next to blind rectangle ("motor")__
    drawMotor: function (): string {
      const dxof = this.geo.dxo() * this.f
      const dyb01 = 3 * this.geo.dyl / 2
      // const dmx = 2 * this.geo.dxm
      const dmx3 = 3 * this.geo.dxm
      const dmy = dyb01 - this.geo.dw2
      const sDir = this.dir.toLowerCase()
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (sDir === 'h1' || sDir === 'h2' || sDir === 'v2' || sDir === 'v4' || sDir === 'd1y' || sDir === 'd2y' || sDir === 'd3x' || sDir === 'd4x') {
        s1 += 'm-' + dxof / 2 + ',-' + dyb01 + ' h' + dmx3 + ' v' + dmy + ' h-' + dmx3 + ' z'
        // s1 += 'm-' + dxof / 2 + ',-' + dyb01 + ' h' + dmx + ' v' + dmy + ' h-' + dmx + ' z'
        s1 += 'm' + dxof + ',0 h-' + dmx3 + ' v' + dmy + ' h' + dmx3 + ' z'
      }
      if (sDir === 'h3' || sDir === 'h4' || sDir === 'v1' || sDir === 'v3' || sDir === 'd1x' || sDir === 'd2x' || sDir === 'd3y' || sDir === 'd4y') {
        s1 += 'm-' + dxof / 2 + ',' + dyb01 + ' h' + dmx3 + ' v-' + dmy + ' h-' + dmx3 + ' z'
        s1 += 'm' + dxof + ',0 h-' + dmx3 + ' v-' + dmy + ' h' + dmx3 + ' z'
      }
      return s1
    },
    // _______draw blind rectangle______________________________
    drawRect: function (): string {
      if (this.dir.length < 2) return ''
      const dxof = this.geo.dxo() * this.f
      const dmx = 2 * this.geo.dxm
      const dxb = dxof - 2 * dmx
      const dyb01 = 3 * this.geo.dyl / 2
      const dyb0 = this.geo.dyl * 3 / 4
      const dyb1 = dyb01 - dyb0
      const dyb2 = this.geo.dyi() / 2 - dyb0
      const sDir = this.dir.toLowerCase()
      let dybx = dyb1
      if (this.dir.charAt(0) === 'H' || this.dir.charAt(0) === 'V' || this.dir.charAt(0) === 'D') dybx = dyb2
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (sDir === 'h1' || sDir === 'h2' || sDir === 'v2' || sDir === 'v4' || sDir === 'd1y' || sDir === 'd2y' || sDir === 'd3x' || sDir === 'd4x') {
        s1 = s1 + ' m-' + (dxof / 2 - dmx) + ',-' + dyb0 + ' h' + dxb + 'v-' + dybx + ' h-' + dxb + ' z'
        if ((this.iBlind1State === ciBlind1Controller.stateClose) ||
          (this.iBlind1State === ciBlind1Controller.stateLock)) {
          s1 += ' l' + dxb + ',-' + dybx + ' m0,' + dybx + ' l-' + dxb + ',-' + dybx
        }
      }
      if (sDir === 'h3' || sDir === 'h4' || sDir === 'v1' || sDir === 'v3' || sDir === 'd1x' || sDir === 'd2x' || sDir === 'd3y' || sDir === 'd4y') {
        s1 = s1 + ' m-' + (dxof / 2 - dmx) + ',' + dyb0 + ' h' + dxb + 'v' + dybx + ' h-' + dxb + ' z'
        if ((this.iBlind1State === ciBlind1Controller.stateClose) ||
          (this.iBlind1State === ciBlind1Controller.stateLock)) {
          s1 += ' l' + dxb + ',' + dybx + ' m-' + dxb + ',0' + ' l' + dxb + ',-' + dybx
        }
      }
      return s1
    },
    // _______rotate x-Blind____________________________________
    rotateGroup: function (): string {
      let grad = 0
      // if (this.blind) {
      const sDir = this.dir.toLowerCase()
      if (this.dir.length > 1) {
        if (sDir.charAt(0) === 'v') grad = -90
        if (sDir === 'd1x' || sDir === 'd1y' || sDir === 'd3x' || sDir === 'd3y') grad = -45
        if (sDir === 'd2x' || sDir === 'd2y' || sDir === 'd4x' || sDir === 'd4y') grad = 45
      // }
      }
      return 'rotate(' + grad + ',' + this.x + ',' + this.y + ')'
    }
  },
  // =========act on a click====================================
  methods: {
    onClk: function (): void {
      // console.log(this.sid, 'Button-Click')
      const topic = 'ci/error/button'
      let payload = '-1'
      if (!this.blind1) ciBlind1Controller.publishCi(topic, payload)
      if (this.blind1?.pubTopic) {
        const aPubTopic = this.blind1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.blind1?.pubPayload) payload = this.blind1.pubPayload
          ciBlind1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
