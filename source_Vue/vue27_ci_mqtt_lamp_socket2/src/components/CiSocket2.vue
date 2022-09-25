<!--CiSocket2.vue-->
<!--2022-08-07 khartinger-->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border"></CiBase>
  <!--draw symbol------------------------------------------- -->
  <circle :cx="cx" :cy="cy" :r="cr" :fill="colorSocket" />

  <line   :x1="cx-cr" :y1="cy" :x2="cx+cr" :y2="cy" stroke="LightGreen" stroke-width="2" />

  <circle :cx="cx-cr/2" :cy="cy" :r="cr*0.1" :fill="colorSocket" stroke="black" stroke-width="2"/>
  <circle :cx="cx+cr/2" :cy="cy" :r="cr*0.1" :fill="colorSocket" stroke="black" stroke-width="2"/>
  <rect   :x="cx-cr*0.1" :y="cy-cr" :width="cr*0.2" :height="cr*0.2" />
  <rect   :x="cx-cr*0.1" :y="cy+0.8*cr" :width="cr*0.2" :height="cr*0.2" />
  <circle :cx="cx" :cy="cy" :r="cr" fill="none" stroke="black" stroke-width="2" class="cursor" />
  <!---write ON/OFF-text------------------------------------ -->
  <text   :x="cx-cr/4" :y="geo.yt(3)-0.1*cr" class="ciFont1">ON</text>
  <text   :x="cx-cr/3" :y="geo.yt(4)+0.1*cr" class="ciFont1">OFF</text>
  <!--write text-------------------------------------------- -->
  <text v-if="iLines>0" :x="geo.xt()" :y="geo.yt(1)" class="ciFont1">{{title}}</text>
  <text v-if="iLines>1" :x="geo.xt()" :y="geo.yt(5)" class="ciFont1">{{line5}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClkOn()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="cy-geo.y0()" />
  <rect @click="onClkOff()" class="ciClick" :x="geo.x0()" :y="cy" :width="geo.dxo" :height="geo.y0()+geo.dyo-cy" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Socket, ciSocket2Controller } from '../controller/CiSocket2Controller'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiSocket2',
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
    socket: function (): Socket | undefined {
      return ciSocket2Controller.sockets.find(socket => socket.id === this.sid)
    },
    iSocketState: function (): number {
      return this.socket?.iSocketState ?? -1
    },
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
    // -------socket representation-------------------------------
    colorSocket: function (): string {
      if (this.iSocketState === -2) return this.geo.colorOff
      if (this.iSocketState === -1) return this.geo.colorOn
      if (this.iSocketState < 0) return this.geo.colorUnknown
      const s1 = '#' + this.iSocketState.toString(16).padStart(6, '0')
      // console.log('pushsocketColor=', s1)
      return s1
    },
    // -------text in line 1 and 5------------------------------
    title: function (): string {
      if (this.socket?.name) return this.geo.center(this.socket.name)
      return this.geo.center(this.sid)
    },
    line5: function (): string {
      if (this.socket?.text5) return this.geo.center(this.socket.text5)
      if (this.socket?.battery) return this.geo.center(this.socket.battery)
      return this.geo.center(this.sid)
    }

  },
  methods: {
    onClkOn: function (): void {
      console.log(this.sid, 'Socket-ClickOn')
      const topic = 'ci/error/socket'
      let payload = '-1'
      if (!this.socket) ciSocket2Controller.publishCi(topic, payload)
      if (this.socket?.pubTopic) {
        const aPubTopic = this.socket.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.socket?.pubPayloadOn) payload = this.socket.pubPayloadOn
          ciSocket2Controller.publishCi(topic, payload)
        })
      }
    },
    onClkOff: function (): void {
      console.log(this.sid, 'Socket-ClickOff')
      const topic = 'ci/error/socket'
      let payload = '-1'
      if (!this.socket) ciSocket2Controller.publishCi(topic, payload)
      if (this.socket?.pubTopic) {
        const aPubTopic = this.socket.pubTopic.split(' ')
        aPubTopic.forEach(topic => {
          if (this.socket?.pubPayload) payload = this.socket.pubPayload
          ciSocket2Controller.publishCi(topic, payload)
        })
      }
    }

  }
})
</script>

<style scoped>
</style>
