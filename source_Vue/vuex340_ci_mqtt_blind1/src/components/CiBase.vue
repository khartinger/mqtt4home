<!-- CiBase.vue -----------------------------khartinger----- -->
<!-- 2022-01-08 New                                          -->
<!-- 2022-08-14 Add fx, fy, calctmax                         -->
<!-- 2023-01-02 Class Geo moved to a separate file Geo.ts    -->
<!-- 2023-01-25 new rectangle drawing (area no filled)       -->
<!-- 2023-01-31 update drawBorderFill                        -->
<!-- 2023-02-25 add .ciFont1_5, .ciFont3, .ciFont4, .ciFont5 -->
<template>
  <!-- filled border between out and in line---------------- -->
  <path v-if="borderFill" :d="drawBorderFill" class="ciOut"/>
  <!--border: outer and inner rectangle--------------------- -->
  <rect v-if="borderIn" class="ciIn0"  :x="geo.x1()" :y="geo.y1()" :width="fx*geo.dxo()-2*geo.dxm" :height="fy*geo.dyo()-2*geo.dym" />
  <rect v-if="borderOut" class="ciOut0" :x="geo.x0()" :y="geo.y0()" :width="fx*geo.dxo()" :height="fy*geo.dyo()" />
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { Geo } from '../classes/Geo'
import '@/services/CiMqttClientInstance'

export default defineComponent({
  name: 'CiBase',
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
  emits: ['onClk'],
  computed: {
    geo: function (): Geo {
      const geo1 = new Geo(this.x, this.y)
      return geo1
    },
    borderFill: function (): boolean {
      if (this.border) {
        if (this.border === 3) return true
      }
      return false
    },
    borderOut: function (): boolean {
      if (this.border) {
        if (this.border === 1) return true
      }
      return false
    },
    borderIn: function (): boolean {
      if (this.border) {
        if (this.border === 2) return true
        if (this.border === 3) return true
      }
      return false
    },
    // -------draw yellow border--------------------------------
    drawBorderFill: function (): string {
      // let s1 = 'M' + this.x + ',' + this.y
      const dxof_ = this.geo.dxo() * this.fx
      const dyof_ = this.geo.dyo() * this.fy
      const dxif_ = dxof_ - 2 * this.geo.dxm
      const dyif_ = dyof_ - 2 * this.geo.dym
      let s1 = 'M' + this.geo.x + ',' + this.geo.y
      s1 += ' m-' + this.geo.dxo2() + ',-' + this.geo.dyo2()
      s1 += ' v' + dyof_ + ' h' + dxof_
      s1 += ' v-' + dyof_ + ' h-' + (dxof_ - this.geo.dxm - 1)
      s1 += ' v' + this.geo.dym + ' h' + (dxif_ - 1)
      s1 += ' v' + dyif_ + ' h-' + (dxif_ - 1)
      s1 += ' v-' + (dyif_ + this.geo.dym) + ' z'
      return s1
    }
  },
  mounted: function (): void {
    // ciMqttClientInstance.init()
  },
  methods: {
  }
})
</script>

<style>
  .ciFont1   { font-size: 14px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont1_5 { font-size: 21px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont2   { font-size: 28px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont2_5 { font-size: 35px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont3   { font-size: 42px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont4   { font-size: 56px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont5   { font-size: 70px; fill: black; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFont1k  { font-size: 16px; fill: red; font-weight: bold; font-style: italic; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .ciFontRoom { font-size: 28px; fill: green; font-weight: bold; font-family: "DejaVu Sans Mono","monospace"; white-space: pre; }
  .fontOK    { font-weight: bold; color: lightgreen; }
  .fontNOK   { font-weight: bold; color: red; }
  .cursor    { cursor: pointer; }
  .ciBackground {fill: #ddFFdd; }
  .ciOut     { fill: yellow; stroke: yellow; stroke-width: 1; }
  .ciOut0    { fill: none; stroke: blue; stroke-width: 1; }
  .ciIn0      { fill: none; stroke:blue; stroke-width: 1; }
  .ciLine    { stroke: blue; stroke-width: 1; }
  .ciRect    { fill: none; stroke: blue; stroke-width: 1; }
  .ciFillIt  { stroke: blue; stroke-width: 1; }
  .ciClick   { fill-opacity: 0.0; stroke-width: 1; }
  .ciBorder  { fill-opacity: 0.0; stroke: blue; stroke-width: 1; }
</style>
