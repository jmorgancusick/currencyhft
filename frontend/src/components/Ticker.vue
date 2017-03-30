<template>
  <div class="ticker">
    <div id="content">
      <p>{{ id }}</p>
      <p>{{ percentChange }}</p>
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
