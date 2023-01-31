<!-- CiLamp1.vue ----------------------------khartinger----- -->
<!-- 2022-01-25: new                                         -->
<!-- 2022-01-31: change at CiBase (add Geo.ts)               -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <rect :x="Sx0" :y="Sy0" :rx="Srx" :ry="Sry" :width="Sw" :height="Sh" :fill="colorButton" stroke="black" stroke-width="2" class="cursor" />
  <!--draw lamp cross--------------------------------------- -->
  <path :d="drawLampCross" fill="none" stroke="black" stroke-width="2"/>
  <!--write center text------------------------------------- -->
  <text v-if="lineCenter.length>0" :x="geof.xt()" :y="geof.ytCenter(iLines)" class="ciFont1">{{lineCenter}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="iLines>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>

  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Lamp1, ciLamp1Controller } from '../controller/CiLamp1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiLamp1',
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
      default: 1
    },
    sid: {
      type: String,
      required: true
    },
    lines: {
      type: String,
      required: false,
      default: '1'
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
    shape: {
      type: String,
      required: false,
      default: 'circle'
    },
    color: {
      type: String,
      required: false,
      default: '-'
    }
  },
  computed: {
    lamp1: function (): Lamp1 | undefined {
      return ciLamp1Controller.lamps1.find(lamp1 => lamp1.id === this.sid)
    },
    iLamp1State: function (): number {
      return this.lamp1?.iLamp1State ?? -1
    },
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // -------0=circle, 1=square, 2=round, 3=rect---------------
    iShape: function (): number {
      if (!this.shape) return 6
      if (this.shape.length < 1) return 6
      const sym = this.shape.toLowerCase()
      if (sym.indexOf('circle') >= 0) return 6
      if (sym.indexOf('square') >= 0) return 5
      if (sym.indexOf('round') >= 0) return 2
      if (sym.indexOf('rect') >= 0) return 1
      return 6
    },
    // -------circle parameter: radius--------------------------
    cr: function (): number {
      const crx_ = this.geof.dxo2() - this.geof.dxm - 1
      let cry_ = this.geof.dyo2() - this.geof.dym - 1
      if (this.iLines === 1) cry_ = cry_ - this.geof.dyl / 2
      if (this.iLines === 2) cry_ = cry_ - this.geof.dyl
      if (crx_ < cry_) return crx_
      return cry_
    },
    // -------shape parameters: up left corner------------------
    Sx0: function (): number {
      if (this.iShape > 4) { // circle, square
        return (this.geof.xc() - this.cr)
      }
      return this.geof.x1() // round, rect
    },
    // -------shape parameters: up left corner------------------
    Sy0: function (): number {
      if (this.iShape > 4) { // circle, square
        return (this.geof.yc(this.iLines) - this.cr)
      }
      if (this.iLines > 0) return this.geof.y1() + this.geof.dyl
      return this.geof.y1()
    },
    // -------shape: corner rounding----------------------------
    Srx: function (): number {
      const i1 = 0 + this.iShape
      if ((i1 & 1) === 1) { //        square, rect
        return (this.cr / 10)
      }
      return (this.cr)
    },
    Sry: function (): number {
      return this.Srx
    },
    // -------shape width---------------------------------------
    Sw: function (): number {
      if (this.iShape > 4) { // circle, square
        return 2 * this.cr
      }
      return this.geof.dxi() // round, rect
    },
    // -------shape height--------------------------------------
    Sh: function (): number {
      if (this.iShape > 4) { // circle, square
        return 2 * this.cr
      }
      let dyL_ = 0
      if (this.iLines === 1) dyL_ = this.geof.dyl
      if (this.iLines === 2) dyL_ = 2 * this.geof.dyl
      return this.geof.dyi() - dyL_
    },
    // -------button representation-------------------------------
    colorButton: function (): string {
      if (this.color !== '-') return this.color
      if (this.iLamp1State === -2) return this.geof.colorOff
      if (this.iLamp1State === -1) return this.geof.colorOn
      if (this.iLamp1State <= 0) return this.geof.colorUnknown
      return this.geof.colorError
    },
    // -------text in line 1 and 5------------------------------
    lineHeader: function (): string {
      if (this.lamp1?.name) return this.geof.center(this.lamp1.name)
      return this.geof.center(this.sid)
    },
    lineFooter: function (): string {
      if (this.lamp1?.textFooter) return this.geof.center(this.lamp1.textFooter)
      if (this.lamp1?.battery) return this.geof.center(this.lamp1.battery)
      return this.geof.center(this.sid)
    },
    // -------symbol text in line 3-----------------------------
    lineCenter: function (): string {
      if (this.lamp1?.textCenter) return this.geof.center(this.lamp1.textCenter)
      return ''
    },
    // -------draw an additional symbol-------------------------
    drawLampCross: function (): string {
      const a = this.Sh / this.Sw
      // const SdxRest = this.Srx * (1 - 1 / Math.sqrt(2))
      // const SdyRest = SdxRest
      const SdxRest = this.Srx * (1 + a - Math.sqrt(2 * a)) / (1 + a * a)
      const SdyRest = SdxRest * a
      const dxCross = this.Sw - 2 * SdxRest
      const dyCross = this.Sh - 2 * SdyRest
      let s1 = ''
      s1 = 'M' + (this.Sx0 + SdxRest) + ',' + (this.Sy0 + SdyRest)
      s1 += ' l' + dxCross + ',' + dyCross
      s1 += ' m-' + dxCross + ',0 l' + dxCross + ',-' + dyCross
      return s1
    }
  },
  methods: {
    onClk: function (): void {
      // console.log(this.sid, 'Button-Click')
      const topic = 'ci/error/button'
      let payload = '-1'
      if (!this.lamp1) ciLamp1Controller.publishCi(topic, payload)
      if (this.lamp1?.pubTopic) {
        const aPubTopic = this.lamp1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.lamp1?.pubPayload) payload = this.lamp1.pubPayload
          ciLamp1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
