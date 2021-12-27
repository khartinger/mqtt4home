<!--CiText5.vue-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--Draw rectangle 1, if line 1 is a title---------------- -->
  <rect v-if="isRect1" class="ciRect" :x="geo.x1()" :y="geo.yr(1)" :width="geo.dxi" :height="geo.dyl" />
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
import { Text5, ciText5Controller } from '../controller/CiText5Controller'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiText5',
  components: {
    CiBase
  },
  data () {
    return {
      colorText5: 'yellow'
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
    text5: function (): Text5 | undefined {
      return ciText5Controller.text5s.find(text5 => text5.id === this.sid)
    },
    iText5State: function (): number {
      return this.text5?.iText5State ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      geo1.x = this.x
      geo1.y = this.y
      return geo1
    },
    isRect1: function (): boolean {
      if (this.iText5State === 1) return false
      return true
    },
    // -------text in line ...----------------------------------
    line1: function (): string {
      if (this.text5) {
        if (this.text5.lines.length > 0) return this.geo.checkLen(this.text5.lines[0])
        if (this.text5.name) return this.geo.checkLen(this.text5.name)
      }
      return this.geo.checkLen(this.sid)
    },
    lineX: function (): Array<string> {
      const lineValues = []
      if (this.text5) {
        lineValues.push('') // no line 0
        for (let i = 0; i < 5; i++) {
          let s1 = this.text5.lines[i]
          s1 = this.geo.checkLen(s1)
          lineValues.push(s1)
        }
      }
      return lineValues
    },
    // -------color line 2 to 4 depending on iText5State------
    colorState: function (): string {
      return 'none'
    }
  },

  methods: {
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'Text5-Click')
      if (this.text5) {
        if (this.text5.pubTopic) {
          let payload = '?'
          const aPubTopic = this.text5.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach(topic => {
            if (this.text5?.pubPayload) payload = this.text5.pubPayload
            ciText5Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
