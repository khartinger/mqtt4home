Letzte &Auml;nderung: 3.10.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Vue: Senden und Empfangen von MQTT-Nachrichten</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h23_Vue_Mqtt1_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>
  
## Ziel
* Erstellen einer Vue-Anwendung, mit der MQTT-Nachrichten gesendet und abonniert bzw. angezeigt werden können:   
![MQTT-Sende-/Empfangsprogramm](./images/211003_vue_mqtt1_start.png "MQTT-Sende-/Empfangsprogramm")   
_Bild 1: Startseite des MQTT-Sende- und Empfangsprogramms_   

__Das Programm soll aus 5 Teilen bestehen:__   
1. Ein MQTT Client, der die MQTT Funktionen zur Verfügung stellt.   
2. Eine GUI-Komponente, mit der man sich mit einem Broker verbinden kann.   
   Dabei kann der Host, auf dem der Broker läuft (zB IP 10.1.1.1) und der Port für die WebSocket-Verbindung (zB 1884) in Eingabefeldern eingegeben werden.   
3. Eine GUI-Komponente, mit deren Hilfe man eine Nachricht versenden kann.   
   Es ist die Eingabe von Topic, Payload, ob die Nachricht gespeichert werden soll ("retain") und vom QoS-Level (Quality of Service) möglich.   
4. Eine GUI-Komponente, mit der man ein Topic abonnieren ("subscribe") kann.   
5. Eine GUI-Komponente, die für das abonnierte Topic die letzten 5 Nachrichten anzeigt.   

## Voraussetzungen
* Etwas Wissen &uuml;ber HTML, CSS und JavaScript/Typescript :)   
* Visual Studio Code, das für Vue-Anwendungen bereits vorbereitet ist.   
   (Dh. es wurde bereits (mindestens) eine Vue-Anwendung in Visual Code erstellt.)   

## Erforderliche Hilfsmittel
* Hardware: PC oder Laptop mit Internetzugang, Browser
* Software: Visual Studio Code, node.js, npm


## Vorbereitung des Vue-Projektes in VSC (Kurzfassung)   
1. Visual Studio Code (VSC) starten.   
2. VSC: Terminal-Fenster &ouml;ffnen: `Men&uuml; Terminal - New Terminal`.   
3. VSC-Terminal: In den Ordner wechseln, unter dem das Vue-Projekt erzeugt werden soll: `cd /g/github/mqtt4home/source_Vue`   
4. VSC-Terminal: Vue.js Applikation erzeugen: `vue create mqtt1`  
   Mit Cursortasten, Leertaste und &lt;Enter&gt; Folgendes auswählen:   
   `> Manually select features`   
   `(*) Choose Vue version`   
   `(*) Babel`   
   `(*) TypeScript`   
   `(*) Router`   
   `(*) Linter / Formatter`   
   `> 3.x`   
   `? Use class-style component syntax? No`   
   `? Use Babel alongside TypeScript (required for modern mode, auto-detected polyfills, transpiling JSX)? No`   
   `? Use history mode for router? (Requires proper server setup for index fallback in production) No`   
   `? Pick a linter / formatter config: ESLint + Standard config`   
   `? Pick additional lint features: Lint on save`   
   `? Where do you prefer placing config for Babel, ESLint, etc.? In dedicated config file`   
   `? Save this as a preset for future projects? (y/N) N`   
5. In den Projektordner wechseln: VSC Menü Datei - Ordner öffnen..
6. MQTT Bibliothek installieren: `npm install mqtt --save`   


## Erstellung des Projektes in VSC

