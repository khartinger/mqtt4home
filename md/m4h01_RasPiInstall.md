Letzte &Auml;nderungen: 8.12.2021 - 12.07.2025   
<a name="up"></a>   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Grundinstallation des Raspberry Pi</h1>
<a href="../LIESMICH.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h01_RasPiInstall_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint.md">==> RasPi Access Point</a>
</td></tr></table><hr>

<a name="x10"></a>   

# 1. Ziele
* Installation des Betriebssystems "Raspberry Pi OS" auf dem RasPi.   
* Vorbereitung des RasPis zur Verwendung von Node.js, Zigbee2Mqtt, Kiosk-Mode und selbst erstellter C++-Programme.   
* Verwendung eines Modems zum Senden von SMS.   

In diesem Demo-Projekt werden folgende Einstellungen gew&auml;hlt, die aus Sicherheitsgr&uuml;nden ge&auml;ndert werden sollten:   

| Kriterium      | Wert                      |   
| -------------- | ------------------------- |   
| Hostname:      | `raspi` (statt raspberry) |   
| Username:      | `pi_`   (statt pi)        |   
| Passwort:      | `pass`  (statt raspberry) |   
| SSID (WLAN):   | `Raspi11`                 |   
| WLAN-Passwort: | `12345678`                |   
   
<a name="x20"></a>   

# 2. Ben&ouml;tigte Hilfsmittel

