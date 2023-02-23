<!-- CiWall1.vue ----------------------------khartinger----- -->
<!-- 2022-01-08: new                                         -->
<!-- 2023-02-12: change at CiBase (add Geo.ts)               -->
<!-- 2023-02-23: replace .at() by []                         -->

<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="border" :fx="1" :fy="1"></CiBase>
  <!--draw wall1-------------------------------------------- -->
  <path :d="drawWall1" :fill="colorWall1" :stroke="colorWall1" stroke-width="1" />
  <!--write header and footer line-------------------------- -->
  <text v-if=" header.length>0" class="ciFont1" :x="geof.xt()" :y="geof.yt(1)">{{header}}</text>
  <text v-if=" footer.length>0" class="ciFont1" :x="geof.xt()" :y="geof.yt(5)">{{ footer }}</text>
  <!--two more text lines ;) ------------------------------- -->
  <text v-if=" line2.length>0" class="ciFont1" :x="geof.xt()" :y="geof.yt(2)">{{line2}}</text>
  <text v-if=" line4.length>0" class="ciFont1" :x="geof.xt()" :y="geof.yt(4)">{{line4}}</text>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiBase from './CiBase.vue'
import { Geof } from '../classes/Geo'
import { toNumber } from '@vue/shared'

export default defineComponent({
  name: 'CiWall1',
  components: {
    CiBase
  },
  data () {
    return {
      fw: 1 as number
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
      default: 0
    },
    sid: {
      type: String,
      required: false
    },
    dir: {
      type: String,
      required: true
    },
    color: {
      type: String,
      required: false,
      default: '-'
    },
    header: {
      type: String,
      required: false,
      default: ''
    },
    footer: {
      type: String,
      required: false,
      default: ''
    },
    line2: {
      type: String,
      required: false,
      default: ''
    },
    line4: {
      type: String,
      required: false,
      default: ''
    }
  },
  computed: {
    // =======standard methods==================================
    // _______provide geometric data____________________________
    geof: function (): Geof {
      const geo1 = new Geof(this.x, this.y, 1, 1)
      return geo1
    },
    // _______draw the full path of the wall (attribute dir)____
    drawWall1: function (): string {
      const s1 = this.scanDirection(this.dir)
      return s1
    },
    // _______color of the wall_________________________________
    colorWall1: function (): string {
      if (this.color !== '-') return this.color
      return this.geof.colorWall
    }
  },
  methods: {
    // _______path command: draw a circle_______________________
    circlePath: function (cx: number, cy: number, r: number): string {
      return 'M ' + cx + ' ' + cy + ' m -' + r + ', 0 a ' + r + ',' + r + ' 0 1,0 ' + (r * 2) + ',0 a ' + r + ',' + r + ' 0 1,0 -' + (r * 2) + ',0 z'
    },

    // _______split dir attribute into path(s)__________________
    // return: path of all walls
    scanDirection: function (sDir: string): string {
      const len_ = sDir.length
      const s0 = sDir + '|'
      let sFullPath = ''
      // ==========for every char in sType======================
      for (let i = 0; i < len_; i++) {
        // --------search for a dir command---------------------
        let iDir = -1
        // const c = s0.at(i)
        const c = s0[i]
        switch (c) {
          case 'h': case 'H': iDir = 0; break
          case 'd': case 'D': iDir = 1; break
          case 'v': case 'V': iDir = 2; break
          case 't': case 'T': iDir = 3; break
          case 'q': case 'Q': iDir = 10; break
          case 'k': case 'K': iDir = 11; break
          case 'c': case 'C': iDir = 12; break
          case 'w': case 'W': iDir = 99; break
          default: break
        }
        if (iDir >= 0) {
          // ------valid dir command found :) ------------------
          let ok = true
          let fDivisor = 0.0
          let fNum = 0.0
          let cSign = '?'
          i++
          // ......is first char after command a special char?..
          let cNext = s0[i]
          if (cNext === '+') { cSign = '+'; i++ }
          if (cNext === '-') { cSign = '-'; i++ }
          if (cNext === '.') { fDivisor = 1; i++ }
          // ......Are the following chars a valid number?......
          cNext = s0[i]
          while (((cNext >= '0') && (cNext <= '9')) || cNext === '.') {
            if (cNext === '.') {
              if (fDivisor > 0) ok = false
              else fDivisor = 1
            } else {
              const iDigit = toNumber(cNext)
              fNum = fNum * 10 + iDigit
              fDivisor = fDivisor * 10
            }
            i++
            cNext = s0[i]
          }
          i-- // use the last char once more in the next loop
          if (ok) {
            // ----get path for the command---------------------
            if (fNum === 0) {
              // --command without a number (set value to 1)----
              let s0 = ''
              switch (cSign) {
                case '+': // command h+ v+ d+ t+ ...............
                  s0 = this.return1Path(iDir, 1)
                  sFullPath += s0
                  break
                case '-': // command h- v- d- t- ...............
                  sFullPath += this.return1Path(iDir + 4, 1)
                  break
                default: // command h v d t q k c w
                  // for h v d t: draw also negative wall!!.....
                  if (iDir < 10) sFullPath += this.return1Path(iDir + 4, 1)
                  sFullPath += this.return1Path(iDir, 1)
              }
            } else {
              // --the command contains a number----------------
              // ..floating point number?.......................
              if (fDivisor > 0) fNum = fNum / fDivisor
              // ..negative number -> correct the direction.....
              if (cSign === '-') {
                if (iDir < 10) iDir = iDir + 4
              }
              // ..get path string..............................
              sFullPath += this.return1Path(iDir, fNum)
            }
          }
        }
      } // ========END for======================================
      return sFullPath
    },

    // _______path string for one direktion with given width____
    return1Path: function (iDirection: number, f: number): string {
      // ----calculation of wall thickness values (integers!)---
      const dw2 = this.fw * Math.round(this.geof.dyl / 2) // start value
      const dw = dw2 * 2
      const dw2W = Math.round(dw2 * Math.sqrt(2)) // = 2 * dw2V // = dw2 * Math.sqrt(2)
      const a = dw2W - dw2
      const aa = 2 * a
      const b = dw2 - a
      // -----half size of the outer square---------------------
      const dx2 = this.geof.dxo2()
      const dy2 = this.geof.dyo2()
      // -----Auxiliary variables for within the case statement-
      let xx = 0
      let yy = 0
      // movement to the center of the figure in absolute coordinates
      const s0 = 'M' + this.x + ',' + this.y + ' '
      // auxiliary variable for forming the drawing string (path)
      let s1 = ''
      switch (iDirection) {
        case 0: // h+
          s1 += s0 + 'm-' + a + ',-' + dw2 + ' l-' + b + ',' + b + ' v' + aa + ' l' + b + ',' + b + ' h' + (a + dx2 * f) + ' v-' + dw + ' z'
          break
        case 1: // d+
          s1 += s0 + 'm-' + dw2 + ',-' + a + ' v' + aa + ' l' + b + ',' + b + ' h' + aa + ' l' + (a + dx2 * f) + ',-' + (a + dy2 * f) + ' l-' + dw2W + ',-' + dw2W + ' z'
          break
        case 2: // v+
          s1 += s0 + 'm-' + dw2 + ',' + a + ' l' + b + ',' + b + ' h' + aa + ' l' + b + ',-' + b + ' v-' + (a + dy2 * f) + ' h-' + dw + ' z'
          break
        case 3: // t+
          s1 += s0 + 'm-' + a + ',' + dw2 + ' h' + aa + ' l' + b + ',-' + b + ' v-' + aa + ' l-' + (a + dy2 * f) + ',-' + (a + dx2 * f) + ' l-' + dw2W + ',' + dw2W + ' z'
          break
        case 4: // h-
          s1 += s0 + 'm' + a + ',' + dw2 + ' l' + b + ',-' + b + ' v-' + aa + ' l-' + b + ',-' + b + ' h-' + (a + dx2 * f) + ' v' + dw + ' z'
          break
        case 5: // d-
          s1 += s0 + 'm' + dw2 + ',' + a + ' v-' + aa + ' l-' + b + ',-' + b + ' h-' + aa + ' l-' + (a + dx2 * f) + ',' + (a + dy2 * f) + ' l' + dw2W + ',' + dw2W + ' z'
          break
        case 6: // v-
          s1 += s0 + 'm' + dw2 + ',-' + a + ' l-' + b + ',-' + b + ' h-' + aa + ' l-' + b + ',' + b + ' v' + (a + dy2 * f) + ' h' + dw + ' z'
          break
        case 7: // t-
          s1 += s0 + 'm' + a + ',-' + dw2 + ' h-' + aa + ' l-' + b + ',' + b + ' v' + aa + ' l' + (a + dy2 * f) + ',' + (a + dx2 * f) + ' l' + dw2W + ',-' + dw2W + ' z'
          break

        case 10: // q
          // ------calculate side length------------------------
          xx = Math.abs(dw2 * f)
          yy = xx
          if (xx > dx2) xx = dx2
          if (yy > dy2) yy = dy2
          s1 += s0 + 'm' + xx + ',-' + yy + ' h-' + (2 * xx) + ' v' + (2 * yy) + ' h' + (2 * xx) + ' z'
          // s1 += s0 + 'm' + dw2 + ',-' + dw2 + ' h-' + dw + ' v' + dw + ' h' + dw + ' z'
          break
        case 11: // k
          // ------calculate side length------------------------
          xx = Math.abs(dw2W * f)
          yy = xx
          if (xx > dx2) xx = dx2
          if (yy > dy2) yy = dy2
          s1 += s0 + 'm0,-' + yy + ' l-' + xx + ',' + yy + ' l' + xx + ',' + yy + ' l' + xx + ',-' + yy + ' z'
          // s1 += s0 + 'm0,-' + dw2W + ' l-' + dw2W + ',' + dw2W + ' l' + dw2W + ',' + dw2W + ' l' + dw2W + ',-' + dw2W + ' z'
          break
        case 12: // c
          // ------calculate radius-----------------------------
          xx = Math.abs(dw2 * f)
          if (xx > dx2) xx = dx2
          if (xx > dy2) xx = dy2
          s1 += this.circlePath(this.x, this.y, xx)
          break
        case 99: // w
          // ------ calc wall thickness: max 3 * dyl------------
          this.fw = f
          if (f > 3) this.fw = 3
          break
        default: break
      }
      return s1
    }
  }
})
</script>

<style scoped>
 .styleComp { font: bold 24px sans-serif; fill: black; cursor: pointer;}
</style>
