<!-- CiDoor1.vue ----------------------------khartinger----- -->
<!-- 2022-01-08: new                                         -->
<!-- 2023-02-18: change at CiBase (add Geo.ts)               -->
<!-- 2023-02-22: add <template v-if..                        -->
<template>
  <!--draw border------------------------------------------- -->
  <!--
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  -->
  <CiBase :x="x" :y="y" :border="border" :fx="1" :fy="1"></CiBase>
  <g id="doorgroup" :transform="rotateGroup">
    <!--draw symbol and color rectangles-------------------- -->
    <path :d="drawWall" :fill="colorWall_" :stroke="colorWall" stroke-width="1"/>
    <path :d="drawRect" :fill="colorDoor_" stroke="black" stroke-width="1"/>
    <template v-if="sid.length > 0">
      <rect v-if="iLines>0" :fill="colorH_" :x="rx_" :y="geo.y1()" :width="rw_" :height="geo.dyl" stroke="colorH_"/>
      <rect v-if="iLines>0" :fill="color4_" :x="rx_" :y="geo.yr(4)" :width="rw_" :height="geo.dyl" stroke="color4_"/>
      <rect v-if="iLines>1" :x="rx_"  :y="geo.yr(5)" :width="rw_" :height="geo.dyl" :fill="colorF_"/>
      <!--write text------------------------------------------ -->
      <text v-if="iLines>0" :x="xt_" :y="geo.ytHeader()" class="ciFont1">{{lineHeader}}</text>
      <text v-if="iLines>0" :x="xt_" :y="geo.yt(4)" class="ciFont1">{{line4}}</text>
      <text v-if="iLines>1" :x="xt_" :y="geo.ytFooter()" class="ciFont1">{{lineFooter}}</text>
      <!--draw open door-------------------------------------- -->
      <path v-if="isDrawArc" :d="drawArc" fill="none" stroke="black" stroke-width="1"/>
    </template>
  </g>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo()" :height="geo.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Door1, ciDoor1Controller } from '../controller/CiDoor1Controller'
import CiBase from './CiBase.vue'
import { Geo } from '../classes/Geo'

