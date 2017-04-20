<template>
  <div class="arbitrage" ref="arbitrage">
    <!-- <h1>{{ testProps }}</h1> -->
    <h1> {{msg}} </h1>

    <!-- Adds dropdown menus for Start and End currencies -->
    <!-- Start currency -->
    <el-select v-model="start" placeholder="Start currency">
      <el-option
        v-for="item in startCurrencies"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>
    <!-- End currency -->
    <el-select v-model="end" placeholder="End currency">
      <el-option
        v-for="item in endCurrencies"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>

    <!-- Multiple selections to exclude currencies -->
    <el-select v-model="exclude" multiple placeholder="Exclude currencies">
      <el-option
        v-for="item in excludeCurrencies"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>

    <!-- Input field for amount and calculate button-->
    <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>
    <el-button @click="fetchPath()">Calculate</el-button>

    <!-- Only show optimal conversion and path when user clicks calculate -->
    <h2 v-if="shouldShow === true">Optimal conversion: {{ optVal }}</h2>
    <h2 v-if="shouldShow === true">Path:</h2>
    <h3> {{apiData}} </h3>
  </div>
</template>

<script>
export default {
  name: 'arbitrage',
  data () {
    return {
      msg: 'Arbitrage', 
      startCurrencies: [{
          value: 'Start currency',
          label: 'Start currency',
          disabled: true
        },{
          value: 'AUD',
          label: 'AUD'
        }, {
          value: 'CAD',
          label: 'CAD'
        }, {
          value: 'CHF',
          label: 'CHF'
        }, {
          value: 'EUR',
          label: 'EUR'
        }, {
          value: 'GBP',
          label: 'GBP'
        }, {
          value: 'JPY',
          label: 'JPY'
        }, {
          value: 'NZD',
          label: 'NZD'
        }, {
          value: 'USD',
          label: 'USD'
        }],
      endCurrencies: [{
          value: 'End currency',
          label: 'End currency',
          disabled: true
        },{
          value: 'AUD',
          label: 'AUD'
        }, {
          value: 'CAD',
          label: 'CAD'
        }, {
          value: 'CHF',
          label: 'CHF'
        }, {
          value: 'EUR',
          label: 'EUR'
        }, {
          value: 'GBP',
          label: 'GBP'
        }, {
          value: 'JPY',
          label: 'JPY'
        }, {
          value: 'NZD',
          label: 'NZD'
        }, {
          value: 'USD',
          label: 'USD'
        }],
      excludeCurrencies: [{
        value: 'Exclude currencies',
        label: 'Exclude currencies',
        disabled: true
        },{
          value: 'AUD',
          label: 'AUD'
        }, {
          value: 'CAD',
          label: 'CAD'
        }, {
          value: 'CHF',
          label: 'CHF'
        }, {
          value: 'EUR',
          label: 'EUR'
        }, {
          value: 'GBP',
          label: 'GBP'
        }, {
          value: 'JPY',
          label: 'JPY'
        }, {
          value: 'NZD',
          label: 'NZD'
        }, {
          value: 'USD',
          label: 'USD'
        }],
      start: '',
      end: '', 
      numEdges: 5,
      exclude: [],
      inputVal: null,
      optRate: .4567,
      apiData: null, 
      shouldShow: false
    }
  }, 
  methods: {
    fetchPath() {
      this.shouldShow = true;

      var exclude = this.exclude;

      var str = "http://localhost:3000/arbitrageData/" + this.start + "/" + this.end + "/" + this.numEdges.toString();

      for (var i = 0; i < exclude.length; ++i){
        if (i === 0){
          str = "?" + str + "exclude=" + exclude[i];
        } else {
          str = "&" + str + "exclude=" + exclude[i];
        }
      }

      console.log(str);
      // call for arbitrageData
      axios.get(str).then( (response) => {
        console.log(response);
        this.apiData = response.data;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })

      console.log(this.apiData);
    }
  }, 
  computed: {
    optVal() {
      return this.optRate * this.inputVal;
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
