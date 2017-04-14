<template>
  <div class="chart">
    <h2>Line chart</h2>
    <div class="uk-margin-bottom">
      <vn-line :model="trends"
               :x-format="formatDate"
               y-format=",f">
      </vn-line>
    </div>
  </div>
</template>

<script>
  import Faker from 'Faker'
  import _ from 'lodash'
  import d3 from 'd3'

  export default {
    data () {
      
      return {
        trafficData: _.map(days, d => gen_item(month, d + 1))
      }
    },
    computed: {
      trends () {
        return [
          {
            key: 'high',
            area: true,
            values: _.map(this.chartData, (t) => {
              return {
                x: t.date,
                y: t.high
              }
            })
          },
          {
            key: 'low',
            bar: true,
            values: _.map(this.chartData, (t) => {
              return {
                x: t.date,
                y: t.low
              }
            })
          },
          {
            key: 'close',
            area: true,
            values: _.map(this.chartData, (t) => {
              return {
                x: t.date,
                y: t.close
              }
            })
          }
        ]
      }
    },
    methods: {
      formatDate (d){
        return d3.time.format('%x')(new Date(d))
      }
    }
  }
</script>