<!-- CiSeg7x.vue ----------------------------2022-02-23----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <template v-for="i in numDigits()" :key="i">
    <CiSeg7 :x="xdigit(i-1)" :y="y" :value="valueForDigit(i-1)" :height="height" :border="border"></CiSeg7>
  </template>
  <rect @click="onClk()" class="ciClick" :x="x0clk" :y="y0clk" :width="wclk" :height="hclk" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Seg7x, ciSeg7xController } from '../controller/CiSeg7xController'
import CiBase, { Geo } from './CiBase.vue'
import CiSeg7 from './CiSeg7.vue'

export default defineComponent({
  name: 'CiSeg7x',
  components: {
    CiBase,
    CiSeg7
  },
  data: function () {
    return {
      digitMaxNumber: 8
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
      required: false,
      default: 'CiSeg7x'
    },
    value: {
      type: String,
      required: false,
      default: '8'
    },
    digits: {
      type: String,
      required: false,
      default: '1'
    },
    height: {
      type: Number,
      required: false,
      default: 20.32
    },
    colorOn: {
      type: String,
      required: false,
      default: '#FF0000'
    },
    colorOff: {
      type: String,
      required: false,
      default: 'transparent'
      // default: '#777777'
    },
    lines: {
      type: String,
      required: false,
      default: '1'
    }
  },
  computed: {
    seg7x: function (): Seg7x | undefined {
      return ciSeg7xController.seg7xs.find(seg7x => seg7x.id === this.sid)
    },
    iSeg7xState: function (): number {
      return this.seg7x?.iSeg7xState ?? -1
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
      if (this.seg7x?.name) return this.geo.center(this.seg7x.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.seg7x?.text5) return this.geo.center(this.seg7x.text5)
      if (this.seg7x?.battery) return this.geo.center(this.seg7x.battery)
      return this.geo.center(this.sid)
    },
    x0clk: function (): number { return this.xdigit(0) - this.height * 0.5 },
    y0clk: function (): number { return this.y - this.height * 0.5 },
    wclk: function (): number { return (this.numDigits() - 1) * this.height },
    hclk: function (): number { return this.height }
  },
  methods: {
    numDigits: function (): number {
      const numDigits_ = Number.parseInt(this.digits, 10)
      if (Number.isNaN(numDigits_)) return 0
      return numDigits_
    },
    xdigit: function (posi: number): number {
      return (this.x - this.height * 0.5 * (this.numDigits() - 1)) + this.height * 1.0 * posi
    },
    valueForDigit: function (digit_: number): string {
      if (digit_ >= this.digitMaxNumber) return ' '
      if (digit_ >= this.numDigits()) return ' '
      if (this.value.length < 1) return ' '
      if (this.value.at(0) === '+') {
        // ---special characters beginning with +---------------
        const aDigits = this.value.split('+')
        if (digit_ < (aDigits.length - 1)) return ('+' + aDigits[digit_ + 1])
        return ' '
      } else {
        // ---number or date or time----------------------------
        let i = 0
        let iDigit = 0
        let iStart = 0
        const len_ = this.value.length
        while (i < len_) {
          // ------for every char in value----------------------
          let c1 = this.value.at(i)
          while ((++i) < len_) {
            c1 = this.value.at(i)
            if (c1 !== '.' && c1 !== ':') break
          }
          const sDigit = this.value.substring(iStart, i)
          if (digit_ === iDigit) return sDigit
          iDigit++
          iStart = i
        } // ------end for every char in value------------------
      }
      return ' '
    },
    onClk: function (): void {
      console.log(this.sid, 'CiSeg7x-Click')
      const topic = 'error/seg7x'
      let payload = '-1'
      if (!this.seg7x) ciSeg7xController.publishCi(topic, payload)
      if (this.seg7x?.pubTopic) {
        const aPubTopic = this.seg7x.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.seg7x?.pubPayload) payload = this.seg7x.pubPayload
          ciSeg7xController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
