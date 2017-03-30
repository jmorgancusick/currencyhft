<template>
  <div class="dashboard">
    <h1>{{ msg }}</h1>
    <ul>
      <el-button>Table</el-button>
      <el-button>Chart</el-button>
      <el-button>Tickers</el-button>
    </ul>
    <ticker v-for="ticker in tickers" :ticker="ticker" />

    <div>
      <svg width="960" height="500">
      <vn-line :model="trends"
              :x-format="formatDate"
              y-format="">
      </vn-line> 
      </svg>
    </div>

  </div>
</template>

<script>
import Ticker from './Ticker.vue';
import _ from 'lodash';
import d3 from 'd3';

export default {
  name: 'dashboard',
  data () {
    return {
      msg: 'Dashboard',
      tickers: null ,
      chartData: null
    }
  },
  components: {
      Ticker
  },
  computed: {
    trends () {
      return [
        {
          key: 'high',
          area: true,
          values: _.map(this.chartData, (t) => {
            return {
              x: t.date,
              y: t.high
            }
          })
        },
        {
          key: 'low',
          bar: true,
          values: _.map(this.chartData, (t) => {
            return {
              x: t.date,
              y: t.low
            }
          })
        },
        {
          key: 'close',
          area: true,
          values: _.map(this.chartData, (t) => {
            return {
              x: t.date,
              y: t.close
            }
          })
        }
      ]
    }
  },
  methods: {
    formatDate (d){
      return d3.time.format('%x')(new Date(d))
    }, 
    formatPercent (){
      return d3.range(0,1).format(".1f");
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

