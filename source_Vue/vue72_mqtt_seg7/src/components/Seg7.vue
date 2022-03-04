<!-- ---------Seg7.vue ----------------------2022-03-03----- -->
<!-- Draw a single seven-segment display                     -->
<template>
  <!-- -------draw segments--------------------------------- -->
  <template v-for="i in 8" :key="i">
    <path :d="pathSeg(i)" :fill="colorSeg(i)" :stroke="strokecolor" :stroke-width="strokewidth" class="cursor" />
  </template>
  <!-- -------draw points----------------------------------- -->
  <circle v-if="isDp1" :cx="cx1" :cy="cy1" :r="cr" :fill="colorSegP"  :stroke="strokecolor" :stroke-width="strokewidth" class="cursor"/>
  <circle v-if="isDp2" :cx="cx2" :cy="cy2" :r="cr" :fill="colorSegP2" :stroke="strokecolor" :stroke-width="strokewidth" class="cursor"/>
  <circle v-if="isDp2" :cx="cx3" :cy="cy3" :r="cr" :fill="colorSegP2" :stroke="strokecolor" :stroke-width="strokewidth" class="cursor"/>
  <!--define click area------------------------------------- -->
  <rect @click="onClick_()" :x="x0click" :y="y0click" :width="wclick" :height="hclick" fill-opacity="0.0" :stroke-width="strokewidth"/>
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'

// -----------convert one digit to segments---------------------
//                   0, 1,  2,  3,   4,   5,   6, 7,   8,   9,   A,   b,  c,  d,   E,   F,  -,  =
const DIGIT2SEGS = [63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 88, 94, 121, 113, 64, 72]

