<template>
  <div class="home">
    <h1>{{ msg }}</h1>
    <h1>{{ testProps }}</h1>
    <el-button @click="test()">Click Here</el-button>
  </div>
</template>

<script>
export default {
  name: 'home',
  data () {
    return {
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