* Hardware: RasPi (incl. Zubeh&ouml;r), Mikro-SD-Karte (8 GB oder 16 GB oder 32 GB)   
* Hardware: PC, Micro-SD-Kartenleser   
* Software: [Programmiersoftware f&uuml;r das Betriebssystem ("imager")](https://www.raspberrypi.org/software)
* Software: Terminal-Programm [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop   
* Software: [Mosquitto](https://mosquitto.org/download/)   
* Software: Eventuell [WinSCP zur Datei-Übertragung](https://winscp.net/eng/download.php)   

<a name="x30"></a>   

# 3. Grundinstallation
## 3.1 Betriebssystem auf die SD-Karte kopieren
1. Micro-SD-Karte (Gr&ouml;&szlig;e 8GB oder mehr) in den Kartenleser einlegen, den Kartenleser am PC anschlie&szlig;en.   
2. Herunterladen des Hilfsprogramms [`Raspberry Pi Imager`](http://www.raspberrypi.org/software) oder [`Win32 Disk Imager`](https://win32diskimager.org/) zum Beschreiben der SD-Card unter Windows.   
 Siehe [http://www.raspberrypi.org/software](http://www.raspberrypi.org/software).   
3. MicroSD-Karten-Programmier-Programm `imager_1.9.4` installieren, starten und folgendes auswählen:   
* Raspberry Pi Modell: RASPBERRY PI 4   
* Betriebssystem (OS): Raspberry Pi OS (64bit) vom 2025-05-13
* SD-Karte: GENERIC-SD/MMC USB DEVICE  
[Weiter]   
[Einstellungen bearbeiten]   
Hostname: `raspi`   
Benutzername: `pi_`   
Passwort: `pass`   
SSID: `Raspi11`   
Passwort: `12345678`   
[Speichern] [JA]   
4. Image direkt auf die Micro-SD-Karte schreiben.   

## 3.2 Erste Inbetriebnahme des RasPi
Nach dem ersten Start des RasPi m&uuml;ssen noch einige Einstellungen vorgenommen werden. Diese k&ouml;nnen sp&auml;ter auch ge&auml;ndert werden.   
1. Micro-SD-Karte in das RasPi einlegen.   
2. Bildschirm, Tastatur, Netzwerkkabel am RasPi anschlie&szlig;en.   
3. Betriebsspannung anlegen: RasPi startet im Grafik-Modus.   

## 3.3 Erste Einstellungen auf dem RasPi
### Sprache und Zeitzone wählen
Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Preferences – Raspberry-Pi-Configuration – [Localisation]   
* Locale:   
  * Language: de (German)   
  * Country: AT (Austria)   
  * Character Set: ISO-8859-1   
  [OK]   
* Timezone:   
  * Area: Europe   
  * Location: Vienna   
  [OK]   
* Keyboard:   
  * Model: Generic 105-key PC   
  * Layout: German   
  * Variant: German   
  [OK]   
* Wireless LAN Country   
[OK]   
Would you like to reboot now? [Yes]   

### Schnittstellen   
Am RasPi sollen ein ZigBee-Adapter und ein Modem betreiben werden. Beide werden am USB angeschlossen, daher werden die Schnittstellen an der 40-poligen Stiftleiste (hier) nicht benötigt. Lediglich SSH muss für den Zugriff mit Putty aktiviert werden.   

Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [Schnittstellen]   
* SSH: ( &nbsp; O)   
[OK]   

### Systemeinstellungen   
Die Systemeinstellungen können in einem Kommando-Fenster getestet werden. Dazu muss man das schwarze Rechteck-Symbol links oben anklicken. Es öffnet sich ein Terminal-Fenster mit einem Prompt:   
`pi_@raspi:~ $`   
Sollten der Username und Hostname noch die Vorgabewerte sein (`pi@raspberry`) so kann man dies und im Grafikbildschirm ändern:   

Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [System]   
* Kennwort: [Kennwort ändern]   
* Hostname: [Change Hostname]   
* Hochfahren: (O) Zum Desktop   
* Console auto login: ( &nbsp; O)   
* Desktop auto login: ( &nbsp; O)   
* Startbildschirm: ( &nbsp; O)   
[OK]   

Die Änderung des User-Namens wird später beschrieben.   

Anzeige der Betriebssystem-Version   
```
cat /etc/os-release
```
ergibt zB   
`PRETTY_NAME="Raspbian GNU/Linux 12 (bookworm)"`   

### WLAN einschalten
Im Grafik-Bildschirm rechts oben auf die Doppelpfeile klicken und "Turn on Wireless LAN" wählen. danach nochmals auf den Doppelpfeil tippen, ein Netzwerk wählen und das WLAN-Passwort eingeben. Es erscheint kurz die Meldung "Sie sind mit dem Netzwerk ... verbunden. IP-Adresse ..."
Die IP-Adressen können auch im Terminal-Fenster angezeigt werden:   
`ifconfig`   

### Grafik-System auf X11 umstellen
Das Raspberry Pi OS "Debian Bookworm" verwendet zur Anzeige den Wayland-Display-Server. Damit arbeiten viele Befehle nicht mehr, die unter X11 funktionierten.   

Die Umstellung zurück auf X11 erfolgt im Terminal durch   
`sudo raspi-config`   
Im Menü `6 Advanced Options` &lt;return&gt; - `A6 Wayland` &lt;return&gt; - `W1 X11` &lt;return&gt; - Openbox on X11 is active [OK]   
&lt;finish&gt;
Would you like to reboot now? [Ja]   

## 3.4 Anzeige um 180 Grad drehen
Verwendet man ein Transparent-Geh&auml;use von Amazon bzw. BetterShopping (ART02314), so muss zus&auml;tzlich die Anzeige UND die Touchscreen-Eingabe um 180&deg; gedreht werden.   

### Bildschirm-Ausgabe und Touchscreen-Eingabe drehen
1. Name des Displays herausfinden   
Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Screen Configuration: Im Bild wird der Name des Screens angezeigt:   `DSI-1`   

2. Namen des Touch-Gerätes herausfinden   
  `DISPLAY=:0 xinput list`  
  Ergibt z.B.   
  `Virtual core pointer`   
  `10-0038 generic ft5x06 (00)     id=10 [slave  pointer  (2)]`   
  Der Name ist somit `10-0038 generic ft5x06 (00)`   

3. Rotations-Skript erstellen   
  `nano /home/pi_/rotate-touchscreen.sh`   
  Inhalt:   
```
#!/bin/bash

# Kurze Wartezeit, bis X vollständig geladen ist
sleep 2

# Setze X-Kontext für Autostart
export DISPLAY=:0
export XAUTHORITY=/home/pi_/.Xauthority

# Bildschirm-Ausgabe drehen (für DSI Touchscreen)
xrandr --output DSI-1 --rotate inverted

# Touchscreen-Eingabe drehen (180°)
xinput set-prop "10-0038 generic ft5x06 (00)" \
"Coordinate Transformation Matrix" -1 0 1 0 -1 1 0 0 1
```

4. Script ausführbar machen   
  `chmod +x ~/rotate-touchscreen.sh`   

### Skript beim Systemstart ausführen   
Lösung für Raspberry Pi OS Bookworm mit X11:   
1. Autostart-Verzeichnis erstellen   
   `mkdir -p ~/.config/autostart`   

2. Autostart-Desktop-Datei erstellen   
  `nano ~/.config/autostart/rotate-touchscreen.desktop`   
  Inhalt:   
```
[Desktop Entry]
Type=Application
Name=Rotate Touchscreen
Exec=/home/pi_/rotate-touchscreen.sh
X-GNOME-Autostart-enabled=true
Comment=Drehe Bildschirm und Touchscreen beim Login
```

3. RasPi neu starten
`sudo reboot`   

### Anmerkung
Der Weg über den Grafik-Bildschirm funktioniert bei X11 nicht:   
Am Grafik-Bildschirm links oben [Menü] anklicken - Einstellungen – Screen Configuration - [Screens] - SDI-1 - Drehung - Inverted   

### Alte Methode
Eintrag in der Datei `/boot/firmware/config.txt`(ehemals `/boot/config.txt`):   
```
sudo nano /boot/firmware/config.txt
```   
Folgende Zeile am Ende der Datei erg&auml;nzen   
```lcd_rotate=2```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   
Der Eintrag wird erst nach dem n&auml;chsten Systemstart wirksam.

## 3.5 Bildschirmschoner dauerhaft abschalten
Das folgende Skript schaltet Bildschirmschoner dauerhaft ab.   
`nano ~/disable_screensaver.sh`   
Inhalt   
```
#!/bin/bash

echo "Deaktiviere Bildschirmschoner und Display-Power-Management..."

# 1. xset-Befehle zu Autostart hinzufügen (für LXDE)
AUTOSTART_FILE="$HOME/.config/lxsession/LXDE-pi/autostart"
mkdir -p "$(dirname "$AUTOSTART_FILE")"

grep -q "xset s off" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset s off" >> "$AUTOSTART_FILE"
grep -q "xset -dpms" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset -dpms" >> "$AUTOSTART_FILE"
grep -q "xset s noblank" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset s noblank" >> "$AUTOSTART_FILE"

echo "xset-Befehle zum Autostart hinzugefügt: $AUTOSTART_FILE"

# 2. LightDM konfigurieren
LIGHTDM_CONF="/etc/lightdm/lightdm.conf"
if [ -f "$LIGHTDM_CONF" ]; then
    sudo sed -i '/^\[Seat:\*\]/,/^\[/ {
        s/^#*xserver-command=.*/xserver-command=X -s 0 -dpms/
        t
        /^xserver-command=/!a xserver-command=X -s 0 -dpms
    }' "$LIGHTDM_CONF"
    echo "LightDM-Konfiguration angepasst: $LIGHTDM_CONF"
else
    echo "LightDM-Konfigurationsdatei nicht gefunden. Übersprungen."
fi

# 3. xscreensaver deinstallieren (falls vorhanden)
if dpkg -l | grep -q xscreensaver; then
    echo "Entferne xscreensaver..."
    sudo apt remove -y xscreensaver
    echo "xscreensaver deinstalliert."
else
    echo "  xscreensaver ist nicht installiert."
fi

# 4. Hinweis zu .xinitrc geben (optional)
echo ""
echo "Wenn du X manuell mit 'startx' startest, füge Folgendes in ~/.xinitrc ein:"
echo '  xset s off'
echo '  xset -dpms'
echo '  xset s noblank'
echo ""
echo "Fertig. Bitte ab- und wieder anmelden oder neu starten, damit die Änderungen wirksam werden."
```

Skript ausführbar machen:   
`chmod +x disable_screensaver.sh`   

Skript ausführen:   
`sudo ~/disable_screensaver.sh`   


## 3.6 Fernsteuerung vorbereiten
F&uuml;r viele Aufgaben ist es einfacher, das RasPi vom PC aus zu bedienen, da man hier Befehle aus Anleitungen direkt kopieren und auf dem RasPi ausf&uuml;hren kann. Ein einfaches Programm dazu ist das Terminalprogramm [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html). Nach der Installation ben&ouml;tigt man lediglich die IP des RasPi sowie den User-Namen und das Passwort.   
Verbindet man das RasPi &uuml;ber ein LAN-Kabel mit dem PC, so kann man die IP &uuml;ber die Kommandozeile so anzeigen:   
```
ifconfig eth0   
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500   
      inet 192.168.0.68  netmask 255.255.248.0  broadcast 192.168.0.255   
```
Das hei&szlig;t, die IP-Adresse ist 192.168.0.68   
Sollte trotz richtiger IP-Adresse die putty-Verbindung verweigert werden, dann ist eventuell SSH bei den Schnittstellen nicht aktiviert (siehe oben!).   

Die IP-Adresse wird vom Router (z.B. einer FRITZ!Box) vergeben. Will man alse eine bestimmte IP-Adresse für das RasPi, muss man dies im Router einstellen.   

## 3.7 Standard-User-Namen pi &auml;ndern
Die Angabe des Standard-User-Namens (pi) kann entweder gleich beim Erstellen der SD-Karte geändert werden oder so wie hier beschrieben.   
Quelle: [https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/](https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/) [24.10.2017]   
__Anmerkung: Falls bereits User-Crontabs f&uuml;r den User pi erstellt wurden, so werden diese gel&ouml;scht. (Sichern!)__   
1. Putty starten (empfohlen)    oder das RasPi in den Kommandozeilen-Modus schalten (siehe oben), neu starten und User-Namen (`pi`) und  Passwort (`pass`) eingeben.      
2.  Hilfs-User temp anlegen:   
 ```
  sudo adduser --no-create-home temp   
  Geben Sie ein neues UNIX-Passwort ein: pass   
  Geben Sie das neue UNIX-Passwort erneut ein: pass   
 ```
 Alle anderen Eingaben leer lassen. (Es werden die Standardwerte &uuml;bernommen)
       
3. Dem User `temp` sudo-Rechte einr&auml;umen   
 ```
 sudo visudo
 ```   
 Am Ende der Datei einf&uuml;gen:   
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
        
6. Den Namen der Gruppe pi &auml;ndern:   
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
 Am Beginn der Zeile ein Kommentarzeichen # einf&uuml;gen:   
 ```#temp ALL=(ALL) NOPASSWD: ALL```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
9. L&ouml;schen des Users `temp`   
 ```
 sudo deluser temp
 ```
       
---   
10. Usernamen f&uuml;r automatisches Login &auml;ndern:   
 ```
 sudo nano /etc/lightdm/lightdm.conf
 ```   
 Folgende Zeilen &auml;ndern bzw. erg&auml;nzen   
   `#autologin-user=pi`   
   `autologin-user=pi_`   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
11. Weiters &auml;ndern:   
 ```
 sudo nano /etc/systemd/system/autologin@.service
 ```   
 Folgende Zeilen &auml;ndern bzw. erg&auml;nzen   
 ```#ExecStart=-/sbin/agetty --autologin pi --noclear %I $TERM```   
 ```ExecStart=-/sbin/agetty --autologin pi_ --noclear %I $TERM```   
 Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
       
12. Von der Kommandozeile aus den automatischen Start der grafischen Oberfl&auml;che einschalten   
 ```
 sudo raspi-config
 ```   
 
 * Es erscheint das Kommandozeilen-Konfigurationsmen&uuml;:   
 
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
  | &lt;Select&gt;           | &lt;Finish&gt;                         |   
 Punkt 1 durch Dr&uuml;cken von &lt;enter&gt; w&auml;hlen.   
 
 * Mit der Cursor-Taste <down> zu Punkt S5 gehen   
 ```S5 Boot / Auto Login Select boot into desktop or to command line```   
 und mit der &lt;enter&gt;-Taste w&auml;hlen.   
    
 * Mit der Cursor-Taste <down> zu Punkt B4 gehen   
 ```B4 Desktop Autologin Desktop GUI, automatically logged in as 'pi_' user```   
 und mit der &lt;enter&gt;-Taste w&auml;hlen.   
    
 * Mit der Tabulator-Taste &lt;Finish&gt; w&auml;hlen und die Frage "Would you like to reboot now?" mit &lt;Ja&gt; beantworten.   
 Das RasPi bootet und startet wieder automatisch den Grafik-Bildschirm.   
    
 Das Einschalten des grafischen Bildschirms h&auml;tte man nat&uuml;rlich auch am RasPi durch Eingabe von   
 ```
 startx
 ```   
 und Einschalten am Grafik-Bildschirm links oben [Men&uuml;] anklicken - Einstellungen - Raspberry-Pi-Konfiguration - [System] durchf&uuml;hren k&ouml;nnen...

<a name="x40"></a>   

# 4. Installation aktualisieren
1. Paketliste aktualisieren, auf Updates pr&uuml;fen   
```   
sudo apt update
sudo apt list --upgradable
```   
2. Alle Updates installieren   
```    
sudo apt full-upgrade -y
```

<a name="x50"></a>   

# 5. Apache-Server
Der Apache HTTP Server dient dazu, dass das RasPi Browser-Anfragen entgegennehmen kann. Die Installation erfolgt durch   
`sudo apt install apache2 -y`   

Kontrolle, ob Apache läuft:   
`sudo systemctl status apache2`   

Apache-Seite testen:   
Am PC im Browser das RasPi aufrufen:   
`http://10.1.1.1`   
Es sollte die Default-Seite "Apache 2 Debian Default Page" angezeigt werden.   

Bei der Installation des Apache-Servers wird das Verzeichnis `var/www/html` erstellt und der Browser-Aufruf zeigt die Datei `var/www/html/index.htm` an.   

Möchte man zum Testen eine andere "Start-Datei", so kann man sie zB folgendermaßen erzeugen:   
Alte Datei sichern:   
`sudo cp /var/www/html/index.html /var/www/html/index_apache2.html`   
Neue Datei erzeugen:   
```
echo '<!DOCTYPE html><html><head><title>RasPi Webserver</title></head><body><h1>Hallo vom Raspberry Pi!</h1></body></html>' | sudo tee /var/www/html/index.htm > /dev/null
```
Test wieder im Browser mit `http://10.1.1.1`   

Ändern der Rechte für das Verzeichnis /var/www/html:   
`sudo chmod 777 /var/www/html`   

<a name="x60"></a>   

# 6. Node.js und npm installieren
1. System aktualisieren   
```
sudo apt update && sudo apt upgrade -y
sudo apt install -y git curl
```
2. `Node.js` (und npm) direkt installieren   
```
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt install -y nodejs
sudo npm install -g pnpm
```
3. Die Installation testen: Versionsnummer abfragen   
```
node -v
npm -v
```
Ergebnis zB `v18.20.6` und `10.8.2`.   

<a name="x70"></a>   

# 7. Zigbee2MQTT installieren
1. Die Installation erfolgt in den Standardordner:   
```
cd /opt
sudo git clone https://github.com/Koenkk/zigbee2mqtt.git
sudo chown -R $USER:$USER zigbee2mqtt
cd /opt/zigbee2mqtt
pnpm install
```
2. Konfigurationsdatei anlegen   
Entweder   
```
cp /opt/zigbee2mqtt/data/configuration.example.yaml /opt/zigbee2mqtt/data/configuration.yaml
```
     oder eine bestehende Konfigurationsdatei verwenden...   

3. Zigbee2mqtt als Service automatisch starten   
```
sudo nano /etc/systemd/system/zigbee2mqtt.service
```
Inhalt:   
```
[Unit]
Description=Zigbee2MQTT
After=network.target

[Service]
ExecStart=/usr/bin/npm start
WorkingDirectory=/opt/zigbee2mqtt
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```

Falls man das Service aktivieren möchte:   
```
sudo systemctl daemon-reexec
sudo systemctl enable zigbee2mqtt
sudo systemctl start zigbee2mqtt
```

<a name="x80"></a>   

# 8. Tipps und Tricks
## 8.1 Eigene Autostart-Datei `autostart.sh`
M&ouml;chte man beim System-Start eigene Programme automatisch starten, so ist es sinnvoll, diese Befehle in einem eigenen Script "`autostart.sh`" zu sammeln. Das folgende Beispiel für eine Autostart-Datei ändert die Anzeige-Farbe, wartet eine Sekunde, wechselt dann ins Arbeitsverzeichnis und erzeugt eine Log-Datei, damit man Fehler beim Starten nachvollziehen kann.   

1. Autostart-Datei erzeugen.   
```
sudo nano /usr/local/bin/autostart.sh
```   
Inhalt der Datei zB   
```
#!/bin/bash
# autostart.sh - gestartet beim Systemstart
# Karl Hartinger, 11.07.2025

# Farbdefinitionen
C_YELLOW='\033[01;33m'
C_RESET='\033[0m'

logfile="/var/log/autostart.log"

# Funktion zum Loggen mit Zeitstempel
log() {
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$logfile"
}

# Skriptstart
#...Farbe der Schrift auf gelb aendern...
echo -e "${C_YELLOW}"
log "_____autostart.sh______11.07.2025___Karl Hartinger_____"

log "1 Sekunde warten..."
sleep 1

cd ~ || log "Fehler: Home-Verzeichnis nicht gefunden"
log "_______________________________________________________"
#...Farbe der Schrift wieder auf weiss aendern...   
echo -e "${C_RESET}"
exit 0
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

__*Anmerkungen:*__   
  * Die Datei &auml;ndert die Schriftfarbe auf gelb und gibt die nach `log` stehenden Meldungen aus. Die Ausführung der Befehle wird in einer log-Datei `/var/log/autostart.log` dokumentiert.   
  * Eigene Programmaufrufe m&uuml;ssen zwischen den beiden `log`-Zeilen stehen.   
  * Tipp: Soll beim Start einer Datei nicht auf das Ende des Programmes gewartet werden, so muss am Ende der Aufruf-Zeile ein "kaufm&auml;nnisches-und"-Zeichen (Ampersand) &amp; stehen oder am Beginn der Zeile muss `nohup ` stehen!   

2. Skript ausführbar machen.   
```   
sudo chmod +x /usr/local/bin/autostart.sh
```   

3. Eigene Autostart-Datei beim Systemstart aufrufen.   
Dazu muss eine Service-Datei erstellt werden:   
```   
sudo nano /etc/systemd/system/autostart.service
```   
Inhalt:   
```   
[Unit]
Description=Autostart Script
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/autostart.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

4. Service aktivieren, damit es beim Booten läuft:   
```   
sudo systemctl enable autostart.service
```   

5. Service testen:
```   
sudo systemctl start autostart.service
```   

6. Service-Status überprüfen:
```   
sudo systemctl status autostart.service
```   

7. Log-Datei anzeigen:   
```   
cat /var/log/autostart.log
```   

## 8.2 Kiosk-Modus
Der "Kiosk-Modus" ist eine Betriebsart von Rechnern bzw. Terminals mit graphischer Anzeige, bei der die Rechte des Users eingeschränkt sind und nur bestimmte Aktionen ausgeführt werden können.   
Welche Schritte sind beim RasPi mit OS "Bookworm" erforderlich, damit eine Web-Seite im Chromium-Browser nach dem Start automatisch im Kios-Modus angezeigt wird?   

1. Chromium installieren (falls nicht vorhanden):   
  `sudo apt update`  
  `sudo apt install chromium-browser`   

2. Auto-login aktivieren (falls es noch nicht eingestellt ist):   
   Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [System]   
   * Hochfahren: * zum Desptop   
   * Desktop auto login: ( &nbsp; o)  

3. Autostart-Datei bearbeiten   
  `mkdir -p ~/.config/lxsession/LXDE-pi`   
  `nano ~/.config/lxsession/LXDE-pi/autostart`   
  Hinzufügen (Bildschirm nie ausschalten, Chromium starten):   
  `@xset s off`   
  `@xset -dpms`   
  `@xset s noblank`   
  `@chromium-browser --noerrdialogs --disable-infobars --kiosk http://localhost`   
   Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

4. RasPi neu starten   
  `sudo reboot`   

## 8.3 Bildschirmschoner abschalten (alte Version)
1. Neues Verzeichnis mit Steuerdatei anlegen:   
`sudo mkdir /etc/X11/xorg.conf.d`   
Steuerdatei erstellen:   
`sudo nano /etc/X11/xorg.conf.d/01-disable-monitor-off.conf`  
Inhalt der Datei:   
```   
Section "ServerFlags"
Option  "BlankTime" "0"
Option  "StandbyTime" "0"
Option  "SuspendTime" "0"
Option  "OffTime" "0"
EndSection
```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
      
2. Weitere Datei anlegen:   
`nano ~/.xinitrc`   
Inhalt der Datei:   
```   
xset s off
xset -dpms
xset s noblank
exec /etc/alternatives/x-session-manager
```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
      
3. Das RasPi neu starten:   
`sudo reboot`

[Zum Seitenanfang](#up)   