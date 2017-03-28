<template>
  <div class="dashboard">
    <h1>{{ msg }}</h1>
    <ul>
      <el-button>Table</el-button>
      <el-button>Chart</el-button>
      <el-button>Tickers</el-button>

      {{tickers}}

      {{chartData}}
    </ul>

  </div>
</template>

<script>
export default {
  name: 'dashboard',
  data () {
    return {
      msg: 'Dashboard',
      tickers: '',
      chartData: ''
    }
  },
  created () {
    // call for tickerData
    axios.get("http://localhost:3000/tickerData/").then( (response) => {
      console.log(response)
      this.tickers = response.data;
    }).catch( (error) => {
      console.log("ERROR:", error)
    })

    // call for chartData
    axios.get("http://localhost:3000/chartData/USDEUR=X/YTD").then( (response) => {
      console.log(response)
      this.chartData = response.data;
    }).catch( (error) => {
      console.log("ERROR:", error)
    })
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
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
