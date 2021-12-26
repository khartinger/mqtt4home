<!-- src/views/About.vue -->
<template>
  <div class="about">
    <h2>About: vue_mqtt3_3webpages</h2>
    Purpose: This MQTT sample project shows how a Vue app with multiple
    pages is built.<br>
    Version: 2021-12-26<br><br>
  </div>
  <svg viewBox="0 0 230 36" width="100%">
  <rect class="ciBackground" x="0" y="0" width="230" height="36" />
  <rect @click="onClk()" x="65" y="3" rx=6 ry=6 width="100" height="30" fill="#FFF831" stroke="gold" stroke-width="4"/>
  <text @click="onClk()" x="100" y="27" textLength="30" lengthAdjust="spacingAndGlyphs" class="fontAbout">OK</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
export default defineComponent({
  components: {
  },
  methods: {
    onClk: function () {
      console.log('CLICK - About: onClk!', '')
      this.$router.push({name: 'Home'})
    }
  }
})
</script>

<style scoped>
 .fontAbout { font: bold 24px sans-serif; fill: red; cursor: pointer; }
</style>
