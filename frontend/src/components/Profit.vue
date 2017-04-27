<template>
  <div class="profit">
    <div style="height: 100%;">
      <h1 style="padding: 30px;">Profits</h1>
      <el-row :gutter="20">
        <el-col :span="20" :offset="2">
          <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>
          <p>Daily Profit: {{dailyReturn * inputVal}}</p>
          <p>Yearly Profit: {{yearlyReturn * inputVal}}</p>
          <p>Cycles: {{cycles}}</p>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
export default {
  name: 'profit',
  data () {
    return {
      inputVal: 1, 
      dailyData: null,
      dailyReturn: null,
      yearlyReturn: null,
      cycles:[]
    }
  },
  created () {
    // call for the Daily Arbitrage most profitable paths
    axios.get("http://currencyhft.com:3000/dailyArbitrage/").then( (response) => {
      console.log(response)
      this.dailyData = response.data;
      this.dailyReturn = this.dailyData.dailyReturn;
      this.yearlyReturn = this.dailyData.yearlyReturn;
      this.cycles = this.dailyData.cycles;
      console.log("Got Daily:", this.dailyReturn);
      console.log("Got Yearly:", this.yearlyReturn);
      console.log("Got Cycles:", this.cycles);
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
h1 {
  font-weight: normal;
  font-family: Didot, "Didot LT STD", "Hoefler Text", Garamond, "Times New Roman", serif;
  color: #0c5a2f;
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
