// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import ElementUI from 'element-ui';
import VueNVD3 from 'vue-nvd3';
import d3 from 'd3';
import locale from 'element-ui/lib/locale/lang/en';
import 'element-ui/lib/theme-default/index.css';
import '../theme/menu.css';
import '../theme/menu-item.css';
import '../theme/select.css';
import '../theme/input.css';
import '../theme/input-number.css';
import '../nvd3_styles.css';
import axios from 'axios';
import Vue2Filters from 'vue2-filters'

// library allowing Ajax calls in Vue components
window.axios = axios;

// Element UI and NVD3 charts
Vue.use(ElementUI, { locale });
Vue.use(VueNVD3);
Vue.use(Vue2Filters);


Vue.config.productionTip = false;

// eslint-disable no-new 
new Vue({
  el: '#app',
  router,
  template: '<App/>',
  components: { App }
})
