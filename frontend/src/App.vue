<template>

  <div id="app">
    <el-menu theme="dark" :default-active="activeIndex" mode="horizontal" @select="handleSelect">
      <el-menu-item index="1">
        <router-link :to="{name: 'home'}">Home</router-link>
      </el-menu-item>
      <el-menu-item index="2">
        <router-link :to="{name: 'dashboard'}">Dashboard</router-link>
      </el-menu-item>
      <el-menu-item index="3">
        <router-link :to="{name: 'arbitrage'}">Arbitrage</router-link>
      </el-menu-item>
      <el-menu-item index="4">
        Calculator
      </el-menu-item> 
    </el-menu>
    <div class="line"></div>

    <div id="img">
      <img src="./assets/temp-logo.svg">
    </div>
    <router-view @testEvent="listener($event)" :testProps="inputMsg"></router-view>
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
  color: #82BF9D;
}
#img {
  display: block;
  margin: auto;
  width: 25%; 
}
a:link{
  text-decoration: none;
}
</style>

