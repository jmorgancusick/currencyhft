<template>
  <div class="ticker">
    <div id="content">
      <p>{{ ticker.ticker }}</p>
      <p>{{ ticker.percentChange }}</p>
    </div>
  </div>
</template>

<script>
export default {
  name: 'ticker',
  data () {
    return {
    }
  },
  created () {
    console.log(this.ticker);
  },
  props: ['ticker']
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
