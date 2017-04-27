<template>

  <div id="app">
    <el-menu theme="dark" :default-active="activeIndex" mode="horizontal" @select="handleSelect">
      <!-- Router Vue components that link to items on the Navigation Bar -->
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
    <!-- Necessary line in template to use router -->
    <router-view></router-view>

    <div>
      <footer>
        <p>Posted by: Currency Exchange Team</p>
        <p><a href="https://github.com/jmcusick/currencyhft">GitHub</a></p>
      </footer>
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
      // handles NavBar selection
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
a {
  color: #42b983;
}
p {
  color: #42b983;
}
</style>

