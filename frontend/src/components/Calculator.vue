<template>
  <div class="calculator">
    <h1 style="padding: 30px;">{{ msg }}</h1>

    <!-- Adds dropdown menus for Start and End currencies -->
    <!-- Start -->
    <el-select v-model="start" placeholder="Start currency" @change="handleSelect()">
      <el-option
        v-for="item in startCurrencies"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>

    <!-- End -->
    <el-select v-model="end" placeholder="End currency" @change="handleSelect()">
      <el-option
        v-for="item in endCurrencies"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>

    <!-- Input field for amount --> 
    <el-row :gutter="20">
      <el-col :span="20" :offset="2">
        <el-input v-model="inputVal" placeholder="Enter amount..." @change="handleSelect()"></el-input>
        <h2 v-if="start !== null && end !== null && convertedVal !== 0"> {{inputVal}} {{start}} = {{ convertedVal }} {{end}} </h2>
      </el-col>
    </el-row>   
    

  </div>
</template>

<script>
export default {
  name: 'calculator',
  data () {
    return {
      msg: 'Currency Calculator',
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
      start: null,
      end: null, 
      inputVal: '',
      startVal: 1,
      apiData: null,
      rate: null,
      convertedVal: 0,
      shouldShow: false
    }
  },
  methods: {
    print() {
      this.$emit("testEvent", {obj: this.data1} );
      this.nicemsg = 'You are awesome!!';
    },
    handleSelect() {
      if (this.start !== null && this.end !== null) {
        console.log('ready!');

        // api call formatting
        var str = "http://currencyhft.com:3000/calculatorData/" + this.start + "/" + this.end;

        console.log(str);

        // call to fetch calculator data from api
        axios.get(str).then( (response) => {
          console.log(response);
          this.apiData = response.data;
          this.shouldShow = true;
        }).catch( (error) => {
          console.log("ERROR:", error);
        })

        // seting data object values from api call
        this.rate = this.apiData.rate;
        this.startVal = this.inputVal;
        this.convertedVal = this.startVal * this.rate;
      }
    }, 
    handleInput() {
      this.startVal = this.inputVal;
      this.convertedVal = this.startVal * this.rate;
    }
  },
  computed: {
    /*
      convertedVal() {
      return this.inputVal * this.rate;
    }*/
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

.calculator{
  min-height:100%;
  position:relative;
  height: 830px;
}
h1 {
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
