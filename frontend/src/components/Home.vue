<template>
  <div class="home">
    <div class="block">
    
      <!-- Carousel Banner -->
      <div class="bgimg">
        <el-row :gutter="20">
          <el-col :span="20" :offset="2">
            <div class="title">
              <img src="../assets/logo-currency.png" style="margin-bottom: 30px;">
              <h1>Currency Exchange</h1>
            </div>
          </el-col>
        </el-row>
        <!--<img src="../assets/currency-green.svg">--> 
      </div>
  
      <div style="height: 400px;">
        <div style="margin-top: 40px;">
        <el-row :gutter="20">
          <el-col :span="4" :offset="3">
            <div>
              <p>CurrencyHFT is a web application that aims to capture the small arbitrage opportunities that might exist among different currency exchange rates. This investigative software reveals what profits could be made using triangular arbitrage in the Foreign Exchange Market.</p>
            </div>
          </el-col>

          <el-col :span="4" :offset="3">
            <div>
              <p>The Foreign Exchange Market (FOREX) is a global decentralized marketplace in which the world's currencies are exchanged. It is the largest and most liquid market, with trading exceeding trillions of dollars each day.</p>
            </div>
          </el-col>

          <el-col :span="4" :offset="3">
            <div>
              <p>CurrencyHFT is an open source project created for Rensselaer Polytechnic Institute's Software Design and Documentation course. The development team includes: Matthew Cordone, John Cusick, Patrick Engelsman, James Nakashian and Ayushi Mishra. </p>
            </div>
          </el-col>
        </el-row>
        </div>
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
  margin-top: 350px;
  opacity: 0.70;
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
p {
  font-size: 14px;
  font-style: normal;
  font-variant: normal;
  color:#0c5a2f;
  line-height: 30px;
  text-align: justify;
  text-justify: inter-word;
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
