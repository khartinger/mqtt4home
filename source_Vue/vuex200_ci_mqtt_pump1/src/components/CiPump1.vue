<!-- CiPump1.vue ----------------------------khartinger----- -->
<!-- 2022-10-19: new                                         -->
<!-- 2023-02-15: change at CiBase (add Geo.ts)               -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--color background-------------------------------------- -->
  <rect class="ciLine" :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="geof.dyi()-2*geof.dyl"/>
  <rect class="ciLine" :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect class="ciLine" :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw symbol (rectangle line 2)------------------------ -->
  <rect v-if="geof.calcLinemax()>2" class="ciLine" :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="geof.dyl"/>
  <!--draw pump symbol-------------------------------------- -->
  <circle v-if="geof.calcLinemax()>2" class="ciLine" :cx="cx" :cy="cy" :r="cr" :fill="colorA_"/>
  <line   v-if="geof.calcLinemax()>2" class="ciLine" :x1="cx" :y1="cy - cr" :x2="cx + cr" :y2="cy" />
  <line   v-if="geof.calcLinemax()>2" class="ciLine" :x1="cx" :y1="cy + cr" :x2="cx + cr" :y2="cy" />
  <!--write text-------------------------------------------- -->
  <text :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt()" :y="geof.yt(2)" class="ciFont1">{{ sSensor }}</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt2()+0.5*geof.dxi2()-geof.dxi()/geof.calctmax()" :y="geof.yt(2)" class="ciFont1">{{ sPump1State }}</text>
  <text v-if="geof.calcLinemax()>3" :x="geof.xt()" :y="geof.yt(3)" class="ciFont1">{{line3}}</text>
  <text v-if="geof.calcLinemax()>4" :x="geof.xt()" :y="geof.yt(4)" class="ciFont1">{{line4}}</text>
  <text v-if="geof.calcLinemax()>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>

  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Pump1, ciPump1Controller } from '../controller/CiPump1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiPump1',
  components: {
    CiBase
  },
  data () {
    return {
      colorPumpOn: 'lightgreen',
      colorPumpOff: 'lightgrey'
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
      default: 2
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
    // _______find pump1 object to given sid_________________
    pump1: function (): Pump1 | undefined {
      return ciPump1Controller.pump1s.find(pump1 => pump1.id === this.sid)
    },
    // _______get iPump1State________________________________
    iPump1State: function (): number {
      // console.log('this.pump1?.iPump1State=', this.pump1?.iPump1State)
      if (this.pump1?.iPump1State) {
        return this.pump1?.iPump1State
      }
      return ciPump1Controller.stateNoState // no state
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // =======date/time yyyymmdd HHMMSS methods=================
    lastPump1Date: function (): string {
      return this.pump1?.lastPump1Date ?? this.geof.noDate
    },
    lastPump1Time: function (): string {
      return this.pump1?.lastPump1Time ?? this.geof.noTime
    },
    // =======draw pump symbol==================================
    cx: function (): number {
      // return (this.geof.x0() + this.geof.dxm + this.geof.dxi() / 4)
      return (this.geof.xc())
    },
    cy: function (): number { return (this.y - this.geof.dyl) },
    cr: function (): number { return (Math.round(this.geof.dyl / 2 - 0.5) - 1) },
    // =======text in line ...==================================
    lineHeader: function (): string {
      if (this.pump1?.name) return this.geof.center(this.pump1.name)
      return this.geof.center(this.sid)
    },
    line3: function (): string {
      if (!this.pump1) return this.geof.center('unknown')
      if (this.pump1?.lastPump1Date) {
        return this.geof.center(this.pump1.lastPump1Date)
      }
      return 'Date?'
    },
    line4: function (): string {
      if (!this.pump1) return this.geof.center('sid')
      if (this.pump1?.lastPump1Time) {
        return this.geof.center(this.pump1.lastPump1Time)
      }
      return 'Time?'
    },
    lineFooter: function (): string {
      if (this.pump1?.textFooter) return this.geof.center(this.pump1.textFooter)
      if (this.pump1?.sStatus) return this.geof.center(this.sStatus)
      return ''
    },
    // =======additional texts==================================
    // _______a sensor value e.g. 0x0F__________________________
    sSensor: function (): string {
      if (!this.pump1?.sSensor) return '?'
      const maxchar = Math.trunc(this.geof.calctmax() / 2) - 1
      return this.pump1.sSensor.substring(0, maxchar)
    },
    // _______state of pump system as text______________________
    sStatus: function (): string {
      return this.pump1?.sStatus ?? '?'
    },
    // _______sPump1State as string_____________________________
    sPump1State: function (): string {
      if (!this.pump1) return '?'
      if (this.iPump1State === ciPump1Controller.stateOn) return 'ON'
      if (this.iPump1State === ciPump1Controller.stateOff) return 'OFF'
      return '?'
    },

    // =======background colors=================================
    // _______color of text area (line 2 to 4)__________________
    // depending on iPump1State
    colorA_: function (): string {
      if (!this.pump1) return this.geof.colorNotOk
      if (this.sStatus.includes('ALARM')) return this.geof.colorAlarm
      if (this.sStatus.includes('ERROR')) return this.geof.colorError
      if (this.iPump1State === ciPump1Controller.stateOn) return this.colorPumpOn
      if (this.iPump1State === ciPump1Controller.stateOff) return this.colorPumpOff
      return this.geof.colorUnknown
    },
    // _______color of header___________________________________
    colorH_: function (): string {
      if (!this.pump1) return this.geof.colorNotOk
      if (this.sStatus.includes('ALARM')) return this.geof.colorAlarm
      if (this.sStatus.includes('ERROR')) return this.geof.colorError
      if (this.colorH !== 'none') return this.colorH
      return 'none'
    },
    // _______color of footer___________________________________
    colorF_: function (): string {
      if (this.colorH !== 'none') return this.colorF
      return this.colorA_
    }
    // =======aditional drawings================================
  },

  methods: {
    // _______react on click: publish a message (if any exists)_
    onClk: function (): void {
      // console.log(this.sid, 'Pump1-Click')
      if (this.pump1) {
        if (this.pump1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.pump1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.pump1?.pubPayload) payload = this.pump1.pubPayload
            ciPump1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
