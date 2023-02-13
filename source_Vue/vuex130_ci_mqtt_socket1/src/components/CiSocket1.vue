<!-- CiSocket1.vue --------------------------khartinger----- -->
<!-- 2022-09-14: new                                         -->
<!-- 2023-01-29: change at CiBase (add Geo.ts)               -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorSocket1" />
  <circle :cx="cx-cr/2" :cy="cy" :r="cr*0.1" fill="silver" stroke="black" stroke-width="2"/>
  <circle :cx="cx+cr/2" :cy="cy" :r="cr*0.1" fill="silver" stroke="black" stroke-width="2"/>
  <path   :d="drawPE" stroke="black" stroke-width="1" fill="silver" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />

  <!--write center text------------------------------------- -->
  <text v-if="lineCenter.length>0" :x="geof.xt()" :y="ytCenter" class="ciFont1">{{lineCenter}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="iLines>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
  </template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Socket1, ciSocket1Controller } from '../controller/CiSocket1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiSocket1',
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
    color: {
      type: String,
      required: false,
      default: '-'
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find socket1 object to given sid__________________
    socket1: function (): Socket1 | undefined {
      return ciSocket1Controller.sockets1.find(socket1 => socket1.id === this.sid)
    },
    // _______get iSocket1State_________________________________
    iSocket1State: function (): number {
      return this.socket1?.iSocket1State ?? -1
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

    // _______socket1 representation____________________________
    colorSocket1: function (): string {
      if (this.color !== '-') return this.color
      if (this.iSocket1State === -2) return this.geof.colorOff
      if (this.iSocket1State === -1) return this.geof.colorOn
      if (this.iSocket1State < 0) return this.geof.colorUnknown
      // .....iSocket1State contains socket color...............
      return '#' + this.iSocket1State.toString(16).padStart(6, '0')
    },
    ytCenter: function (): number {
      return this.geof.ytCenter(this.iLines) - this.cr * 0.1 - this.geof.dyl * 0.5
    },
    // _______text in line 1 and 5______________________________
    lineHeader: function (): string {
      if (this.socket1?.name) return this.geof.center(this.socket1.name)
      return this.geof.center(this.sid)
    },
    lineFooter: function (): string {
      if (this.socket1?.textFooter) return this.geof.center(this.socket1.textFooter)
      if (this.socket1?.battery) return this.geof.center(this.socket1.battery)
      return this.geof.center(this.sid)
    },
    // _______symbol text in line 3_____________________________
    lineCenter: function (): string {
      if (this.socket1?.textCenter) return this.geof.center(this.socket1.textCenter)
      return ''
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
    onClk: function (): void {
      console.log(this.sid, 'Socket-Click')
      const topic = 'ci/error/socket'
      let payload = '-1'
      if (!this.socket1) ciSocket1Controller.publishCi(topic, payload)
      if (this.socket1?.pubTopic) {
        const aPubTopic = this.socket1.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.socket1?.pubPayload) payload = this.socket1.pubPayload
          ciSocket1Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
