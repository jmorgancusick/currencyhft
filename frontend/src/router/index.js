import Vue from 'vue'
import Router from 'vue-router'
import Home from '@/components/Home'
import Dashboard from '@/components/Dashboard'
import Calculator from '@/components/Calculator'
import Arbitrage from '@/components/Arbitrage'
import Profit from '@/components/Profit'

Vue.use(Router)

// Vue router for handling navigation
export default new Router({
  // pages that are acceessible from NavBar
  routes: [
    {
      path: '/',
      name: 'home',
      component: Home
    },
    {
      path: '/dashboard',
      name: 'dashboard',
      component: Dashboard
    }, 
    {
      path: '/calculator',
      name: 'calculator',
      component: Calculator
    }, 
    { 
      path: '/arbitrage',
      name: 'arbitrage',
      component: Arbitrage
    },
    {
      path: '/profit',
      name: 'profit',
      component: Profit
    }
  ]
})
