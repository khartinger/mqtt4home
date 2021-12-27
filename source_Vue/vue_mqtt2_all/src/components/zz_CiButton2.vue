<!--CiButton2.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorButton2" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />
  <!--draw extra symbol or text in the middle--------------- -->
  <path v-if="isSymbol" :d="drawSymbol" fill="none" stroke="black" stroke-width="1"/>
  <text v-if="symbol.length>0" :x="geo.xt()" :y="geo.yt(3)" class="ciFont1">{{line3}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Button2, ciButton2Controller } from '../controller/CiButton2Controller'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiButton2',
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
    border: {
      type: Number,
      required: false,
      default: 2
    },
    sid: {
      type: String,
      required: true
    },
    lines: {
      type: String,
      required: false,
      default: '1'
    },
    symbol: {
      type: String,
      required: false,
      default: ''
    }
  },
  computed: {
    button2: function (): Button2 | undefined {
      return ciButton2Controller.button2s.find(button2 => button2.id === this.sid)
    },
    iButton2State: function (): number {
      return this.button2?.iButton2State ?? -1
    },
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    isSymbol: function (): boolean {
      if (this.symbol.length > 0) {
        if (this.symbol.at(0) === '_') return true
      }
      return false
    },

    // -------circle parameters: center, radius-----------------
    cx: function (): number { return (this.x) },
    cy: function (): number {
      if (this.iLines === 0) return (this.y)
      if (this.iLines === 2) return (this.y)
      return (this.y + this.geo.dyl / 2)
    },
    cr: function (): number {
      if (this.iLines === 0) return (this.geo.dyi / 2 - 1)
      if (this.iLines === 2) return (this.geo.dyi * 3 / 10 - 1)
      return (2 * this.geo.dyl - 1)
    },
    // -------button2 representation------------------------------
    colorButton2: function (): string {
      if (this.iButton2State === -2) return this.geo.colorOff
      if (this.iButton2State === -1) return this.geo.colorOn
      if (this.iButton2State < 0) return this.geo.colorUnknown
      const s1 = '#' + this.iButton2State.toString(16).padStart(6, '0')
      // console.log('pushbutton2Color=', s1)
      return s1
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.button2?.name) return this.geo.center(this.button2.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.button2?.text5) return this.geo.center(this.button2.text5)
      if (this.button2?.battery) return this.geo.center(this.button2.battery)
      return this.geo.center(this.sid)
    },
    // -------symbol text in line 3-----------------------------
    line3: function (): string {
      if (this.symbol.length > 0) {
        if (this.symbol.at(0) !== '_') return this.geo.center(this.symbol)
      }
      return ''
    },
    drawSymbol: function (): string {
      if (this.symbol.length > 0) {
        if (this.symbol.at(0) !== '_') return ''
      }
      let s1 = ''
      const sym = this.symbol.toLowerCase()
      const cR = 0.8 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      s1 = 'M' + this.cx + ',' + this.cy
      if (sym.indexOf('up') === 1) {
        s1 += 'm0,-' + cR + ' l-' + (0.5 * ca) + ',' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('down') === 1) {
        s1 += 'm0,' + cR + ' l-' + (0.5 * ca) + ',-' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('left') === 1) {
        s1 += 'm-' + cR + ',0 l' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      if (sym.indexOf('right') === 1) {
        s1 += 'm' + cR + ',0 l-' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      return s1
    }
  },
  methods: {
    onClk: function (): void {
      console.log(this.sid, 'Button2-Click')
      const topic = 'ci/error/button2'
      let payload = '-1'
      if (!this.button2) ciButton2Controller.publishCi(topic, payload)
      if (this.button2?.pubTopic) {
        const aPubTopic = this.button2.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button2?.pubPayload) payload = this.button2.pubPayload
          ciButton2Controller.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
