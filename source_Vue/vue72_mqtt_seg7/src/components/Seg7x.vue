<!-- ---------Seg7x.vue ---------------------2022-03-03----- -->
<!-- Seven-segment display with multiple digits (1 to 24)    -->
<template>
  <!-- -------draw digits----------------------------------- -->
  <template v-for="i in numDigits()" :key="i">
    <Seg7 :x="xdigit(i-1)" :y="y" :value="valueForDigit(i-1)" :height="height"></Seg7>
  </template>
  <!--define click area------------------------------------- -->
  <rect @click="onClick_()" :x="x0click" :y="y0click" :width="wclick" :height="hclick" fill-opacity="0.0"/>
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import Seg7 from './Seg7.vue'

export default defineComponent({
  name: 'Seg7x',
  components: {
    Seg7
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
      default: 'Seg7x'
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
    }
  },
  computed: {
    // -------value to be displayed-----------------------------
    value7x: function (): string {
      return this.value
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
    // -------react on mouse click------------------------------
    onClick_: function (): void {
      console.log(this.sid, 'Seg7x-Click')
    }
  }
})
</script>

<style scoped>
</style>
