<!-- App.vue -->
<template>
  <Seg7MainMqtt></Seg7MainMqtt>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import Seg7MainMqtt from '@/components/Seg7MainMqtt.vue'

export default defineComponent({
  name: 'App',
  components: {
    Seg7MainMqtt
  }
})
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  text-align: left;
  color: black;
  margin-top: 0px;
}
</style>
