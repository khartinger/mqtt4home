<!-- CiButton.vue ---------------------------2022-01-01----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <rect :x="Rx0" :y="Ry0" :rx="Rrx" :ry="Rry" :width="Rw" :height="Rh" :fill="colorButton" stroke="black" stroke-width="2" class="cursor" />
  <!--draw extra symbol (shape)----------------------------- -->
  <path v-if="isShape" :d="drawShape" fill="none" stroke="black" stroke-width="1"/>
  <!--write center text------------------------------------- -->
  <text v-if="line3.length>0" :x="geo.xt()" :y="dy + geo.yt(3)" class="ciFont1">{{line3}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Button, ciButtonController } from '../controller/CiButtonController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiButton',
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
    }
  },
  computed: {
    button: function (): Button | undefined {
      return ciButtonController.buttons.find(button => button.id === this.sid)
    },
    iButtonState: function (): number {
      return this.button?.iButtonState ?? -1
    },
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
    },
    isShape: function (): boolean {
      if (this.button?.shape) {
        if (this.button.shape.length > 0) return true
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
      const i = 0 + this.iLines
      if (i === 0) return (this.geo.dyi / 2 - 1)
      if (i === 2) return (this.geo.dyi * 3 / 10 - 1)
      return (2 * this.geo.dyl - 1)
    },
    // -------rectangle parameters: up left, ...----------------
    Rx0: function (): number { return (this.cx - this.cr) },
    Ry0: function (): number { return (this.cy - this.cr) },
    Rrx: function (): number {
      if (!this.button?.shape) return this.cr / 10
      const sym = this.button.shape.toLowerCase()
      if ((sym.indexOf('circle') >= 0) || (sym.indexOf('round') >= 0)) return this.cr
      return (this.cr / 10)
    },
    Rry: function (): number {
      return this.Rrx
    },
    Rw: function (): number {
      return 2 * this.cr
    },
    Rh: function (): number {
      return 2 * this.cr
    },
    // -------center text position depending on number of lines_
    dy: function (): number {
      if (this.iLines === 1) return this.geo.dyl / 2
      return 0
    },
    // -------button representation-------------------------------
    colorButton: function (): string {
      if (this.button?.color) return this.button.color
      if (this.iButtonState === -2) return this.geo.colorOff
      if (this.iButtonState === -1) return this.geo.colorOn
      if (this.iButtonState <= 0) return this.geo.colorUnknown
      return this.geo.colorError
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.button?.name) return this.geo.center(this.button.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.button?.text5) return this.geo.center(this.button.text5)
      if (this.button?.battery) return this.geo.center(this.button.battery)
      return this.geo.center(this.sid)
    },
    // -------symbol text in line 3-----------------------------
    line3: function (): string {
      if (this.button?.text3) return this.geo.center(this.button.text3)
      return ''
    },
    drawShape: function (): string {
      if (!this.button?.shape) return ''
      if (this.button.shape.length < 1) return ''
      let s1 = ''
      const sym = this.button.shape.toLowerCase()
      const cR = 0.8 * this.cr //           radius of perimeter
      const ca = cR * Math.sqrt(3) //       Side length of triangle
      s1 = 'M' + this.cx + ',' + this.cy
      if (sym.indexOf('up') >= 0) {
        s1 += 'm0,-' + cR + ' l-' + (0.5 * ca) + ',' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('down') >= 0) {
        s1 += 'm0,' + cR + ' l-' + (0.5 * ca) + ',-' + (1.5 * cR) + ' h' + ca + ' z'
      }
      if (sym.indexOf('left') >= 0) {
        s1 += 'm-' + cR + ',0 l' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      if (sym.indexOf('right') >= 0) {
        s1 += 'm' + cR + ',0 l-' + (1.5 * cR) + ',-' + (0.5 * ca) + ' v' + ca + ' z'
      }
      return s1
    }
  },
  methods: {
    onClk: function (): void {
      console.log(this.sid, 'Button-Click')
      const topic = 'ci/error/button'
      let payload = '-1'
      if (!this.button) ciButtonController.publishCi(topic, payload)
      if (this.button?.pubTopic) {
        const aPubTopic = this.button.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.button?.pubPayload) payload = this.button.pubPayload
          ciButtonController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
