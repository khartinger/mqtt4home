<!--CiButton.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorButton" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="styleM11">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="styleM11">{{line5}}</text>
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
      const geo1 = new Geo()
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    iLines: function (): number {
      return parseInt(this.lines)
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
    // -------button representation-------------------------------
    colorButton: function (): string {
      if (this.iButtonState === -2) return 'grey' //       off
      if (this.iButtonState === -1) return 'lightgreen' // on
      const s1 = '#' + this.iButtonState.toString(16).padStart(6, '0')
      // console.log('pushbuttonColor=', s1)
      return s1
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
