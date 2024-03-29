<!-- CiLamp.vue -----------------------------2022-01-01----- -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorLamp" />
  <path   :d="lampCross" fill="none" stroke="black" stroke-width="2" class="cursor" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Lamp, ciLampController } from '../controller/CiLampController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiLamp',
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
    lamp: function (): Lamp | undefined {
      return ciLampController.lamps.find(lamp => lamp.id === this.sid)
    },
    iLampState: function (): number {
      return this.lamp?.iLampState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
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
    // -------lamp representation-------------------------------
    colorLamp: function (): string {
      if (this.iLampState === 0) return this.geo.colorOff
      if (this.iLampState === 1) return this.geo.colorOn
      if (this.iLampState === 2) return this.geo.colorNotOk
      return this.geo.colorUnknown
    },
    lampCross: function ():string {
      const dxd2 = Math.round(this.cr / Math.sqrt(2))
      const dxd = 2 * dxd2
      // console.log('cr=' + this.cr + ', dxd2 = ' + dxd2 + ', dxd=' + dxd)
      let s1 = 'M' + this.cx + ',' + this.cy + ' '
      s1 = s1 + 'm-' + dxd2 + ',-' + dxd2 + ' l' + dxd + ',' + dxd + ' '
      s1 = s1 + 'm-' + dxd + ',0 l' + dxd + ',-' + dxd
      return s1
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.lamp?.name) return this.geo.center(this.lamp.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.lamp?.text5) return this.geo.center(this.lamp.text5)
      if (this.lamp?.battery) return this.geo.center(this.lamp.battery)
      return this.geo.center(this.sid)
    }
  },
  methods: {
    onClk: function (): void {
      // console.log(this.sid, 'Lamp-Click')
      // -----just for DEMO: change lamp status on click--------
      if (this.lamp?.iLampState === 1) this.lamp.iLampState = 0
      else {
        if (this.lamp?.iLampState === 0) this.lamp.iLampState = 1
      }
      // -----end DEMO------------------------------------------
      const topic = 'error/lamp'
      let payload = '-1'
      if (!this.lamp) ciLampController.publishCi(topic, payload)
      if (this.lamp?.pubTopic) {
        const aPubTopic = this.lamp.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          // -prepare payload for publishing--------------------
          if (this.lamp?.pubPayload) payload = this.lamp.iLampState.toString()
          console.log('-t ' + topic + '-m ', payload)
          ciLampController.publishCi(topic, payload)
        })
      }
    }
  }
})
</script>

<style scoped>
</style>
