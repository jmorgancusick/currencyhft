import Vue from 'vue'
import Router from 'vue-router'
import Home from '@/components/Home'
import Dashboard from '@/components/Dashboard'
import Arbitrage from '@/components/Arbitrage'


Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'home',
      component: Home
    },
    {
      path:'/dashboard',
      name: 'dashboard',
      component: Dashboard
    }, 
    {
      path:'/arbitrage',
      name: 'arbitrage',
      component: Arbitrage
    }
  ]
})
