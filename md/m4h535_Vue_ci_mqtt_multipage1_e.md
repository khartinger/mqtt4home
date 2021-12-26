
Last modified: 2021-12-26 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>How to ...</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h535_Vue_ci_mqtt_multipage1.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_Vue/vue10_ci_mqtt_multipage1">==> Code @ GitHub</a><hr>

## Target
This Vue project shows how to create a web application with three pages:   
1. Main page ("Home") : display of a lamp and a button that can be used to switch the lamp.   
2. History page ("Page2"): representation of the last five MQTT commands received.   
3. About page ("About"): info about the application.   

The three pages look like this:   

![main page multi-webpages project](./images/vue_mqtt3_3webpages_home.png "main page two-webpages project")   
_Fig. 1: Main page of the multi-webpages project (Home)_   

---   

![History page](./images/vue_mqtt3_3webpages_page2.png "History page")   
_Fig. 2: History page (Page2)_   

---   

![about-page](./images/vue_mqtt3_3webpages_about.png "history-page")   
_Fig. 3: About page (About)_   

## Required tools
* Hardware: PC or laptop with internet access, browser
* Hardware: Raspberry Pi (or PC) running an MQTT broker (e.g. Mosquitto)
* Software: Visual Studio Code ("VSC"), which is already prepared for Vue applications.   
   (I.e. (at least) one Vue application has already been created in Visual Code).   

# Creating the project
## 1. Overview of the files involved
![Overview Files](./images/vue35_ci_mqtt_multipage1_files_1.png "Overview Files")   
_Fig. 4: Overview of the involved files_   

## 2. Preparation of the Vue project in VSC   
1. Start Visual Studio Code (VSC).   
2. VSC: Open terminal window: Menu Terminal - New Terminal.   
3. VSC terminal: Change to the folder under which the Vue project is to be created:   
   `cd /g/github/mqtt4home/source_Vue`   
4. VSC-Terminal: Create Vue.js application: `vue create vue35_ci_mqtt_multipage1`  
   Use cursor keys, space bar and &lt;Enter&gt; to select the following:   
   `> Manually select features` &nbsp; &lt;Enter&gt;   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `(*) Router`   
   `(*) Linter / Formatter`   
   &lt;Enter&gt;   
   _`> 3.x`_ &nbsp; &lt;Enter&gt;      
   _`? Use class-style component syntax`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)?`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Use history mode for router? (Requires proper server setup for index fallback in production)`_ &nbsp; __`N`__ &lt;Enter&gt;   
   _`? Pick a linter / formatter config:`_ &nbsp; __`ESLint + Standard config`__ &lt;Enter&gt;   
   _`? Pick additional lint features: `_ &nbsp; __`Lint on save`__ &lt;Enter&gt;   
   _`? Where do you prefer placing config for Babel, ESLint, etc.?`_ &nbsp; __`In dedicated config file`__ &lt;Enter&gt;   
   _`? Save this as a preset for future projects? (y/N)`_ &nbsp; __`N`__ &lt;Enter&gt;   
5. Switch to the project folder: _VSC Menu File - Open Folder..
   `/github/mqtt4home/source_Vue/vue35_ci_mqtt_multipage1` [select folder].   
6. Install MQTT library:   
   VSC: Open terminal window: Menu Terminal - New Terminal.   
   `npm install mqtt --save`   
&nbsp;   

## 3. Customizing the automatically created files
### 3.1 Complementing the Vue configuration   
Create the file `vue.config.js`: click on the plus next to `VUE35_CI_MQTT_MULTIPAGE1`, enter name.   

_Contents of the file_:   

```   
// ______vue.config.js__________________________________________
module.exports = {
  lintOnSave: false,
  publicPath: './',
  // publicPath: process.env.NODE_ENV === 'production' ? './vue_pubsub2/' : './',
  configureWebpack: {
    devtool: 'source-map'
  },
  chainWebpack: config => {
    config.performance
      .maxEntrypointSize(400000)
      .maxAssetSize(400000)
  }
}

```   

With `publicPath: './',` the relative path is set and the `chainWebpack` entry avoids warnings about the file size (by setting the maximum file size higher ;) ).

