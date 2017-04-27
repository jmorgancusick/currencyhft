<template>
  <div class="profit">
    <el-row :gutter="20">
      <el-col :span="20" :offset="2">
        <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>
        <p>{{dailyData}}</p>
      </el-col>
    </el-row>
  </div>
</template>

<script>
export default {
  name: 'profit',
  data () {
    return {
      inputVal: null, 
      dailyData: null
    }
  },
  created () {
    // call for the Daily Arbitrage most profitable paths
    axios.get("http://currencyhft.com:3000/dailyArbitrage/").then( (response) => {
      console.log(response)
      this.dailyData = response.data;
    }).catch( (error) => {
      console.log("ERROR:", error)
    })
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.profit {
  margin:0;
  padding:0;
  min-height: 100%;
  height: 100%;
  box-sizing: border-box;
}
.el-col {
  padding: 25px;
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
