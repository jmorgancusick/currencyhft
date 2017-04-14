<template>
  <div class="ticker">
    <div id="textbox">
      <el-col :span="3">
        <p style="text-align:left;">{{ id }}
        <span style="float:right;">{{ percentChange }}</span>
        </p>
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
      percentChange: null
    }
  },
  created () {
    console.log(this.ticker);
    this.id = this.ticker.ticker;
    this.percentChange = this.ticker.percentChange;

    /*console.log(this.id);
    console.log(this.percentChange);*/
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
