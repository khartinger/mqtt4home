<!--CiTextarea.vue---------------------------khartinger----- -->
<!--2022-08-14: New                                          -->
<!--2023-02-05: change at CiBase (add Geo.ts)                -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--convert subPayload into text lines-------------------- -->
  <div>{{ convertPayloadIntoTextLines }}</div>
  <!--color background-------------------------------------- -->
  <rect :fill="colorA_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyi()" />
  <rect :fill="colorH_" :x="geof.x1()" :y="geof.y1()" :width="geof.dxi()" :height="geof.dyl" />
  <rect :fill="colorF_" :x="geof.x1()" :y="geof.y2()-geof.dyl" :width="geof.dxi()" :height="geof.dyl" />
  <!--draw header and footer line--------------------------- -->
  <line v-if="isHeader" :x1="geof.x1()" :y1="geof.y1()+geof.dyl" :x2="geof.x2()" :y2="geof.y1()+geof.dyl" stroke="blue" stroke-width="1"></line>
  <line v-if="isFooter" :x1="geof.x1()" :y1="geof.y2()-geof.dyl" :x2="geof.x2()" :y2="geof.y2()-geof.dyl" stroke="blue" stroke-width="1"></line>
  <!--show lines-------------------------------------------- -->
  <text v-for="index in maxLine" :key="index" class="ciFont1" :x="geof.xt()" :y="_yt(index)">{{_yLine(index)}}</text>
  <!--write text-------------------------------------------- -->
  <text v-if="isHeader" :x="geof.xt()" :y="geof.ytHeader()" class="ciFont1">{{lineHeader}}</text>
  <text v-if="isFooter" :x="geof.xt()" :y="geof.ytFooter()" class="ciFont1">{{lineFooter}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geof.x0()" :y="geof.y0()" :width="geof.dxo()" :height="geof.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Textarea1, ciTextarea1Controller } from '../controller/CiTextarea1Controller'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'

let gLines: string[] = []

