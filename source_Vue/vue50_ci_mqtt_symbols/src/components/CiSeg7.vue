<!-- CiSeg7.vue -----------------------------2022-01-01----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>

  <path :d="segA" :fill="colorSegA" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segB" :fill="colorSegB" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segC" :fill="colorSegC" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segD" :fill="colorSegD" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segE" :fill="colorSegE" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segF" :fill="colorSegF" stroke="black" :stroke-width="strokew" class="cursor" />
  <path :d="segG" :fill="colorSegG" stroke="black" :stroke-width="strokew" class="cursor" />
  <circle v-if="dp1" :cx="cx1" :cy="cy1" :r="cr" :fill="colorSegP" stroke="black" :stroke-width="strokew" class="cursor"/>
  <circle v-if="dp2" :cx="cx2" :cy="cy2" :r="cr" :fill="colorSegP2" stroke="black" :stroke-width="strokew" class="cursor"/>
  <circle v-if="dp2" :cx="cx3" :cy="cy3" :r="cr" :fill="colorSegP2" stroke="black" :stroke-width="strokew" class="cursor"/>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
// import { slotFlagsText } from '@vue/shared'
import { Seg7, ciSeg7Controller } from '../controller/CiSeg7Controller'
import CiBase, { Geo } from './CiBase.vue'

const VALUE2SEGS = [63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 88, 94, 121, 113]

