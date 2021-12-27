<!--CiMotion.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <rect class="ciRect"   :x="geo.xr()" :y="geo.yr(1)" :width="geo.dxi" :height="geo.dyl" />
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(2)" :width="geo.dxi" :height="3 * geo.dyl" :fill="colorState"/>
  <rect class="ciFillIt" :x="geo.xr()" :y="geo.yr(5)" :width="geo.dxi" :height="geo.dyl" :fill="colorBattery"/>
  <rect class="ciRect"   :x="geo.xr()" :y="geo.yr(2)" :width="geo.dxi" :height="geo.dyl" />
  <line class="ciLine" :x1="geo.xr() + geo.dxi2" :y1="geo.yr(2)" :x2="geo.xr() + geo.dxi2" :y2="geo.yr(2) + geo.dyl" />
  <!--write text-------------------------------------------- -->
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
  <text class="ciFont1" :x="geo.xt()+0.3*geo.dxi2" :y="geo.yt(2)">))</text>
  <text class="ciFont1" :x="geo.xt2()+0.3*geo.dxi2" :y="geo.yt(2)">{{ iMotionState }}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(3)" >{{line3}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(4)">{{line4}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Motion, ciMotionController } from '../controller/CiMotionController'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiMotion',
  components: {
    CiBase
  },
  data () {
    return {
      colorMotion: 'yellow',
      colorNoMotion: 'lightgrey'
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
      required: true
    }
  },
  computed: {
    motion: function (): Motion | undefined {
      return ciMotionController.motions.find(motion => motion.id === this.sid)
    },
    iMotionState: function (): number {
      return this.motion?.iMotionState ?? -1
    },
    lastMotionDate: function (): string {
      if (this.motion) {
        if (this.iMotionState >= 0) return this.motion?.lastMotionDate ?? this.geo.noDate
      }
      return this.geo.noDate
    },
    lastMotionTime: function (): string {
      if (this.motion) {
        if (this.iMotionState >= 0) return this.motion?.lastMotionTime ?? this.geo.noTime
      }
      return this.geo.noTime
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    // -------text in line ...----------------------------------
    title: function (): string {
      if (this.motion?.name) return this.geo.center(this.motion.name)
      return this.geo.center(this.sid)
    },
    line3: function (): string {
      if (this.iMotionState < 0) return this.geo.center('unknown')
      return this.geo.center(this.lastMotionDate)
    },
    line4: function (): string {
      if (this.iMotionState < 0) return ''
      return this.geo.center(this.lastMotionTime)
    },
    line5: function (): string {
      if (this.motion?.battery) return this.geo.center(this.motion.battery.toString())
      return ''
    },
    // -------color line 2 to 4 depending on iMotionState------
    colorState: function (): string {
      // console.log('colorState: ', 'iMotionState=' + this.iMotionState)
      if (this.iMotionState === 0) return this.colorNoMotion
      if (this.iMotionState === 1) return this.colorMotion
      return this.geo.colorUnknown
      // return 'none'
    },

    // -------color of rectangle 5 depending on battery value---
    colorBattery: function (): string {
      if (this.motion?.battery) {
        try {
          const batt = parseInt(this.geo.center(this.motion.battery).valueOf())
          if (batt > this.geo.batteryMin) return this.geo.colorOk
          return this.geo.colorNotOk
        } catch (error) { return 'none' }
      }
      return this.geo.colorUnknown
      // return 'none'
    }
  },

  methods: {
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'Motion-Click')
      if (this.motion) {
        if (this.motion.pubTopic) {
          let payload = '?'
          const aPubTopic = this.motion.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.motion?.pubPayload) payload = this.motion.pubPayload
            ciMotionController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
