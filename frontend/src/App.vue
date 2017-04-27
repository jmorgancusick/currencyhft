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
      <router-link :to="{name: 'calculator'}">
        <el-menu-item index="3">Calculator</el-menu-item>
      </router-link>
      <router-link :to="{name: 'arbitrage'}">
        <el-menu-item index="4">Arbitrage</el-menu-item>
      </router-link>
      <router-link :to="{name: 'profit'}">
        <el-menu-item index="5">Profit</el-menu-item>
      </router-link>
    </el-menu>
    <div class="line"></div>
    <!-- Necessary line in template to use router -->
    <router-view></router-view>

    <div>
      <footer>
        <div style="margin-top:10px;">
          <p>The  <strong>Currency Exchange</strong>  Team</p>
          <p><a href="https://github.com/jmcusick/currencyhft">GitHub</a></p>
        </div>
        <!--<img class="img" src="./assets/temp-logo.svg">-->
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
  min-height:100%;
  position:relative;
}
.img {
  display: block;
  position: relative;
  margin: auto;
  margin-bottom: -10px;
  height: 30px;
  width: 5%;
}
footer {
  position: absolute;
  right: 0;
  bottom: 0;
  left: 0;
  padding: 1rem;
  text-align: center;
  height:80px;
  width:100%;
  background-color:#EEF6F1;
}
a:link{
  text-decoration: none;
  color: #42b983;
}
a {
  color: #42b983;
}
p {
  color: #42b983;
}
</style>

