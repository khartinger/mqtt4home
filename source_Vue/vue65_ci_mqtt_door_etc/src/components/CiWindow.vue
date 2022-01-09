<!-- CiWindow.vue ---------------------------2022-01-08----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <g id="windowgroup" :transform="rotateGroup">
    <path :d="windowWall" :fill="colorWall" :stroke="colorWall" stroke-width="1"/>
    <path :d="windowRect" :fill="colorWindow" stroke="black" stroke-width="1"/>
    <!--write text------------------------------------------ -->
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
    <rect :x="geo.x1()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
    <!--draw open window-------------------------------------- -->
    <path v-if="iWindowState===1" :d="windowArc" fill="none" stroke="black" stroke-width="1"/>
  </g>
    <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Window, ciWindowController } from '../controller/CiWindowController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiWindow',
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
    shape: {
      type: String,
      required: false,
      default: ''
    }
  },
  computed: {
    window: function (): Window | undefined {
      return ciWindowController.windows.find(window => window.id === this.sid)
    },
    iWindowState: function (): number {
      return this.window?.iWindowState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.window?.name) {
        if (this.window.name.includes('?')) {
          const i2 = this.window.name.indexOf('?')
          const s1 = this.window.name.substring(0, i2)
          let s2 = '?'
          if (this.iWindowState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iWindowState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iWindowState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.window.name)
      }
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.text5.length > 0) return this.text5
      if (this.window?.text5) {
        if (this.window.text5.includes('?')) {
          const i2 = this.window.text5.indexOf('?')
          const s1 = this.window.text5.substring(0, i2)
          let s2 = '?'
          if (this.iWindowState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iWindowState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iWindowState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.window.text5)
      }
      if (this.window?.battery) return this.geo.center(this.window.battery)
      return ''
      // return this.geo.center(this.sid)
    },
    // -------color wall----------------------------------------
    colorWall: function (): string {
      return this.geo.colorWall
    },
    // -------window color----------------------------------------
    colorWindow: function (): string {
      if (this.iWindowState === 0) return this.geo.colorClose //    closed (light red)
      if (this.iWindowState === 1) return this.geo.colorOpen // open (light green)
      if (this.iWindowState === 2) return this.geo.colorLock //    locked (red)
      return this.geo.colorUnknown
    },
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.window?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.window.battery).valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorOk
          if (batt > this.geo.batteryMin) return this.geo.colorBackground
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    windowWall: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dwy2 = 0.5 * this.geo.dyl
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 += 'm-' + dxof / 2 + ',-' + dwy2 + ' h' + dwx + ' v' + (2 * dwy2) + ' h-' + dwx + ' z'
      s1 += 'm' + dxof + ',0' + ' h-' + dwx + ' v' + (2 * dwy2) + ' h' + dwx + ' z'
      return s1
    },
    windowRect: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dby = this.geo.dyl
      const dxd = dxof - 2 * dwx
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 = s1 + ' m' + (-dxof / 2 + dwx) + ',-' + dby / 2
      s1 = s1 + ' v' + dby + ' h' + dxd + ' v-' + dby + ' z'
      s1 = s1 + ' m0,' + dby / 3 + ' h' + dxd
      s1 = s1 + ' m0,' + dby / 3 + ' h-' + dxd
      // s1 = s1 + 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0,-' + dby + ' v' + 2 * dby
      return s1
    },
    windowArc: function (): string {
      if (this.dir.length < 2) return ''
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const r = dxof - 2 * dwx
      const r2 = Math.round(r / 2)
      const dr = this.geo.dyl / 2 // this.geo.dyl // 15
      let a1 = r
      let a2 = r
      let dxy = r / 2 // 3 * this.geo.dxm
      const sDir = this.dir.toLowerCase()
      if (this.dir.charAt(0) === 'h' || this.dir.charAt(0) === 'v' || this.dir.charAt(0) === 'd') {
        a1 = this.geo.dyo2 - this.geo.dyl // 72
        a2 = r - Math.sqrt(r * r - a1 * a1) // 48
        dxy = 0.9 * this.geo.dyl
      }
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      // console.log('windowArc: this.type=', this.type)
      if (this.shape === 'rect') {
        if (sDir === 'h1' || sDir === 'v2' || sDir === 'd1y' || sDir === 'd4x') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' v-' + dxy + ' h-' + r + ' z'
        if (sDir === 'h2' || sDir === 'v3' || sDir === 'd3x' || sDir === 'd2y') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' v-' + dxy + ' h' + r + ' z'
        if (sDir === 'h3' || sDir === 'v4' || sDir === 'd3y' || sDir === 'd2x') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' v' + dxy + ' h' + r + ' z'
        if (sDir === 'h4' || sDir === 'v1' || sDir === 'd1x' || sDir === 'd4y') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' v' + dxy + ' h-' + r + ' z'
      } else {
        if (sDir === 'h1' || sDir === 'v2' || sDir === 'd1y' || sDir === 'd4x') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 0 -' + a2 + ',-' + a1 + ' z'
        if (sDir === 'h2' || sDir === 'v3' || sDir === 'd3x' || sDir === 'd2y') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 1 ' + a2 + ',-' + a1 + ' z'
        if (sDir === 'h3' || sDir === 'v4' || sDir === 'd3y' || sDir === 'd2x') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 0 ' + a2 + ',' + a1 + ' z'
        if (sDir === 'h4' || sDir === 'v1' || sDir === 'd1x' || sDir === 'd4y') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 1 -' + a2 + ',' + a1 + ' z'
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
      console.log(this.sid, 'Window-Click')
      const topic = 'ci/error/window'
      let payload = '-1'
      if (!this.window) ciWindowController.publishCi(topic, payload)
      if (this.window?.pubTopic) {
        const aPubTopic = this.window.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.window?.pubPayload) payload = this.window.pubPayload
          ciWindowController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
