<template>
  <div class="ticker">
    <div id="textbox">
      <el-col :span="4">
        <!-- Data displayed for each ticker -->
        <el-button style="width: 150px;">
          <p style="text-align:left;"> {{ id }} {{ space }}

          <span class="percentChange" style="float:right;">
            <strong>{{ percentChange }}</strong>
          </span>
          </p>
        </el-button>
      </el-col>
    </div>
  </div>
</template>

<script>
export default {
  name: 'ticker',
  data () {
    return {
      id: null,
      space: "   ",
      percentChange: null
    }
  },
  created () {
    // sets ticker data object values from props
    console.log(this.ticker);
    this.id = this.ticker.ticker.slice(0,6) + "   ";
    this.percentChange = this.ticker.percentChange.toFixed(2);

    if (this.ticker.percentChange > -0.005 && this.ticker.percentChange < 0){
      this.percentChange = "0.00";
    }
  },
  props: ['ticker']
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
