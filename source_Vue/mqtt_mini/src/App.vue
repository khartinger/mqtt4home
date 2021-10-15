<template>
  <mqttMini></mqttMini>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import mqttMini from './components/mqttMini.vue'

export default defineComponent({
  name: 'App',
  components: {
    mqttMini
  }
})
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;     /* center    */
  color: black;       /* #2c3e50;  */
  margin-top: 0px;      /* 60px;     */
}
</style>