### 3.2 Disable linter warning "Unexpected any" for "(value: any)".    
  In the file `.eslintrc.js` under "`rules: {`" add:   
  ```   
  '@typescript-eslint/no-explicit-any': 'off',
  '@typescript-eslint/explicit-module-boundary-types': 'off',
  ```   

### 3.3 Create file for page "Page2   
Left click on `src/views/About.vue`, copy the file with &lt;strg&gt; c and &lt;strg&gt; v and rename the copy to `Page2.vue` (e.g. with the right mouse button help menu).   

### 3.4 Customizing the "routes" in the automatically created file `router/index.ts`:   

_Change the content to:_

```   
// ___________router/index.ts___________________________________
import { createRouter, createWebHashHistory, RouteRecordRaw } from 'vue-router'
import Home from '../views/Home.vue'
import Page2 from '../views/Page2.vue'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'Home',
    component: Home,
    meta: {
      title: 'vue35_ci_mqtt_multipage1-Home'
    }
  },
  {
    path: '/page2',
    name: 'Page2',
    component: Page2,
    meta: {
      title: 'vue35_ci_mqtt_multipage1-Page2'
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
      title: 'vue35_ci_mqtt_multipage1-About'
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

```   

### 3.5 Customizing the `App.vue` file   
The `src/App.vue` file is responsible for the following items:   
  * Displaying a link line with links to the individual pages.   
    Therefore: Add the line `<router-link to="/page2">Page2</router-link> |`.   
  * Establishing the connection to the broker.   
    This should not be done in the "Home" page, otherwise the connection will be re-established every time the page is loaded.   
  * The state of the connection is displayed in the link line.   
  * Definition of uniform styles for all pages.   
    Therefore: Add to the styles that start with a dot.   

_Content of the file_:   

```   
<!--App.vue-->
<template>
  <div id="nav">
    <router-link to="/">Home</router-link> |
    <router-link to="/page2">Page2</router-link> |
    <router-link to="/about">About</router-link>
    &nbsp; &nbsp; &nbsp;
    <span v-if="isConnected">
      <b><font color="lightgreen">Connected to {{ getConnectUrl }}</font></b>
    </span>
    <span v-if="!isConnected && !isConnecting">
      <b><font color="red"> Not connected! &nbsp; </font></b>
      <button @click="reconnect()">Reconnect</button>
    </span>
    <span v-if=isConnecting>
      <b><font color="red"> Is connecting... &nbsp; </font></b>
      &nbsp; <button @click="cancel()">Cancel</button>
    </span>
  <router-view/>
</div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'

export default defineComponent({
  name: 'Home',
  components: {
  },
  computed: {
    isConnected: function (): boolean {
      return ciMqttClientInstance.mqttState.connected
    },
    isConnecting: function (): boolean {
      if (ciMqttClientInstance.mqttState.iConnMqttState === 2) return true
      return false
    },
    isSubscribed: function (): boolean {
      return ciMqttClientInstance.mqttSubscription.subscribed
    },
    getMqttState: function (): string {
      return ciMqttClientInstance.sConnMqttState()
    },
    getConnectUrl: function (): string {
      return ciMqttClientInstance.connectUrl()
    }
  },
  methods: {
    reconnect: async function (): Promise<void> {
      ciMqttClientInstance.reconnectBroker()
    },
    end: async function (): Promise<void> {
      ciMqttClientInstance.disconnect()
    },
    cancel: async function (): Promise<void> {
      ciMqttClientInstance.disconnect()
    }
  }
})

</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

#nav {
  padding: 3px;
}

#nav a {
  font-weight: bold;
  color: #2c3e50;
}

#nav a.router-link-exact-active {
  color: #42b983;
}

  .ciFont1   { font-size: 11px; font-weight: bold; font-family: monospace; fill: black; white-space: pre; }
  .fontOK    { font-weight: bold; fill: lightgreen; }
  .fontNOK   { font-weight: bold; fill: red; }
  .cursor    { cursor: pointer; }
  .ciBackground {fill: #ddFFdd; }
  .ciOut     { fill: yellow; stroke: yellow; stroke-width: 1; }
  .ciInColor { fill: #F4F4F4; }
  .ciIn      { fill: #F4F4F4; stroke:blue; stroke-width: 1; }
  .ciLine    { stroke: blue; stroke-width: 1; }
  .ciRect    { fill: none; stroke: blue; stroke-width: 1; }
  .ciFillIt  { stroke: blue; stroke-width: 1; }
  .ciClick   { fill-opacity: 0.0; stroke-width: 1; }
  .ciBorder  { fill-opacity: 0.0; stroke: blue; stroke-width: 1; }
</style>
```   

