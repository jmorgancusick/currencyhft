<template>
  <div class="calculator">
    <h1>{{ msg }}</h1>

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
    <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>

    <h2>Conversion: {{ convertedVal }}</h2>

    <h3>{{nicemsg}}</h3>
  </div>
</template>

<script>
export default {
  name: 'calculator',
  data () {
    return {
      msg: 'Currency Calculator',
      nicemsg: '**future calculator implementation will remove full page view',
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
      start: '',
      end: '', 
      inputVal: '',
      rate: 0.4567
    }
  },
  methods: {
    print() {
      this.$emit("testEvent", {obj: this.data1} );
      this.nicemsg = 'You are awesome!!';
    },
    handleSelect() {
      if (this.start !== '' && this.end !== '') {
        console.log('ready!');
      }
    }
  },
  computed: {
    convertedVal() {
      return this.inputVal * this.rate;
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
