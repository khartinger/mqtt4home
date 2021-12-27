<!--CiText5L.vue-->
<template>
  <!--draw border: : outer and inner rectangle-------------- -->
  <rect v-if="border0" class="ciOut" :x="geo.x0()" :y="geo.y0()" :width="2*geo.dxo" :height="geo.dyo" />
  <rect v-if="border1" class="ciIn"  :x="geo.x1()" :y="geo.y1()" :width="2*geo.dxo-2*geo.dxm" :height="geo.dyi" />
  <!--Draw rectangle 1, if line 1 is a title---------------- -->
  <rect v-if="isRect1" class="ciRect" :x="geo.x1()" :y="geo.yr(1)" :width="2*geo.dxo-2*geo.dxm" :height="geo.dyl" />
  <!--write text-------------------------------------------- -->
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{line1}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(2)">{{lineX[2]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(3)">{{lineX[3]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(4)">{{lineX[4]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{lineX[5]}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Text5L, ciText5LController } from '../controller/CiText5LController'
import { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiText5L',
  data () {
    return {
      colorText5L: 'yellow'
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
    text5L: function (): Text5L | undefined {
      return ciText5LController.text5Ls.find(text5L => text5L.id === this.sid)
    },
    iText5LState: function (): number {
      return this.text5L?.iText5LState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    isRect1: function (): boolean {
      if (this.iText5LState === 1) return false
      return true
    },
    // -------text in line ...----------------------------------
    line1: function (): string {
      if (this.text5L) {
        if (this.text5L.lines.length > 0) return this.checkLenL(this.text5L.lines[0])
        if (this.text5L.name) return this.checkLenL(this.text5L.name)
      }
      return this.checkLenL(this.sid)
    },
    lineX: function (): Array<string> {
      const lineValues = []
      if (this.text5L) {
        lineValues.push('') // no line 0
        for (let i = 0; i < 5; i++) {
          let s1 = this.text5L.lines[i]
          s1 = this.checkLenL(s1)
          lineValues.push(s1)
        }
      }
      return lineValues
    },
    // -------color line 2 to 4 depending on iText5LState------
    colorState: function (): string {
      return 'none'
    },
    border0: function (): boolean {
      if (this.border > 1) return true
      return false
    },
    border1: function (): boolean {
      if (this.border > 0) return true
      return false
    }
  },

  methods: {
    // -------check text length---------------------------------
    checkLenL: function (text: string): string {
      const charmax = 2 * this.geo.tmax + 2
      if (text) {
        if (text.length > charmax) return text.substr(0, charmax)
        return text
      }
      return ''
    },
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'Text5L-Click')
      if (this.text5L) {
        if (this.text5L.pubTopic) {
          let payload = '?'
          const aPubTopic = this.text5L.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.text5L?.pubPayload) payload = this.text5L.pubPayload
            ciText5LController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
