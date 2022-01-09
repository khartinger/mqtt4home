<!-- CiDoor.vue -----------------------------2022-01-08----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <g id="doorgroup" :transform="rotateGroup">
    <path :d="doorWall" :fill="colorWall" :stroke="colorWall" stroke-width="1"/>
    <path :d="doorRect" :fill="colorDoor" stroke="black" stroke-width="1"/>
    <!--write text------------------------------------------ -->
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
    <rect :x="geo.x1()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
    <!--draw open door-------------------------------------- -->
    <path v-if="iDoorState===1" :d="doorArc" fill="none" stroke="black" stroke-width="1"/>
  </g>
    <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Door, ciDoorController } from '../controller/CiDoorController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiDoor',
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
    }
  },
  computed: {
    door: function (): Door | undefined {
      return ciDoorController.doors.find(door => door.id === this.sid)
    },
    iDoorState: function (): number {
      return this.door?.iDoorState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.door?.name) {
        if (this.door.name.includes('?')) {
          const i2 = this.door.name.indexOf('?')
          const s1 = this.door.name.substring(0, i2)
          let s2 = '?'
          if (this.iDoorState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iDoorState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iDoorState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.door.name)
      }
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.text5.length > 0) return this.text5
      if (this.door?.text5) {
        if (this.door.text5.includes('?')) {
          const i2 = this.door.text5.indexOf('?')
          const s1 = this.door.text5.substring(0, i2)
          let s2 = '?'
          if (this.iDoorState === 0) s2 = this.geo.textClose // CLOSED
          if (this.iDoorState === 1) s2 = this.geo.textOpen // OPEN
          if (this.iDoorState === 2) s2 = this.geo.textLock // LOCKED
          return this.geo.center(s1 + s2)
        }
        return this.geo.center(this.door.text5)
      }
      if (this.door?.battery) return this.geo.center(this.door.battery)
      return ''
      // return this.geo.center(this.sid)
    },
    // -------color wall----------------------------------------
    colorWall: function (): string {
      return this.geo.colorWall
    },
    // -------door color----------------------------------------
    colorDoor: function (): string {
      if (this.iDoorState === 0) return this.geo.colorClose //    closed (light red)
      if (this.iDoorState === 1) return this.geo.colorOpen // open (light green)
      if (this.iDoorState === 2) return this.geo.colorLock //    locked (red)
      return this.geo.colorUnknown
    },
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.door?.text5) return 'none' // this.geo.colorOk
      if (this.door?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.door.battery).valueOf())
          if (batt > this.geo.batteryMin) return this.geo.colorBackground
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    // -------two rectangles next to door rectangle ("wall")----
    doorWall: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dwy2 = 1.2 * this.geo.dw2
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 += 'm-' + dxof / 2 + ',-' + dwy2 + ' h' + dwx + ' v' + (2 * dwy2) + ' h-' + dwx + ' z'
      s1 += 'm' + dxof + ',0' + ' h-' + dwx + ' v' + (2 * dwy2) + ' h' + dwx + ' z'
      return s1
    },
    // -------door rectangle and middle line--------------------
    doorRect: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dyd = this.geo.dyl
      const dxd = dxof - 2 * dwx
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 = s1 + ' m' + (-dxof / 2 + dwx) + ',-' + dyd / 2
      s1 = s1 + ' v' + dyd + ' h' + dxd + ' v-' + dyd + ' z'
      s1 = s1 + 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0,-' + dyd + ' v' + 2 * dyd
      return s1
    },
    // -------door arch (door area) small or 90 degree----------
    doorArc: function (): string {
      if (this.dir.length < 2) return ''
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const r = dxof - 2 * dwx
      const r2 = Math.round(r / 2)
      const dr = this.geo.dyl / 2 // this.geo.dyl // 15
      let a1 = r
      let a2 = r
      const sDir = this.dir.toLowerCase()
      if (this.dir.charAt(0) === 'h' || this.dir.charAt(0) === 'v' || this.dir.charAt(0) === 'd') {
        a1 = this.geo.dyo2 - this.geo.dyl // 72
        a2 = r - Math.sqrt(r * r - a1 * a1) // 48
      }
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (sDir === 'h1' || sDir === 'v2' || sDir === 'd1y' || sDir === 'd4x') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 0 -' + a2 + ',-' + a1 + ' z'
      if (sDir === 'h2' || sDir === 'v3' || sDir === 'd3x' || sDir === 'd2y') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 1 ' + a2 + ',-' + a1 + ' z'
      if (sDir === 'h3' || sDir === 'v4' || sDir === 'd3y' || sDir === 'd2x') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 0 ' + a2 + ',' + a1 + ' z'
      if (sDir === 'h4' || sDir === 'v1' || sDir === 'd1x' || sDir === 'd4y') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 1 -' + a2 + ',' + a1 + ' z'
      return s1
    },
    // -------rorate x-Door-------------------------------------
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

  methods: {
    onClk: function (): void {
      console.log(this.sid, 'Door-Click')
      const topic = 'ci/error/door'
      let payload = '-1'
      if (!this.door) ciDoorController.publishCi(topic, payload)
      if (this.door?.pubTopic) {
        const aPubTopic = this.door.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.door?.pubPayload) payload = this.door.pubPayload
          ciDoorController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
