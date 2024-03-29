<!-- CiSocket2.vue --------------------------khartinger----- -->
<!-- 2022-08-07: new                                         -->
<!-- 2023-01-29: change at CiBase (add Geo.ts)               -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw shape-------------------------------------------- -->
  <path :d="drawUpper" :fill="colorUpper" stroke="black" stroke-width="2" />
  <path :d="drawLower" :fill="colorLower" stroke="black" stroke-width="2" />
  <!---draw contacts---------------------------------------- -->
  <circle :cx="cx-cr/2" :cy="cy" :r="cr*0.1" fill="silver" stroke="black" stroke-width="2"/>
  <circle :cx="cx+cr/2" :cy="cy" :r="cr*0.1" fill="silver" stroke="black" stroke-width="2"/>
  <path   :d="drawPE" stroke="black" stroke-width="1" fill="silver" />
  <!--write upper and lower text---------------------------- -->
  <text v-if="lineUpper.length>0" :x="xtUpper" :y="ytUpper" class="ciFont1">{{lineUpper}}</text>
  <text v-if="lineLower.length>0" :x="xtLower" :y="ytLower" class="ciFont1">{{lineLower}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="iLines>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClkUpper()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="dy0c" />
  <rect @click="onClkLower()" class="ciClick" :x="geof.x0()" :y="cy" :width="geof.dxo()" :height="geof.dyo()-dy0c" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Socket2, ciSocket2Controller } from '../controller/CiSocket2Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiSocket2',
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
    // _______find socket2 object to given sid__________________
    socket2: function (): Socket2 | undefined {
      return ciSocket2Controller.sockets2.find(socket2 => socket2.id === this.sid)
    },
    // _______get iSocket2State_________________________________
    iSocket2State: function (): number {
      return this.socket2?.iSocket2State ?? -1
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
    // _______circle parameter: radius__________________________
    cr: function (): number {
      const crx_ = this.geof.dxo2() - this.geof.dxm - 1
      let cry_ = this.geof.dyo2() - this.geof.dym - 1
      if (this.iLines === 1) cry_ = cry_ - this.geof.dyl / 2
      if (this.iLines === 2) cry_ = cry_ - this.geof.dyl
      if (crx_ < cry_) return crx_
      return cry_
    },
    // _______circle parameter: center__________________________
    cx: function (): number { return this.geof.xc() },
    cy: function (): number { return this.geof.yc(this.iLines) },
    // _______distance y0 to center y___________________________
    dy0c: function (): number {
      if (this.iLines === 1) return (this.geof.dyo() + this.geof.dyl) / 2
      return this.geof.dyo2()
    },
    // _______x start position of upper text____________________
    xtUpper: function (): number {
      let dxt_ = 0
      if (this.socket2?.textUpper) {
        if (this.socket2.textUpper.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______x start position of lower text____________________
    xtLower: function (): number {
      let dxt_ = 0
      if (this.socket2?.textLower) {
        if (this.socket2.textLower.length % 2 === 1) dxt_ = this.geof.dxi() / this.geof.calctmax() / 2
      }
      return this.geof.xt() + dxt_
    },
    // _______y text position upper text________________________
    ytUpper: function (): number {
      return this.geof.yc(this.iLines) - 2 * this.cr * 0.20 - this.geof.dyl / 2 + this.geof.dyt
    },
    // _______y text position upper text________________________
    ytLower: function (): number {
      return this.geof.yc(this.iLines) + 2 * this.cr * 0.20 - this.geof.dyl / 2 + this.geof.dyt
    },
    // _______socket representation_____________________________
    colorUpper: function (): string {
      console.log('iSocket2State=', this.iSocket2State)
      if (this.colorup !== '-') return this.colorup
      if (this.iSocket2State === -2) return this.geof.colorOff
      if (this.iSocket2State === -1) return this.geof.colorOn
      if (this.iSocket2State <= 0) return this.geof.colorUnknown
      return this.geof.colorError
    },
    colorLower: function (): string {
      if (this.colordn !== '-') return this.colordn
      if (this.iSocket2State === -1) return this.geof.colorOff
      if (this.iSocket2State === -2) return this.geof.colorOn
      if (this.iSocket2State <= 0) return this.geof.colorUnknown
      return this.geof.colorError
    },
    // _______text in line 1 and 5______________________________
    lineHeader: function (): string {
      if (this.socket2?.name) return this.geof.center(this.socket2.name)
      return this.geof.center(this.sid)
    },
    lineFooter: function (): string {
      if (this.socket2?.textFooter) return this.geof.center(this.socket2.textFooter)
      if (this.socket2?.battery) return this.geof.center(this.socket2.battery)
      return this.geof.center(this.sid)
    },
    // _______symbol text in upper half_________________________
    lineUpper: function (): string {
      if (this.socket2?.textUpper) return this.geof.center(this.socket2.textUpper)
      return ''
    },
    // _______symbol text in upper half_________________________
    lineLower: function (): string {
      if (this.socket2?.textLower) return this.geof.center(this.socket2.textLower)
      return ''
    },
    // _______draw upper half of socket symbol__________________
    drawUpper: function (): string {
      let s1 = 'M' + (this.cx - this.cr) + ',' + this.cy
      s1 += ' a' + this.cr + ',' + this.cr + ',0,1,1,' + (2 * this.cr) + ',0 z'
      return s1
    },
    // _______draw upper half of socket symbol__________________
    drawLower: function (): string {
      let s1 = 'M' + (this.cx - this.cr) + ',' + this.cy
      s1 += ' a' + this.cr + ',' + this.cr + ',0,1,0,' + (2 * this.cr) + ',0 z'
      return s1
    },
    // _______draw PE contacts__________________________________
    drawPE: function (): string {
      const cr02 = 0.2 * this.cr //                   contact width
      const x0k = this.cx - this.cr * 0.1 //          x start point
      const dyk = this.cr * Math.sqrt(99) * 0.1 //    dy to circle
      const y0k = this.cy - dyk //                    y start point
      let s1 = 'M' + x0k + ',' + y0k
      s1 += ' v' + cr02 + ' h' + cr02 + ' v-' + cr02 + ' a' + this.cr + ',' + this.cr + ',0,0,0,-' + cr02 + ',0 z'
      s1 += ' m0,' + (2 * dyk)
      s1 += ' v-' + cr02 + ' h' + cr02 + ' v' + cr02 + ' a' + this.cr + ',' + this.cr + ',0,0,1,-' + cr02 + ',0 z'
      return s1
    }
  },
  methods: {
    onClkUpper: function (): void {
      // console.log(this.sid, 'CiSocket2-ClickUpper')
      const topic = 'ci/error/socket2'
      let payload = '-1'
      if (!this.socket2) ciSocket2Controller.publishCi(topic, payload)
      if (this.socket2?.pubTopic) {
        const aPubTopic = this.socket2.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.socket2?.pubPayload) payload = this.socket2.pubPayload
          ciSocket2Controller.publishCi(topic, payload)
        })
      }
    },
    onClkLower: function (): void {
      // console.log(this.sid, 'CiSocket2-ClickLower')
      const topic = 'ci/error/socket2'
      let payload = '-1'
      if (!this.socket2) ciSocket2Controller.publishCi(topic, payload)
      if (this.socket2?.pubTopic) {
        const aPubTopic = this.socket2.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.socket2?.pubPayloadLower) payload = this.socket2.pubPayloadLower
          ciSocket2Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
