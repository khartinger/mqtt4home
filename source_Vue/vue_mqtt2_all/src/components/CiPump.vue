<!--CiPump.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <rect class="ciRect"   :x="geo.xr()" :y="geo.yr(1)" :width="geo.dxi" :height="geo.dyl" />
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(2)" :width="geo.dxi" :height="3 * geo.dyl" :fill="colorState"/>
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorState"/>
  <rect class="ciRect"   :x="geo.xr()" :y="geo.yr(2)" :width="geo.dxi" :height="geo.dyl" />
  <line class="ciLine" :x1="geo.xr() + geo.dxi2" :y1="geo.yr(2)" :x2="geo.xr() + geo.dxi2" :y2="geo.yr(2) + geo.dyl" />
  <!--draw pump symbol-------------------------------------- -->
  <circle class="ciFillIt" :cx="cx" :cy="cy" :r="cr" :fill="colorState"/>
  <line class="ciLine" :x1="cx" :y1="cy - cr" :x2="cx + cr" :y2="cy" />
  <line class="ciLine" :x1="cx" :y1="cy + cr" :x2="cx + cr" :y2="cy" />
  <!--write text-------------------------------------------- -->
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
  <text class="ciFont1" :x="geo.xt2()+0.3*geo.dxi2" :y="geo.yt(2)">{{ iPumpState }}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(3)" >{{line3}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(4)">{{line4}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Pump, ciPumpController } from '../controller/CiPumpController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiPump',
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
    }
  },
  computed: {
    pump: function (): Pump | undefined {
      return ciPumpController.pumps.find(pump => pump.id === this.sid)
    },
    iPumpState: function (): number {
      return this.pump?.iPumpState ?? -1
    },
    lastPumpDate: function (): string {
      if (this.pump) {
        if (this.iPumpState >= 0) return this.pump?.lastPumpDate ?? this.geo.noDate
      }
      return this.geo.noDate
    },
    lastPumpTime: function (): string {
      if (this.pump) {
        if (this.iPumpState >= 0) return this.pump?.lastPumpTime ?? this.geo.noTime
      }
      return this.geo.noTime
    },
    sStatus: function (): string {
      return this.pump?.sStatus ?? '??'
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    // -------draw pump symbol----------------------------------
    cx: function (): number { return (this.x - Math.round(this.geo.dxi2 / 2)) },
    cy: function (): number { return (this.y - this.geo.dyl) },
    cr: function (): number { return (Math.round(this.geo.dyl / 2 - 0.5) - 1) },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.pump?.name) return this.geo.center(this.pump.name)
      return this.geo.center(this.sid)
    },
    line3: function (): string {
      if (this.iPumpState < 0) return this.geo.center('unknown')
      return this.geo.center(this.lastPumpDate)
    },
    line4: function (): string {
      if (this.iPumpState < 0) return ''
      return this.geo.center(this.lastPumpTime)
    },
    line5: function (): string {
      if (this.pump) {
        return this.geo.center(this.sStatus)
      }
      return this.geo.center('STATE unknown')
    },
    // -------color line 2 to 4 depending on iPumpState------
    colorState: function (): string {
      // console.log('colorState: ', 'iPumpState=' + this.iPumpState)
      if (this.iPumpState === 0) return this.geo.colorOff
      if (this.iPumpState === 1) return this.geo.colorOn
      return this.geo.colorUnknown
    }
    /*
    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.pump?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.pump.battery).valueOf())
          if (batt > this.geo.batteryMin) return this.geo.colorOk
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return 'none'
    }
    */
  },

  methods: {
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'Pump-Click')
      if (this.pump) {
        if (this.pump.pubTopic) {
          let payload = '?'
          const aPubTopic = this.pump.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.pump?.pubPayload) payload = this.pump.pubPayload
            ciPumpController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
