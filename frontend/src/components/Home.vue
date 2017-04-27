<template>
  <div class="home">
    <div class="block">
    
      <!-- Carousel Banner -->
      <div class="bgimg">
        <el-row :gutter="20">
          <el-col :span="20" :offset="2">
            <div class="title">
              <h1>Currency Exchange</h1>
            </div>
          </el-col>
        </el-row>
        <!--<img src="../assets/currency-green.svg">-->
      </div>

      <div style="height: 400px;">

      </div>

      <div class="secimg" style="margin-bottom: -50px;">
        <!--<el-carousel trigger="click">
          <el-carousel-item v-for="item in 3">
            <img src="../assets/temp-banner.svg">
          </el-carousel-item>
        </el-carousel> -->
      </div>
    </div>
    
    <!--
    <div class="img">
      <img src="../assets/temp-logo.svg">
    </div> -->

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
.bgimg {
  background-image: url('../assets/currency-green.svg');
  background-repeat: no-repeat;
  background-size: 100%;
  width: 100%;
  height: 900px;
}
.title {
  margin-top: 495px;
  opacity: 0.75;
}
.secimg {
  background-image: url('../assets/temp-banner.svg');
  background-repeat: no-repeat;
  background-size: 100%;
  width: 100%;
  height: 500px;
}
.el-carousel__item {
  display: block;
  color: #475669;
  opacity: 1;
}
.img {
  display: block;
  position: relative;
  margin: auto;
  width: 15%; 
  height: 10px;
}
h1 {
  font-family: Didot, "Didot LT STD", "Hoefler Text", Garamond, "Times New Roman", serif;
  font-size: 60px;
  font-style: normal;
  font-variant: normal;
  font-weight: bold;
  line-height: 26.4px;
  color: white;
  text-align: center;
}
h2 {
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
