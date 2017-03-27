<template>

  <div id="app">
    <el-menu theme="dark" :default-active="activeIndex" mode="horizontal" @select="handleSelect">
      <router-link :to="{name: 'home'}">
        <el-menu-item index="1">Home</el-menu-item>
      </router-link>
      <router-link :to="{name: 'dashboard'}">
        <el-menu-item index="2">Dashboard</el-menu-item>
      </router-link>
      <router-link :to="{name: 'arbitrage'}">
        <el-menu-item index="3">Arbitrage</el-menu-item>
      </router-link>
      <router-link :to="{name: 'calculator'}">
        <el-menu-item index="4">Calculator</el-menu-item>
      </router-link>
    </el-menu>
    <div class="line"></div>

    <router-view @testEvent="listener($event)" :testProps="inputMsg"></router-view>
   <!-- <div>
      <input v-model="inputMsg" type="text">
    </div> -->
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
a:link{
  text-decoration: none;
}
</style>

