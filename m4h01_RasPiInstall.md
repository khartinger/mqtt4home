Letzte Änderung: 31.7.2021
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Grundinstallation des Raspberry Pi</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h01_RaspiInstall_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RaspiAccessPoint.md">==> RasPi Access Point</a>
</td></tr></table><hr>

## Ziele

* Installation des Betriebssystems "Raspberry Pi OS" auf dem RasPi.   
Im Projekt werden folgende Einstellungen gewählt, die aus Sicherheitsgründen geändert werden sollten:   

|           |                           |   
| --------- | ------------------------- |   
| Hostname: | `raspi` (statt raspberry) |   
| Username: | `pi_`   (statt pi)        |   
| Passwort: | `pass`  (statt raspberry) |   
   

## Benötigte Hilfsmittel

* Hardware: RasPi (incl. Zubehör), Micro-SD-Kartenleser   
* Software: [Programmiersoftware für das Betriebssystem ("imager")](https://www.raspberrypi.org/software)
* Software: Terminal-Programm [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   

 
## Vorgangsweise
### Betriebssystem auf die SD-Karte kopieren
1. Micro-SD-Karte (Größe 8GB oder mehr) in den Kartenleser einlegen, den Kartenleser am PC anschließen.   
2. Herunterladen des Hilfsprogramms zum Beschreiben der SD-Card unter Windows.   
 Siehe [http://www.raspberrypi.org/software](http://www.raspberrypi.org/software). Download von "`imager_1.6.2.exe`".   
3. Programmier-Programm starten und Betriebssystem-Image direkt auf die Micro-SD-Karte schreiben.   

### Erste Inbetriebnahme des RasPi
Beim ersten Start des RasPi müssen einige Einstellungen vorgenommen werden. Diese können später auch geändert werden.   
1. Micro-SD-Karte in das RasPi einlegen.   
2. Bildschirm, Tastatur, Maus, Netzwerkkabel am RasPi anschließen.   
3. Betriebsspannung anlegen: RasPi startet im Konfigurationsmodus.   
4. Betriebssystem "Raspberry Pi OS" konfigurieren:   
Land wählen, neues Passwort `pass` eingeben. In dieser Phase noch kein WiFi-Netzwerk wählen.   
Software-Update starten, RasPi danach neu starten.   

### Erste Einstellungen auf dem RasPi

* __Schnittstellen freischalten__   
Am Grafik-Bildschirm links oben [Menü] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [Schnittstellen]   

|                   |                    |                      |   
| ----------------- | ------------------ | -------------------- |
| Kamera:           | (&nbsp;) Aktiviert | (*) Deaktiviert      |   
| SSH:              | (*) Aktiviert      | (&nbsp;) Deaktiviert |   
| VNC:              | (&nbsp;) Aktiviert | (*) Deaktiviert      |   
| SPI:              | (*) Aktiviert      | (&nbsp;) Deaktiviert |   
| I2C:              | (*) Aktiviert      | (&nbsp;) Deaktiviert |   
| Serial Port:      | (*) Aktiviert      | (&nbsp;) Deaktiviert |   
| Serial Console:   | (&nbsp;) Aktiviert | (*) Deaktiviert      |   
| Eindraht-Bus:     | (&nbsp;) Aktiviert | (*) Deaktiviert      |   
| GPIO-Fernzugriff: &nbsp; | (&nbsp;) Aktiviert | (*) Deaktiviert      |   
[OK] anklicken aber noch nicht neu starten: [Nein] anklicken.
&nbsp;   
* __Passwort und Hostname ändern, automatische Anmeldung abschalten__   
Am Grafik-Bildschirm links oben [Menü] anklicken -  Einstellungen - Raspberry-Pi-Konfiguration - [System]   

|           |                        |       |   
| --------- | ---------------------- | ----- |   
| Passwort: | [Passwort ändern..]    |       |   
| _...im Untermenü eingeben..._ | Neues Passwort eingeben: `pass`  |   |   
|           | Neues Passwort bestätigen: `pass`   [OK][OK] |  |   
| Hostname: |  `raspi`               | &nbsp;      |   
| Boot:     |  ( &nbsp;) Zum Desktop | (*) Zum CLI |   
| Automatische Anmeldung:            | ( &nbsp;) Als Benutzer 'pi' anmelden | (*) Disabled |   
[OK] anklicken und Neustart des Systems zulassen [OK].   
   

Sollte nicht nach einem <b>Neustart</b> gefragt werden, so kann man den Neustart so auslösen:   

|                         |                                                |   
| ----------------------- | ---------------------------------------------- |   
| Grafik-Bildschim:       | Am Grafik-Bildschirm links oben [Menü] anklicken -  Abmelden - [Neu starten] |   
| Von der Kommandozeile:  | ```sudo reboot``` |   

Das RasPi startet nun im Terminal-Modus und fragt nach dem User-Namen (`pi`) und dem Passwort (`pass`).   

Der Grafik-Bildschirm kann jederzeit durch folgende Kommandozeilen-Eingabe gestartet werden:  
 
```
startx
```

### Fernsteuerung vorbereiten
Für viele Aufgaben ist es einfacher, das RasPi vom PC aus zu bedienen, da man dann Befehle aus Anleitungen direkt kopieren und auf dem RasPi ausführen kann. Ein einfaches Programm dazu ist das Terminalprogramm [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html). Nach der Installation benötigt man lediglich die IP des RasPi sowie den User-Namen und das Passwort.   
Verbindet man das RasPi über ein LAN-Kabel mit dem PC, so kann man die IP über die Kommandozeile so anzeigen:   
```
ifconfig eth0   
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500   
      inet 192.168.0.68  netmask 255.255.248.0  broadcast 192.168.0.255   
```
Das heißt, die IP-Adresse ist 192.168.0.68   
Sollte trotz richtiger IP-Adresse die putty-Verbindung verweigert werden, dann ist eventuell SSH bei den Schnittstellen nicht aktiviert (siehe oben!).   

### Standard-User-Namen pi ändern
Quelle: [https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/](https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/) [24.10.2017]</a>   
__Anmerkung: Falls bereits User-Crontabs für den User pi erstellt wurden, so werden diese gelöscht. (Sichern!)__   
1. Putty starten (empfohlen)    oder das RasPi in den Kommandozeilen-Modus schalten (siehe oben), neu starten und User-Namen (`pi`) und  Passwort (`pass`) eingeben.      
2.  Hilfs-User temp anlegen:   
 ```
  sudo adduser --no-create-home temp   
  Geben Sie ein neues UNIX-Passwort ein: pass   
  Geben Sie das neue UNIX-Passwort erneut ein: pass   
 ```
 Alle anderen Eingaben leer lassen. (Es werden die Standardwerte übernommen)
       
3. Dem User `temp` sudo-Rechte einräumen   
 ```
 sudo visudo
 ```   
 Am Ende der Datei einfügen:   
 ```temp ALL=(ALL) NOPASSWD: ALL```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
4. Raspberry Pi neu starten (```sudo reboot```) und als User `temp` einloggen:   
 ```
 raspi login: temp   password: pass
 ```
 Wenn man mit putty arbeitet, muss dieses auch neu gestartet werden ;)
       
5. Benutzer pi auf pi_ umbenennen:   
 ```
 sudo usermod --move-home --login pi_ --home /home/pi_ pi
 ```   
 Anmerkung: Falls jetzt der englische Zeichensatz eingestellt ist: Taste - bei ?, / bei -
        
6. Den Namen der Gruppe pi ändern:   
 ```
 sudo groupmod -n pi_ pi
 ```
       
7. Raspberry Pi neu starten (```sudo reboot```) und als User `pi_` einloggen:   
 ```
 raspi login: pi_   password: pass
 ```
 Wenn man mit putty arbeitet, muss dieses auch neu gestartet werden ;)
       
8. Dem User `temp` die sudo-Rechte entziehen:   
 ```
 sudo visudo
 ```   
 Am Beginn der Zeile ein Kommentarzeichen # einfügen:   
 ```#temp ALL=(ALL) NOPASSWD: ALL```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
9. Löschen des Users `temp`   
 ```
 sudo deluser temp
 ```
       
---   

10. Usernamen für automatisches Login ändern:   
 ```
 sudo nano /etc/lightdm/lightdm.conf
 ```   
 Folgende Zeilen ändern bzw. ergänzen   
 ```#autologin-user=pi   autologin-user=pi_```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
11. Weiters ändern:   
 ```
 sudo nano /etc/systemd/system/autologin@.service
 ```   
 Folgende Zeilen ändern bzw. ergänzen   
 ```#ExecStart=-/sbin/agetty --autologin pi --noclear %I $TERM```   
 ```ExecStart=-/sbin/agetty --autologin pi_ --noclear %I $TERM```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
12. Von der Kommandozeile aus den automatischen Start der grafischen Oberfläche einschalten   
 ```
 sudo raspi-config
 ```   
 
 * Es erscheint das Kommandozeilen-Konfigurationsmenü:   
 
  | Raspberry Pi Software Configuration Tool | (raspi-config) |   
  | ---------------------------------------- | -------------- |   
  | 1 System Options         | Configure system settings                 |   
  | 2 Display Options        | Configure display settings                |   
  | 3 Interface Options      | Configure connections to peripherals      |   
  | 4 Performance Options    | Configure performance settings            |   
  | 5 Localisation Options   | Configure language and regional settings  |   
  | 6 Advanced Options       | Configure advanced settings               |   
  | 8 Update                 | Update this tool to the latest version    |   
  | 9 About raspi-config     | Information about this configuration tool |   
  | <Select>                 | <Finish>                                  |   
 Punkt 1 durch Drücken von <enter> wählen.   
 
 * Mit der Cursor-Taste <down> zu Punkt S5 gehen   
 ```S5 Boot / Auto Login Select boot into desktop or to command line```   
 und mit der <enter>-Taste wählen.   
    
 * Mit der Cursor-Taste <down> zu Punkt B4 gehen   
 ```B4 Desktop Autologin Desktop GUI, automatically logged in as 'pi_' user```   
 und mit der <enter>-Taste wählen.   
    
 * Mit der Tabulator-Taste <Finish> wählen und die Frage "Would you like to reboot now?" mit <Ja> beantworten.   
 Das RasPi bootet und startet wieder automatisch den Grafik-Bildschirm.   
    
 Das Einschalten des grafischen Bildschirms hätte man natürlich auch am RasPi durch Eingabe von   
 ```
 startx
 ```   
 und Einschalten am Grafik-Bildschirm links oben [Menü] anklicken - Einstellungen - Raspberry-Pi-Konfiguration - [System] durchführen können...
   
## Weitere Einstellmöglichkeiten
### Bildschirmschoner abschalten
1. Neues Verzeichnis mit Steuerdatei anlegen:   
`sudo mkdir /etc/X11/xorg.conf.d`   
Steuerdatei erstellen:   
`sudo nano /etc/X11/xorg.conf.d/01-disable-monitor-off.conf`  
Inhalt der Datei:   
`Section "ServerFlags"`   
`Option  "BlankTime" "0"`    
`Option  "StandbyTime" "0"`   
`Option  "SuspendTime" "0"`   
`Option  "OffTime" "0"`   
`EndSection`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
      
2. Weitere Datei anlegen:   
`nano ~/.xinitrc`   
Inhalt der Datei:   
`xset s off`   
`xset -dpms`   
`xset s noblank`   
`exec /etc/alternatives/x-session-manager`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
      
3. Das RasPi neu starten:   
`sudo reboot`

### Anzeige um 180 Grad drehen
Verwendet man ein Transparent-Gehäuse von Amazon bzw. BetterShopping (ART02314), so muss zusätzlich noch die Anzeige um 180&deg; gedreht werden.   
Dies geschieht einfach durch einen Eintrag in der Datei `/boot/config.txt`:   
```
sudo nano /boot/config.txt
```   
Folgende Zeile am Ende der Datei ergänzen   
```lcd_rotate=2```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
Der Eintrag wird erst nach dem nächsten Systemstart wirksam.

### Eigene Autostart-Datei `autostart.sh`
Möchte man beim System-Start eigene Programme automatisch starten, so ist es sinnvoll, diese Befehle in einem eigenen Script "`autostart.sh`" zu sammeln. Dazu sind die nachfolgenden Schritte erforderlich.   

1. Autostart-Datei erzeugen.   
```
sudo nano /usr/local/bin/autostart.sh
```   
Inhalt der Datei zB   
```
#!/bin/bash   
#...Farbe der Schrift auf gelb aendern...   
echo -e "\\033[01;33m"   
printf "_____autostart.sh______26.07.2021_______khartinger_____\n"   
printf "_______________________________________________________\n"   
#...Farbe der Schrift wieder auf weiß aendern...   
echo -e "\\033[00m"   
exit 0   
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
__*Anmerkungen:*__   
  * Die Datei ändert lediglich die Schriftfarbe auf gelb und gibt die nach `printf` stehenden Meldungen aus.   
  * Eigene Programmaufrufe müssen zwischen den beiden `printf`-Zeilen stehen.   
  * Tipp: Soll beim Start einer Datei nicht auf das Ende des Programmes gewartet werden, so muss am Ende der Aufruf-Zeile ein "kaufmännisches-und"-Zeichen (Ampersand) &amp; stehen!   

      
2. Eigene Autostart-Datei beim Systemstart aufrufen.   
System-Initialisierungsdatei bearbeiten:   
```
sudo nano /etc/rc.local
```   
Vor `exit 0` folgendes einfügen:   
```
#-----Calling a script with my own commands-----   
/usr/local/bin/autostart.sh   
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
      
3. Skript für alle User ausführbar machen   
```
sudo chmod 777 /usr/local/bin/autostart.sh
```
   