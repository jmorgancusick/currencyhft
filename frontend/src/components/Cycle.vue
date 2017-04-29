<template>
  <div class="cycle">
    <div id="textbox">
      <el-col :span="4">
        <!-- Data displayed for each ticker -->
          <p style="text-align:left;"> {{ timestamp }} 
          <br/>
          {{ rate * 100 | currency('',6)}}%
          <br/>
          <span class="percentChange">
            <strong>{{ currencies}}</strong>
          </span>
          </p>
      </el-col>
    </div>
  </div>
</template>

<script>
import d3 from 'd3';

export default {
  name: 'cycle',
  data () {
    return {
      timestamp: null,
      rate: nulll,
      currencies: null
    }
  },
  created () {
    // sets ticker data object values from props
    console.log(this.cycle);

    this.timestamp = d3.time.format.utc('%I:%M %p')(new Date(this.cycle.timestamp*1000))


    this.rate = this.cycle.rate-1
    this.rate = this.rate.toFixed(8)


    var currencyStr = "";
    for(var i = 0; i < this.cycle.currencies.length; i++){
      currencyStr+=this.cycle.currencies[i];
      if(i !== this.cycle.currencies.length-1){
        currencyStr+=" -> "
      }
    }
    this.currencies = currencyStr;
  },
  props: ['cycle']
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
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
