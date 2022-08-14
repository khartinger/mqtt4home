<!--CiTextarea.vue---------------------------khartinger----- -->
<!--2022-08-14: New                                          -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="fx" :fy="fy"></CiBase>
  <!--Draw rectangle 1, if line 1 is a title---------------- -->
  <rect v-if="isRect1" class="ciRect" :x="geo.x1()" :y="geo.yr(1)" :width="fx*geo.dxo-2*geo.dxm" :height="geo.dyl" />
  <!--convert payload into text lines----------------------- -->
  <div>{{ convertPayloadIntoTextLines }}</div>
  <!--show lines-------------------------------------------- -->
  <text v-for="index in maxLine" :key="index" class="ciFont1" :x="geo.xt()" :y="_yt(index)">{{lineY(index)}}</text>
  <!--define click area------------------------------------- -->
  <rect @click="onClk()" class="ciClick" :x="geo.x0()" :y="geo.y0()" :width="geo.dxo" :height="geo.dyo" />
</template>

<script lang="ts">
import { defineComponent } from '@vue/runtime-core'
import { Textarea, ciTextareaController } from '../controller/CiTextareaController'
import CiBase, { Geo } from './CiBase.vue'

let gLines: string[] = []

export default defineComponent({
  name: 'CiTextarea',
  components: {
    CiBase
  },
  data () {
    return {
      colorTextarea: 'yellow'
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
      default: 1
    },
    sid: {
      type: String,
      required: true
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
    }
  },
  computed: {
    textarea: function (): Textarea | undefined {
      return ciTextareaController.textareas.find(textarea => textarea.id === this.sid)
    },
    iTextareaState: function (): number {
      return this.textarea?.iTextareaState ?? -1
    },
    // -------provide geometric data----------------------------
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    isRect1: function (): boolean {
      if (this.iTextareaState > 0) return true
      return false
    },
    maxLine: function (): number {
      const temp = Math.trunc((this.fy * this.geo.dyo - 2 * this.geo.dym) / this.geo.dyl)
      // console.log('maxLine=', temp)
      return temp
    },
    maxCharPerLine: function (): number {
      return this.geo.calctmax(this.fx)
    },

    // -------text in line ...----------------------------------
    // convertPayloadIntoTextLines: function (): Array<string> {
    // eslint-disable-next-line vue/return-in-computed-property
    convertPayloadIntoTextLines: function (): void {
      const aLines = []
      if (this.textarea) {
        aLines.push('') // no line 0

        let s1 = this.textarea.payload
        s1 = s1.replaceAll('\\n', '^~') // replace "text" \\n
        s1 = s1.replaceAll('\n', '^~') //  replace "text" \n
        let aPay: string[] = []
        aPay = s1.split('^~')
        if (aPay === null) {
          const aPay: string[] = []
          aPay[0] = s1
        }
        let iPay = 0 //                index of payload "lines"
        const numPay = aPay.length
        // console.log('Anzahl \\n=', numPay)
        // console.log('Payload: ', aPay)
        // ---should line 1 be a title?-------------------------
        let iLine = 0 //               index of lines to display
        if (this.textarea.iTextareaState === 1) { // title = name
          if (this.textarea.name) aLines[iLine++] = this.textarea.name
          else aLines[iLine++] = ''
        }
        if (this.textarea.iTextareaState === 2) { // title = id
          aLines[iLine++] = this.textarea.id
        }
        if (this.textarea.iTextareaState === 4) { // title = payload line 0
          aLines[iLine++] = aPay[iPay++]
        }
        // ---convert payload lines into display lines----------
        let aHelp: string[] = []
        for (let i1 = iPay; i1 < numPay; i1++) {
          if (!aPay[iPay]) break
          aHelp = this.splitToArray(aPay[iPay++], this.maxCharPerLine)
          // console.log('aHelp: ', aHelp)
          const lenHelp = aHelp.length
          // -transfer help lines to display lines--------------
          for (let i2 = 0; i2 < lenHelp; i2++) {
            aLines[iLine++] = aHelp[i2]
            if (iLine > this.maxLine) { // finished. End for loops
              i2 = lenHelp
              i1 = numPay
            }
          }
        }
      }
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      gLines = aLines
      // return aLines
    },

    // -------color line 2 to 4 depending on iTextareaState------
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
    _yt: function (index: number): number {
      const yt1to5 = this.geo.yt(1 + ((index - 1) % 5))
      const temp = yt1to5 + this.geo.dyi * Math.trunc((index - 1) / 5)
      return temp
    },
    // -------text in line ...----------------------------------
    lineY: function (linenumber: number): string {
      if (linenumber < 1 || linenumber > this.maxLine) return ''
      return gLines[linenumber]
    },

    // -------check text length---------------------------------
    _checkLen: function (text: string): string {
      if (text) {
        if (text.length > this.maxCharPerLine) return text.substring(0, this.maxCharPerLine)
        return text
      }
      return ''
    },
    // ----split a string to a array of strings with max len------
    // s1: given string
    // numCharPerLine: max number of char per element in a1
    splitToArray: function (s1: string, numCharPerLine: number): Array<string> {
      const a1: string[] = []
      let i1 = 0 //            Index of char in s1
      let iStart = 0 //        Index of start char in s1 of this line
      let ia1 = 0 //           Index of line in a1
      // console.log('splitToArray s1: ', s1)
      // console.log('splitToArray numCharPerLine: ', numCharPerLine)
      const lens1 = s1.length
      for (i1 = 0; i1 < lens1; i1++) {
        let iEnd = iStart + numCharPerLine
        if (iEnd > lens1) {
          iEnd = lens1
          i1 = lens1 // end of for loop
        }
        if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
        else {
          if (s1.at(iEnd) === ' ') iEnd++
        }
        a1[ia1++] = s1.substring(iStart, iEnd)
        iStart = iEnd
      }
      // console.log('splitToArray a1: ', a1)
      return a1
    },
    // -------react on click: publish a message (if any exists)-
    onClk: function (): void {
      // console.log(this.sid, 'Textarea-Click')
      if (this.textarea) {
        if (this.textarea.pubTopic) {
          let payload = '?'
          const aPubTopic = this.textarea.pubTopic.split(' ')
          // -publish all given topics--------------------------
          aPubTopic.forEach((topic: string) => {
            if (this.textarea?.pubPayload) payload = this.textarea.pubPayload
            ciTextareaController.publishCi(topic, payload)
          })
        }
      }
    }
  }
})
</script>

<style scoped>
</style>
