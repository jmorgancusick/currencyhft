<template>
  <div class="arbitrage">
    <!-- <h1>{{ testProps }}</h1> -->
    <h1> {{msg}} </h1>

    <!-- Adds dropdown menus for Start and End currencies -->
    <!-- Start currency -->
    <el-select v-model="start" placeholder="Start currency">
      <el-option
        v-for="item in startOptions"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>
    <!-- End currency -->
    <el-select v-model="end" placeholder="End currency">
      <el-option
        v-for="item in endOptions"
        :label="item.label"
        :value="item.value"
        :disabled="item.disabled">
      </el-option>
    </el-select>

    <!-- Input field for amount -->
    <el-input v-model="inputVal" placeholder="Enter amount..."></el-input>
    <h2>Conversion : {{ inputVal }}</h2>

    <el-button @click="print()">Calculate</el-button>
  </div>
</template>

<script>
export default {
  name: 'arbitrage',
  data () {
    return {
      msg: 'Arbitrage', 
      startOptions: [{
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
      endOptions: [{
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
      inputVal: null,
      optRate: .4567,
      optVal: null
    }
  },  
  methods: {
    print() {
      this.$emit("testEvent", {obj: this.data1} )
      this.nicemsg = 'You are awesome!!'
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
