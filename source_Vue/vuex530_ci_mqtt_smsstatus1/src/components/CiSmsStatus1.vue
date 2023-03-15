<!-- CiSmsStatus1.vue -----------------------khartinger----- -->
<!-- 2023-03-15: new                                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--color background-------------------------------------- -->
  <rect :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw buttons------------------------------------------ -->
  <path :d="drawUpper" :fill="colorA_1" stroke="black" stroke-width="2" />
  <path :d="drawMiddle" :fill="colorA_2" stroke="black" stroke-width="2" />
  <path :d="drawLower" :fill="colorA_3" stroke="black" stroke-width="2" />
  <!--write upper, middle and lower button text------------- -->
  <text v-if="lineUpper.length>0" :x="xtUpper" :y="ytUpper" class="ciFont1">{{lineUpper}}</text>
  <text v-if="lineMiddle.length>0" :x="xtLower" :y="ytMiddle" class="ciFont1">{{lineMiddle}}</text>
  <text v-if="lineLower.length>0" :x="xtLower" :y="ytLower" class="ciFont1">{{lineLower}}</text>
  <!--write header and footer text-------------------------- -->
  <text :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area(s)---------------------------------- -->
  <!--
  <rect @mouseover="onHeaderOver()" @mouseleave="onHeaderLeave()" class="ciClick" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect @click="onClkFooter()" class="ciClick" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <line :x1="geof.x0()" :y1="geof.y0()" :x2="geof.xc()" :y2="geof.yc()" stroke="green" stroke-width="2"></line>
