<!-- App.vue -->
<template>
  <CiMain1></CiMain1>
  <CiMain2></CiMain2>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import CiMain1 from '@/components/CiMain1.vue'
import CiMain2 from '@/components/CiMain2.vue'

export default defineComponent({
  name: 'App',
  components: {
    CiMain1,
    CiMain2
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
