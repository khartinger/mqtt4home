<!-- CiButton1.vue --------------------------khartinger----- -->
<!-- 2022-01-25: new                                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw shape-------------------------------------------- -->
  <rect :x="Sx0" :y="Sy0" :rx="Srx" :ry="Sry" :width="Sw" :height="Sh" :fill="colorButton" stroke="black" stroke-width="2" class="cursor" />
  <!--draw extra symbol (shape)----------------------------- -->
  <path v-if="isPath" :d="drawPath" fill="none" stroke="black" stroke-width="1"/>
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
import { Button1, ciButton1Controller } from '../controller/CiButton1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiButton1',
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
    button1: function (): Button1 | undefined {
      return ciButton1Controller.buttons1.find(button1 => button1.id === this.sid)
    },
    iButtonState: function (): number {
      return this.button1?.iButtonState ?? -1
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
      if (this.iButtonState === -2) return this.geof.colorOff
      if (this.iButtonState === -1) return this.geof.colorOn
      if (this.iButtonState <= 0) return this.geof.colorUnknown
      if (this.iButtonState === 9) return 'green' //      motor down
      if (this.iButtonState === 10) return 'lightgrey' // motor off
      if (this.iButtonState === 11) return 'yellow' //    motor up
      return this.geof.colorError
    },
    // -------text in line 1 and 5------------------------------
    lineHeader: function (): string {
      if (this.button1?.name) return this.geof.center(this.button1.name)
      return this.geof.center(this.sid)
    },
    lineFooter: function (): string {
      if (this.button1?.textFooter) return this.geof.center(this.button1.textFooter)
      if (this.button1?.battery) return this.geof.center(this.button1.battery)
      return this.geof.center(this.sid)
    },
    // -------symbol text in line 3-----------------------------
    lineCenter: function (): string {
      if (this.button1?.textCenter) return this.geof.center(this.button1.textCenter)
      return ''
    },
    // -------should I draw an additional symbol?---------------
    isPath: function (): boolean {
      const sym = this.shape.toLowerCase()
      if (sym.indexOf('up') >= 0) return true
      if (sym.indexOf('down') >= 0) return true
      if (sym.indexOf('left') >= 0) return true
      if (sym.indexOf('right') >= 0) return true
      return false
    },
    // -------draw an additional symbol-------------------------
    drawPath: function (): string {
      if (!this.shape) return ''
      if (this.shape.length < 1) return ''
      let s1 = ''
      const sym = this.shape.toLowerCase()
      const cR = 0.8 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      s1 = 'M' + this.geof.xc() + ',' + this.geof.yc(this.iLines) + ' '
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
    onClk: function (): void {
      console.log(this.sid, 'Button-Click')
      const topic = 'ci/error/button'
      let payload = '-1'
      if (!this.button1) ciButton1Controller.publishCi(topic, payload)
      if (this.button1?.pubTopic) {
        const aPubTopic = this.button1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button1?.pubPayload) payload = this.button1.pubPayload
          ciButton1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
