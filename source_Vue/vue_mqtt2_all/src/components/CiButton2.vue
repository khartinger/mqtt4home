<!--CiButton2.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <!--
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorButton2" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />
  -->
  <rect :x="Rx0" :y="Ry0" :rx="Rrx" :ry="Rry" :width="Rw" :height="Rh" :fill="colorButton2" stroke="black" stroke-width="4" class="cursor" />
  <line :x1="Rx0" :y1="cy" :x2="Rx0 + Rw" :y2="cy" stroke="black" stroke-width="2" />
  <!--draw extra symbol (shape)----------------------------- -->
  <path v-if="isShape" :d="drawShape" fill="none" stroke="black" stroke-width="1"/>
  <!--write on/off text------------------------------------- -->
  <text v-if="line2.length>0" :x="geo.xt()" :y="dy + dyt + geo.yt(2)" class="ciFont1">{{line2}}</text>
  <text v-if="line4.length>0" :x="geo.xt()" :y="dy - dyt + geo.yt(4)" class="ciFont1">{{line4}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClkOn()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo/2" />
  <rect @click="onClkOff()" class="ciClick" :x="geo.x0()" :y="y" :width="geo.dxo" :height="geo.dyo/2" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Button2, ciButton2Controller } from '../controller/CiButton2Controller'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiButton2',
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
    lines: {
      type: String,
      required: false,
      default: '1'
    }
  },
  computed: {
    button2: function (): Button2 | undefined {
      return ciButton2Controller.button2s.find(button2 => button2.id === this.sid)
    },
    iButton2State: function (): number {
      return this.button2?.iButton2State ?? -1
    },
    geo: function (): Geo {
      const geo1 = new Geo()
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    isShape: function (): boolean {
      if (this.button2?.shape) {
        if (this.button2.shape.length > 0) return true
      }
      return false
    },
    // -------circle parameters: center, radius-----------------
    cx: function (): number { return (this.x) },
    cy: function (): number {
      if (this.iLines === 0) return (this.y)
      if (this.iLines === 2) return (this.y)
      return (this.y + this.geo.dyl / 2)
    },
    cr: function (): number {
      const i = 0 + this.iLines
      if (i === 0) return (this.geo.dyi / 2 - 1)
      if (i === 2) return (this.geo.dyi * 3 / 10 - 1)
      return (2 * this.geo.dyl - 1)
    },
    // -------rectangle parameters: up left, ...----------------
    Rx0: function (): number { return (this.cx - this.cr) },
    Ry0: function (): number { return (this.cy - this.cr) },
    Rrx: function (): number {
      if (!this.button2?.shape) return this.cr / 10
      const sym = this.button2.shape.toLowerCase()
      if ((sym.indexOf('circle') >= 0) || (sym.indexOf('round') >= 0)) return this.cr
      return (this.cr / 10)
    },
    Rry: function (): number {
      return this.Rrx
    },
    Rw: function (): number {
      return 2 * this.cr
    },
    Rh: function (): number {
      return 2 * this.cr
    },
    // -------center text position depending on number of lines_
    dy: function (): number {
      if (this.iLines === 1) return this.geo.dyl / 2
      return 0
    },
    dyt: function (): number {
      if (this.iLines === 0) return -this.geo.dyl / 4
      if (this.iLines === 2) return this.geo.dyl / 3
      return 0
    },
    // -------button2 representation-------------------------------
    colorButton2: function (): string {
      if (this.iButton2State === -2) return this.geo.colorOff
      if (this.iButton2State === -1) return this.geo.colorOn
      if (this.iButton2State < 0) return this.geo.colorUnknown
      return this.button2?.color ?? this.geo.colorError
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.button2?.name) return this.geo.center(this.button2.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.button2?.text5) return this.geo.center(this.button2.text5)
      if (this.button2?.battery) return this.geo.center(this.button2.battery)
      return this.geo.center(this.sid)
    },
    // -------symbol text in line 3-----------------------------
    line2: function (): string {
      if (this.button2?.textOn) return this.geo.center(this.button2.textOn)
      return ''
    },
    line4: function (): string {
      if (this.button2?.textOff) return this.geo.center(this.button2.textOff)
      return ''
    },
    drawShape: function (): string {
      if (!this.button2?.shape) return ''
      if (this.button2.shape.length < 1) return ''
      let s1 = ''
      const sym = this.button2.shape.toLowerCase()
      const cR = 0.8 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      s1 = 'M' + this.cx + ',' + this.cy
      if (sym.indexOf('up') >= 0) {
        s1 += 'm0,-' + cR + ' l-' + (0.5 * ca) + ',' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('down') >= 0) {
        s1 += 'm0,' + cR + ' l-' + (0.5 * ca) + ',-' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('left') >= 0) {
        s1 += 'm-' + cR + ',0 l' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      if (sym.indexOf('right') >= 0) {
        s1 += 'm' + cR + ',0 l-' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      return s1
    }
  },
  methods: {
    onClkOn: function (): void {
      console.log(this.sid, 'Button2-ClickOn')
      const topic = 'ci/error/button2'
      let payload = '-1'
      if (!this.button2) ciButton2Controller.publishCi(topic, payload)
      if (this.button2?.pubTopic) {
        const aPubTopic = this.button2.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button2?.pubPayload) payload = this.button2.pubPayload
          ciButton2Controller.publishCi(topic, payload)
        })
      }
    },
    onClkOff: function (): void {
      console.log(this.sid, 'Button2-ClickOff')
      const topic = 'ci/error/button2'
      let payload = '-1'
      if (!this.button2) ciButton2Controller.publishCi(topic, payload)
      if (this.button2?.pubTopicOff) {
        const aPubTopic = this.button2.pubTopicOff.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button2?.pubPayloadOff) payload = this.button2.pubPayloadOff
          ciButton2Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
