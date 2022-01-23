<!--CiSmsOut.vue-->
<template>
  <!--draw border: : outer and inner rectangle-------------- -->
  <rect v-if="border0" class="ciOut" :x="geo.x0()" :y="geo.y0()" :width="2*geo.dxo" :height="geo.dyo" />
  <rect v-if="border1" class="ciIn"  :x="geo.x1()" :y="geo.y1()" :width="2*geo.dxo-2*geo.dxm" :height="geo.dyi" />
  <!--Draw rectangle 1, if line 1 is a title---------------- -->
  <rect class="ciRect1" :x="geo.x1()" :y="geo.yr(1)" :width="2*geo.dxo-2*geo.dxm" :height="2*geo.dyl" />
  <rect :x="geo.x1()" :y="geo.yr(1)" :width="6.5*geo.dxo/geo.tmax" :height="geo.dyl" :fill="colorState" stroke="blue" />
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">SMS-OUT</text>
  <!--write text-------------------------------------------- -->
  <text class="ciFont1" :x="geo.xt()+6.6*geo.dxo/geo.tmax" :y="geo.yt(1)">{{line1}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(2)">{{lineX[2]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(3)">{{lineX[3]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(4)">{{lineX[4]}}</text>
  <text class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{lineX[5]}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { SmsOut, ciSmsOutController } from '../controller/CiSmsOutController'
import { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiSmsOut',
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
    smsOut: function (): SmsOut | undefined {
      return ciSmsOutController.smsOuts.find(smsOut => smsOut.id === this.sid)
    },
    iSmsOutState: function (): number {
      return this.smsOut?.iSmsOutState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    isRect1: function (): boolean {
      if (this.iSmsOutState === 1) return false
      return true
    },
    // -------text in line ...----------------------------------
    line1: function (): string {
      if (this.smsOut) {
        if (this.smsOut.lines.length > 0) return this.checkLenL(this.smsOut.lines[0])
        if (this.smsOut.name) return this.checkLenL(this.smsOut.name)
      }
      return this.checkLenL(this.sid)
    },
    lineX: function (): Array<string> {
      const lineValues = []
      if (this.smsOut) {
        lineValues.push('') // no line 0
        for (let i = 0; i < 5; i++) {
          let s1 = this.smsOut.lines[i]
          s1 = this.checkLenL(s1)
          lineValues.push(s1)
        }
      }
      return lineValues
    },
    // -------color line 2 to 4 depending on iSmsOutState------
    colorState: function (): string {
      if (this.iSmsOutState === 0) return this.geo.colorError // red
      if (this.iSmsOutState === 1) return this.geo.colorNotOk // light red
      if (this.iSmsOutState === 2) return this.geo.colorOn // yellow
      if (this.iSmsOutState === 3) return this.geo.colorOk // light green 2
      return this.geo.colorUnknown
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
    getNow: function (): string {
      const now = new Date()
      let day = `${now.getDate()}`
      if (day.length < 2) day = '0' + day
      let month = `${(1 + now.getMonth())}`
      if (month.length < 2) month = '0' + month
      let year = `${now.getFullYear()}`
      if (year.length < 2) year = '0' + year
      let hour = `${now.getHours()}`
      if (hour.length < 2) hour = '0' + hour
      let min = `${now.getMinutes()}`
      if (min.length < 2) min = '0' + min
      let sec = `${now.getSeconds()}`
      if (sec.length < 2) sec = '0' + sec
      const date = day + '.' + month + '.' + year
      // const date = year + '-' + month + '-' + day
      const time = hour + ':' + min + ':' + sec
      return date + ' ' + time
    },
    // -------check text length---------------------------------
    checkLenL: function (text: string): string {
      const charmax = 2 * this.geo.tmax + 2
      if (text) {
        if (text.length > charmax) return text.substring(0, charmax)
        return text
      }
      return ''
    },
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      console.log(this.sid, 'SmsOut-Click')
      const s0 = `${prompt('Enter phone number, space and SMS text:', '+43680... sms-text')}`
      if (s0.length > 0) {
        if (this.smsOut) this.smsOut.iSmsOutState = 0 // colorError
        const posBlank = s0.indexOf(' ')
        if (posBlank > 0) {
          const smsPhone = s0.substring(0, posBlank)
          const smsText = s0.substring(posBlank + 1)
          // console.log('phone=|' + smsPhone + '| smstext=|' + smsText + '|')
          // -puplish message to send sms-----------------------
          if (this.smsOut) {
            if (this.smsOut.pubTopic) {
              let payload = s0
              const aPubTopic = this.smsOut.pubTopic.split(' ')
              // -publish all given topics----------------------
              aPubTopic.forEach(topic => {
                if (this.smsOut?.pubPayload) payload = this.smsOut.pubPayload
                ciSmsOutController.publishCi(topic, payload)
              })
              this.smsOut.iSmsOutState = 2 // sms sending
            }
            // -copy input to vue symbol--------------------------
            this.smsOut.lines[0] = smsPhone
            this.smsOut.lines[1] = this.getNow()
            const iLine = 2
            const max = 2 * this.geo.tmax
            let iStart = 0
            const s1 = smsText
            for (let iL = iLine; iL < 5; iL++) {
              let iEnd = iStart + max
              if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
              else {
                if (s1.at(iEnd) === ' ') iEnd++
              }
              this.smsOut.lines[iL] = s1.substring(iStart, iEnd)
              iStart = iEnd
            }
          } // END this.smsOut
        } // END split input text
      } // END s1.length
    }
  }
})
</script>

<style scoped>
  .ciRect1   { fill: white; stroke: blue; stroke-width: 1; }
  .ciRect2   { fill: yellow; stroke: blue; stroke-width: 1; }
</style>
