<!-- CiMotion1.vue --------------------------khartinger----- -->
<!-- 2022-01-01: new                                         -->
<!-- 2023-02-09: change at CiBase (add Geo.ts)               -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--color background-------------------------------------- -->
  <rect class="ciLine" :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="geof.dyi()-2*geof.dyl"/>
  <rect class="ciLine" :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect class="ciLine" :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw symbol (rectangles line 2)----------------------- -->
  <rect v-if="geof.calcLinemax()>2" class="ciLine" :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi2()" :height="geof.dyl"/>
  <rect v-if="geof.calcLinemax()>2" class="ciLine" :fill="colorA_" :x="geof.x1()+geof.dxi2()" :y="geof.y1()+geof.dyl" :width="geof.dxi2()" :height="geof.dyl"/>
  <!--write text-------------------------------------------- -->
  <text :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt()+0.5*geof.dxi2()-geof.dxi()/geof.calctmax()" :y="geof.yt(2)" class="ciFont1">))</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt2()+0.5*geof.dxi2()-1.5*geof.dxi()/geof.calctmax()" :y="geof.yt(2)" class="ciFont1">{{ sMotion1State }}</text>
  <text v-if="geof.calcLinemax()>3" :x="geof.xt()" :y="geof.yt(3)" class="ciFont1">{{line3}}</text>
  <text v-if="geof.calcLinemax()>4" :x="geof.xt()" :y="geof.yt(4)" class="ciFont1">{{line4}}</text>
  <text v-if="geof.calcLinemax()>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>

  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Motion1, ciMotion1Controller } from '../controller/CiMotion1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiMotion1',
  components: {
    CiBase
  },
  data () {
    return {
      batteryMin: 15 as number,
      colorMotion: 'yellow',
      colorNoMotion: 'lightgrey'
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
    // _______find motion1 object to given sid_________________
    motion1: function (): Motion1 | undefined {
      return ciMotion1Controller.motion1s.find(motion1 => motion1.id === this.sid)
    },
    // _______get iMotion1State________________________________
    iMotion1State: function (): number {
      // console.log('this.motion1?.iMotion1State=', this.motion1?.iMotion1State)
      if (this.motion1?.iMotion1State) {
        return this.motion1?.iMotion1State
      }
      return ciMotion1Controller.stateNoState // no state
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // =======date/time yyyymmdd HHMMSS methods=================
    lastMotion1Date: function (): string {
      return this.motion1?.lastMotion1Date ?? this.geof.noDate
    },
    lastMotion1Time: function (): string {
      return this.motion1?.lastMotion1Time ?? this.geof.noTime
    },
    // =======text in line ...==================================
    lineHeader: function (): string {
      if (this.motion1?.name) return this.geof.center(this.motion1.name)
      return this.geof.center(this.sid)
    },
    line3: function (): string {
      if (!this.motion1) return this.geof.center('unknown')
      if (this.motion1?.lastMotion1Date) {
        return this.geof.center(this.motion1.lastMotion1Date)
      }
      return 'Date?'
    },
    line4: function (): string {
      // console.log('line4:', this.iMotion1State)
      if (!this.motion1) return this.geof.center('sid')
      if (this.motion1?.lastMotion1Time) {
        return this.geof.center(this.motion1.lastMotion1Time)
      }
      return 'Time?'
    },
    lineFooter: function (): string {
      if (this.motion1?.textFooter) return this.geof.center(this.motion1.textFooter)
      if (this.motion1?.battery) return this.geof.center(this.motion1.battery.toString())
      return this.geof.center(this.sid)
    },
    // _______sMotion1State as string___________________________
    sMotion1State: function (): string {
      if (!this.motion1) return '?'
      if (this.iMotion1State === ciMotion1Controller.stateOn) return 'on'
      if (this.iMotion1State === ciMotion1Controller.stateOff) return 'off'
      return '?'
    },
    // =======background colors=================================
    // _______color of text area (line 2 to 4)__________________
    // depending on iMotion1State
    colorA_: function (): string {
      if (!this.motion1) return this.geof.colorNotOk
      if (this.iMotion1State === ciMotion1Controller.stateOff) return this.colorNoMotion
      if (this.iMotion1State === ciMotion1Controller.stateOn) return this.colorMotion
      return this.geof.colorUnknown
    },
    // _______color of header___________________________________
    colorH_: function (): string {
      if (this.colorH !== 'none') return this.colorH
      return 'none'
    },
    // _______color of footer depending on battery value________
    colorF_: function (): string {
      if (!this.motion1) return this.geof.colorNotOk
      if (this.iMotion1State !== ciMotion1Controller.stateOn &&
      this.iMotion1State !== ciMotion1Controller.stateOff) return this.geof.colorNotOk
      if (this.motion1?.battery) {
        try {
          const batt = parseInt(this.geof.center(this.motion1.battery).valueOf())
          if (batt > this.batteryMin) return this.geof.colorOk
          return this.geof.colorNotOk
        } catch (error) { return this.geof.colorUnknown }
      }
      if (this.colorF !== 'none') return this.colorF
      return 'none'
    }

    // =======aditional drawings================================
  },

  methods: {
    // _______react on click: publish a message (if any exists)_
    onClk: function (): void {
      // console.log(this.sid, 'Motion1-Click')
      if (this.motion1) {
        if (this.motion1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.motion1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.motion1?.pubPayload) payload = this.motion1.pubPayload
            ciMotion1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
