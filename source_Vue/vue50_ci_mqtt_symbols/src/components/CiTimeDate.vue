<!--CiTimeDate.vue-->
<template>
  <!--draw border: : outer and inner rectangle-------------- -->
  <rect v-if="border0" class="ciOut" :x="geo.x0()" :y="geo.y0()" :width="2*geo.dxo" :height="geo.dyo" />
  <rect v-if="border1" class="ciIn"  :x="geo.x1()" :y="geo.y1()" :width="2*geo.dxo-2*geo.dxm" :height="geo.dyi" />

  <rect class="w2Back"  :x="geo.x1()" :y="geo.yr(1)" :width="2*geo.dxo-2*geo.dxm" :height="5*geo.dyl" />
  <text class="w2Font1" :x="xTime" :y="geo.y1() + 2.05*geo.dyl">{{ timeHMS }}</text>
  <text class="w2Font1" :x="xDate" :y="geo.y1() + 4.45*geo.dyl">{{ dateDM }}</text>

<!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { TimeDate, ciTimeDateController } from '../controller/CiTimeDateController'
import { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiTimeDate',
  data () {
    return {
      colorTimeDate: 'yellow',
      intervalHandle: null as any,
      timeHMS: new Date().toLocaleTimeString(),
      dateDM: this.fDateDM()
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
    timeDate: function (): TimeDate | undefined {
      return ciTimeDateController.timeDates.find(timeDate => timeDate.id === this.sid)
    },
    iTimeDateState: function (): number {
      return this.timeDate?.iTimeDateState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    border0: function (): boolean {
      if (this.border > 1) return true
      return false
    },
    border1: function (): boolean {
      if (this.border > 0) return true
      return false
    },
    xTime: function (): number {
      return this.geo.xt() + 1.5 * this.geo.dxm
    },
    xDate: function (): number {
      const len1 = this.fDateDM().length
      if (len1 === 7) return this.geo.xt() + 4.2 * this.geo.dxm
      if (len1 === 8) return this.geo.xt() + 2.1 * this.geo.dxm
      return this.geo.xt() + 0.0 * this.geo.dxm // len=9
    }
  },
  mounted: function (): void {
    this.intervalHandle = setInterval(() => {
      this.timeHMS = new Date().toLocaleTimeString()
      this.dateDM = this.fDateDM()
    }, 1000)
  },
  unmounted: function (): void {
    clearInterval(this.intervalHandle)
  },
  methods: {
    // -------day and date e.g. DO 17.2.------------------------
    fDateDM: function (): string {
      const wochentag = ['SO', 'MO', 'DI', 'MI', 'DO', 'FR', 'SA']
      const now = new Date()
      const s1 = wochentag[now.getDay()] + ' ' + now.getDate() + '.' + (now.getMonth() + 1) + '.'
      // const s1 = 'MO 1.1.'
      return s1
    },
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'TimeDate-Click')
      if (this.timeDate) {
        if (this.timeDate.pubTopic) {
          let payload = '?'
          const aPubTopic = this.timeDate.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.timeDate?.pubPayload) payload = this.timeDate.pubPayload
            ciTimeDateController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
.w2Back {fill: blue; }
.w2Font1 { font-size: 35px; font-weight: bold; fill: yellow; color: lightgreen; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }

</style>
