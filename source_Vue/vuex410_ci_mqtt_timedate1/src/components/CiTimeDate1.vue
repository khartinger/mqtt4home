<!--CiTimeDate1.vue--------------------------khartinger----- -->
<!-- 2023-02-25: new                                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--color background-------------------------------------- -->
  <rect :fill="colorA_" :x="geof.x1()" :y="geof.y1()+geof.dyl" :width="geof.dxi()" :height="geof.dyi()-2*geof.dyl" />
  <rect :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw header and footer line--------------------------- -->
  <line v-if="isHeader" :x1="geof.x1()" :y1="geof.y1()+geof.dyl" :x2="geof.x2()" :y2="geof.y1()+geof.dyl" stroke="blue" stroke-width="1"></line>
  <line v-if="isFooter" :x1="geof.x1()" :y1="geof.y2()-geof.dyl" :x2="geof.x2()" :y2="geof.y2()-geof.dyl" stroke="blue" stroke-width="1"></line>
  <!--show lines-------------------------------------------- -->
  <text v-for="index in maxLine" :key="index" :class="textfontA_" :x="geof.xt()" :y="_yt(index)">{{_yLine(index)}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="isHeader" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="isFooter" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { TimeDate1, ciTimeDate1Controller } from '../controller/CiTimeDate1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'

export default defineComponent({
  name: 'CiTextarea',
  components: {
    CiBase
  },
  data () {
    return {
      intervalHandle: null as any,
      gLines: [] as string[]
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
    sid: {
      type: String,
      required: true
    },
    border: {
      type: Number,
      required: false,
      default: 2
    },
    lines: {
      type: String,
      required: false,
      default: '1'
    },
    fx: {
      type: Number,
      required: false,
      default: 1
    },
    fy: {
      type: Number,
      required: false,
      default: 1
    },
    color: {
      type: String,
      required: false,
      default: 'none'
    },
    colorH: {
      type: String,
      required: false,
      default: 'none'
    },
    colorF: {
      type: String,
      required: false,
      default: 'none'
    },
    textsize: {
      type: Number,
      required: false,
      default: 1
    },
    textcenter: {
      type: Boolean,
      required: false,
      default: false
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find timedate1 object to given sid________________
    timedate1: function (): TimeDate1 | undefined {
      return ciTimeDate1Controller.timedate1s.find(timedate1 => timedate1.id === this.sid)
    },
    // _______get iTimeDate1State_______________________________
    iTimeDate1State: function (): number {
      return this.timedate1?.iTimeDate1State ?? -99
    },
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // _______number of displayed lines_________________________
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // =======other methods=====================================
    // _______max number of lines for text area_________________
    maxLine: function (): number {
      const iTemp = Math.trunc((this.geof.calcLinemax() - this.iLines) / this.iTextsize)
      return iTemp
    },
    // _______maximal number of characters per line_____________
    maxCharPerLine: function (): number {
      const iTemp = Math.trunc(this.geof.calctmax() / this.iTextsize)
      return iTemp
    },
    // _______textsize (1 = default or 1.5 | 2 | 3)_____________
    iTextsize: function (): number {
      try {
        const i1 = this.textsize // parseInt(this.textsize)
        if (i1 === 1) return 1
        if (i1 === 1.5 || i1 === 2 || i1 === 2.5 ||
          i1 === 3 || i1 === 4 || i1 === 5) return i1
        return 1
      } catch (error) { return 1 }
    },
    // _______textfont (textsize), defined in CiBase.vue________
    textfontA_: function (): string {
      if (this.iTextsize === 1) return 'ciFont1'
      if (this.iTextsize === 1.5) return 'ciFont1_5'
      if (this.iTextsize === 2) return 'ciFont2'
      if (this.iTextsize === 2.5) return 'ciFont2_5'
      if (this.iTextsize === 3) return 'ciFont3'
      if (this.iTextsize === 4) return 'ciFont4'
      if (this.iTextsize === 5) return 'ciFont5'
      return 'ciFont1'
    },
    // =======background colors=================================
    // _______color timedate1 depending on iTimeDate1State______
    colorA_: function (): string {
      if (this.iTimeDate1State >= 0) return this.iTimeDate1State.toString()
      return this.color
    },
    // _______color timedate1 header____________________________
    colorH_: function (): string {
      if (this.iLines < 1) return this.colorA_
      return this.colorH
    },
    // _______color timedate1 footer____________________________
    colorF_: function (): string {
      if (this.iLines < 2) return this.colorA_
      return this.colorF
    },
    // =======text in line first and last line==================
    // _______write a header line?______________________________
    isHeader: function (): boolean {
      if (this.iLines > 0) {
        return true
      }
      return false
    },
    // _______content of header line____________________________
    lineHeader: function (): string {
      // if (this.title.length > 0) return this.title
      if (this.timedate1?.name) return this.geof.center(this.timedate1.name)
      return this.geof.center(this.sid)
    },
    // _______write a footer line?______________________________
    isFooter: function (): boolean {
      if (this.iLines > 1) return true
      return false
    },
    // _______content of footer line____________________________
    lineFooter: function (): string {
      if (this.timedate1?.textFooter) return this.geof.center(this.timedate1.textFooter)
      return this.geof.center(this.sid)
    },
    // =======MQTT methods======================================
    // _______check mqtt connection to broker___________________
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    }
  },
  // _________update text area ervery 30 minutes________________
  mounted: function (): void {
    this.intervalHandle = setInterval(() => {
      this.gLines[0] = new Date().toLocaleTimeString()
      this.gLines[1] = this.fDateDM()
    }, 1000)
  },
  unmounted: function (): void {
    clearInterval(this.intervalHandle)
  },
  methods: {
    // _______y position of textarea line "index" 1...__________
    _yt: function (index: number): number {
      const dy = (this.iTextsize - 1) * (this.geof.dyl * (index - 0.2))
      if (this.iLines > 0) return this.geof.yt(1 + index) + dy
      return this.geof.yt(index) + dy
    },
    // _______get text for line number 1 ...____________________
    _yLine: function (linenumber: number): string {
      const len_ = this.gLines.length
      if (len_ < 1 || linenumber < 1) return ''
      if (linenumber > len_) return ''
      const s1 = `${this.gLines[linenumber - 1]}`
      if (s1.length < 1) return ''
      if (this.textcenter) return this.centerText(s1)
      return s1
      // return this.gLines[linenumber - 1]
    },
    // _______center Text using fx, textsize____________________
    centerText: function (text: string): string {
      const len = text.length
      if (len < 1) return ''
      const tmax_ = this.maxCharPerLine
      if (len >= tmax_) return text.substring(0, tmax_)
      const numBlank = Math.round((tmax_ - len - 1) / 2)
      // console.log('center: text=', '|' + text + '| numBlank=' + numBlank)
      const s1 = text.padStart(numBlank + len, ' ')
      return s1
    },
    // -------day and date e.g. DO 17.2.------------------------
    fDateDM: function (): string {
      const wochentag = ['SO', 'MO', 'DI', 'MI', 'DO', 'FR', 'SA']
      const now = new Date()
      const s1 = wochentag[now.getDay()] + ' ' + now.getDate() + '.' + (now.getMonth() + 1) + '.'
      return s1
    },

    // _______react on click: publish a message (if any exists)-
    onClk: function (): void {
      // console.log(this.sid, 'Textarea-Click')
      if (this.timedate1) {
        if (this.timedate1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.timedate1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach((topic: string) => {
            if (this.timedate1?.pubPayload) payload = this.timedate1.pubPayload
            ciTimeDate1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
