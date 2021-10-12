Last modified: 2021-09-15   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Vue: First application with Visual Studio Code</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h502_Vue_Hello_VSC.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Prepare Visual Studio Code (VSC for short) for Vue.   
* Display a "Hello World from Vue!" created with Vue in the browser.   
   

## Prerequisites
* Some knowledge about HTML, CSS and JavaScript :)

## Required tools
* Hardware: PC or laptop with Internet access
* Software: Visual Studio Code, node.js, npm

## Preparation
1. install Visual Studio Code.   
   Instructions e.g. [https://code.visualstudio.com/docs/setup/setup-overview](https://code.visualstudio.com/docs/setup/setup-overview).   
2. install Node.js.   
   Instructions e.g. [https://nodejs.org/de/download/](https://nodejs.org/de/download/) or   
   [https://docs.npmjs.com/downloading-and-installing-node-js-and-npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm)   
3. Install npm (Node.js package manager).   
---   
4. start Visual Studio Code (VSC)   
5. test if node and npm are installed correctly:   
   `VSC: Terminal menu - New Terminal... (or <Ctrl>+<Shift>+ö)`   
   __`node --version`__   
   `v14.15.4`   
   __`npm --version`__   
   `6.14.10`   
6. install VSC: Vue Command Line Interface (CLI) (if not already done)
   __`npm install -g @vue/cli`__   
   `+ @vue/cli@4.5.11`   
   `updated 3 packages in 31.526s`   
7. install VSC: Ventur extension for syntax highlighting (if not already done)   
   open extension selection (&lt;Ctrl&gt;+&lt;Shift&gt;+X)   
   ```   
   <Ctrl> + <Shift> + X
   Search Ventur [Install]
   ```   
## Create HTML page
1. start Visual Studio Code (VSC) (if not already done).
2. VSC: Open a terminal window (`Menu Terminal - New Terminal`).
3. VSC terminal: Change to the folder under which the Vue project should be created:   
   `cd /g/github/mqtt4home/source_Vue`.   
4. VSC terminal: Create a Vue.js application (select "Manually select features" with the cursor down button):   
   __`vue create vue_mini_vsc`__   
   ```   
   Vue CLI v4.5.11
   ? Please pick a preset:
     Default ([Vue 2] babel, eslint)
     vue3-typescript ([Vue 3] babel, typescript, router, eslint) 
     Default (Vue 3 Preview) ([Vue 3] babel, eslint)
   > Manually select features
   <Enter>
   ```   
   In the next step, use the cursor keys and the space bar to select the following items:   
      ```   
   ? Check the features needed for your project: (mit Cursor- und Leertaste auswählen)
    (*) Choose Vue version
    (*) Babel
    (*) TypeScript
    ( ) Progressive Web App (PWA) Support        
    (*) Router
    ( ) Vuex
    ( ) CSS Pre-processors
   >(*) Linter / Formatter
    ( ) Unit Testing
    ( ) E2E Testing
   <Enter>
   ```   
   Select Vue version 3.x:   
   ```   
   Check the features needed for your project: Choose Vue version, TS, Router, Linter 
   ? Choose a version of Vue.js that you want to start the project with 
     2.x
   > 3.x
   <Enter>
   ```   

   Answer the other questions e.g. as follows (in between always confirm with &lt;Enter&gt;):   
   ```   
   Vue CLI v4.5.13
   ? Please pick a preset: Manually select features
   ? Check the features needed for your project: Choose Vue version, TS, Router, Linter 
   ? Choose a version of Vue.js that you want to start the project with 3.x 
   ? Use class-style component syntax? No
   ? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)? No
   ? Use history mode for router? (Requires proper server setup for index fallback in production) No
   ? Pick a linter / formatter config: ESLint + Standard config
   ? Pick additional lint features: Lint on save
   ? Where do you prefer placing config for Babel, ESLint, etc.? In dedicated config files
   ? Save this as a preset for future projects? (y/N) N
   <Enter>
   ```   
   (babel: transpile JavaScript to HTML, eslint: helps to find code errors).   
   Building the application can take a while, after all about 200 MB of disk space are written ;)   
   ```   
   ...
   Successfully created project vue_mini_vsc.
   Get started with the following commands:
    $ cd vue_mini_vsc
    $ npm run serve
   ```   

5. VSC: Change to the project directory and start the internal test server:   
   ```   
   cd vue_mini_vsc
   npm run serve
   ```   

6. VSC: Open project folder   
   `Menu: File - Open Folder... and select directory vue_mini_vsc`.   

7. view application: Start browser and enter the following:   
   `localhost:8080`   
   The following info page appears:   
   ![Vue startup image](./images/210915_vue_startpage.png "Vue startup image")   
   _Fig. 1: Vue start page_

8. Comment out the start image and output the text "Hello World from Vue!   
   * VSC: Show Explorer (CTRL+SHIFT+E).   
   * VSC: Click on file src - views - Home.vue and change it:   
   ```   
   <template>
   Hello World from Vue!
   <!--
   <div class="home">
       <img alt="Vue logo" src="../assets/logo.png">
       <HelloWorld msg="Welcome to Your Vue.js + TypeScript App"/>
   </div>
   -->
   </template>
   ```   
   * VSC: After saving the file, the display in the browser changes immediately!

9. comment out the navigation links (Home | About):
   * VSC: Click file src - views - App.vue and change:   
   ```   
   <template>
     <div id="nav">
   <!--
       <router-link to="/">Home</router-link> |
       <router-link to="/about">About</router-link>
   -->
     </div>
     <router-view/>
   </template>
   ```   
   * VSC: After saving the file immediately changes the display in the browser!   

If the display should be left-aligned, the style must be changed accordingly in `App.vue`:   
` text-align: left; /* center; */`   
