<template>
  <div class="arbitrage">
    <h1>{{ msg }}</h1>
    <h1>{{ testProps }}</h1>
    <el-button @click="print()">Click Here</el-button>
    <h2> {{nicemsg}} </h2>
  </div>
</template>

<script>
export default {
  name: 'arbitrage',
  data () {
    return {
      msg: 'Arbitrage Coming Soon...',
      nicemsg: ''
    }
  },
  methods: {
    print() {
      this.$emit("testEvent", {obj: this.data1} )
      this.nicemsg = 'You are awesome!!'
    }
  }, 
  props: ["testProps"]
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
