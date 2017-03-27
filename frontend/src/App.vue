<template>
  <div id="app">
    <div id="img">
      <img src="./assets/temp-logo.svg">
    </div>
    <router-view @testEvent="listener($event)" :testProps="inputMsg"></router-view>
    <router-link :to="{name: 'home'}">Home</router-link>
    <router-link :to="{name: 'dashboard'}">Dashboard</router-link>
    <div>
      <input v-model="inputMsg" type="text">
    </div>
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
#img {
  display: block;
  margin: auto;
  margin-bottom: -20px;
  width: 25%; 
}
</style>