export default defineComponent({
  name: 'CiSeg7',
  components: {
    CiBase
  },
  data () {
    return {
      height0: 20.32
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
      default: 'CiSeg7'
    },
    value: {
      type: String,
      required: false,
      default: '8'
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
    seg7: function (): Seg7 | undefined {
      return ciSeg7Controller.seg7s.find(seg7 => seg7.id === this.sid)
    },
    iSeg7State: function (): number {
      return this.seg7?.iSeg7State ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // -------SevenSegment--------------------------------------
    // ' m' + (-2.900 * f) + ',' + (-10.850 * f) + ' l' + (8.750 * f) + ',' + (0.000 * f) + ' l' + (1.870 * f) + ',' + (-1.530 * f) + ' l' + (-0.770 * f) + ',' + (-0.720 * f) + ' l' + (-10.170 * f) + ',' + (0.000 * f) + ' l' + (-1.180 * f) + ',' + (0.850 * f) + ' z'
    // ' m' + (5.110 * f) + ',' + (-1.450 * f) + ' l' + (1.040 * f) + ',' + (1.100 * f) + ' l' + (1.250 * f) + ',' + (-0.900 * f) + ' l' + (1.320 * f) + ',' + (-9.950 * f) + ' l' + (-0.720 * f) + ',' + (-0.850 *f) + ' l' + (-1.600 * f) + ',' + (1.360 * f) + ' z'
    // ' m' + (3.470 * f) + ',' + (10.590 * f) + ' l' + (1.230 * f) + ',' + (1.360 * f) + ' l' + (1.000 * f) + ',' + (-0.700 * f) + ' l' + (1.300 * f) + ',' + (-10.000 * f) + ' l' + (-1.000 * f) + ',' + (-1.030 *f) + ' l' + (-1.250 * f) + ',' + (1.010 * f) + ' z'
    // ' m' + (-6.900 * f) + ',' + (13.100 * f) + ' l' + (10.120 * f) + ',' + (0.000 * f) + ' l' + (1.180 * f) + ',' + (-0.850 * f) + ' l' + (-1.250 * f) + ',' + (-1.400 * f) + ' l' + (-9.020 * f) + ',' + (0.000 *f) + ' l' + (-1.810 * f) + ',' + (1.400 * f) + ' z'
    // ' m' + (-8.650 * f) + ',' + (11.150 * f) + ' l' + (0.750 * f) + ',' + (0.800 * f) + ' l' + (1.600 * f) + ',' + (-1.250 * f) + ' l' + (1.340 * f) + ',' + (-9.370 * f) + ' l' + (-1.040 * f) + ',' + (-1.110 *f) + ' l' + (-1.300 * f) + ',' + (1.060 * f) + ' z'
    // ' m' + (-6.950 * f) + ',' + (-1.400 * f) + ' l' + (0.990 * f) + ',' + (1.050 * f) + ' l' + (1.360 * f) + ',' + (-1.000 * f) + ' l' + (1.300 * f) + ',' + (-9.280 * f) + ' l' + (-1.350 * f) + ',' + (-1.420 *f) + ' l' + (-0.950 * f) + ',' + (0.770 * f) + ' z'
    // ' m' + (-4.700 * f) + ',' + (1.050 * f) + ' l' + (8.900 * f) + ',' + (0.000 * f) + ' l' + (1.560 * f) + ',' + (-1.100 * f) + ' l' + (-1.010 * f) + ',' + (-1.150 * f) + ' l' + (-8.880 * f) + ',' + (0.000 *f) + ' l' + (-1.570 * f) + ',' + (1.150 * f) + ' z'

    segA: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (-2.900 * f) + ',' + (-10.850 * f) + ' l' + (8.750 * f) + ',' + (0.000 * f) + ' l' + (1.870 * f) + ',' + (-1.530 * f) + ' l' + (-0.770 * f) + ',' + (-0.720 * f) + ' l' + (-10.170 * f) + ',' + (0.000 * f) + ' l' + (-1.180 * f) + ',' + (0.850 * f) + ' z'
      return s1
    },
    segB: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (5.110 * f) + ',' + (-1.450 * f) + ' l' + (1.040 * f) + ',' + (1.100 * f) + ' l' + (1.250 * f) + ',' + (-0.900 * f) + ' l' + (1.320 * f) + ',' + (-9.950 * f) + ' l' + (-0.720 * f) + ',' + (-0.750 * f) + ' l' + (-1.600 * f) + ',' + (1.260 * f) + ' z'
      return s1
    },
    segC: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (3.470 * f) + ',' + (10.590 * f) + ' l' + (1.230 * f) + ',' + (1.360 * f) + ' l' + (1.000 * f) + ',' + (-0.700 * f) + ' l' + (1.300 * f) + ',' + (-10.000 * f) + ' l' + (-1.000 * f) + ',' + (-1.030 * f) + ' l' + (-1.250 * f) + ',' + (1.010 * f) + ' z'
      return s1
    },
    segD: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (-6.900 * f) + ',' + (13.100 * f) + ' l' + (10.120 * f) + ',' + (0.000 * f) + ' l' + (1.180 * f) + ',' + (-0.850 * f) + ' l' + (-1.250 * f) + ',' + (-1.400 * f) + ' l' + (-9.020 * f) + ',' + (0.000 * f) + ' l' + (-1.810 * f) + ',' + (1.400 * f) + ' z'
      return s1
    },
    segE: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (-8.650 * f) + ',' + (11.150 * f) + ' l' + (0.750 * f) + ',' + (0.800 * f) + ' l' + (1.600 * f) + ',' + (-1.250 * f) + ' l' + (1.340 * f) + ',' + (-9.370 * f) + ' l' + (-1.040 * f) + ',' + (-1.110 * f) + ' l' + (-1.300 * f) + ',' + (1.060 * f) + ' z'
      return s1
    },
    segF: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (-6.950 * f) + ',' + (-1.400 * f) + ' l' + (0.990 * f) + ',' + (1.050 * f) + ' l' + (1.360 * f) + ',' + (-1.000 * f) + ' l' + (1.300 * f) + ',' + (-9.280 * f) + ' l' + (-1.350 * f) + ',' + (-1.420 * f) + ' l' + (-0.950 * f) + ',' + (0.770 * f) + ' z'
      return s1
    },
    segG: function (): string {
      const f = this.height / this.height0
      let s1 = 'M' + this.x + ',' + this.y
      s1 += ' m' + (-4.700 * f) + ',' + (1.050 * f) + ' l' + (8.900 * f) + ',' + (0.000 * f) + ' l' + (1.560 * f) + ',' + (-1.100 * f) + ' l' + (-1.010 * f) + ',' + (-1.150 * f) + ' l' + (-8.880 * f) + ',' + (0.000 * f) + ' l' + (-1.570 * f) + ',' + (1.150 * f) + ' z'
      return s1
    },
    dp1: function (): boolean {
      if (this.value.search('.') > 0) return true
      if (this.value.search(',') > 0) return true
      if (this.val2seg(this.value) & 0x80) return true
      return false
    // if (this.value.search(':') > 0) return false
    // return true
    },
    dp2: function (): boolean {
      if (this.value.search(':') > 0) return true
      if (this.val2seg(this.value) & 0x100) return true
      return false
    },
    // -------circle parameters: center, radius-----------------
    cx1: function (): number {
      const f = this.height / this.height0
      // return (this.x + 9.12 * f)
      return (this.x + 9.12 * f)
    },
    cy1: function (): number {
      const f = this.height / this.height0
      return (this.y + (13.10 - 1.45) * f)
    },
    cx2: function (): number {
      const f = this.height / this.height0
      return (this.cx1 + (1.672 - 0.703) * f)
    },
    cy2: function (): number {
      const f = this.height / this.height0
      return (this.y + 5 * f)
    },
    cx3: function (): number {
      const f = this.height / this.height0
      return (this.cx1 + (1.672 + 0.703) * f)
    },
    cy3: function (): number {
      const f = this.height / this.height0
      return (this.y - 5 * f)
    },
    cr: function (): number {
      const f = this.height / this.height0
      return (1.45 * f)
    },
    colorSegA: function (): string {
      if ((this.val2seg(this.value) & 0x01) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegB: function (): string {
      if ((this.val2seg(this.value) & 0x02) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegC: function (): string {
      if ((this.val2seg(this.value) & 0x04) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegD: function (): string {
      if ((this.val2seg(this.value) & 0x08) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegE: function (): string {
      if ((this.val2seg(this.value) & 0x10) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegF: function (): string {
      if ((this.val2seg(this.value) & 0x20) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegG: function (): string {
      if ((this.val2seg(this.value) & 0x40) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegP: function (): string {
      if ((this.val2seg(this.value) & 0x80) > 0) return this.colorOn
      else return this.colorOff
    },
    colorSegP2: function (): string {
      if ((this.val2seg(this.value) & 0x100) > 0) return this.colorOn
      else return this.colorOff
    },
    strokew: function (): number {
      if (this.height < this.height0) return 0
      if (this.height > 5 * this.height0) return 1
      return 0.25 * (this.height / this.height0 - 1)
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.seg7?.name) return this.geo.center(this.seg7.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.seg7?.text5) return this.geo.center(this.seg7.text5)
      if (this.seg7?.battery) return this.geo.center(this.seg7.battery)
      return this.geo.center(this.sid)
    }
  },
  methods: {
    val2seg: function (value: string): number {
      let retSeg = 0
      if (value.length < 1) return retSeg
      if (value.length > 1) {
        if (value.at(1) === '.') retSeg = 128
        if (value.at(1) === ':') retSeg = 256
        if (value.at(0) === '+') {
          const numx = Number.parseInt(value, 10)
          if (Number.isNaN(numx)) return 0
          return numx
        }
      }
      const value1 = value.substring(0, 1)
      const num = Number.parseInt(value1, 16)
      if (!Number.isNaN(num)) {
        if ((num >= 0) && (num < 16)) retSeg += VALUE2SEGS[num]
      }
      return retSeg
    },
    onClk: function (): void {
      console.log(this.sid, 'Seg7-Click')
      const topic = 'error/seg7'
      let payload = '-1'
      if (!this.seg7) ciSeg7Controller.publishCi(topic, payload)
      if (this.seg7?.pubTopic) {
        const aPubTopic = this.seg7.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.seg7?.pubPayload) payload = this.seg7.pubPayload
          ciSeg7Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
