<!-- CiZ2Mstatus1.vue -----------------------khartinger----- -->
<!-- 2023-02-28: new                                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--color background-------------------------------------- -->
  <rect :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw buttons------------------------------------------ -->
  <path :d="drawUpper" :fill="colorA_1" stroke="black" stroke-width="2" />
  <path :d="drawLower" :fill="colorA_2" stroke="black" stroke-width="2" />
  <!--write upper and lower text---------------------------- -->
  <text v-if="lineUpper.length>0" :x="xtUpper" :y="ytUpper" class="ciFont1">{{lineUpper}}</text>
  <text v-if="lineLower.length>0" :x="xtLower" :y="ytLower" class="ciFont1">{{lineLower}}</text>
  <!--write header and footer text-------------------------- -->
  <text :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area(s)---------------------------------- -->
  <!--
  <rect @mouseover="onHeaderOver()" @mouseleave="onHeaderLeave()" class="ciClick" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect @click="onClkFooter()" class="ciClick" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  -->
  <rect @click="onClkStatus()" class="ciClick" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="Bh" />
  <rect @click="onClkRestart()" class="ciClick" :x="geof.x1()" :y="geof.y1()+geof.dyl+Bh" :width="geof.dxi()" :height="Bh"/>
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Z2Mstatus1, ciZ2Mstatus1Controller } from '../controller/CiZ2Mstatus1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiZ2Mstatus1',
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
    // _______find z2mstatus1 object to given sid__________________
    z2mstatus1: function (): Z2Mstatus1 | undefined {
      return ciZ2Mstatus1Controller.z2Mstatus1s.find(z2mstatus1 => z2mstatus1.id === this.sid)
    },
    // _______get iZ2Mstatus1State_________________________________
    iZ2Mstatus1State: function (): number {
      return this.z2mstatus1?.iZ2MstatusState ?? -1
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
      return this.colorA_common
    },
    // _______lower button color________________________________
    colorA_2: function (): string {
      if (this.colorA2 !== '-') return this.colorA2
      return this.colorA_common
    },
    // _______common part of button color A_1 and A_2___________
    colorA_common: function (): string {
      if (!this.z2mstatus1) return this.geof.colorError
      const i_ = this.z2mstatus1.iZ2MstatusState
      if (i_ <= 0) return this.geof.colorError
      if ((i_ & ciZ2Mstatus1Controller.maskZ2mOnlineValid) === 0) return this.geof.colorNotOk
      if ((i_ & ciZ2Mstatus1Controller.maskZ2mOnline) === 0) return this.geof.colorNotOk
      if ((i_ & ciZ2Mstatus1Controller.maskZ2mStatusValid) > 0) {
        if ((i_ & ciZ2Mstatus1Controller.maskZ2mHealthy) > 0) return this.geof.colorOk
        if ((i_ & ciZ2Mstatus1Controller.maskZ2mStatusOk) > 0) return this.geof.colorOk
        return this.geof.colorNotOk
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
      return this.geof.dyi() / 2 - this.geof.dyl
    },
    // _______x start position of upper text____________________
    xtUpper: function (): number {
      let dxt_ = 0
      if (this.z2mstatus1?.textUpper) {
        if (this.z2mstatus1.textUpper.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______x start position of lower text____________________
    xtLower: function (): number {
      let dxt_ = 0
      if (this.z2mstatus1?.textLower) {
        if (this.z2mstatus1.textLower.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______y text position upper text________________________
    ytUpper: function (): number {
      return this.geof.yc(this.iLines) - this.Bh * 0.40 - this.geof.dyl / 2 + this.geof.dyt - 3
    },
    // _______y text position upper text________________________
    ytLower: function (): number {
      return this.geof.yc(this.iLines) + this.Bh * 0.40 - this.geof.dyl / 2 + this.geof.dyt + 3
    },
    // _______text in line 1____________________________________
    lineHeader: function (): string {
      let s1 = this.sid
      if (this.z2mstatus1?.name) {
        s1 = this.z2mstatus1.name
      }
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      this.header_ = s1
      return this.geof.center(s1)
    },
    // _______text in line 5____________________________________
    lineFooter: function (): string {
      if (!this.z2mstatus1) return this.geof.center('missing object')
      if (this.z2mstatus1.iZ2MstatusState <= 0) return this.geof.center('unknown')
      let s1 = '?'
      const i_ = this.z2mstatus1.iZ2MstatusState
      if ((i_ & ciZ2Mstatus1Controller.maskZ2mOnlineValid) > 0) {
        if ((i_ & ciZ2Mstatus1Controller.maskZ2mOnline) > 0) s1 = 'online'
        else s1 = 'offline'
      } else {
        s1 = '??line'
      }
      if ((i_ & ciZ2Mstatus1Controller.maskZ2mStatusValid) > 0) {
        if ((i_ & ciZ2Mstatus1Controller.maskZ2mStatusOk) > 0) {
          if ((i_ & ciZ2Mstatus1Controller.maskZ2mHealthy) > 0) s1 += '|perfect'
          else s1 += '|ok status'
        } else {
          if ((i_ & ciZ2Mstatus1Controller.maskZ2mHealthy) > 0) s1 += '|healthy'
          else s1 += '|bad :('
        }
      } else {
        s1 += '|no state'
      }
      // console.log('lineFooter: s1=', s1)
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      this.footer_ = s1
      return this.geof.center(s1)
    },
    // _______symbol text in upper button_______________________
    lineUpper: function (): string {
      if (this.z2mstatus1?.textUpper) return this.geof.center(this.z2mstatus1.textUpper)
      return ''
    },
    // _______symbol text in lower button_______________________
    lineLower: function (): string {
      if (this.z2mstatus1?.textLower) return this.geof.center(this.z2mstatus1.textLower)
      return ''
    },
    // _______draw upper button symbol__________________________
    drawUpper: function (): string {
      let s1 = ''
      const Bh_ = this.Bh - 2 * this.Bry - 2
      const Bw_ = this.Bw - 2 * this.Brx
      s1 = 'M' + this.geof.x1() + ',' + (this.geof.yc(this.iLines) - this.Bry - 2)
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
      s1 = 'M' + this.geof.x1() + ',' + (this.geof.yc(this.iLines) + Bh_ + this.Bry + 2)
      s1 += 'v-' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',-' + this.Bry
      s1 += ' h' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,' + this.Brx + ',' + this.Bry
      s1 += ' v' + Bh_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',' + this.Bry
      s1 += ' h-' + Bw_ + ' a' + this.Brx + ',' + this.Bry + ',0,0,1,-' + this.Brx + ',-' + this.Bry
      s1 += ' z'
      return s1
    }
  },
  methods: {
    /* Possible future extension: show whole text (tooltip, alert?)
    onHeaderOver: function (): void {
      console.log(this.sid, 'onHeaderOver')
      // alert(this.header_)
    },
    onHeaderLeave: function (): void {
      console.log(this.sid, 'onHeaderLeave')
    },
    onClkFooter: function (): void {
      console.log(this.sid, 'Click Footer')
      alert(this.footer_)
    },
    */
    onClkStatus: function (): void {
      console.log(this.sid, 'Click Status (=1)')
      const topic = 'ci/error/z2mstatus1'
      let payload = ''
      if (!this.z2mstatus1) ciZ2Mstatus1Controller.publishCi(topic, payload)
      if (this.z2mstatus1?.pubTopic) {
        const aPubTopic = this.z2mstatus1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.z2mstatus1?.pubPayload) payload = this.z2mstatus1.pubPayload
          ciZ2Mstatus1Controller.publishCi(topic, payload)
        })
      }
    },
    onClkRestart: function (): void {
      console.log(this.sid, 'Click Restart (=2)')
      if (this.z2mstatus1) this.z2mstatus1.iZ2MstatusState = 0
      const topic = 'ci/error/z2mstatus1'
      let payload = ''
      if (!this.z2mstatus1) ciZ2Mstatus1Controller.publishCi(topic, payload)
      if (this.z2mstatus1?.pubTopic2) {
        const aPubTopic = this.z2mstatus1.pubTopic2.split(' ')
        aPubTopic.forEach(topic => {
          if (this.z2mstatus1?.pubPayload2) payload = this.z2mstatus1.pubPayload2
          ciZ2Mstatus1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
