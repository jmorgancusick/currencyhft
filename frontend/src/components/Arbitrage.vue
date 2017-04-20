<template>
  <div class="arbitrage" ref="arbitrage">
    <!-- <h1>{{ testProps }}</h1> -->
    <h1> {{msg}} </h1>
    <el-row :gutter="20" style="margin-bottom: 10ps;">
      <el-col :span="20" :offset="2">
        <!-- Adds dropdown menus for Start and End currencies -->
        <!-- Start currency -->
        <el-select v-model="start" placeholder="Start currency" @change="handleChange()">
          <el-option
            v-for="item in startCurrencies"
            :label="item.label"
            :value="item.value"
            :disabled="item.disabled">
          </el-option>
        </el-select>
        <!-- End currency -->
        <el-select v-model="end" placeholder="End currency" @change="handleChange()">
          <el-option
            v-for="item in endCurrencies"
            :label="item.label"
            :value="item.value"
            :disabled="item.disabled">
          </el-option>
        </el-select>

        <!-- Multiple selections to exclude currencies -->
        <el-select v-model="exclude" multiple placeholder="Exclude currencies" @change="handleChange()">
          <el-option
            v-for="item in excludeCurrencies"
            :label="item.label"
            :value="item.value"
            :disabled="item.disabled">
          </el-option>
        </el-select>

        <el-input-number v-model="numEdges" :min="1" :max="7" @change="handleChange()" style="margin-bottom: -13px;"></el-input-number>
      </el-col>
    </el-row>

    <el-row :gutter="20">
      <el-col :span="20" :offset="2">
        <!-- Input field for amount and calculate button-->
        <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>
      </el-col>
    </el-row>


    <el-button @click="fetchPath()">Calculate</el-button>

    <!-- Only show optimal conversion and path when user clicks calculate -->
    <h2 v-if="shouldShow === true">Optimal conversion: {{ optVal }}</h2>
    <h2 v-if="shouldShow === true">Path: {{ optPath }} </h2>
    <h3 v-if="shouldShow === true">Regular Rate: {{ regRate }} </h3>
    <h3 v-if="shouldShow === true">Optimal Rate: {{ optRate }} </h3>
    <!-- <h3> {{apiData}} </h3> -->
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
      numEdges: null,
      exclude: [],
      regRate: null,
      inputVal: null,
      //optRate: null,
      //optVal: null, 
      //optPath: [],
      apiData: null, 
      shouldShow: false
    }
  }, 
  methods: {
    fetchPath() {
      // fetching regualr rate
      var regStr = "http://localhost:3000/calculatorData/" + this.start + "/" + this.end;

      // fetching optimal rate and path ro achieve optimal rate
      var exclude = this.exclude;
      var arbStr = "http://localhost:3000/arbitrageData/" + this.start + "/" + this.end + "/" + this.numEdges.toString();

      for (var i = 0; i < exclude.length; ++i){
        if (i === 0){
          arbStr = arbStr + "?" + "exclude=" + exclude[i];
        } else {
          arbStr = arbStr + "&" + "exclude=" + exclude[i];
        }
      }

      axios.get(arbStr).then( (response) => {
        console.log(response);
        this.apiData = response.data;
        this.shouldShow = true;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })

      axios.get(regStr).then( (response) => {
        console.log(response);
        this.regRate = response.data.rate;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })

      /*if ( this.apiData !== null ){
        this.shouldShow = true;
      }*/
      console.log("API Data", this.apiData);

      //this.optRate = this.apiData.totalRate;

      //this.optPath = this.apiData.currencies;

      //this.optVal = this.optRate * this.inputVal;
    }, 
    handleChange() {
      this.apiData = null;
      this.shouldShow = false;
    }
  }, 
  computed: {
    optRate() {
      return this.apiData.totalRate;
    },
    optPath() {
      var retStr = "";
      for(var i = 0; i < this.apiData.currencies.length; i++){
        retStr+=this.apiData.currencies[i];
        if(i !== this.apiData.currencies.length-1){
          retStr+=", "
        }
      }
      return retStr;
    },
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
