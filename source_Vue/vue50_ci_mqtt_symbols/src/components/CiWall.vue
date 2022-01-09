<!-- CiWall.vue --------------------------------- 2022-01-06 -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="wallBorder"></CiBase>
  <path :d="wallPath" :fill="wallColor" :stroke="wallColor" stroke-width="1" />
  <text v-if=" title.length>0" class="ciFont1" :x="geo.xt()" :y="geo.yt(1)">{{title}}</text>
  <text v-if=" text5.length>0" class="ciFont1" :x="geo.xt()" :y="geo.yt(5)">{{ text5 }}</text>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiBase, { Geo } from './CiBase.vue'

export default defineComponent({
  name: 'CiWall',
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
      required: false
    },
    sid: {
      type: String,
      required: true
    },
    type: {
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
    },
    title: {
      type: String,
      required: false,
      default: ''
    },
    text5: {
      type: String,
      required: false,
      default: ''
    }
  },
  computed: {
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    wallColor: function (): string {
      return this.geo.colorWall
    },
    wallBorder: function (): number {
      if (this.border) return this.border
      return 0
    },
    wallPath: function (): string {
      if (this.type.length < 1) return '' // no type
      // ----calculation of wall thickness values (integers!)---
      const dw2 = this.geo.dw2 // start value
      const dw = dw2 * 2
      const dw2W = Math.round(dw2 * Math.sqrt(2)) // = 2 * dw2V // = dw2 * Math.sqrt(2)
      const a = dw2W - dw2
      const aa = 2 * a
      const b = dw2 - a
      // -----half size of the outer square---------------------
      const dx2 = this.geo.dxo2
      const dy2 = this.geo.dyo2
      // -----auxiliary factors for drawing---------------------
      let fx = 1 // may be either Math.abs(this.fx) or 1
      let fy = 1 // may be either Math.abs(this.fy) or 1
      // movement to the center of the figure in absolute coordinates
      const s0 = 'M' + this.x + ',' + this.y + ' '
      // auxiliary variable for forming the drawing string (path)
      let s1 = ''
      try {
        let type = 0
        if (this.type.includes('0x')) type = parseInt(this.type, 16)
        else type = parseInt(this.type, 10)
        if (type & 1) {
          if (this.fx >= 0) fx = this.fx; else fx = 1
          // s1 += s0 + 'm-' + dw2 + ',-' + dw2 + ' v' + dw + ' h' + (dw2 + dx2 * fx) + ' v-' + dw + ' z'
          s1 += s0 + 'm-' + a + ',-' + dw2 + ' l-' + b + ',' + b + ' v' + aa + ' l' + b + ',' + b + ' h' + (a + dx2 * fx) + ' v-' + dw + ' z'
        }
        if (type & 2) {
          if (this.fy <= 0) fy = -this.fy; else fy = 1
          // s1 += s0 + 'm-' + dw2 + ',' + dw2 + ' h' + dw + ' v-' + (dw2 + dy2 * fy) + ' h-' + dw + ' z'
          s1 += s0 + 'm-' + dw2 + ',' + a + ' l' + b + ',' + b + ' h' + aa + ' l' + b + ',-' + b + ' v-' + (a + dy2 * fy) + ' h-' + dw + ' z'
        }
        if (type & 4) {
          if (this.fx <= 0) fx = -this.fx; else fx = 1
          // s1 += s0 + 'm' + dw2 + ',' + dw2 + ' v-' + dw + ' h-' + (dw2 + dx2 * fx) + ' v' + dw + ' z'
          s1 += s0 + 'm' + a + ',' + dw2 + ' l' + b + ',-' + b + ' v-' + aa + ' l-' + b + ',-' + b + ' h-' + (a + dx2 * fx) + ' v' + dw + ' z'
        }
        if (type & 8) {
          if (this.fy >= 0) fy = this.fy; else fy = 1
          // s1 += s0 + 'm' + dw2 + ',-' + dw2 + ' h-' + dw + ' v' + (dw2 + dy2 * fy) + ' h' + dw + ' z'
          s1 += s0 + 'm' + dw2 + ',-' + a + ' l-' + b + ',-' + b + ' h-' + aa + ' l-' + b + ',' + b + ' v' + (a + dy2 * fy) + ' h' + dw + ' z'
        }
        if (type & 16) {
          if (this.fx >= 0) fx = this.fx; else fx = 1
          // s1 += s0 + 'm-' + dw2W + ',0 l' + dw2W + ',' + dw2W + ' l' + (dw2V + dx2 * fx) + ',-' + (dw2V + dy2 * fx) + ' l-' + dw2W + ',-' + dw2W + ' z'
          s1 += s0 + 'm-' + dw2 + ',-' + a + ' v' + aa + ' l' + b + ',' + b + ' h' + aa + ' l' + (a + dx2 * fx) + ',-' + (a + dy2 * fx) + ' l-' + dw2W + ',-' + dw2W + ' z'
        }
        if (type & 32) {
          if (this.fy <= 0) fy = -this.fy; else fy = 1
          // s1 += s0 + 'm0,' + dw2W + ' l' + dw2W + ',-' + dw2W + ' l-' + (dw2V + dy2 * fy) + ',-' + (dw2V + dx2 * fy) + ' l-' + dw2W + ',' + dw2W + ' z'
          s1 += s0 + 'm-' + a + ',' + dw2 + ' h' + aa + ' l' + b + ',-' + b + ' v-' + aa + ' l-' + (a + dy2 * fy) + ',-' + (a + dx2 * fy) + ' l-' + dw2W + ',' + dw2W + ' z'
        }
        if (type & 64) {
          if (this.fx <= 0) fx = -this.fx; else fx = 1
          // s1 += s0 + 'm' + dw2W + ',0 l-' + dw2W + ',-' + dw2W + ' l-' + (dw2V + dx2 * fx) + ',' + (dw2V + dy2 * fx) + ' l' + dw2W + ',' + dw2W + ' z'
          s1 += s0 + 'm' + dw2 + ',' + a + ' v-' + aa + ' l-' + b + ',-' + b + ' h-' + aa + ' l-' + (a + dx2 * fx) + ',' + (a + dy2 * fx) + ' l' + dw2W + ',' + dw2W + ' z'
        }
        if (type & 128) {
          if (this.fy >= 0) fy = this.fy; else fy = 1
          // s1 += s0 + 'm0,-' + dw2W + ' l-' + dw2W + ',' + dw2W + ' l' + (dw2V + dy2 * fy) + ',' + (dw2V + dx2 * fy) + ' l' + dw2W + ',-' + dw2W + ' z'
          s1 += s0 + 'm' + a + ',-' + dw2 + ' h-' + aa + ' l-' + b + ',' + b + ' v' + aa + ' l' + (a + dy2 * fy) + ',' + (a + dx2 * fy) + ' l' + dw2W + ',-' + dw2W + ' z'
        }
        if (type & 0x100) s1 += s0 + 'm' + dw2 + ',-' + dw2 + ' h-' + dw + ' v' + dw + ' h' + dw + ' z'
        if (type & 0x200) s1 += s0 + 'm0,-' + dw2W + ' l-' + dw2W + ',' + dw2W + ' l' + dw2W + ',' + dw2W + ' l' + dw2W + ',-' + dw2W + ' z'
      } catch (error) { console.error(error); s1 = '' }
      return s1
    }
  },
  methods: {
  }
})
</script>

<style scoped>
 .styleComp { font: bold 24px sans-serif; fill: black; cursor: pointer;}
</style>
