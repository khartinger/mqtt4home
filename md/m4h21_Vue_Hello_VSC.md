Letzte &Auml;nderung: 15.9.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Vue: Erste Anwendung mit Visual Studio Code</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h21_Vue_Hello_VSC_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziele
* Visual Studio Code (kurz "VSC") für Vue vorbereiten.   
* Im Browser ein mit Vue erstelltes "Hello World from Vue!" anzeigen.   
   

## Voraussetzungen
* Etwas Wissen &uuml;ber HTML, CSS und JavaScript :)

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang
* Software: Visual Studio Code, node.js, npm

## Vorbereitung
1. Visual Studio Code installieren.   
   Anleitung zB [https://code.visualstudio.com/docs/setup/setup-overview](https://code.visualstudio.com/docs/setup/setup-overview)   
2. Node.js installieren.   
   Anleitung zB [https://nodejs.org/de/download/](https://nodejs.org/de/download/) oder   
   [https://docs.npmjs.com/downloading-and-installing-node-js-and-npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm)   
3. npm (Node.js package manager) installieren.   
---   
4. Visual Studio Code (VSC) starten   
5. Testen, ob node und npm richtig installiert wurden:   
   `VSC: Menü Terminal – New Terminal... (oder <Strg>+<Shift>+ö)`   
   __`node –-version`__   
   `v14.15.4`   
   __`npm –-version`__   
   `6.14.10`   
6. VSC: Vue Command Line Interface (CLI) installieren (falls noch nicht erfolgt)
   __`npm install -g @vue/cli`__   
   `+ @vue/cli@4.5.11`   
   `updated 3 packages in 31.526s`   
7. VSC: Ventur Erweiterung für Syntax Highlighting installieren (falls noch nicht erfolgt)   
   Extensions-Auswahl öffnen (&lt;Strg&gt;+&lt;Shift&gt;+X)   
   ```   
   <Strg> + <Shift> + X
   Ventur suchen [Install]
   ```   
## HTML-Seite erstellen
1. Visual Studio Code (VSC) starten (falls noch nicht erfolgt).
2. VSC: Ein Terminal-Fenster öffnen (`Menü Terminal - New Terminal`).
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll:   
   `cd /g/github/mqtt4home/source_Vue`   
5. VSC-Terminal: Eine Vue.js Applikation erzeugen, die die Verwendung von TypeScript erlaubt (mit den Cursortasten "Manually select features" auswählen):   
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
   Im nächsten Schritt mit den Cursortasten und der Leertaste folgende Punkte auswählen:   
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
   Vue-Version 3.x auswählen:   
   ```   
   Check the features needed for your project: Choose Vue version, TS, Router, Linter 
   ? Choose a version of Vue.js that you want to start the project with 
     2.x
   > 3.x
   <Enter>
   ```   

   Die weiteren Fragen zB folgendermaßen beantworten (dazwischen immer mit <Enter> bestätigen):   
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
   (babel: transpile JavaScript to HTML, eslint: hilft beim Finden von Code-Fehlern).   
   Das Erstellen der Applikation kann schon ein Weilchen dauern, immerhin werden 196 MB Festplattenspeicher beschrieben ;)   
   ```   
   ...
   Successfully created project vue_mini_vsc.
   Get started with the following commands:
    $ cd vue_mini_vsc
    $ npm run serve
   ```   


   5. VSC: Wechsel ins Projektverzeichnis und Starten des internen Test-Servers:   
   ```   
   cd vue_mini_vsc
   npm run serve
   ```   

5. VSC: Projektordner öffnen   
   `Menü: File – Open Folder... und Verzeichnis vue_mini_vsc wählen`   

6. Applikation anschauen: Browser starten und folgendes eingeben:   
   `localhost:8080`
   Es erscheint folgende Info-Seite:   




---   
---   

## Was ist Vue?
`Vue.js` ist ein beliebtes Framework f&uuml;r Java Script zum Erstellen von Web-Anwendungen. Es hilft bei der Anzeige von Daten kann durch zahlreiche Plugins (zB zur HTTP-Behandlung) erweitert werden.   
Informationen (in Deutsch) gibt es zB auf folgenden Seiten:   
* [https://de.wikipedia.org/wiki/Vue.js](https://de.wikipedia.org/wiki/Vue.js)   
* [https://t3n.de/news/vuejs-1097879/](https://t3n.de/news/vuejs-1097879/)   
   
## Webseite mit Vue erstellen
1. Ein Anwendungsverzeichnis anlegen (zB mit dem Explorer):   
   `/source_Vue/vue_mini`   

2. Das Vue-Framework downloaden zB von [`https://vuejs.org/v2/guide/installation.html`](https://vuejs.org/v2/guide/installation.html)   
"Development Version" (Datei `vue.js`)   

3.  Das Framework bzw. die Datei `vue.js` ins Arbeitsverzeichnis (zB `/source_Vue/vue_mini`) kopieren.   

4. HTML-Seite im Texteditor erstellen:   
   ```   
   <!DOCTYPE html>
   <html>
   <head>
     <title>HelloVue</title>
     <script src="vue.js"></script>            <!--1-->
   </head>
   <body>
     <div id="app">                            <!--2-->
       {{ message }}                           <!--3-->
     </div>

     <script>
       var app = new Vue({                     //<!--4-->
         el: '#app',
         data: {
           message: 'Hello World from Vue!'    //<!--5-->
         }
       })
     </script>
   </body>
   </html>
   ```   
   Speichern zB unter dem Namen `halloVue.htm`   
   Die wichtigsten Zeilen sind:   
   * `<!--1-->` Einbinden der Vue-Funktionen (Vue-Framework)   
   * `<!--2-->` Mit Hilfe dieses &lt;div&gt;-Tags und der Id "app" wird hier der vom Vue-Script erzeugte "Text" eingebaut.   
   * `<!--3-->` Ausgabe des Wertes der Variablen `message`.   
   * `<!--4-->` Erzeugen der Vue-Instanz, wobei bei `el:` die Id steht.   
   * `<!--5-->` Erzeugen der Variablen `message` und Zuweisung des Textes.   

5. Die Datei `halloVue.htm` im Browser (zB Chrome) &ouml;ffnen.   
   Mit der Taste &lt; F12&gt; kann der Debug-Mode angezeigt werden.   
   ![Erstes Vue-Programm](./images/210915_HelloVue1.png "Erstes Vue-Programm")   
   _Bild 1: Erstes Vue-Programm_   
   Im Bild 1 sieht man links die Ausgabe der HTML-Seite und rechts erkennt man im HTML-Code, dass der Inhalt des `<div>`-Tags durch den Wert der Variablen `message` ersetzt wurde.   
