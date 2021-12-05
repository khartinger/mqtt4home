// ___________router/index.ts___________________________________
import { createRouter, createWebHashHistory, RouteRecordRaw } from 'vue-router'
import Home from '../views/Home.vue'
// import Page2 from '../views/Page2.vue'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'Home',
    component: Home,
    meta: {
      title: 'vue_mqtt3_3webpages-Home'
    }
  },
  {
    path: '/page2',
    name: 'Page2',
    component: () => import('../views/Page2.vue'),
    meta: {
      title: 'vue_mqtt3_3webpages-Home'
    }
  },
  {
    path: '/about',
    name: 'About',
    // route level code-splitting
    // this generates a separate chunk (about.[hash].js) for this route
    // which is lazy-loaded when the route is visited.
    component: () => import(/* webpackChunkName: "about" */ '../views/About.vue'),
    meta: {
      title: 'vue_mqtt3_3webpages-About'
    }
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

router.beforeEach((to, from, next) => {
  document.title = <string> to.meta.title
  next()
})

export default router