export default defineComponent({
  name: 'CiTextarea',
  components: {
    CiBase
  },
  data () {
    return {
      intervalHandle: null as any,
      title: '' as string
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
    }
  },
  computed: {
    // =======standard methods==================================
    textarea1: function (): Textarea1 | undefined {
      return ciTextarea1Controller.textarea1s.find(textarea1 => textarea1.id === this.sid)
    },
    iTextarea1State: function (): number {
      return this.textarea1?.iTextarea1State ?? -99
    },
    // -------provide geometric data----------------------------
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, this.fx, this.fy)
      return geo1
    },
    // -------line number (1...) as int-------------------------
    iLines: function (): number {
      return parseInt(this.lines)
    },
    // -------max number of lines for text area-----------------
    maxLine: function (): number {
      return this.geof.calcLinemax() - this.iLines
    },
    // -------maximal number of characters per line-------------
    maxCharPerLine: function (): number {
      return this.geof.calctmax()
    },
    // =======background colors=================================
    // -------color textarea1 depending on iTextarea1State------
    colorA_: function (): string {
      if (this.iTextarea1State >= 0) return this.iTextarea1State.toString()
      return this.color
    },
    // -------color textarea1 header----------------------------
    colorH_: function (): string {
      return this.colorH
    },
    // -------color textarea1 footer----------------------------
    colorF_: function (): string {
      return this.colorF
    },
    // =======text in line first and last line==================
    // -------write a header line?------------------------------
    isHeader: function (): boolean {
      if (this.iLines > 0) {
        if (this.iTextarea1State === -1 || this.iTextarea1State === -2 ||
         this.iTextarea1State === -4) return true
      }
      return false
    },
    // -------content of header line----------------------------
    lineHeader: function (): string {
      if (this.title.length > 0) return this.title
      if (this.textarea1?.name) return this.geof.center(this.textarea1.name)
      return this.geof.center(this.sid)
    },
    // -------write a footer line?------------------------------
    isFooter: function (): boolean {
      if (this.iLines > 1) return true
      return false
    },
    // -------content of footer line----------------------------
    lineFooter: function (): string {
      if (this.textarea1?.textFooter) return this.geof.center(this.textarea1.textFooter)
      return this.geof.center(this.sid)
    },
    // =======
    // -------convert subPayload into text line array-----------
    // convertPayloadIntoTextLines: function (): Array<string> {
    // eslint-disable-next-line vue/return-in-computed-property
    convertPayloadIntoTextLines: function (): void {
      gLines = [] //                   clear textarea lines
      if (!this.textarea1?.subPayload) return // (unknown) error
      let s1 = this.textarea1.subPayload // received text
      // -----contains subPayload line feed symbols?------------
      // if yes: replace \n symbol(s) by ^~ for splitting lines
      s1 = s1.replaceAll('\\n', '^~') // replace "text" \\n
      s1 = s1.replaceAll('\n', '^~') // replace "text" \n
      let aPay: string[] = [] //       help array for subPayload lines
      aPay = s1.split('^~') //         generate subPayload lines
      if (aPay === null) { //          no \n e.g. ^~ symbols found
        const aPay: string[] = [] //   empty help array
        aPay[0] = s1 //                only one subPayload line
      }
      // =====convert subPayload line(s) to textarea lines======
      const aLines_ = [] //            help array for textarea lines
      let iTxtLine_ = 0 //             index of textarea lines
      let iPayLine_ = 0 //             index of subPayload lines
      // ---convert subPayload lines into textarray lines-------
      const numPayLines = aPay.length // number of subPayload lines
      let aHelp: string[] = [] //      textarea lines for ONE subPayload line
      // .....for every subPayload line.........................
      for (let i1 = iPayLine_; i1 < numPayLines; i1++) {
        if (!aPay[iPayLine_]) break
        // console.log('Payload aPay: ', aPay)
        // ...split ONE subPayload line into textarray lines....
        aHelp = this.splitToArray(aPay[iPayLine_++], this.maxCharPerLine)
        // console.log('aHelp: ', aHelp)
        // ...copy help lines (=1 subPayload line) to textarea lines
        const lenHelp = aHelp.length
        for (let i2 = 0; i2 < lenHelp; i2++) {
          //  should the first line be a title?.................
          if (iTxtLine_ === 0) {
            if (this.textarea1.iTextarea1State === -4) { // title = subPayload line 0
              console.log('New title:', aHelp[0])
              // eslint-disable-next-line vue/no-side-effects-in-computed-properties
              this.title = aHelp[0]
            } else {
              // eslint-disable-next-line vue/no-side-effects-in-computed-properties
              this.title = ''
              aLines_[iTxtLine_++] = aHelp[i2]
            }
          } else {
            aLines_[iTxtLine_++] = aHelp[i2]
          }
          if (iTxtLine_ >= this.maxLine) { // finished. End for loops
            i2 = lenHelp
            i1 = numPayLines
          }
        }
      } // ...END of for every subPayload line..................
      // console.log('END aLines_: ', aLines_)
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      gLines = aLines_
      // return aLines
    },
    // =======MQTT methods======================================
    // -------check mqtt connection to broker-------------------
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    }
  },
  // ---------update text area ervery 30 minutes----------------
  mounted: function (): void {
    this.intervalHandle = setInterval(() => {
      this.onClk()
    }, 1800000) // 1800s = 30min
  },
  unmounted: function (): void {
    clearInterval(this.intervalHandle)
  },
  // ---------react on mqtt connection changes------------------
  watch: {
    isSubscribed: {
      // handler: function (value: boolean, oldValue: boolean) {
      // console.log('State Changed', value, ', ', oldValue)
      handler: function () {
        if (this.textarea1?.clkWhenMounted) {
          this.onClk() // send mqqt text request
        }
      },
      immediate: true
    }
  },
  methods: {
    // -------y position of textarea line "index"---------------
    _yt: function (index: number): number {
      if (this.iLines > 0) return this.geof.yt(1 + index)
      return this.geof.yt(index)
    },
    // -------text in line ...----------------------------------
    _yLine: function (linenumber: number): string {
      return gLines[linenumber - 1]
    },

    // =======text functions part 2=============================
    // _______are the next 1 or 2 (= num_) char single char?____
    isNextCharSingle: function (s1_: string, iEnd_: number, num_: number): boolean {
      if (num_ < 1 || num_ > 3) return false
      const len_ = s1_.length
      if (iEnd_ >= len_) return false
      if (s1_.at(iEnd_) === ' ') return false
      const iEnd1_ = iEnd_ + num_
      if (iEnd1_ === len_) return true
      if (iEnd1_ < len_) {
        if (s1_.at(iEnd1_) === ' ') return true
      }
      return false
    },

    // _______are the previous 1 or 2 (= num_) char single char?
    isPrevCharSingle: function (s1_: string, iEnd_: number, num_: number): boolean {
      if (num_ < 1 || num_ > 3) return false
      const iEnd1_ = iEnd_ - num_
      if (iEnd1_ < 1) return false
      if (s1_.at(iEnd_ - 1) === ' ') return false
      if (s1_.at(iEnd1_ - 1) === ' ') return true
      return false
    },

    // -------split a string to an array of strings with max len
    // Special: (try to) do not split words with 2 char (-> next line)
    // s1: given string
    // maxCharPerLine_: max number of char per element in a1
    splitToArray: function (s1: string, maxCharPerLine_: number): Array<string> {
      const a1: string[] = []
      // ----------check for valid parameters-------------------
      if (maxCharPerLine_ < 1) return a1
      if (this.textarea1?.useRawText || maxCharPerLine_ < 6) {
        const ia1max_ = Math.ceil(s1.length / maxCharPerLine_)
        const s1len_ = s1.length
        let iStart_ = 0
        for (let ia1_ = 0; ia1_ < ia1max_; ia1_++) {
          let iEnd_ = iStart_ + maxCharPerLine_
          if (iEnd_ >= s1len_) iEnd_ = s1len_
          a1[ia1_] = s1.substring(iStart_, iEnd_)
          iStart_ = iEnd_
        }
        return a1
      }
      // ..........remove trailing blanks.......................
      let doloop = true
      let s1len = s1.length
      do {
        if (s1len < 1) return a1
        if (s1.at(s1len - 1) === ' ') s1len--
        else doloop = false
      }
      while (doloop)
      s1 = s1.substring(0, s1len)
      // ----------define variable------------------------------
      const sFill = '          '
      let ia1 = 0 //         Index of line in a1
      let iStart = 0 //      Index of start char in s1 of this line
      doloop = true
      // ----------for each (maxCharPerLine_ long) part in s1---
      do {
        // ........skip leading blanks..........................
        while (s1[iStart] === ' ') {
          iStart++
          if (iStart > s1len) iStart = s1len
        }
        // ........index of next char after this part string....
        let iEnd = iStart + maxCharPerLine_
        if (iEnd >= s1len) {
          // ......this is the last part string (last line).....
          iEnd = s1len
          doloop = false
        }
        // ........special situations while splitting...........
        if (maxCharPerLine_ > 1) {
          if (this.isPrevCharSingle(s1, iEnd, 1) && this.isNextCharSingle(s1, iEnd, 1)) iEnd = iEnd - 1
        }
        if (maxCharPerLine_ > 2) {
          if (this.isPrevCharSingle(s1, iEnd, 1) && this.isNextCharSingle(s1, iEnd, 2)) iEnd = iEnd - 2
          if (this.isPrevCharSingle(s1, iEnd, 2) && this.isNextCharSingle(s1, iEnd, 1)) iEnd = iEnd - 3
        }
        if (maxCharPerLine_ > 3) {
          if (this.isPrevCharSingle(s1, iEnd, 2) && this.isNextCharSingle(s1, iEnd, 2)) iEnd = iEnd - 3
          if (this.isPrevCharSingle(s1, iEnd, 3) && this.isNextCharSingle(s1, iEnd, 1)) iEnd = iEnd - 4
          if (this.isPrevCharSingle(s1, iEnd, 1) && this.isNextCharSingle(s1, iEnd, 3)) iEnd = iEnd - 2
        }
        // ......store line.....................................
        const sPart = s1.substring(iStart, iEnd)
        a1[ia1++] = (sPart + sFill).substring(0, maxCharPerLine_)
        iStart = iEnd //   prepare start of next line
      }
      while (doloop)
      // console.log('splitToArray a1: ', a1)
      return a1
    },

    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      // console.log(this.sid, 'Textarea-Click')
      if (this.textarea1) {
        if (this.textarea1.pubTopic) {
          let payload = '?'
          const aPubTopic = this.textarea1.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach((topic: string) => {
            if (this.textarea1?.pubPayload) payload = this.textarea1.pubPayload
            ciTextarea1Controller.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