### 3.6 Deleting files and directories that are not needed   
  * Delete the file `components/HelloWorld.vue   
  * delete directory `assets   

## 4. Add MQTT client
## 4.1 Include the required files
* Create the directory `controller   
  Right click on the directory `src`, select "New folder" and enter the name `controller`   

* Creating the "services" directory   
  Right-click on the `src` directory, select "New folder" and enter the name `services`   

* Copy the files [`CiMqttClient.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/services/CiMqttClient.ts) and [`CiMqttClientInstance.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/services/CiMqttClientInstance.ts) to the `src/services` directory.   

* Copying the [`CiBaseController.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/controller/CiBaseController.ts) file to the `src/controller` directory.   

* Copy the [`CiBase.vue`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/components/CiBase.vue) file to the `src/components` directory.   

## 4.2 Customizing the file CiMqttClientInstance
* Specify that when the app is started with the default values, it should connect to the broker and subscribe to the topic (constructor value `true`).   

* Enter all controllers that should send or receive Mqtt messages.   

_Result:_   

```  
// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButton2Controller } from '@/controller/CiButton2Controller'
import { ciMqttLastXController } from '@/controller/CiMqttLastXController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciButton2Controller)
ciMqttClientInstance.registerController(ciMqttLastXController)

```   

## Create start page
## Preparation
The start page ("Home") contains the two controller/display elements (CI elements) "CiLamp" and "CiButton2", each consisting of a controller (`.ts`) and the display (`.vue`).   

The controllers have to be copied into the directory `/src/controller`:   
* [`controller/CiLampController.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/controller/CiLampController.ts): Definition of the lamp properties, realization of the `onMessage` and `publish` methods (`publishCi`) and creation of an object `ciLampController`.   
* [`controller/CiButton2Controller.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/controller/CiButton2Controller.ts): Definition of the switch properties, realization of the `onMessage` and `publish` methods (`publishCi`) and creation of an object `ciButton2Controller`.   

The display files must be copied into the directory `/src/components`:   
* [`components/CiLamp.vue`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/components/CiLamp.vue): Display of the lamp symbol and realization of the function `onClk`, which is executed when the symbol is clicked.   
* [`components/CiButton2.vue`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/components/CiButton2.vue): Representation of the switch symbol and realization of the `onClkON` and `onClkOff` functions, which are executed when clicking on the "ON" and "OFF" areas of the symbol.   

## Code start page
The elements "CiLamp" and "CiButton2" are included in the `src/views/Home.vue` page in the following way:   

```   
<!--src/views/Home.vue-->
<template>
    <svg viewBox="0 0 230 120" width="100%">
    <rect class="ciBackground" x="0" y="0" width="230" height="120" />
    <CiLamp    :x="60" :y="60" sid="lamp_1" :border="0"></CiLamp>
    <CiButton2 :x="170" :y="60" sid="button2_1" lines="0" :border="0"></CiButton2>
    </svg>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import CiLamp from '@/components/CiLamp.vue'
import CiButton2 from '@/components/CiButton2.vue'

export default defineComponent({
  name: 'Home',
  components: {
    CiLamp,
    CiButton2
  }
})
</script>
```   

* First, an svg-viewbox is created that represents the background of the page. The size ("0 0 230 120") is chosen so that the two elements have exactly enough space.   
* The lamp needs as parameters the center x and y and the ID. With `:border="0"` the drawing of a border is prevented. Since the number of lines (`lines=`) is not specified, the default `lines="1"` is used and the name of the lamp is displayed.   
* The switch needs as parameters the center x and y and the ID. With `:border="0"` the drawing of a border and with `lines="0"` the display of further information is prevented.   

