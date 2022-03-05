<!-- ---------CiSeg7x.vue -------------------2022-03-04----- -->
<!-- Seven-segment display with multiple digits (1 to 24)    -->
<template>
  <!-- -------draw digits----------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <template v-for="i in numDigits()" :key="i">
    <CiSeg7 :x="xdigit(i-1)" :y="y" :value="valueForDigit(i-1)" :height="height" :border="0"></CiSeg7>
  </template>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClick_()" :x="x0click" :y="y0click" :width="wclick" :height="hclick" fill-opacity="0.0"/>
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import CiBase, { Geo } from './CiBase.vue'
import { CiSeg7x, ciSeg7xController } from '../controller/CiSeg7xController'
import CiSeg7 from './CiSeg7.vue'

export default defineComponent({
  name: 'CiSeg7x',
  components: {
    CiBase,
    CiSeg7
  },
  data: function () {
    return {
      digitMaxNumber: 24
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
      default: 0
    },
    sid: {
      type: String,
      required: false,
      default: ''
    },
    value: {
      type: String,
      required: false,
      default: '8:.'
    },
    digits: {
      type: String,
      required: false,
      default: '1'
    },
    height: {
      type: Number,
      required: false,
      default: 26.2
    },
    colorOn: {
      type: String,
      required: false,
      default: '#FF0000'
    },
    colorOff: {
      type: String,
      required: false,
      default: 'rgba(255,0,0,0.05)'
      // default: 'transparent' // '#777777'
    },
    lines: {
      type: String,
      required: false,
      default: '0'
    }
  },
  computed: {
    ciSeg7x: function (): CiSeg7x | undefined {
      return ciSeg7xController.ciSeg7xs.find(ciSeg7x => ciSeg7x.id === this.sid)
    },
    iSeg7xState: function (): number {
      return this.ciSeg7x?.iSeg7xState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.ciSeg7x?.name) return this.geo.center(this.ciSeg7x.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.ciSeg7x?.text5) return this.geo.center(this.ciSeg7x.text5)
      return this.geo.center(this.sid)
    },
    // -------value to be displayed-----------------------------
    value7x: function (): string {
      if (this.ciSeg7x?.value7x) return this.ciSeg7x.value7x
      if (this.value) return this.value
      return '+64+64+64+64'
    },
    // -------click area: start point, extension----------------
    x0click: function (): number { return this.xdigit(0) - this.height * 0.35 },
    y0click: function (): number { return this.y - this.height * 0.5 },
    wclick: function (): number { return (this.numDigits() - 0.16) * this.height },
    hclick: function (): number { return this.height }
  },
  methods: {
    // -------number of digits as number------------------------
    numDigits: function (): number {
      const numDigits_ = Number.parseInt(this.digits, 10)
      if (Number.isNaN(numDigits_)) return 0
      return numDigits_
    },
    // -------x value of center of digit------------------------
    xdigit: function (posi: number): number {
      return (this.x - this.height * 0.5 * (this.numDigits() - 1)) + this.height * 1.0 * posi
    },
    // -------find given digit number, return string value------
    valueForDigit: function (digit_: number): string {
      if (digit_ >= this.digitMaxNumber) return ' '
      if (digit_ >= this.numDigits()) return ' '
      if (this.value7x.length < 1) return ' '
      if (this.value7x.at(0) === '+') {
        // ---special characters beginning with +---------------
        const aDigits = this.value7x.split('+')
        if (digit_ < (aDigits.length - 1)) return ('+' + aDigits[digit_ + 1])
        return ' '
      } else {
        // ---number or date or time----------------------------
        let i = 0
        let iDigit = 0
        let iStart = 0
        const len_ = this.value7x.length
        while (i < len_) {
          // ------for every char in value----------------------
          let c1 = this.value7x.at(i)
          while ((++i) < len_) {
            c1 = this.value7x.at(i)
            if (c1 !== '.' && c1 !== ':') break
          }
          const sDigit = this.value7x.substring(iStart, i)
          if (digit_ === iDigit) return sDigit
          iDigit++
          iStart = i
        } // ------end for every char in value------------------
      }
      return ' '
    },
    // -------On Click: publish the displayed value by MQTT-----
    onClick_: function (): void {
      console.log(this.sid, 'CiSeg7x-Click')
      const topic = this.ciSeg7x?.pubTopic ?? 'ci/seg7x/ret/error'
      let payload = this.value7x
      if (!this.ciSeg7x) ciSeg7xController.publishCi(topic, payload)
      if (this.ciSeg7x?.pubTopic) {
        const aPubTopic = this.ciSeg7x.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.ciSeg7x?.pubPayload) payload = this.ciSeg7x.pubPayload
          ciSeg7xController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
