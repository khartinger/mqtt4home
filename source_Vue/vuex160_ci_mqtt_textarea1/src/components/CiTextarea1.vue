<!--CiTextarea.vue---------------------------khartinger----- -->
<!--2022-08-14 New                                           -->
<!--2023-02-05 change at CiBase (add Geo.ts)                 -->
<!--2023-02-23 replace .at() by []                           -->
<!--2023-02-24 add textsize                                  -->

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
  <text v-for="index in maxLine" :key="index" :class="textfontA_" :x="geof.xt()" :y="_yt(index)">{{_yLine(index)}}</text>
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
    }
  },
  computed: {
    // =======standard methods==================================
    // _______find textarea1 object to given sid________________
    textarea1: function (): Textarea1 | undefined {
      return ciTextarea1Controller.textarea1s.find(textarea1 => textarea1.id === this.sid)
    },
    // _______get iTextarea1State_______________________________
    iTextarea1State: function (): number {
      return this.textarea1?.iTextarea1State ?? -99
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
      // const iTemp = ((this.geof.calcLinemax() - this.iLines) / this.iTextsize)
      return iTemp
    },
    // _______maximal number of characters per line_____________
    maxCharPerLine: function (): number {
      const iTemp = Math.trunc(this.geof.calctmax() / this.iTextsize)
      // const iTemp = (this.geof.calctmax() / this.iTextsize)
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
    // _______color textarea1 depending on iTextarea1State______
    colorA_: function (): string {
      if (this.iTextarea1State >= 0) return this.iTextarea1State.toString()
      return this.color
    },
    // _______color textarea1 header____________________________
    colorH_: function (): string {
      if (this.iLines < 1) return this.colorA_
      return this.colorH
    },
    // _______color textarea1 footer____________________________
    colorF_: function (): string {
      if (this.iLines < 2) return this.colorA_
      return this.colorF
    },
    // =======text in line first and last line==================
    // _______write a header line?______________________________
    isHeader: function (): boolean {
      if (this.iLines > 0) {
        if (this.iTextarea1State === -1 || this.iTextarea1State === -2 ||
         this.iTextarea1State === -4) return true
      }
      return false
    },
    // _______content of header line____________________________
    lineHeader: function (): string {
      if (this.title.length > 0) return this.title
      if (this.textarea1?.name) return this.geof.center(this.textarea1.name)
      return this.geof.center(this.sid)
    },
    // _______write a footer line?______________________________
    isFooter: function (): boolean {
      if (this.iLines > 1) return true
      return false
    },
    // _______content of footer line____________________________
    lineFooter: function (): string {
      if (this.textarea1?.textFooter) return this.geof.center(this.textarea1.textFooter)
      return this.geof.center(this.sid)
    },
    // =======text specific methods=============================
    // _______convert subPayload into text line array___________
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
    // _______check mqtt connection to broker___________________
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    }
  },
  // _________update text area ervery 30 minutes________________
  mounted: function (): void {
    this.intervalHandle = setInterval(() => {
      this.onClk()
    }, 1800000) // 1800s = 30min
  },
  unmounted: function (): void {
    clearInterval(this.intervalHandle)
  },
  // _________react on mqtt connection changes__________________
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
    // _______y position of textarea line "index" 1...__________
    _yt: function (index: number): number {
      const dy = (this.iTextsize - 1) * (this.geof.dyl * (index - 0.2))
      if (this.iLines > 0) return this.geof.yt(1 + index) + dy
      return this.geof.yt(index) + dy
    },
    // _______get text for line number ...______________________
    _yLine: function (linenumber: number): string {
      return gLines[linenumber - 1]
    },

    // =======text functions part 2=============================
    // _______are the next 1 or 2 (= num_) char single char?____
    isNextCharSingle: function (s1_: string, iEnd_: number, num_: number): boolean {
      if (num_ < 1 || num_ > 3) return false
      const len_ = s1_.length
      if (iEnd_ >= len_) return false
      // if (s1_.at(iEnd_) === ' ') return false
      if (s1_[iEnd_] === ' ') return false
      const iEnd1_ = iEnd_ + num_
      if (iEnd1_ === len_) return true
      if (iEnd1_ < len_) {
        // if (s1_.at(iEnd1_) === ' ') return true
        if (s1_[iEnd1_] === ' ') return true
      }
      return false
    },

    // _______are the previous 1 or 2 (= num_) char single char?
    isPrevCharSingle: function (s1_: string, iEnd_: number, num_: number): boolean {
      if (num_ < 1 || num_ > 3) return false
      const iEnd1_ = iEnd_ - num_
      if (iEnd1_ < 1) return false
      // if (s1_.at(iEnd_ - 1) === ' ') return false
      if (s1_[iEnd_ - 1] === ' ') return false
      // if (s1_.at(iEnd1_ - 1) === ' ') return true
      if (s1_[iEnd1_ - 1] === ' ') return true
      return false
    },

    // _______split a string to an array of strings with max len
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
        // if (s1.at(s1len - 1) === ' ') s1len--
        if (s1[s1len - 1] === ' ') s1len--
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

    // _______react on click: publish a message (if any exists)-
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
