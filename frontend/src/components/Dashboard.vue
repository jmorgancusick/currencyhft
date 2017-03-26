<template>
  <div class="dashboard">
    <h1>{{ msg }}</h1>
    <h2>Essential Links</h2>
    <ul>
      <li>Chart</li>
      <li>Tickers</li>
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
