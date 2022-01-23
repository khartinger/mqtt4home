<!--CiSmsIn.vue-->
<template>
  <!--draw border: : outer and inner rectangle-------------- -->
  <rect v-if="border0" class="ciOut" :x="geo.x0()" :y="geo.y0()" :width="2*geo.dxo" :height="geo.dyo" />
  <rect v-if="border1" class="ciIn"  :x="geo.x1()" :y="geo.y1()" :width="2*geo.dxo-2*geo.dxm" :height="geo.dyi" />
  <!--Draw rectangle 1, if line 1 is a title---------------- -->
  <rect class="ciRect1" :x="geo.x1()" :y="geo.yr(1)" :width="2*geo.dxo-2*geo.dxm" :height="2*geo.dyl" />
  <rect class="ciRect2" :x="geo.x1()" :y="geo.yr(1)" :width="6.5*geo.dxo/geo.tmax" :height="geo.dyl" />
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">SMS-IN</text>
  <!--write text-------------------------------------------- -->
  <text class="ciFont1" :x="geo.xt()+6.6*geo.dxo/geo.tmax" :y="geo.yt(1)">{{line1}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(2)">{{lineX[2]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(3)">{{lineX[3]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(4)">{{lineX[4]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{lineX[5]}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { SmsIn, ciSmsInController } from '../controller/CiSmsInController'
import { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiSmsIn',
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
    }
  },
  computed: {
    smsIn: function (): SmsIn | undefined {
      return ciSmsInController.smsIns.find(smsIn => smsIn.id === this.sid)
    },
    iSmsInState: function (): number {
      return this.smsIn?.iSmsInState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    isRect1: function (): boolean {
      if (this.iSmsInState === 1) return false
      return true
    },
    // -------text in line ...----------------------------------
    line1: function (): string {
      if (this.smsIn) {
        if (this.smsIn.lines.length > 0) return this.checkLenL(this.smsIn.lines[0])
        if (this.smsIn.name) return this.checkLenL(this.smsIn.name)
      }
      return this.checkLenL(this.sid)
    },
    lineX: function (): Array<string> {
      const lineValues = []
      if (this.smsIn) {
        lineValues.push('') // no line 0
        for (let i = 0; i < 5; i++) {
          let s1 = this.smsIn.lines[i]
          s1 = this.checkLenL(s1)
          lineValues.push(s1)
        }
      }
      return lineValues
    },
    // -------color line 2 to 4 depending on iSmsInState------
    colorState: function (): string {
      return 'none'
    },
    border0: function (): boolean {
      if (this.border > 1) return true
      return false
    },
    border1: function (): boolean {
      if (this.border > 0) return true
      return false
    }
  },

  methods: {
    // -------check text length---------------------------------
    checkLenL: function (text: string): string {
      const charmax = 2 * this.geo.tmax + 2
      if (text) {
        if (text.length > charmax) return text.substring(0, charmax)
        return text
      }
      return ''
    },
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'SmsIn-Click')
      if (this.smsIn) {
        if (this.smsIn.pubTopic) {
          let payload = '?'
          const aPubTopic = this.smsIn.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.smsIn?.pubPayload) payload = this.smsIn.pubPayload
            ciSmsInController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
  .ciRect1   { fill: white; stroke: blue; stroke-width: 1; }
  .ciRect2   { fill: yellow; stroke: blue; stroke-width: 1; }
</style>
