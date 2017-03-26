<template>
  <div id="app">
    <img src="./assets/logo.png">
    <router-view @testEvent="listener($event)" :testProps="inputMsg"></router-view>
    <router-link :to="{name: 'home'}">Home</router-link>
    <router-link :to="{name: 'dashboard'}">Dashboard</router-link>
    <input v-model="inputMsg" type="text">
  </div>
</template>

<script>
export default {
  name: 'app',
  data () {
    return {
      inputMsg: ""
    }
  },
  methods: {
    listener(e) {
      console.log(e)
    }
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
