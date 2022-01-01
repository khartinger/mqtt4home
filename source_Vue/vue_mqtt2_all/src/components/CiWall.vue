<!-- CiWall.vue --------------------------------- 2021-12-27 -->
<template>
  <!--draw border------------------------------------------- -->
  <CiBase :x="x" :y="y" :border="wallBorder"></CiBase>
  <path :d="wallPath" :fill="wallColor" :stroke="wallColor" stroke-width="1" />
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
      type: Number,
      required: true
    }
  },
  computed: {
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    wallColor: function (): string {
      return '#600000'
    },
    wallBorder: function (): number {
      if (this.border) return this.border
      return 0
    },
    wallPath: function (): string {
      const dwx2 = this.geo.dyl / 2
      const dwx = dwx2 * 2
      const dwx4 = dwx2 / 2
      const dx2 = this.geo.dxo2
      const dxd = dx2 - dwx2
      const dxs = dx2 + dwx2

      const dwy2 = this.geo.dyl / 2
      const dwy = dwy2 * 2
      const dwy4 = dwy2 / 2
      const dy2 = this.geo.dyo2
      const dyd = dy2 - dwy2
      const dys = dy2 + dwy2

      let s1 = 'M' + this.x + ',' + this.y + ' '
      switch (this.type & 15) {
        case 1: s1 += 'm-' + dwx4 + ',-' + dwy2 + ' v' + dwy + ' h' + (dx2 + dwx4) + ' v-' + dwy + ' z'; break
        case 2: s1 += 'm-' + dwx2 + ',' + dwy4 + ' h' + dwx + 'v-' + (dy2 + dwy4) + ' h-' + dwx + ' z'; break
        case 3: s1 += 'm-' + dwx2 + ',-' + dy2 + ' v' + dys + ' h' + dxs + ' v-' + dwy + ' h-' + dxd + ' v-' + dyd + ' z'; break
        case 4: s1 += 'm' + dwx4 + ',' + dwy2 + ' v-' + dwy + ' h-' + (dx2 + dwx4) + ' v' + dwy + ' z'; break
        case 5: s1 += 'm-' + dx2 + ',-' + dwy2 + ' v' + dwy + ' h' + (2 * dx2) + ' v-' + dwy + ' z'; break
        case 6: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + dyd + ' h-' + dxd + ' v' + dwy + ' h' + dxs + ' z'; break
        case 7: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + dyd + ' h-' + dxd + ' v' + dwy
          s1 += ' h' + (2 * dx2) + ' v-' + dwy + ' h-' + dxd + ' z'; break
        case 8: s1 += 'm' + dwx2 + ',-' + dwy4 + ' h-' + dwx + 'v' + (dy2 + dwy4) + ' h' + dwx + ' z'; break
        case 9: s1 += 'm-' + dwx2 + ',' + dy2 + ' h' + dwx + ' v-' + dyd + ' h' + dxd + ' v-' + dwy + ' h-' + dxs + ' z'; break
        case 10: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + (2 * dy2) + ' h' + dwx + ' z'; break
        case 11: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + (2 * dy2) + ' h' + dwx
          s1 += ' v-' + dyd + ' h' + dxd + ' v-' + dwy + ' h-' + dxd + ' z'; break
        case 12: s1 += 'm-' + dx2 + ',-' + dwy2 + ' v' + dwy + ' h' + dxd + ' v' + dyd + ' h' + dwx + ' v-' + dys + ' z'; break
        case 13: s1 += 'm-' + dx2 + ',-' + dwy2 + ' v' + dwy + ' h' + dxd + ' v' + dyd + ' h' + dwx
          s1 += ' v-' + dyd + ' h' + dxd + ' v-' + dwy + ' z'; break
        case 14: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + dyd + ' h-' + dxd + ' v' + dwy
          s1 += ' h' + dxd + ' v' + dyd + ' h' + dwx + ' z'; break
        case 15: s1 += 'm' + dwx2 + ',-' + dy2 + ' h-' + dwx + ' v' + dyd + ' h-' + dxd + ' v' + dwy
          s1 += ' h' + dxd + ' v' + dyd + ' h' + dwx + ' v-' + dyd + ' h' + dxd + ' v-' + dwy + ' h-' + dxd + ' z'; break
        default: break
      }
      if (this.type > 15) {
        let s2 = ''
        const dwxV = Math.round(dwx2 * Math.sqrt(2))
        const dwyV = Math.round(dwx2 * Math.sqrt(2))
        const ax = dx2 // - dwxV / 4 // dx2 + dwx4 - dwxV
        const ay = dy2 // - dwyV / 4 // dy2 + dwy4 - dwyV
        if ((this.type & 0x10) > 0) s2 += ' M' + this.x + ',' + this.y + ' ' + 'm' + dx2 + ',-' + dy2 + ' h-' + dwxV + ' l-' + ax + ',' + ay + ' l' + dwxV + ',' + dwyV + ' l' + ax + ',-' + ay + ' z'
        if ((this.type & 0x20) > 0) s2 += ' M' + this.x + ',' + this.y + ' ' + 'm-' + dx2 + ',-' + dy2 + ' v' + dwyV + ' l' + ax + ',' + ay + ' l' + dwxV + ',-' + dwyV + ' l-' + ax + ',-' + ay + ' z'
        if ((this.type & 0x40) > 0) s2 += ' M' + this.x + ',' + this.y + ' ' + 'm-' + dx2 + ',' + dy2 + ' h' + dwxV + ' l' + ax + ',-' + ay + ' l-' + dwxV + ',-' + dwyV + ' l-' + ax + ',' + ay + ' z'
        if ((this.type & 0x80) > 0) s2 += ' M' + this.x + ',' + this.y + ' ' + 'm' + dx2 + ',' + dy2 + ' v-' + dwyV + ' l-' + ax + ',-' + ay + ' l-' + dwxV + ',' + dwyV + ' l' + ax + ',' + ay + ' z'
        s1 += s2
      }
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