-->
  <rect @click="onClkUpper()" class="ciClick" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="Bh" />
  <rect @click="onClkMiddle()" class="ciClick" :x="geof.x1()" :y="geof.y1()+geof.dyl+Bh" :width="geof.dxi()" :height="Bh"/>
  <rect @click="onClkLower()" class="ciClick" :x="geof.x1()" :y="geof.y1()+geof.dyl+2*Bh" :width="geof.dxi()" :height="Bh"/>
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { SmsStatus1, ciSmsStatus1Controller } from '../controller/CiSmsStatus1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiSmsStatus1',
  components: {
    CiBase
  },
  data () {
    return {
      header_: '',
      footer_: '',
      lines: '2'
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
    sid: {
      type: String,
      required: true
    },
    border: {
      type: Number,
      required: false,
      default: 1
    },
    /*
    lines: {
      type: String,
      required: false,
      default: '2'
    },
    */
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
    colorA1: {
      type: String,
      required: false,
      default: '-'
    },
    colorA2: {
      type: String,
      required: false,
      default: '-'
    },
    colorA3: {
      type: String,
      required: false,
      default: '-'
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
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find smsstatus1 object to given sid__________________
    smsstatus1: function (): SmsStatus1 | undefined {
      return ciSmsStatus1Controller.z2Mstatus1s.find(smsstatus1 => smsstatus1.id === this.sid)
    },
    // _______get iSmsStatus1State_________________________________
    iSmsStatus1State: function (): number {
      return this.smsstatus1?.iSmsStatusState ?? -1
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
    // =======background colors=================================
    // _______color timedate1 header____________________________
    colorH_: function (): string {
      return this.colorH
    },
    // _______color timedate1 footer____________________________
    colorF_: function (): string {
      return this.colorF
    },
    // _______upper button color________________________________
    colorA_1: function (): string {
      if (this.colorA1 !== '-') return this.colorA1
      if (!this.smsstatus1) return this.geof.colorError
      let i_ = this.smsstatus1.iSmsStatusState
      if (i_ < 0) i_ = 0
      console.log('i1=', i_)
      if ((i_ & ciSmsStatus1Controller.maskSmsValidModule) > 0) {
        if ((i_ & ciSmsStatus1Controller.maskSmsModule) > 0) {
          return this.geof.colorOk
        } else {
          return this.geof.colorNotOk
        }
      }
      return this.geof.colorUnknown
    },
    // _______middle button color_______________________________
    colorA_2: function (): string {
      if (this.colorA1 !== '-') return this.colorA1
      if (!this.smsstatus1) return this.geof.colorError
      let i_ = this.smsstatus1.iSmsStatusState
      if (i_ < 0) i_ = 0
      console.log('i2=', i_)
      if ((i_ & ciSmsStatus1Controller.maskSmsValidSim) > 0) {
        if ((i_ & ciSmsStatus1Controller.maskSmsSim) > 0) {
          return this.geof.colorOk
        } else {
          return this.geof.colorNotOk
        }
      }
      return this.geof.colorUnknown
    },
    // _______lower button color________________________________
    colorA_3: function (): string {
      if (this.colorA1 !== '-') return this.colorA1
      if (!this.smsstatus1) return this.geof.colorError
      let i_ = this.smsstatus1.iSmsStatusState
      if (i_ < 0) i_ = 0
      console.log('i3=', i_)
      if ((i_ & ciSmsStatus1Controller.maskSmsValidCredit) > 0) {
        if ((i_ & ciSmsStatus1Controller.maskSmsCredit) > 0) {
          return this.geof.colorOk
        } else {
          return this.geof.colorNotOk
        }
      }
      return this.geof.colorUnknown
    },
    // =======other methods=====================================
    // _______circle parameter: radius__________________________
    cr: function (): number {
      const crx_ = this.geof.dxo2() - this.geof.dxm - 1
      let cry_ = this.geof.dyo2() - this.geof.dym - 1
      if (this.iLines === 1) cry_ = cry_ - this.geof.dyl / 2
      if (this.iLines === 2) cry_ = cry_ - this.geof.dyl
      if (crx_ < cry_) return crx_
      return cry_
    },
    // _______button: corner rounding___________________________
    Brx: function (): number {
      return (this.cr / 10)
    },
    Bry: function (): number {
      return this.Brx
    },
    // _______button width______________________________________
    Bw: function (): number {
      return this.geof.dxi()
    },
    // _______button height of one button_______________________
    Bh: function (): number {
      // return this.geof.dyi() - 2 * this.geof.dyl
      return (this.geof.dyi() - 2 * this.geof.dyl) / 3
    },
    // _______x start position of upper text____________________
    xtUpper: function (): number {
      let dxt_ = 0
      if (this.smsstatus1?.textUpper) {
        if (this.smsstatus1.textUpper.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______x start position of lower text____________________
    xtLower: function (): number {
      let dxt_ = 0
      if (this.smsstatus1?.textLower) {
        if (this.smsstatus1.textLower.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______y text position upper text________________________
    ytUpper: function (): number {
      return this.geof.yc(this.iLines) - this.Bh - this.geof.dyl / 2 + this.geof.dyt - 1
    },
    // _______y text position middle text_______________________
    ytMiddle: function (): number {
      return this.geof.yc(this.iLines) - this.geof.dyl / 2 + this.geof.dyt
    },
    // _______y text position upper text________________________
    ytLower: function (): number {
      return this.geof.yc(this.iLines) + this.Bh - this.geof.dyl / 2 + this.geof.dyt + 1
    },
    // _______text in line 1____________________________________
    lineHeader: function (): string {
      let s1 = this.sid
      if (this.smsstatus1?.name) {
        s1 = this.smsstatus1.name
      }
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      this.header_ = s1
      return this.geof.center(s1)
    },
    // _______text in line 5____________________________________
    lineFooter: function (): string {
      if (!this.smsstatus1) return this.geof.center('missing object')
      if (this.smsstatus1.iSmsStatusState <= 0) return this.geof.center('unknown')
      let s1 = '?'
      const i_ = this.smsstatus1.iSmsStatusState
      if ((i_ & ciSmsStatus1Controller.maskSmsValidAny) > 0) {
        if (this.smsstatus1.textFooter) s1 = this.smsstatus1.textFooter
      }
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      this.footer_ = s1
      return this.geof.center(s1)
    },
    // _______symbol text in upper button_______________________
    lineUpper: function (): string {
      if (this.smsstatus1?.textUpper) return this.geof.center(this.smsstatus1.textUpper)
      return ''
    },
    // _______symbol text in lower button_______________________
    lineMiddle: function (): string {
      if (this.smsstatus1?.textMiddle) return this.geof.center(this.smsstatus1.textMiddle)
      return ''
    },
    // _______symbol text in lower button_______________________
    lineLower: function (): string {
      if (this.smsstatus1?.textLower) return this.geof.center(this.smsstatus1.textLower)
      return ''
    },
    // _______draw upper button symbol__________________________
    drawUpper: function (): string {
      let s1 = ''
      const Bh_ = this.Bh - 2 * this.Bry - 2
      const Bw_ = this.Bw - 2 * this.Brx
      // -----start position: bottom left after radius----------
      s1 = 'M' + this.geof.x1() + ',' + (this.geof.yc(this.iLines) - this.Bh / 2 - this.Bry - 1)
      s1 += 'v-' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',-' + this.Bry
      s1 += ' h' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',' + this.Bry
      s1 += ' v' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',' + this.Bry
      s1 += ' h-' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',-' + this.Bry
      s1 += ' z'
      return s1
    },
    drawMiddle: function (): string {
      let s1 = ''
      const Bh_ = this.Bh - 2 * this.Bry - 2
      const Bw_ = this.Bw - 2 * this.Brx
      s1 = 'M' + this.geof.x1() + ',' + (this.geof.yc(this.iLines) + this.Bh / 2 - this.Bry)
      s1 += 'v-' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',-' + this.Bry
      s1 += ' h' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',' + this.Bry
      s1 += ' v' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',' + this.Bry
      s1 += ' h-' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',-' + this.Bry
      s1 += ' z'
      return s1
    },
    // _______draw lower button symbol__________________________
    drawLower: function (): string {
      let s1 = ''
      const Bh_ = this.Bh - 2 * this.Bry - 2
      const Bw_ = this.Bw - 2 * this.Brx
      s1 = 'M' + this.geof.x1() + ',' + (this.geof.yc(this.iLines) + 1.5 * this.Bh - this.Bry + 1)
      s1 += 'v-' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',-' + this.Bry
      s1 += ' h' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',' + this.Bry
      s1 += ' v' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',' + this.Bry
      s1 += ' h-' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',-' + this.Bry
      s1 += ' z'
      return s1
    }
  },
  methods: {
    onClkUpper: function (): void {
      // console.log(this.sid, 'Click Modem Status (=1)')
      const topic = 'ci/error/smsstatus1'
      let payload = '?'
      if (!this.smsstatus1) ciSmsStatus1Controller.publishCi(topic, payload)
      if (this.smsstatus1?.pubTopic) {
        const aPubTopic = this.smsstatus1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.smsstatus1?.pubPayload) payload = this.smsstatus1.pubPayload
          ciSmsStatus1Controller.publishCi(topic, payload)
        })
      }
    },
    onClkMiddle: function (): void {
      // console.log(this.sid, 'Click SIM (=2)')
      const topic = 'ci/error/smsstatus1'
      let payload = '?'
      if (!this.smsstatus1) ciSmsStatus1Controller.publishCi(topic, payload)
      if (this.smsstatus1?.pubTopic2) {
        const aPubTopic = this.smsstatus1.pubTopic2.split(' ')
        aPubTopic.forEach(topic => {
          if (this.smsstatus1?.pubPayload2) payload = this.smsstatus1.pubPayload2
          ciSmsStatus1Controller.publishCi(topic, payload)
        })
      }
    },
    onClkLower: function (): void {
      // console.log(this.sid, 'Click credit (=3)')
      const topic = 'ci/error/smsstatus1'
      let payload = '?'
      if (!this.smsstatus1) ciSmsStatus1Controller.publishCi(topic, payload)
      if (this.smsstatus1?.pubTopic3) {
        const aPubTopic = this.smsstatus1.pubTopic3.split(' ')
        this.smsstatus1.textFooter = 'wait...'
        aPubTopic.forEach(topic => {
          if (this.smsstatus1?.pubPayload3) payload = this.smsstatus1.pubPayload3
          ciSmsStatus1Controller.publishCi(topic, payload)
          console.log('publish -t', topic, ' -m ', payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