## Create page "Page2
## Preparation
The page "Page2" uses the component `CiMqttLastX` to display the last f&uuml;nf received MQTT messages and consists of the following three parts:   
* [`components/CiMqttLastX.vue`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/components/CiMqttLastX.vue): Representation of the last (five) messages in the form of a table.   
* [`controller/CiMqttLastXController.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/controller/CiMqttLastXController.ts): Definition of `CiMqttLastXController` class, which uses `onMessage` method to write the received message to MessageStore memory.   
* [`store/CiMessageStore.ts`](https://github.com/khartinger/mqtt4home/blob/main/source_Vue/vue35_ci_mqtt_multipage1/src/store/CiMessageStore.ts): Encode and export the `messages` store and the `addMessage` and `clearMessage` methods.   

## Encode Page2
These elements are added to the `src/views/Page2.vue` page in the following way:   

```   
<!--views/Page2.vue-->
<template>
  <div class="about">
    <h2>Page2: Last messages received</h2>
    <CiMqttLastX></CiMqttLastX>
    <hr>
  </div>
</template>

<script lang="ts">
// @ is an alias to /src
import { defineComponent } from 'vue'
import CiMqttLastX from '@/components/CiMqttLastX.vue'
export default defineComponent({
  name: 'Home',
  components: {
    CiMqttLastX
  }
})
</script>
```   

# 7. Create "About" page
The "About" page outputs text and shows how to create a link to another page within an SVG graphic.   

```   
<!-- src/views/About.vue -->
<template>
  <div class="about">
    <h2>About: vue_mqtt3_3webpages</h2>
    Purpose: This MQTT sample project shows how a Vue app with multiple
    pages is built.<br>
    Version: 2021-12-26<br><br>
  </div>
  <svg viewBox="0 0 230 36" width="100%">
  <rect class="ciBackground" x="0" y="0" width="230" height="36" />
  <rect @click="onClk()" x="65" y="3" rx=6 ry=6 width="100" height="30" fill="#FFF831" stroke="gold" stroke-width="4"/>
  <text @click="onClk()" x="100" y="27" textLength="30" lengthAdjust="spacingAndGlyphs" class="fontAbout">OK</text>
</svg>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
export default defineComponent({
  components: {
  },
  methods: {
    onClk: function () {
      console.log('CLICK - About: onClk!', '')
      this.$router.push({name: 'Home'})
    }
  }
})
</script>

<style scoped>
 .fontAbout { font: bold 24px sans-serif; fill: red; cursor: pointer; }
</style>
```   

## 8. testing the application
## Requirements for the test   
1. the Raspberry Pi (RasPi) has been installed according to the [manual (section "Central")](../../LIESMICH.md), i.e. on the RasPi with IP '10.1.1.1' the broker program (Mosquitto) is running   
2. the Vue application is loaded in Visual Studio Code (VSC) and the internal server is running (input in terminal: `npm run serve`).   
3. there is a network/WLAN connection between the computer running VSC and the RasPi.   
4. a command/terminal window is open on the PC or Raspberry Pi showing received MQTT messages (enter `mosquitto_sub -h 10.1.1.1 -t "#" -v`).   

Note: If the RasPi has a different IP than 10.1.1.1, the IP must be adjusted in the `App.vue` file (`const hostip = '...'`).   
## Test   
* If you enter the address `localhost:8080` in the browser, _picture 1_ appears in the brower.   

  * The lamp symbol is colored blue, because the lamp status is unknown.   

  * In the 1st line should be "Connected to ws://10.1.1:1884".   
    If it says "Not connected!" the app could not connect to the broker. There could be ewin WLAN error or the broker IP 10.1.1.1 is not correct (change it to `src/services/CiMqttClient.ts`).

* Clicking "ON" sends an MQTT message `ci/lamp/1/set/lamp` with payload `1`.   
The message is displayed in the command/terminal window and the lamp icon turns yellow.   

* Clicking "OFF" sends an MQTT message `ci/lamp/1/set/lamp` with the payload `0`.   
The message is displayed in the command/terminal window and the lamp icon turns gray.   

* Clicking on the lamp symbol sends an MQTT message `ci/lamp/1/set/lamp` with the payload `-1`.   
The message is displayed in the command/terminal window and the lamp icon turns blue.   

---   
 * If you click on "Page2" in the first line, the messages received so far are displayed. If more than 5 messages should be displayed, this can be changed in `src/store/CiessageStore.ts` (line `const maxSize = 5`).   

* If you click on "About" in the first line, you will see some info and a big button "OK". If you click on this button, you will get back to the start page.   

[To the top of the page](#up)
