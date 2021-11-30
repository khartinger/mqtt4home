<!--CiDoor.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <g id="doorgroup" :transform="rotateGroup">
    <path :d="doorWall" fill="black" stroke="black" stroke-width="1"/>
    <path :d="doorRect" :fill="colorDoor" stroke="black" stroke-width="1"/>
<!--write text-------------------------------------------- -->
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
    <rect :x="x1" :y="y5" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
    <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
    <!--draw open door-------------------------------------- -->
    <path v-if="iDoorState===1" :d="doorArc" fill="none" stroke="black" stroke-width="1"/>
  </g>
    <rect @click="onClk()" class="ciClick" :x="x0" :y="y0" :width="geo.dxo" :height="geo.dyo" />
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
  data () {
    return {
      colorWall: '#000000'
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
    doorDir: {
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
    door: function (): Door | undefined {
      return ciDoorController.doors.find(door => door.id === this.sid)
    },
    iDoorState: function (): number {
      return this.door?.iDoorState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo()
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.door?.name) return this.geo.center(this.door.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.door?.text5) return this.geo.center(this.door.text5)
      if (this.door?.battery) return this.geo.center(this.door.battery)
      return this.geo.center(this.sid)
    },
    // -------door color----------------------------------------
    colorDoor: function (): string {
      if (this.iDoorState === 0) return '#FF6666' //    closed (light red)
      if (this.iDoorState === 1) return 'lightgreen' // open (light green)
      if (this.iDoorState === 2) return '#C00000' //    locked (red)
      return 'lightblue'
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
    cy: function (): number { return (this.y + this.geo.dyl / 2) },
    cr: function (): number { return (2 * this.geo.dyl - 1) },
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.door?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.door.battery).valueOf())
          // if (batt > this.geo.batteryMin) return this.geo.colorOk
          if (batt > this.geo.batteryMin) return this.geo.colorBackground
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    },
    doorWall: function (): string {
      let s1 = ''
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dwy2 = this.geo.dyl
      s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 += 'm-' + dxof / 2 + ',-' + dwy2 + ' h' + dwx + ' v' + (2 * dwy2) + ' h-' + dwx + ' z'
      s1 += 'm' + dxof + ',0' + ' h-' + dwx + ' v' + (2 * dwy2) + ' h' + dwx + ' z'
      // s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      // s1 += 'm-' + this.geo.dxo2 + ',-' + this.geo.dyl + ' h' + 2 * this.geo.dxm + ' v' + (2 * this.geo.dyl) + ' h-' + 2 * this.geo.dxm + ' z'
      // s1 += 'm' + this.geo.dxo + ',0' + ' h-' + 2 * this.geo.dxm + ' v' + (2 * this.geo.dyl) + ' h' + 2 * this.geo.dxm + ' z'
      return s1
    },
    // doorRect: function (): string {
    //   const dxd = this.geo.dxo - 2 * 2 * this.geo.dxm
    //   let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
    //   s1 = s1 + ' m' + (-this.geo.dxo2 + 2 * this.geo.dxm) + ',-' + this.geo.dyl / 2
    //   s1 = s1 + ' v' + this.geo.dyl + ' h' + dxd + ' v-' + this.geo.dyl + ' z'
    //   s1 += 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0 -' + this.geo.dyl + ' v' + 2 * this.geo.dyl
    //   return s1
    // },
    doorRect: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const dby = this.geo.dyl
      const dxd = dxof - 2 * dwx
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      s1 = s1 + ' m' + (-dxof / 2 + dwx) + ',-' + dby / 2
      s1 = s1 + ' v' + dby + ' h' + dxd + ' v-' + dby + ' z'
      // s1 = s1 + ' m0,' + dby / 3 + ' h' + dxd
      // s1 = s1 + ' m0,' + dby / 3 + ' h-' + dxd
      s1 = s1 + 'M' + this.x.toString(10) + ',' + this.y.toString(10) + ' m0,-' + dby + ' v' + 2 * dby
      return s1
    },
    doorArc: function (): string {
      const dxof = this.geo.dxo * this.f
      const dwx = 2 * this.geo.dxm
      const r = dxof - 2 * dwx
      const r2 = Math.round(r / 2)
      const dr = this.geo.dyl / 2 // this.geo.dyl // 15
      let a1 = r
      let a2 = r
      if (this.doorDir.charAt(0) === 'h' || this.doorDir.charAt(0) === 'v') {
        a1 = this.geo.dyo2 - this.geo.dyl // 72
        a2 = r - Math.sqrt(r * r - a1 * a1) // 48
      }
      let s1 = 'M' + this.x.toString(10) + ',' + this.y.toString(10)
      if (this.doorDir === 'H1' || this.doorDir === 'h1' || this.doorDir === 'V2' || this.doorDir === 'v2') s1 += ' m-' + r2 + ',-' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 0 -' + a2 + ',-' + a1 + ' z'
      if (this.doorDir === 'H2' || this.doorDir === 'h2' || this.doorDir === 'V3' || this.doorDir === 'v3') s1 += ' m' + r2 + ',-' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 1 ' + a2 + ',-' + a1 + ' z'
      if (this.doorDir === 'H3' || this.doorDir === 'h3' || this.doorDir === 'V4' || this.doorDir === 'v4') s1 += ' m' + r2 + ',' + dr + ' h-' + r + ' a' + r + ',' + r + ' 0 0 0 ' + a2 + ',' + a1 + ' z'
      if (this.doorDir === 'H4' || this.doorDir === 'h4' || this.doorDir === 'V1' || this.doorDir === 'v1') s1 += ' m-' + r2 + ',' + dr + ' h' + r + ' a' + r + ',' + r + ' 0 0 1 -' + a2 + ',' + a1 + ' z'
      return s1
    },
    rotateGroup: function (): string {
      let grad = 0
      if (this.doorDir.charAt(0) === 'V' || this.doorDir.charAt(0) === 'v') grad = -90
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
