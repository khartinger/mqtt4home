<!-- CiRepeater1.vue ------------------------khartinger----- -->
<!-- 2022-10-19: new                                         -->
<!-- 2023-02-10: change at CiBase (add Geo.ts)               -->

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
  <text v-if="geof.calcLinemax()>2" :x="geof.xt()+0.5*geof.dxi2()-1.5*geof.dxi()/geof.calctmax()" :y="geof.yt(2)" class="ciFont1">Rep</text>
  <text v-if="geof.calcLinemax()>2" :x="geof.xt2()+0.5*geof.dxi2()-geof.dxi()/geof.calctmax()" :y="geof.yt(2)" class="ciFont1">{{ sRepeater1State }}</text>
  <text v-if="geof.calcLinemax()>3" :x="geof.xt()" :y="geof.yt(3)" class="ciFont1">{{line3}}</text>
  <text v-if="geof.calcLinemax()>4" :x="geof.xt()" :y="geof.yt(4)" class="ciFont1">{{line4}}</text>
  <text v-if="geof.calcLinemax()>1" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>

  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Repeater1, ciRepeater1Controller } from '../controller/CiRepeater1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'

export default defineComponent({
  name: 'CiRepeater1',
  components: {
    CiBase
  },
  data () {
    return {
      colorRepeater: 'lightgreen',
      colorNoRepeater: 'lightgrey'
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
    border: {
      type: Number,
      required: false,
      default: 2
    },
    sid: {
      type: String,
      required: true
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
    // _______find repeater1 object to given sid_________________
    repeater1: function (): Repeater1 | undefined {
      return ciRepeater1Controller.repeater1s.find(repeater1 => repeater1.id === this.sid)
    },
    // _______get iRepeater1State________________________________
    iRepeater1State: function (): number {
      // console.log('this.repeater1?.iRepeater1State=', this.repeater1?.iRepeater1State)
      if (this.repeater1?.iRepeater1State) {
        return this.repeater1?.iRepeater1State
      }
      return ciRepeater1Controller.stateNoState // no state
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // =======date/time yyyymmdd HHMMSS methods=================
    lastRepeater1Date: function (): string {
      return this.repeater1?.lastRepeater1Date ?? this.geof.noDate
    },
    lastRepeater1Time: function (): string {
      return this.repeater1?.lastRepeater1Time ?? this.geof.noTime
    },
    // =======text in line ...==================================
    lineHeader: function (): string {
      if (this.repeater1?.name) return this.geof.center(this.repeater1.name)
      return this.geof.center(this.sid)
    },
    line3: function (): string {
      if (!this.repeater1) return this.geof.center('unknown')
      if (this.repeater1?.lastRepeater1Date) {
        return this.geof.center(this.repeater1.lastRepeater1Date)
      }
      return 'Date?'
    },
    line4: function (): string {
      // console.log('line4:', this.iRepeater1State)
      if (!this.repeater1) return this.geof.center('sid')
      if (this.repeater1?.lastRepeater1Time) {
        return this.geof.center(this.repeater1.lastRepeater1Time)
      }
      return 'Time?'
    },
    lineFooter: function (): string {
      if (this.repeater1?.textFooter) return this.geof.center(this.repeater1.textFooter)
      if (this.repeater1?.linkquality) return this.geof.center(this.repeater1.linkquality)
      return ''
    },
    // _______sRepeater1State as string___________________________
    sRepeater1State: function (): string {
      if (!this.repeater1) return '?'
      if (this.iRepeater1State === ciRepeater1Controller.stateOk) return 'ok'
      if (this.iRepeater1State === ciRepeater1Controller.stateLastSeen) return 'OK'
      return '?'
    },
    // =======background colors=================================
    // _______color of text area (line 2 to 4)__________________
    // depending on iRepeater1State
    colorA_: function (): string {
      if (!this.repeater1) return this.geof.colorNotOk
      if (this.iRepeater1State === ciRepeater1Controller.stateOk) return this.colorRepeater
      if (this.iRepeater1State === ciRepeater1Controller.stateLastSeen) return this.colorRepeater
      return this.colorNoRepeater
    },
    // _______color of header___________________________________
    colorH_: function (): string {
      if (this.colorH !== 'none') return this.colorH
      return 'none'
    },
    // _______color of footer depending on battery value________
    colorF_: function (): string {
      if (!this.repeater1) return this.geof.colorNotOk
      if (this.iRepeater1State !== ciRepeater1Controller.stateOk &&
      this.iRepeater1State !== ciRepeater1Controller.stateLastSeen) return this.geof.colorNotOk
      if (this.colorF !== 'none') return this.colorF
      return 'none'
    }
    // =======aditional drawings================================
  },

  methods: {
    // _______react on click: publish a message (if any exists)_
    onClk: function (): void {
      // console.log(this.sid, 'Repeater1-Click')
      if (this.repeater1) {
        if (this.repeater1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.repeater1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.repeater1?.pubPayload) payload = this.repeater1.pubPayload
            ciRepeater1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