export default defineComponent({
  name: 'Seg7',
  components: {
  },
  data () {
    return {
      height0: 26.2,
      strokecolor: '#cccccc'
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
    value: {
      type: String,
      required: false,
      default: '8:.'
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
    }
  },
  computed: {
    // -------should the dot or the colon be displayed?---------
    isDp1: function (): boolean {
      if (this.value.search('.') > 0) return true
      if (this.value.search(',') > 0) return true
      if (this.digit2seg(this.value) & 0x80) return true
      return false
    },
    isDp2: function (): boolean {
      if (this.value.search(':') > 0) return true
      if (this.digit2seg(this.value) & 0x100) return true
      return false
    },
    // -------circle parameters: center, radius-----------------
    cx1: function (): number {
      const f = this.height / this.height0
      return (this.x + 8.5 * f)
    },
    cy1: function (): number {
      const f = this.height / this.height0
      return (this.y + (13.10 - 1.45) * f)
    },
    cx2: function (): number {
      const f = this.height / this.height0
      return (this.cx1 + (1.6 - 0.7) * f)
    },
    cy2: function (): number {
      const f = this.height / this.height0
      return (this.y + 5 * f)
    },
    cx3: function (): number {
      const f = this.height / this.height0
      return (this.cx1 + (1.6 + 0.7) * f)
    },
    cy3: function (): number {
      const f = this.height / this.height0
      return (this.y - 5 * f)
    },
    cr: function (): number {
      const f = this.height / this.height0
      return (1.45 * f)
    },
    // -------circle parameters: color--------------------------
    colorSegP: function (): string {
      if ((this.digit2seg(this.value) & 0x80) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegP2: function (): string {
      if ((this.digit2seg(this.value) & 0x100) > 0) return this.colorOn
      else return this.colorOff
    },
    // -------Width of segment margin depending on size of display
    strokewidth: function (): number {
      if (this.height < this.height0) return 0
      if (this.height > 5 * this.height0) return 1
      return 0.25 * (this.height / this.height0 - 1)
    },
    // -------click area: start point, extension----------------
    x0click: function (): number { return this.x - this.height * 0.35 },
    y0click: function (): number { return this.y - this.height * 0.5 },
    wclick: function (): number { return 0.84 * this.height },
    hclick: function (): number { return this.height }
  },
  // =========special methods===================================
  methods: {
    // -------convert digit to segments (value 0...511)---------
    digit2seg: function (value: string): number {
      let retSeg = 0
      if (value.length < 1) return retSeg
      if (value.indexOf('.') >= 0) retSeg += 128
      if (value.indexOf(':') >= 0) retSeg += 256
      if (value.at(0) === '+') {
        const numx = Number.parseInt(value, 10)
        if (Number.isNaN(numx)) return 0
        return numx
      }
      const value1 = value.substring(0, 1)
      if (value1 === '-') { return retSeg + DIGIT2SEGS[16] }
      if (value1 === '=') { return retSeg + DIGIT2SEGS[17] }
      const num = Number.parseInt(value1, 16)
      if (!Number.isNaN(num)) {
        if ((num >= 0) && (num < 16)) retSeg += DIGIT2SEGS[num]
      }
      return retSeg
    },
    // -------draw segment 1=a to 7=g (path d value)------------
    pathSeg: function (iSeg: number): string {
      if (iSeg < 1 && iSeg > 7) return ''
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      switch (iSeg) {
        case 1: // segA
          s1 += ' m' + (-2.900 * f) + ',' + (-10.850 * f) + ' l' + (8.750 * f) + ',' + (0.000 * f) + ' l' + (1.870 * f) + ',' + (-1.530 * f) + ' l' + (-0.770 * f) + ',' + (-0.720 * f) + ' l' + (-10.170 * f) + ',' + (0.000 * f) + ' l' + (-1.180 * f) + ',' + (0.850 * f) + ' z'
          break
        case 2: // segB
          s1 += ' m' + (5.110 * f) + ',' + (-1.450 * f) + ' l' + (1.040 * f) + ',' + (1.100 * f) + ' l' + (1.250 * f) + ',' + (-0.900 * f) + ' l' + (1.320 * f) + ',' + (-9.950 * f) + ' l' + (-0.720 * f) + ',' + (-0.850 * f) + ' l' + (-1.600 * f) + ',' + (1.360 * f) + ' z'
          break
        case 3: // segC
          s1 += ' m' + (3.470 * f) + ',' + (10.590 * f) + ' l' + (1.230 * f) + ',' + (1.360 * f) + ' l' + (1.000 * f) + ',' + (-0.700 * f) + ' l' + (1.300 * f) + ',' + (-10.000 * f) + ' l' + (-1.000 * f) + ',' + (-1.030 * f) + ' l' + (-1.250 * f) + ',' + (1.010 * f) + ' z'
          break
        case 4: // seg D
          s1 += ' m' + (-6.900 * f) + ',' + (13.100 * f) + ' l' + (10.120 * f) + ',' + (0.000 * f) + ' l' + (1.180 * f) + ',' + (-0.850 * f) + ' l' + (-1.250 * f) + ',' + (-1.400 * f) + ' l' + (-9.020 * f) + ',' + (0.000 * f) + ' l' + (-1.810 * f) + ',' + (1.400 * f) + ' z'
          break
        case 5: // seg E
          s1 += ' m' + (-8.650 * f) + ',' + (11.150 * f) + ' l' + (0.750 * f) + ',' + (0.800 * f) + ' l' + (1.600 * f) + ',' + (-1.250 * f) + ' l' + (1.340 * f) + ',' + (-9.370 * f) + ' l' + (-1.040 * f) + ',' + (-1.110 * f) + ' l' + (-1.300 * f) + ',' + (1.060 * f) + ' z'
          break
        case 6: // seg F
          s1 += ' m' + (-6.950 * f) + ',' + (-1.400 * f) + ' l' + (0.990 * f) + ',' + (1.050 * f) + ' l' + (1.360 * f) + ',' + (-1.000 * f) + ' l' + (1.300 * f) + ',' + (-9.280 * f) + ' l' + (-1.350 * f) + ',' + (-1.420 * f) + ' l' + (-0.950 * f) + ',' + (0.770 * f) + ' z'
          break
        case 7: // seg G
          s1 += ' m' + (-4.700 * f) + ',' + (1.050 * f) + ' l' + (8.900 * f) + ',' + (0.000 * f) + ' l' + (1.560 * f) + ',' + (-1.100 * f) + ' l' + (-1.010 * f) + ',' + (-1.150 * f) + ' l' + (-8.880 * f) + ',' + (0.000 * f) + ' l' + (-1.570 * f) + ',' + (1.150 * f) + ' z'
          break
        default: break
      }
      return s1
    },
    // -------color of segment 1=a to 7=g-----------------------
    colorSeg: function (iSeg: number): string {
      const mask = 1 << (iSeg - 1)
      if ((this.digit2seg(this.value) & mask) > 0) return this.colorOn
      else return this.colorOff
    },
    // -------react on mouse click------------------------------
    onClick_: function (): void {
      console.log('Ciseg7', 'click!')
    }
  }
})
</script>

<style scoped>
</style>
