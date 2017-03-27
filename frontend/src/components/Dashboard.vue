<template>
  <div class="dashboard">
    <h1>{{ msg }}</h1>
    <h2>Essential Links</h2>
    <ul>
      <el-button>Table</el-button>
      <el-button>Chart</el-button>
      <el-button>Tickers</el-button>
    </ul>

    {{msg}}

  </div>
</template>

<script>
export default {
  name: 'dashboard',
  data () {
    return {
      msg: 'Dashboard'
    }
  },
  created () {
    // get will make a call to api 
    axios.get("localhost:3000/tickerData/").then( (response) => {
      console.log(response)
      this.msg = response.data;
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
