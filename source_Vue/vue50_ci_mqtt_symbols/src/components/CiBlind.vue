<!--CiBlind.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <g id="blindgroup" :transform="rotateGroup">
    <path :d="blindWall" fill="black" stroke="black" stroke-width="1"/>
    <path :d="blindRect" :fill="colorBlind" stroke="black" stroke-width="1"/>
  <!--write text-------------------------------------------- -->
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
    <rect :x="x1" :y="y5" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
  </g>
    <rect @click="onClk()" class="ciClick" :x="x0" :y="y0" :width="geo.dxo" :height="geo.dyo" />
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
    blindDir: {
      type: String,
      required: true
    },
    f: {
      type: Number,
      required: false,
      default: 1
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
          if (this.iBlindState === 0) s2 = 'ZU' // CLOSED
          if (this.iBlindState === 1) s2 = 'AUF' // OPEN
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.blind.name)
      }
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.blind?.text5) return this.geo.center(this.blind.text5)
      if (this.blind?.battery) return this.geo.center(this.blind.battery)
      return ''
    },
    // -------color wall----------------------------------------
    colorWall: function (): string {
      return this.geo.colorWall
    },

    // -------blind color----------------------------------------
    colorBlind: function (): string {
      if (this.iBlindState === 0) return this.geo.colorClose //    closed (light red)
      if (this.iBlindState === 1) return this.geo.colorOpen // open (light green)
      if (this.iBlindState === 2) return this.geo.colorLock //    locked (red)
      return this.geo.colorUnknown
    },

    x0: function (): number { return (this.x - this.geo.dxo2) },
    y0: function (): number { return (this.y - this.geo.dyo2) },
    x1: function (): number { return (this.x - this.geo.dxi2) },
    y1: function (): number { return (this.y - this.geo.dyi2) },
    y2: function (): number { return (this.y - this.geo.dyi2 + 1 * this.geo.dyl) },
    y3: function (): number { return (this.y - this.geo.dyi2 + 2 * this.geo.dyl) },
    y4: function (): number { return (this.y - this.geo.dyi2 + 3 * this.geo.dyl) },
    y5: function (): number { return (this.y - this.geo.dyi2 + 4 * this.geo.dyl) },
    cx: function (): number { return (this.x) },
    cy: function (): number {
      return (this.y + this.geo.dyl / 2)
    },
    cr: function (): number {
      return (2 * this.geo.dyl - 1)
    },
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.blind?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.blind.battery).valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorOk
          if (batt > this.geo.batteryMin) return this.geo.colorBackground
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    blindWall: function (): string {
      let s1 = ''
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dwy2 = this.geo.dyl
      s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 += 'm-' + dxof / 2 + ',-' + dwy2 + ' h' + dwx + ' v' + (2 * dwy2) + ' h-' + dwx + ' z'
      s1 += 'm' + dxof + ',0' + ' h-' + dwx + ' v' + (2 * dwy2) + ' h' + dwx + ' z'
      return s1
    },
    blindRect: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dby = this.geo.dyl
      const dxd = dxof - 2 * dwx
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 = s1 + ' m' + (-dxof / 2 + dwx) + ',-' + dby / 2
      s1 = s1 + ' v' + dby + ' h' + dxd + ' v-' + dby + ' z'
      s1 = s1 + ' m0,' + dby / 3 + ' h' + dxd
      s1 = s1 + ' m0,' + dby / 3 + ' h-' + dxd
      s1 = s1 + 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0,-' + dby + ' v' + 2 * dby
      return s1
    },
    rotateGroup: function (): string {
      let grad = 0
      if (this.blindDir.charAt(0) === 'V' || this.blindDir.charAt(0) === 'v') grad = -90
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