export default defineComponent({
  name: 'CiDoor1',
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
      default: 0
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
    shape: {
      type: String,
      required: false,
      default: 'arc'
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
    colorWall: {
      type: String,
      required: false,
      default: 'brown'
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find door1 object to given sid____________________
    door1: function (): Door1 | undefined {
      return ciDoor1Controller.door1s.find(door1 => door1.id === this.sid)
    },
    // _______get iDoor1State___________________________________
    iDoor1State: function (): number {
      // console.log('iDoor1State=', this.door1?.iDoor1State)
      if (this.door1?.iDoor1State) {
        return this.door1?.iDoor1State
      }
      return ciDoor1Controller.stateNoState
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
    // _______s start point text________________________________
    xt_: function (): number {
      if (this.f > 1) {
        return this.geo.xt() - (this.f - 1) * this.geo.dxo2()
      } return this.geo.xt()
    },
    // _______text in line 1____________________________________
    lineHeader: function (): string {
      if (this.door1?.name) {
        if (this.door1.name.includes('?')) {
          const i2 = this.door1.name.indexOf('?')
          const s1 = this.door1.name.substring(0, i2)
          let s2 = '?'
          if (this.iDoor1State === ciDoor1Controller.stateClose) s2 = this.geo.textClose // CLOSED
          if (this.iDoor1State === ciDoor1Controller.stateOpen) s2 = this.geo.textOpen // OPEN
          if (this.iDoor1State === ciDoor1Controller.stateLock) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2, this.f)
        }
        return this.geo.center(this.door1.name, this.f)
      }
      return this.geo.center(this.sid, this.f)
    },
    // ______text in line 4_____________________________________
    line4: function (): string {
      if (!this.door1) return this.geo.center('unknown', this.f)
      if (this.door1?.battery) return this.geo.center(`${this.door1.battery}` + '%', this.f)
      return ''
    },
    // _______text in line 5____________________________________
    lineFooter: function (): string {
      if (!this.door1) return this.geo.center('sid', this.f)
      if (this.textF) return this.geo.center(this.textF, this.f)
      if (this.door1?.textFooter) {
        if (this.door1.textFooter.includes('?')) {
          const i2 = this.door1.textFooter.indexOf('?')
          const s1 = this.door1.textFooter.substring(0, i2)
          let s2 = '?'
          if (this.iDoor1State === ciDoor1Controller.stateClose) s2 = this.geo.textClose // CLOSED
          if (this.iDoor1State === ciDoor1Controller.stateOpen) s2 = this.geo.textOpen // OPEN
          if (this.iDoor1State === ciDoor1Controller.stateLock) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2, this.f)
        }
        return this.geo.center(this.door1.textFooter, this.f)
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
      if (!this.door1) return this.geo.colorNotOk
      if (this.door1?.battery) {
        try {
          const batt = parseInt(this.door1.battery.valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorBackground
          if (batt > this.geo.batteryMin) return 'none'
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    // _______background color of footer________________________
    colorF_: function (): string {
      if (!this.door1) return this.geo.colorNotOk
      if (this.colorF !== 'none') return this.colorF
      return 'none'
    },
    // _______wall color________________________________________
    colorWall_: function (): string {
      if (this.colorWall !== '-') return this.colorWall
      return this.geo.colorWall
    },
    // _______door color________________________________________
    colorDoor_: function (): string {
      // console.log('this.iDoor1State', this.iDoor1State)
      if (this.iDoor1State === ciDoor1Controller.stateClose) return this.geo.colorClose // closed (light red)
      if (this.iDoor1State === ciDoor1Controller.stateOpen) return this.geo.colorOpen // open (light green)
      if (this.iDoor1State === ciDoor1Controller.stateLock) return this.geo.colorLock // locked (red)
      return this.geo.colorUnknown
    },
    // =======deciding whether to draw something================
    // _______draw a open door arc?_____________________________
    isDrawArc: function (): boolean {
      if (this.iDoor1State === ciDoor1Controller.stateOpen &&
        (this.shape === 'arc' || this.shape === 'tri')) return true
      return false
    },
    // =======drawing methods===================================
    // _______two rectangles next to door rectangle ("wall")____
    drawWall: function (): string {
      const dxof = this.geo.dxo() * this.f
      const dwx = 2 * this.geo.dxm
      const dwy2 = 1.2 * this.geo.dw2
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 += 'm-' + dxof / 2 + ',-' + dwy2 + ' h' + dwx + ' v' + (2 * dwy2) + ' h-' + dwx + ' z'
      s1 += 'm' + dxof + ',0' + ' h-' + dwx + ' v' + (2 * dwy2) + ' h' + dwx + ' z'
      return s1
    },
    // _______door rectangle and middle line____________________
    drawRect: function (): string {
      const dxof = this.geo.dxo() * this.f
      const dwx = 2 * this.geo.dxm
      const dyd = this.geo.dyl
      const dxd = dxof - 2 * dwx
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 = s1 + ' m' + (-dxof / 2 + dwx) + ',-' + dyd / 2
      s1 = s1 + ' v' + dyd + ' h' + dxd + ' v-' + dyd + ' z'
      s1 = s1 + 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0,-' + dyd + ' v' + 2 * dyd
      return s1
    },
    // _______door arch (door area) small or 90 degree__________
    drawArc: function (): string {
      if (this.dir.length < 2) return ''
      const dxof = this.geo.dxo() * this.f
      const dwx = 2 * this.geo.dxm
      const r = dxof - 2 * dwx
      const r2 = Math.round(r / 2)
      const dr = this.geo.dyl / 2 // this.geo.dyl // 15
      let a1 = r
      let a2 = r
      const sDir = this.dir.toLowerCase()
      if (this.dir.charAt(0) === 'h' || this.dir.charAt(0) === 'v' || this.dir.charAt(0) === 'd') {
        a1 = this.geo.dyi2() - dr // 36
        a2 = r - Math.sqrt(r * r - a1 * a1) // 48
      }
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (this.shape === 'tri') {
        if (sDir === 'h1' || sDir === 'v2' || sDir === 'd1y' || sDir === 'd4x') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' v-' + a1 + ' z'
        if (sDir === 'h2' || sDir === 'v3' || sDir === 'd3x' || sDir === 'd2y') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' v-' + a1 + ' z'
        if (sDir === 'h3' || sDir === 'v4' || sDir === 'd3y' || sDir === 'd2x') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' v' + a1 + ' z'
        if (sDir === 'h4' || sDir === 'v1' || sDir === 'd1x' || sDir === 'd4y') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' v' + a1 + ' z'
      } else {
        if (sDir === 'h1' || sDir === 'v2' || sDir === 'd1y' || sDir === 'd4x') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 0 -' + a2 + ',-' + a1 + ' z'
        if (sDir === 'h2' || sDir === 'v3' || sDir === 'd3x' || sDir === 'd2y') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 1 ' + a2 + ',-' + a1 + ' z'
        if (sDir === 'h3' || sDir === 'v4' || sDir === 'd3y' || sDir === 'd2x') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 0 ' + a2 + ',' + a1 + ' z'
        if (sDir === 'h4' || sDir === 'v1' || sDir === 'd1x' || sDir === 'd4y') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 1 -' + a2 + ',' + a1 + ' z'
      }
      console.log('sDir=', sDir, 's1=', s1)
      return s1
    },
    // _______rotate x-Door_____________________________________
    rotateGroup: function (): string {
      let grad = 0
      // if (this.door) {
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
      if (!this.door1) ciDoor1Controller.publishCi(topic, payload)
      if (this.door1?.pubTopic) {
        const aPubTopic = this.door1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.door1?.pubPayload) payload = this.door1.pubPayload
          ciDoor1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
