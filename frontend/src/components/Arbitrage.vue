<template>
  <div class="arbitrage" ref="arbitrage">
    <h1 style="padding: 30px; margin-bottom: -10px;"> {{msg}} </h1>
    <el-row :gutter="20" style="margin-bottom: 10px;">
      <el-col :span="16" :offset="4">
        <div style="align:left;">
          <!-- Adds Forex or Banks Selection and Response
          <el-radio class="radio" v-model="ifBank" label="0" @change="handleRadio()">Forex</el-radio>
          <el-radio class="radio" v-model="ifBank" label="1" @change="handleRadio()">Banks</el-radio> -->

          <!-- Adds dropdown menus for Start and End currencies -->
          <!-- Start currency -->
          <el-select v-model="start" placeholder="Start currency" @change="handleChange()" style="padding: 15px;">
            <el-option
              v-for="item in startCurrencies"
              :label="item.label"
              :value="item.value"
              :disabled="item.disabled">
            </el-option>
          </el-select>
          <!-- End currency -->
          <el-select v-model="end" placeholder="End currency" @change="handleChange()" style="padding: 15px;">
            <el-option
              v-for="item in endCurrencies"
              :label="item.label"
              :value="item.value"
              :disabled="item.disabled">
            </el-option>
          </el-select>

          <!-- Multiple selections to exclude currencies -->
          <el-select v-model="exclude" multiple placeholder="Exclude currencies" @change="handleChange()" style="padding: 15px;">
            <el-option
              v-for="item in excludeCurrencies"
              :label="item.label"
              :value="item.value"
              :disabled="item.disabled">
            </el-option>
          </el-select>

          <el-row>
            <!--<h3>Max Number of Edges in Path: </h3>-->
            <div style="margin-top: 10px; margin-bottom: 10px;">
              <el-input-number v-model="numEdges" :min="1" :max="7" @change="handleChange()"></el-input-number>
            </div>
          </el-row>

          <!-- Input field for amount and calculate button-->
          <el-row :gutter="20">
            <el-col :span="16" :offset="4">
              <div style="margin-bottom: 10px;">
                <el-input v-model="inputVal" placeholder="Enter amount..." style="padding: 10px;"></el-input>
              </div>
            </el-col>
          </el-row>

          <el-row :gutter="20">
            <el-button @click="fetchPath()">Calculate</el-button>
          </el-row>
        </div>
      </el-col>
    </el-row>

    <el-row :gutter="20">
      <el-col :span="16" :offset="4" >
        <!-- Only show optimal conversion and path when user clicks calculate -->  
        <div> 
          <h2 v-if="shouldShow === true">Path: {{ optPath }} </h2>
          <h3 v-if="shouldShow === true">Direct Rate: {{ regRate | round 10 }} </h3>
          <h3 v-if="shouldShow === true">Optimal Rate: {{ optRate | round 10 }} </h3>
          <h2 v-if="shouldShow === true">Direct conversion: {{ regVal | round 2 }}</h2>
          <h2 v-if="shouldShow === true">Optimal conversion: {{ optVal | round 2 }}</h2>
          <h2 v-if="shouldShow === true">Profit: {{ profit | round 4 }}</h2>
          <h2 v-if="shouldShow === true">Percent Return: {{ percReturn | round 4 }}%</h2>
        </div>
      </el-col>
    </el-row>
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
      type: 'forex',
      ifBank: '0',
      start: '',
      end: '', 
      numEdges: null,
      exclude: [],
      regRate: null,
      inputVal: null,
      apiData: null,
      cyclesData: null,
      cyclesArr: [],
      numCycles: null,
      shouldShow: false
    }
  }, 
  methods: {
    fetchPath() {
      // formatting for string for 
      var regStr = "http://currencyhft.com:3000/calculatorData/" + this.start + "/" + this.end;
      var arbStr = "http://currencyhft.com:3000/arbitrageData/" + this.start + "/" + this.end + "/" + this.numEdges.toString() + '/' + this.type;
      var proStr = "http://currencyhft.com:3000/profitablePathsData/" + this.numCycles + "/" + this.type;

      // formatting for excluded array parameter
      var exclude = this.exclude;
      for (var i = 0; i < exclude.length; ++i){
        if (i === 0){
          arbStr = arbStr + "?" + "exclude=" + exclude[i];
        } else {
          arbStr = arbStr + "&" + "exclude=" + exclude[i];
        }
      }

      // fetching optimal rate
      axios.get(arbStr).then( (response) => {
        console.log(response);
        this.shouldShow = true;
        this.apiData = response.data;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })

      // fetching optimal cycles
      /*axios.get(proStr).then( (response) => {
        console.log(response);
        this.cyclesData = response.data;
        console.log(this.cyclesData);
        //this.shouldShow = true;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })*/

      // fetching regualer rate
      axios.get(regStr).then( (response) => {
        console.log(response);
        this.regRate = response.data.rate;
      }).catch( (error) => {
        console.log("ERROR:", error);
      })
    }, 
    handleChange() {
      this.apiData = null;
      this.cyclesData = null;
      this.shouldShow = false;
    },
    handleRadio() {

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
          retStr+=" -> "
        }
      }
      return retStr;
    },
    optCycles() {
      var retArr = [];
      var retStr = "";
      console.log(this.cyclesData)
      if (this.cyclesData !== null){
        for(var j = 0; j < this.cyclesData.length; j++){
          retArr[j] = this.cyclesData[j];
          retStr += j + ". ";
          for(var k = 0; k < this.cyclesData[j].currencies.length; k++){
            retStr += this.cyclesData
          }
        }
      }
      console.log(retStr);
      return retArr;
    },
    optVal() {
      return this.optRate * this.inputVal;
    },
    regVal() {
      return this.regRate * this.inputVal;
    },
    profit() {
      return (this.optRate * this.inputVal) - (this.regRate * this.inputVal);
    },
    percReturn() {
      if(!this.inputVal){
        return 0;
      }
      return (this.optRate - this.regRate) * 100;
    }
  }, 
  props: ["testProps"]
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.arbitrage{
  margin:0;
  padding:0;
  min-height: 100%;
  height: 830px;
  box-sizing: border-box;
}

h1{
  font-weight: normal;
  font-family: Didot, "Didot LT STD", "Hoefler Text", Garamond, "Times New Roman", serif;
  color: #0c5a2f;
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
