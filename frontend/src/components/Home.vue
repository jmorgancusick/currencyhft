<template>
  <div class="home">
    <div class="block">
    <!--<span class="demonstration">Switch when indicator is clicked</span>-->
      <el-carousel trigger="click">
        <el-carousel-item v-for="item in 3">
          <img src="../assets/temp-banner.svg">
        </el-carousel-item>
      </el-carousel>
    </div>
    
    <div id="img">
      <img src="../assets/temp-logo.svg">
    </div>

    <div>
      <h1>{{ msg }}</h1>
      <h1>{{ testProps }}</h1>
      <el-button @click="test()">Click Here</el-button>
    </div>
  </div>
</template>

<script>
export default {
  name: 'home',
  data () {
    return {
      item: '',
      msg: 'Currency Home',
      data1: 100
    }
  },
  methods: {
    test() {
      this.$emit("testEvent", {obj: this.data1} )
    }
  }, 
  props: ["testProps"]
}
</script>


<!--
data 
computed
watchers
methods
 -->

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.el-carousel__item {
  display: block;
  color: #475669;
  opacity: 1;
}
#img {
  display: block;
  margin: auto;
  width: 25%; 
  margin-bottom: -45px;
}
h1, h2 {
  font-weight: normal;
}
ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
</style>
