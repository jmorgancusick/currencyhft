<template>
  <div id="app">
    <el-menu theme="dark" :default-active="activeIndex" class="el-menu-demo" mode="horizontal" @select="handleSelect">
      <el-menu-item index="1">Dashboard</el-menu-item>
      <el-menu-item index="2">Arbitrage</el-menu-item>
      <el-menu-item index="3">Calculator</el-menu-item>
    </el-menu>
    <div class="line"></div>

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
    data() {
      return {
        activeIndex: '1'
      };
    },
    methods: {
      handleSelect(key, keyPath) {
        console.log(key, keyPath);
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

