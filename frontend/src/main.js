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

import axios from 'axios';

window.axios = axios;

Vue.use(ElementUI, { locale });
Vue.use(VueNVD3);

Vue.config.productionTip = false;

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  template: '<App/>',
  components: { App }
})
