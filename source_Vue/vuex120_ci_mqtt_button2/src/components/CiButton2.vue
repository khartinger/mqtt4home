<!-- CiButton2.vue --------------------------khartinger----- -->
<!-- 2022-01-28: new                                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw shape-------------------------------------------- -->
  <path :d="drawUpper" :fill="colorUpper" stroke="black" stroke-width="2" />
  <path :d="drawLower" :fill="colorLower" stroke="black" stroke-width="2" />
  <!--draw additional symbols (shape)----------------------- -->
  <path v-if="isAddUpper" :d="drawAddUpper" fill="none" stroke="black" stroke-width="1"/>
  <path v-if="isAddLower" :d="drawAddLower" fill="none" stroke="black" stroke-width="1"/>
  <!--write upper and lower text---------------------------- -->
  <text v-if="lineUpper.length>0" :x="xtUpper" :y="ytUpper" class="ciFont1">{{lineUpper}}</text>
  <text v-if="lineLower.length>0" :x="xtLower" :y="ytLower" class="ciFont1">{{lineLower}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="iLines>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClkUpper()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="dyc" />
  <rect @click="onClkLower()" class="ciClick" :x="geof.x0()" :y="Syc" :width="geof.dxo()" :height="geof.dyo()-dyc" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Button2, ciButton2Controller } from '../controller/CiButton2Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

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
    sid: {
      type: String,
      required: true
    },
    border: {
      type: Number,
      required: false,
      default: 1
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
    colorup: {
      type: String,
      required: false,
      default: '-'
    },
    colordn: {
      type: String,
      required: false,
      default: '-'
    }

  },
  computed: {
    // =======standard methods==================================
    // _______find button2 object to given sid__________________
    button2: function (): Button2 | undefined {
      return ciButton2Controller.buttons2.find(button2 => button2.id === this.sid)
    },
    // _______get iButton2State_________________________________
    iButton2State: function (): number {
      return this.button2?.iButton2State ?? -1
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // _______number of displayed lines_________________________
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // =======other methods=====================================
    // _______0=circle, 1=square, 2=round, 3=rect_______________
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
    // _______circle parameter: radius__________________________
    cr: function (): number {
      const crx_ = this.geof.dxo2() - this.geof.dxm - 1
      let cry_ = this.geof.dyo2() - this.geof.dym - 1
      if (this.iLines === 1) cry_ = cry_ - this.geof.dyl / 2
      if (this.iLines === 2) cry_ = cry_ - this.geof.dyl
      if (crx_ < cry_) return crx_
      return cry_
    },
    // _______distance y0 to center y___________________________
    dyc: function (): number {
      if (this.iLines === 1) return (this.geof.dyo() + this.geof.dyl) / 2
      return this.geof.dyo2()
    },
    // _______shape parameters: up left corner__________________
    Sx0: function (): number {
      if (this.iShape > 4) { // circle, square
        return (this.geof.xc() - this.cr)
      }
      return this.geof.x1() // round, rect
    },
    // _______shape parameters: up left corner__________________
    Sy0: function (): number {
      if (this.iShape > 4) { // circle, square
        return (this.geof.yc(this.iLines) - this.cr)
      }
      if (this.iLines > 0) return this.geof.y1() + this.geof.dyl
      return this.geof.y1()
    },
    // _______shape: graphical center___________________________
    Syc: function (): number {
      return this.geof.yc(this.iLines)
    },
    // _______shape: corner rounding____________________________
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
    // _______shape width_______________________________________
    Sw: function (): number {
      if (this.iShape > 4) { // circle, square
        return 2 * this.cr
      }
      return this.geof.dxi() // round, rect
    },
    // _______shape height______________________________________
    Sh: function (): number {
      if (this.iShape > 4) { // circle, square
        return 2 * this.cr
      }
      let dyL_ = 0
      if (this.iLines === 1) dyL_ = this.geof.dyl
      if (this.iLines === 2) dyL_ = 2 * this.geof.dyl
      return this.geof.dyi() - dyL_
    },
    /*
    xtStart: function (text: string): number {
      let dxt_ = 0
      if (text.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      return this.geof.xt() + dxt_
    },
    */
    // _______x start position of upper text____________________
    xtUpper: function (): number {
      let dxt_ = 0
      if (this.button2?.textUpper) {
        if (this.button2.textUpper.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______x start position of lower text____________________
    xtLower: function (): number {
      let dxt_ = 0
      if (this.button2?.textLower) {
        if (this.button2.textLower.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______y text position upper text________________________
    ytUpper: function (): number {
      return this.geof.yc(this.iLines) - this.Sh * 0.20 - this.geof.dyl / 2 + this.geof.dyt
    },
    // _______y text position upper text________________________
    ytLower: function (): number {
      return this.geof.yc(this.iLines) + this.Sh * 0.20 - this.geof.dyl / 2 + this.geof.dyt
    },
    // _______button representation_____________________________
    colorUpper: function (): string {
      if (this.colorup !== '-') return this.colorup
      if (this.iButton2State === -2) return this.geof.colorOff
      if (this.iButton2State === -1) return this.geof.colorOn
      if (this.iButton2State <= 0) return this.geof.colorUnknown
      return this.geof.colorError
    },
    colorLower: function (): string {
      if (this.colordn !== '-') return this.colordn
      if (this.iButton2State === -1) return this.geof.colorOff
      if (this.iButton2State === -2) return this.geof.colorOn
      if (this.iButton2State <= 0) return this.geof.colorUnknown
      return this.geof.colorError
    },
    // _______text in line 1 and 5______________________________
    lineHeader: function (): string {
      if (this.button2?.name) return this.geof.center(this.button2.name)
      return this.geof.center(this.sid)
    },
    lineFooter: function (): string {
      if (this.button2?.textFooter) return this.geof.center(this.button2.textFooter)
      if (this.button2?.battery) return this.geof.center(this.button2.battery)
      return this.geof.center(this.sid)
    },
    // _______symbol text in upper half_________________________
    lineUpper: function (): string {
      if (this.button2?.textUpper) return this.geof.center(this.button2.textUpper)
      return ''
    },
    // _______symbol text in upper half_________________________
    lineLower: function (): string {
      if (this.button2?.textLower) return this.geof.center(this.button2.textLower)
      return ''
    },
    // _______draw upper half of button symbol__________________
    drawUpper: function (): string {
      let s1 = ''
      s1 = 'M' + this.Sx0 + ',' + this.geof.yc(this.iLines) + ' z'
      if (this.iShape === 6) {
        s1 += 'a' + this.cr + ',' + this.cr + ',0,1,1,' + (2 * this.cr) + ',0 z'
      } else {
        const Sh_ = this.Sh / 2 - this.Sry
        const Sw_ = this.Sw - 2 * this.Srx
        s1 += 'v-' + Sh_ + ' a' + this.Srx + ',' + this.Sry + ',0,0,1,' + this.Srx + ',-' + this.Sry
        s1 += ' h' + Sw_ + ' a' + this.Srx + ',' + this.Sry + ',0,0,1,' + this.Srx + ',' + this.Sry
        s1 += ' v' + Sh_ + ' z'
      }
      return s1
    },
    // _______draw upper half of button symbol__________________
    drawLower: function (): string {
      let s1 = ''
      s1 = 'M' + this.Sx0 + ',' + this.geof.yc(this.iLines) + ' z'
      if (this.iShape === 6) {
        s1 += 'a' + this.cr + ',' + this.cr + ',0,1,0,' + (2 * this.cr) + ',0 z'
      } else {
        const Sh_ = this.Sh / 2 - this.Sry
        const Sw_ = this.Sw - 2 * this.Srx
        s1 += 'v' + Sh_ + ' a' + this.Srx + ',' + this.Sry + ',0,0,0,' + this.Srx + ',' + this.Sry
        s1 += ' h' + Sw_ + ' a' + this.Srx + ',' + this.Sry + ',0,0,0,' + this.Srx + ',-' + this.Sry
        s1 += ' v-' + Sh_ + ' z'
      }
      return s1
    },
    // _______should I draw an additional upper symbol?_________
    isAddUpper: function (): boolean {
      if (!this.shape) return false
      if (this.shape.length < 1) return false
      const sym = this.shape.toLowerCase()
      if (sym.indexOf('1up') >= 0) return true
      if (sym.indexOf('1down') >= 0) return true
      if (sym.indexOf('1left') >= 0) return true
      if (sym.indexOf('1right') >= 0) return true
      return false
    },
    // _______should I draw an additional lower symbol?_________
    isAddLower: function (): boolean {
      if (!this.shape) return false
      if (this.shape.length < 1) return false
      const sym = this.shape.toLowerCase()
      if (sym.indexOf('2up') >= 0) return true
      if (sym.indexOf('2down') >= 0) return true
      if (sym.indexOf('2left') >= 0) return true
      if (sym.indexOf('2right') >= 0) return true
      return false
    },
    // _______draw an additional upper symbol___________________
    drawAddUpper: function (): string {
      if (!this.isAddUpper) return ''
      let s1 = ''
      const sym = this.shape.toLowerCase()
      const cR = 0.4 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      let yc_ = (this.Sy0 + this.geof.yc(this.iLines)) / 2 + this.Sh * 0.05
      s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
      if (sym.indexOf('1up') >= 0) {
        yc_ += this.Sh * 0.01
        s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
        s1 += 'm0,-' + cR + ' l-' + (0.5 * ca) + ',' + (1.5 * cR) + ' h' + ca + ' z'
      } else {
        if (sym.indexOf('1down') >= 0) {
          yc_ -= this.Sh * 0.05
          s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
          s1 += 'm0,' + cR + ' l-' + (0.5 * ca) + ',-' + (1.5 * cR) + ' h' + ca + ' z'
        } else {
          if (sym.indexOf('1left') >= 0) {
            s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
            s1 += 'm-' + cR + ',0 l' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
          } else {
            if (sym.indexOf('1right') >= 0) {
              s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
              s1 += 'm' + cR + ',0 l-' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
            }
          }
        }
      }
      return s1
    },
    // _______draw an additional lower symbol___________________
    drawAddLower: function (): string {
      if (!this.isAddLower) return ''
      let s1 = ''
      const sym = this.shape.toLowerCase()
      const cR = 0.4 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      let yc_ = ((this.Sy0 + this.Sh) + this.geof.yc(this.iLines)) / 2 - this.Sh * 0.05
      s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
      if (sym.indexOf('2up') >= 0) {
        yc_ += this.Sh * 0.05
        s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
        s1 += 'm0,-' + cR + ' l-' + (0.5 * ca) + ',' + (1.5 * cR) + ' h' + ca + ' z'
      } else {
        if (sym.indexOf('2down') >= 0) {
          yc_ -= this.Sh * 0.01
          s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
          s1 += 'm0,' + cR + ' l-' + (0.5 * ca) + ',-' + (1.5 * cR) + ' h' + ca + ' z'
        } else {
          if (sym.indexOf('2left') >= 0) {
            s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
            s1 += 'm-' + cR + ',0 l' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
          } else {
            if (sym.indexOf('2right') >= 0) {
              s1 = 'M' + this.geof.xc() + ',' + yc_ + ' '
              s1 += 'm' + cR + ',0 l-' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
            }
          }
        }
      }
      return s1
    }
  },
  methods: {
    onClkUpper: function (): void {
      // console.log(this.sid, 'CiButton2-ClickUpper')
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
    onClkLower: function (): void {
      // console.log(this.sid, 'Socket-ClickLower')
      const topic = 'ci/error/button2'
      let payload = '-1'
      if (!this.button2) ciButton2Controller.publishCi(topic, payload)
      if (this.button2?.pubTopic) {
        const aPubTopic = this.button2.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button2?.pubPayloadLower) payload = this.button2.pubPayloadLower
          ciButton2Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
