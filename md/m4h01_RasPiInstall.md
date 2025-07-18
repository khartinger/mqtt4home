Letzte &Auml;nderungen: 8.12.2021 - 18.07.2025   
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

Diese Ziele werden in folgenden Unterkapiteln erreicht:   
* [2. Ben&ouml;tigte Hilfsmittel](#x20)   
* [3. Grundinstallation](#x30)   
* [4. Installation aktualisieren](#x40)   
* [5. Apache-Server](#x50)   
* [6. Node.js und npm installieren](#x60)   
* [7. MQTT-Broker `mosquitto` installieren](#x70)   
* [8. C++ Programm kompilieren und ausf&uuml;hren (Beispiel m4hFindSimModule)](#x80)   
* [9. Feste USB-Schnittstellen-Namen durch udev-Regeln](#x90)   
* [10. Eigene Autostart-Datei `autostart.sh`](#x100)   
* [11. Zigbee2MQTT installieren](#x110)   
* [12. Kiosk-Modus](#x120)   
* [13. Dies und Das](#x130)   

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
* Software: Eventuell [WinSCP zur Datei-&Uuml;bertragung](https://winscp.net/eng/download.php)   

[Zum Seitenanfang](#up)   
<a name="x30"></a>   

# 3. Grundinstallation
## 3.1 Betriebssystem auf die SD-Karte kopieren
1. Micro-SD-Karte (Gr&ouml;&szlig;e 8GB oder mehr) in den Kartenleser einlegen, den Kartenleser am PC anschlie&szlig;en.   
2. Herunterladen des Hilfsprogramms [`Raspberry Pi Imager`](http://www.raspberrypi.org/software) oder [`Win32 Disk Imager`](https://win32diskimager.org/) zum Beschreiben der SD-Card unter Windows.   
 Siehe [http://www.raspberrypi.org/software](http://www.raspberrypi.org/software).   
3. MicroSD-Karten-Programmier-Programm `imager_1.9.4` installieren, starten und folgendes ausw&auml;hlen:   
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
### Sprache und Zeitzone w&auml;hlen
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
Am RasPi sollen ein ZigBee-Adapter und ein Modem betrieben werden. Beide werden am USB angeschlossen, daher werden die Schnittstellen an der 40-poligen Stiftleiste (hier) nicht ben&ouml;tigt. Lediglich SSH muss f&uuml;r den Zugriff mit Putty aktiviert werden.   

Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [Schnittstellen]   
* SSH: ( &nbsp; O)   
[OK]   

### Systemeinstellungen   
Die Systemeinstellungen k&ouml;nnen in einem Kommando-Fenster getestet werden. Dazu muss man das schwarze Rechteck-Symbol links oben anklicken. Es &ouml;ffnet sich ein Terminal-Fenster mit einem Prompt:   
`pi_@raspi:~ $`   
Sollten der Username und Hostname noch die Vorgabewerte sein (`pi@raspberry`) so kann man dies und im Grafikbildschirm &auml;ndern:   

Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Raspberry-Pi-Konfiguration – [System]   
* Kennwort: [Kennwort &auml;ndern]   
* Hostname: [Change Hostname]   
* Hochfahren: (O) Zum Desktop   
* Console auto login: ( &nbsp; O)   
* Desktop auto login: ( &nbsp; O)   
* Startbildschirm: ( &nbsp; O)   
[OK]   

Die &Auml;nderung des User-Namens wird sp&auml;ter beschrieben.   

Anzeige der Betriebssystem-Version   
```
cat /etc/os-release
```
ergibt zB   
`PRETTY_NAME="Raspbian GNU/Linux 12 (bookworm)"`   

### WLAN einschalten
Im Grafik-Bildschirm rechts oben auf die Doppelpfeile klicken und "Turn on Wireless LAN" w&auml;hlen. danach nochmals auf den Doppelpfeil tippen, ein Netzwerk w&auml;hlen und das WLAN-Passwort eingeben. Es erscheint kurz die Meldung "Sie sind mit dem Netzwerk ... verbunden. IP-Adresse ..."
Die IP-Adressen k&ouml;nnen auch im Terminal-Fenster angezeigt werden:   
`ifconfig`   

### Grafik-System auf X11 umstellen
Das Raspberry Pi OS "Debian Bookworm" verwendet zur Anzeige den Wayland-Display-Server. Damit arbeiten viele Befehle nicht mehr, die unter X11 funktionierten.   

Die Umstellung zur&uuml;ck auf X11 erfolgt im Terminal durch   
`sudo raspi-config`   
Im Men&uuml; `6 Advanced Options` &lt;return&gt; - `A6 Wayland` &lt;return&gt; - `W1 X11` &lt;return&gt; - Openbox on X11 is active [OK]   
&lt;finish&gt;
Would you like to reboot now? [Ja]   

## 3.4 Anzeige um 180 Grad drehen
Verwendet man ein Transparent-Geh&auml;use von Amazon bzw. BetterShopping (ART02314), so muss zus&auml;tzlich die Anzeige UND die Touchscreen-Eingabe um 180&deg; gedreht werden.   

### Bildschirm-Ausgabe und Touchscreen-Eingabe drehen
1. Den Namen des Displays herausfinden   
Am Grafik-Bildschirm links oben [Men&uuml;] anklicken -  Einstellungen – Screen Configuration: Im Bild wird der Name des Screens angezeigt:   `DSI-1`   
[Close]   

2. Den Namen des Touch-Ger&auml;tes herausfinden   
  In einer Konsole eingeben:   
  `DISPLAY=:0 xinput list`  
  Ergibt z.B.   
  `Virtual core pointer`   
  `10-0038 generic ft5x06 (00)     id=10 [slave  pointer  (2)]`   
  Der Name ist somit `10-0038 generic ft5x06 (00)`   

3. Rotations-Skript erstellen   
  `nano /home/pi_/rotate-touchscreen.sh`   
  _Falls der Username nicht `pi_` ist: Den richtigen Namen beim Aufruf des Skripts und im Inhalt angeben!_    
  Inhalt:   
```
#!/bin/bash

# Kurze Wartezeit, bis X vollst&auml;ndig geladen ist
sleep 2

# Setze X-Kontext f&uuml;r Autostart
export DISPLAY=:0
export XAUTHORITY=/home/pi_/.Xauthority

# Bildschirm-Ausgabe drehen (f&uuml;r DSI Touchscreen)
xrandr --output DSI-1 --rotate inverted

# Touchscreen-Eingabe drehen (180°)
xinput set-prop "10-0038 generic ft5x06 (00)" \
"Coordinate Transformation Matrix" -1 0 1 0 -1 1 0 0 1
```

4. Script ausf&uuml;hrbar machen   
  `chmod +x ~/rotate-touchscreen.sh`   

### Skript beim Systemstart ausf&uuml;hren   
L&ouml;sung f&uuml;r Raspberry Pi OS Bookworm mit X11:   
1. Autostart-Verzeichnis erstellen   
   `mkdir -p ~/.config/autostart`   

2. Autostart-Desktop-Datei erstellen   
  `nano ~/.config/autostart/rotate-touchscreen.desktop`   
  Inhalt (_Falls n&ouml;tig: User-Namen anpassen!_):   
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
Das Drehen des Grafik-Bildschirms &uuml;ber das Men&uuml; in der grafischen Oberfl&auml;che funktioniert __bei X11 nicht__:   
Am Grafik-Bildschirm links oben [Men&uuml;] anklicken - Einstellungen – Screen Configuration - [Screens] - SDI-1 - Drehung - Inverted   

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

# 1. xset-Befehle zu Autostart hinzuf&uuml;gen (f&uuml;r LXDE)
AUTOSTART_FILE="$HOME/.config/lxsession/LXDE-pi/autostart"
mkdir -p "$(dirname "$AUTOSTART_FILE")"

grep -q "xset s off" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset s off" >> "$AUTOSTART_FILE"
grep -q "xset -dpms" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset -dpms" >> "$AUTOSTART_FILE"
grep -q "xset s noblank" "$AUTOSTART_FILE" 2>/dev/null || echo "@xset s noblank" >> "$AUTOSTART_FILE"

echo "xset-Befehle zum Autostart hinzugef&uuml;gt: $AUTOSTART_FILE"

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
    echo "LightDM-Konfigurationsdatei nicht gefunden. &Uuml;bersprungen."
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
echo "Wenn du X manuell mit 'startx' startest, f&uuml;ge Folgendes in ~/.xinitrc ein:"
echo '  xset s off'
echo '  xset -dpms'
echo '  xset s noblank'
echo ""
echo "Fertig. Neu starten, damit die &Auml;nderungen wirksam werden."
```

Skript ausf&uuml;hrbar machen:   
`chmod +x disable_screensaver.sh`   

Skript ausf&uuml;hren:   
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

Die IP-Adresse wird vom Router (z.B. einer FRITZ!Box) vergeben. Will man alse eine bestimmte IP-Adresse f&uuml;r das RasPi, muss man dies im Router einstellen.   

## 3.7 Standard-User-Namen pi &auml;ndern
Die Angabe des Standard-User-Namens (pi) kann entweder gleich beim Erstellen der SD-Karte ge&auml;ndert werden oder so wie hier beschrieben.   
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
Das Update kann schon einige Minuten dauern...   

[Zum Seitenanfang](#up)   
<a name="x50"></a>   

# 5. Apache-Server
Der Apache HTTP Server dient dazu, dass das RasPi Browser-Anfragen entgegennehmen kann. Die Installation erfolgt durch   
`sudo apt install apache2 -y`   

Kontrolle, ob Apache l&auml;uft:   
`sudo systemctl status apache2`   
Ende mit &lt;strg&gt;c   

Apache-Seite testen:   
Am PC im Browser das RasPi aufrufen:   
`http://10.1.1.1`   
Es sollte die Default-Seite "Apache 2 Debian Default Page" angezeigt werden.   

Bei der Installation des Apache-Servers wird das Verzeichnis `var/www/html` erstellt und der Browser-Aufruf zeigt die Datei `var/www/html/index.htm` an.   

M&ouml;chte man zum Testen eine andere "Start-Datei", so kann man diese zB ganz einfach so erzeugen:   
```
echo '<!DOCTYPE html><html><head><title>RasPi Webserver</title></head><body><h1>Hallo vom Raspberry Pi!</h1></body></html>' | sudo tee /var/www/html/index.html > /dev/null
```
Test wieder im Browser mit `http://10.1.1.1`   

&Auml;ndern der Rechte f&uuml;r das Verzeichnis /var/www/html:   
`sudo chmod 777 /var/www/html`   

[Zum Seitenanfang](#up)   
<a name="x60"></a>   

# 6. Node.js und npm installieren
1. System aktualisieren   
```
sudo apt update && sudo apt upgrade -y
sudo apt install -y git curl
```
2. `Node.js` (und npm) direkt installieren   
```
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -
sudo apt install -y nodejs
sudo npm install -g pnpm
```
3. Die Installation testen: Versionsnummer abfragen   
```
node -v
npm -v
```
Ergebnis zB `v20.19.1` und `10.8.2`.   

[Zum Seitenanfang](#up)   
<a name="x70"></a>   

# 7. MQTT-Broker `mosquitto` installieren
Die Installation ist im Kapitel [/md/m4h03_RasPiMQTTBroker.md](/md/m4h03_RasPiMQTTBroker.md) beschrieben.   


[Zum Seitenanfang](#up)   
<a name="x80"></a>   

# 8. C++ Programm kompilieren und ausf&uuml;hren
## 8.1 Das Beispielprogramm `m4hFindSimModule`
Das folgende Beispiel zeigt, wie man ein C++-Programm auf dem RasPi zum Laufen bringt und es f&uuml;r alle User verf&uuml;gbar macht. Der Einfachheit halber befindet sich der gesamte Programmcode in einer Datei.   
Dieses einfache Programm versucht, ein Modem zu finden. Dazu 
&ouml;ffnet es der Reihe nach verschiedene serielle Schnittstellen, sendet an diese das Kommando `ATE0`,  wartet auf eine Antwort `OK` und schlie&szlig;t die Schnittstelle.   
Das Ergebnis der Sendevorg&auml;nge wird auf der Konsole angezeigt.   
Die Schnittstellen, die ausprobiert werden, stehen im Hauptprogramm in der Zeile   
`std::string devices[]={"/dev/ttyS0", "/dev/ttyACM0", "/dev/ttyUSB0", "/dev/ttyUSB1"};`   

Eine m&ouml;gliche Programmausgabe ist:   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM-Modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
In diesem Beispiel h&auml;ngt das Modem also am Device `/dev/ttyUSB0`.   

## 8.2 Erzeugung des Programms 
1. Arbeitsverzeichnis erstellen   
   `mkdir ~/m4hFindSimModule`  
2. Ins Verzeichnis wechseln   
   `cd ~/m4hFindSimModule`  
3. Datei erzeugen   
   `nano ~/m4hFindSimModule/m4hFindSimModule.cpp`   
Codierung:   
```
//_____m4hFindSimModule.cpp______________________khartinger_____
// g++ m4hFindSimModule.cpp -o m4hFindSimModule
// Search for GSM Modem e.g. SIM808 @ /dev/ttyS0, /dev/ttyUSB0, ...
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
// Hardware: (1) Raspberry Pi
//           (2) GSM Modem e.g. SIM808
// Updates:
// 2021-08-26 First release
// Released into the public domain.
#include <string>       // std::string, find
#include <stdio.h>      // printf
#include <string.h>     // strerror
#include <fcntl.h>      // file controls like O_RDWR
#include <errno.h>      // Error integer and strerror()
#include <termios.h>    // POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

//_______close serial interface_________________________________
void ttyClose(int fd) { close(fd); }

//_______open serial interface__________________________________
// return: file descriptor or -1|-2|-3 on error
int ttyOpen(std::string device_)
{
 int fd=open(device_.c_str(), O_RDWR);
 if(fd==-1) return -1;
 //-----------device open---------------------------------------
 //-----------set settings--------------------------------------
 struct termios tty;
 if(tcgetattr(fd, &tty) != 0) return -2;
 tty.c_cflag &= ~PARENB;     // Clear parity bit
 tty.c_cflag &= ~CSTOPB;     // Clear stop field, only one stop bit
 tty.c_cflag |= CS8;         // 8 bits per byte (most common)
 tty.c_cflag &= ~CRTSCTS;    // Disable RTS/CTS hardware flow control
 tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines
 tty.c_lflag &= ~ICANON;     // Disable canonical mode
 tty.c_lflag &= ~ECHO;       // Disable echo
 tty.c_lflag &= ~ECHOE;      // Disable erasure
 tty.c_lflag &= ~ECHONL;     // Disable new-line echo
 tty.c_lflag &= ~ISIG;       // Disable interpretation INTR, QUIT, SUSP
 tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
 tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
 tty.c_oflag &= ~OPOST;      // Prevent special interpretation of output bytes (e.g. newline chars)
 tty.c_oflag &= ~ONLCR;      // Prevent conversion of newline to carriage return/line feed
 tty.c_cc[VTIME] = 10;       // Wait for up to 1s (10 deciseconds), ...
 tty.c_cc[VMIN] = 0;         // ...returning as soon as any data is received.
 cfsetispeed(&tty, B9600);   // input baudrate
 cfsetospeed(&tty, B9600);   // output baudrate
 // Save tty settings, also checking for error
 if (tcsetattr(fd, TCSANOW, &tty) != 0) return -3;
 return fd;
}

//_______write AT to serial_____________________________________
// return: 0=OK, -1 on error
int ttyWriteAT(int fd)
{
 int iRet=0;
 unsigned char cmd[] = { 'A', 'T', 'E','0', '\r', '\n'};
 int cmdLen=sizeof(cmd);
 for(int j=0; j<cmdLen; j++)
 {//--------write char by char--------------------------------
  char c1=cmd[j];
  int  n=write(fd, &c1, 1);
  if(n<1) 
  {//-------char not written, try again-----------------------
   usleep(2000);
   n = write(fd, &c1, 1);
   if(n<1) return -1;
  }
 } // end write char by char
 return 0;
}

//_______(try to) read OK from modem____________________________
// return: 0|1 = OK, -1 on error
int ttyReadOK(int fd)
{
 char cmdIn[256];
 int n=read(fd, &cmdIn, sizeof(cmdIn));// OK\r\n received?
 if(n<4) {
  usleep(500000);                      // wait 500ms
  n+=read(fd, &cmdIn, sizeof(cmdIn)); 
 }
 cmdIn[n]=0;
 //------search for "OK"----------------------------------------
 std::string s1=cmdIn;
 int pos1=s1.find("OK");
 if(pos1==std::string::npos) return -1;
 return 0;
}

//_______main program___________________________________________
int main(int argc, char *argv[])
{ 
 std::string devices[]={"/dev/ttyS0", "/dev/ttyACM0", "/dev/ttyUSB0", "/dev/ttyUSB1"};
 int devicesNum=4;
 std::string device;
 int    fd;
 int    iRet=0;
 printf("Searching for modem...\n");
 //-----------for all devices-----------------------------------
 for(int i=0; i<devicesNum; i++)
 {
  //----------try to open serial interface----------------------
  device=devices[i];
  printf("%s: ",device.c_str()); fflush(stdout);
  fd=ttyOpen(device);
  if(fd<0) {
   if(fd==-1) printf("Could not open device (Error %i: %s)\n", errno, strerror(errno));
   else if(fd==-2) printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
   else if(fd==-3) printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
   else printf("Unknown error %i: %s\n", errno, strerror(errno));
   continue;
  }
  //----------send AT to serial---------------------------------
  iRet=ttyWriteAT(fd);
  if(iRet!=0) {
   ttyClose(fd);
   printf("Error writing AT command!\n");
   continue;
  }
  //----------(try to) read OK from module----------------------
  iRet=ttyReadOK(fd);
  ttyClose(fd);
  if(iRet>=0) 
  {//---------read OK success-----------------------------------
   printf("GSM-Modem found!\n");
  } else 
  {//---------error on reading OK-------------------------------
   printf("NO modem found (error on reading OK)\n");
  }
 }// end for all devices
 return 0;
}
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

4. Erzeugen des Programmes   
  `g++ m4hFindSimModule.cpp -o m4hFindSimModule`   
5. Programm ausf&uuml;hrbar machen   
  `chmod 777 ~/m4hFindSimModule/m4hFindSimModule`   
6. Programm ausf&uuml;hren   
  `~/m4hFindSimModule/m4hFindSimModule`   
  Ergebnis siehe oben.   
  Falls kein Modem gefunden wird:   
```
Searching for modem...
/dev/ttyS0: Could not open device (Error 2: No such file or directory)
/dev/ttyACM0: Could not open device (Error 2: No such file or directory)
/dev/ttyUSB0: NO modem found (error on reading OK)
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```

## 8.3 Programm f&uuml;r alle User verf&uuml;gbar machen
Das ausf&uuml;hrbare Programm oben befindet sich im Home-Bereich des Users pi_ und ist nur f&uuml;r diesen verf&uuml;gbar.   
Die folgenden Befehle kopieren das Programm `m4hFindSimModule` ins Verzeichnis `/usr/local/bin`, &auml;ndern den Besitzer auf root und machen das Programm f&uuml;r alle ausf&uuml;hrbar.   
```
sudo cp ~/m4hFindSimModule/m4hFindSimModule /usr/local/bin/m4hFindSimModule
sudo chown root /usr/local/bin/m4hFindSimModule
sudo chmod 777 /usr/local/bin/m4hFindSimModule
```
Testen des Programms mit   
`/usr/local/bin/m4hFindSimModule`   

[Zum Seitenanfang](#up)   
<a name="x90"></a>   

## 8.4 m4hControl
Das Programm `m4hControl` ist die MQTT-Haussteuerung auf dem RasPi. Teile davon werden [im Bereich "Hilfsprogramme"](/md/m4h310_RasPiCppDemos.md) beschrieben.   


# 9. Feste USB-Schnittstellen-Namen durch udev-Regeln   
## 9.1 Einleitung   
Beim Booten weist das RasPi die Schnittstellen-Namen `ttyUSB0` und `ttyUSB1` zuf&auml;llig den USB-Anschl&uuml;ssen zu. Das bedeutet, dass der ZigBee-Adapter manchmal an  
`ttyUSB0` oder manchmal an `ttyUSB1` h&auml;ngt, obwohl er hardwarem&auml;&szlig;ig nicht umgesteckt wurde.   
Eine M&ouml;glichkeit, in einem Programm immer den gleichen Schnittstellennamen zu verwenden, ist, einen symbolischen Link zu definieren.
Beispiel: `ttyUSB_Zigbee` soll auf `/dev/ttyUSB0` zeigen   
```
sudo ln -s /dev/ttyUSB0 /dev/ttyUSB_Zigbee
```
Kontrolle:   
`ls -l /dev/ttyUSB_Zigbee`   

Besser ist es, eine fixe - und richtige - Zuordnung zu einem Schnittstellen-Namen mit Hilfe von udev-Regeln zu erreichen. Die folgende Anleitung stammt von der Seite   
[https://www.heise.de/ratgeber/RasPi-Feste-USB-Schnittstellen-Namen-durch-udev-Regeln-4836365.html?seite=all](https://www.heise.de/ratgeber/RasPi-Feste-USB-Schnittstellen-Namen-durch-udev-Regeln-4836365.html?seite=all)   

## 9.2 Eigenschaften der verwendeten USB-Ger&auml;te ermitteln
Anzeige der USB-Ger&auml;te durch   
`lsusb`   
ergibt zum Beispiel   
```
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 005: ID 1997:2433 Shenzhen Riitek Technology Co., Ltd wireless mini keyboard with touchpad
Bus 001 Device 004: ID 0403:6001 Future Technology Devices International, Ltd FT232 Serial (UART) IC
Bus 001 Device 003: ID 10c4:ea60 Silicon Labs CP210x UART Bridge
Bus 001 Device 002: ID 2109:3431 VIA Labs, Inc. Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```
Man erkennt:   
* An Bus 001 Device 005 h&auml;ngt eine Tastatur
* An Bus 001 Device 003 h&auml;ngt ein USB-UART-Umsetzer (Bridge zu einem Modem)
* An Bus 001 Device 002 h&auml;ngt ein ZigBee-Stick

Die Eingabe von   
`udevadm info -a -n /dev/ttyUSB0`   
ergibt f&uuml;r den Zigbee-Stick "Sonoff_Zigbee_3.0_USB_Dongle_Plus" die Ger&auml;te-Information im udev-Format   
```
looking at device '/devices/platform/scb/fd500000.pcie/pci0000:00/0000:00:00.0/00
    KERNEL=="ttyUSB0"
    SUBSYSTEM=="tty"
    DRIVER==""
    ATTR{power/control}=="auto"
    ATTR{power/runtime_active_time}=="0"
    ATTR{power/runtime_status}=="unsupported"
    ATTR{power/runtime_suspended_time}=="0"

  looking at parent device '/devices/platform/scb/fd500000.pcie/pci0000:00/0000:00:
    KERNELS=="ttyUSB0"
    SUBSYSTEMS=="usb-serial"
    DRIVERS=="cp210x"
    ATTRS{port_number}=="0"
    ATTRS{power/control}=="auto"
    ATTRS{power/runtime_active_time}=="0"
    ATTRS{power/runtime_status}=="unsupported"
    ATTRS{power/runtime_suspended_time}=="0"

  looking at parent device '/devices/platform/scb/fd500000.pcie/pci0000:00/0000:00:
    KERNELS=="1-1.1:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="cp210x"
    ATTRS{authorized}=="1"
    ATTRS{bAlternateSetting}==" 0"
    ATTRS{bInterfaceClass}=="ff"
    ATTRS{bInterfaceNumber}=="00"
    ATTRS{bInterfaceProtocol}=="00"
    ATTRS{bInterfaceSubClass}=="00"
    ATTRS{bNumEndpoints}=="02"
    ATTRS{supports_autosuspend}=="1"

  looking at parent device '/devices/platform/scb/fd500000.pcie/pci0000:00/0000:00:
    KERNELS=="1-1.1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="00"
    ATTRS{bDeviceProtocol}=="00"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="100mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0100"
    ATTRS{bmAttributes}=="80"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="3"
    ATTRS{devpath}=="1.1"
    ATTRS{devspec}=="(null)"
    ATTRS{idProduct}=="ea60"
    ATTRS{idVendor}=="10c4"
```
Ende mit &lt;strg&gt;c   

Folgende Eigenschaften werden zum Identifizieren des ZigBee-Sticks verwendet:   
```
SUBSYSTEM==`tty`
ATTRS{idProduct}=="ea60"
ATTRS{idVendor}=="10c4"
```
Auf die gleiche Weise erh&auml;lt man mit   
`udevadm info -a -n /dev/ttyUSB1`   
f&uuml;r den USB-Serial-Wandler (an dem das Modem h&auml;ngt)   
```
SUBSYSTEM==`tty`
ATTRS{idProduct}=="6001"
ATTRS{idVendor}=="0403"
```

Mit diesen Informationen werden die Zuordnungsregeln erstellt:   
`sudo nano /etc/udev/rules.d/99-usb.rules`   
Inhalt   
```   
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", SYMLINK+="ttyUSB_Zigbee"
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", SYMLINK+="ttyUSB_Modem"
```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x

Neustarten des RasPi mit   
`sudo reboot`   

Kontrolle der Schnittstellenzuordnung:   
`dir -lh /dev/ttyUSB*`   

Die Schnittstellenzuordnung kann auch mit dem zuvor erstellten Programm `m4hFindSimModule` &uuml;berpr&uuml;ft werden. Es m&uuml;sste das gleiche Ergebnis liefern wie die Zuordnung von `ttyUSB_Modem`:   
`/usr/local/bin/m4hFindSimModule`   

[Zum Seitenanfang](#up)   
<a name="x100"></a>   

# 10. Eigene Autostart-Datei `autostart.sh`
M&ouml;chte man beim System-Start eigene Programme automatisch starten, so kann man diese zum Beispiel in einem eigenen Script "`autostart.sh`" sammeln. Das folgende Beispiel f&uuml;r eine Autostart-Datei macht folgendes:   
* Definition einer log()-Funktion, die Ausgaben in die log-Datei `/var/log/autostart.log` schreibt,
* &auml;ndern der Anzeige-Farbe auf "gelb",
* maximal 8 Sekunden (`ZB_TIMEOUT`) warten, bis ein symbolischer Link `ttyUSB_Zigbee` erstellt wurde und Ausgabe des Ergebnisses in die log-Datei,
* eine Sekunde warten,
* Steuerprogramm `/usr/local/bin/m4hControl` starten, 
* 5 Sekunden warten,
* ins Verzeichnis /opt/zigbee2mqtt wechseln und dort zigbee2mqtt starten,
* ins Arbeitsverzeichnis des Benutzers wechseln,
* die Anzeige-Farbe zur&uuml;cksetzen.

1. Autostart-Datei erzeugen.   
```
sudo nano /usr/local/bin/autostart.sh
```   
Inhalt der Datei   
```
#!/bin/bash
# autostart.sh - gestartet beim Systemstart
# Karl Hartinger, 11.07.2025

ZB_TIMEOUT=8
ZB_SECS_WAITED=0

# Farbdefinitionen
YELLOW='\033[01;33m'
RESET='\033[0m'

logfile="/var/log/autostart.log"

# Funktion zum Loggen mit Zeitstempel
log() {
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$logfile"
}

# Skriptstart
echo -e "${YELLOW}"
log "_____autostart.sh______11.07.2025___Karl Hartinger_____"

while [ ! -e /dev/ttyUSB_Zigbee ] && [ $ZB_SECS_WAITED -lt $ZB_TIMEOUT ]; do
  echo "Warte auf /dev/ttyUSB_Zigbee..."
  sleep 1
  SECONDS_WAITED=$((SECONDS_WAITED + 1))
done
if [ ! -e /dev/ttyUSB_Zigbee ]; then
  log "Fehler: /dev/ttyUSB_Zigbee nach $TIMEOUT Sekunden nicht gefunden."
else
  # Ziel des symbolischen Links ermitteln (zB /dev/ttyUSB0)
  link_target=$(readlink -f /dev/ttyUSB_Zigbee)
  log "OK: /dev/ttyUSB_Zigbee zeigt auf $link_target."
  #/usr/local/bin/zigbee-config-switch.sh
fi

log "1 Sekunde warten.."
sleep 1

log "MQTT Steuerung starten (m4hControl)"
nohup /usr/local/bin/m4hControl /usr/local/bin/m4h.conf -q >> "$logfile" 2>&1 &

log "5 Sekunden warten..."
sleep 5

log "Zigbee2MQTT starten"
if cd /opt/zigbee2mqtt; then
  sudo -u pi7 nohup npm run start >> "$logfile" 2>&1 &
else
  log "Fehler: Verzeichnis /opt/zigbee2mqtt nicht gefunden!"
fi

cd ~ || log "Fehler: Home-Verzeichnis nicht gefunden"

log "_______________________________________________________"
echo -e "${RESET}"
exit 0
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

__*Anmerkung:*__   
  * Soll beim Start einer Datei nicht auf das Ende des Programmes gewartet werden, so muss am Ende der Aufruf-Zeile ein "kaufm&auml;nnisches-und"-Zeichen (Ampersand) &amp; stehen oder am Beginn der Zeile muss `nohup ` stehen!   

2. Skript f&uuml;r alle User ausf&uuml;hrbar machen.   
```   
sudo chown root /usr/local/bin/autostart.sh
sudo chmod 777 /usr/local/bin/autostart.sh
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
After=dev-ttyUSB_Zigbee.device
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/autostart.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
```   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

4. Service aktivieren, damit es beim Booten l&auml;uft:   
```   
sudo systemctl enable autostart.service
```   

5. Service testen:
```   
sudo systemctl start autostart.service
```   

6. Service-Status &uuml;berpr&uuml;fen:
```   
sudo systemctl status autostart.service
```   

7. Log-Datei anzeigen:   
```   
cat /var/log/autostart.log
```   

[Zum Seitenanfang](#up)   
<a name="x110"></a>   

# 11. Zigbee2MQTT
## 11.1 Zigbee2mqtt installieren
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
`cp /opt/zigbee2mqtt/data/configuration.example.yaml /opt/zigbee2mqtt/data/configuration.yaml
`   
  oder eine bestehende Konfigurationsdatei verwenden...   

In der Konfigurationsdatei sollte die Schnittstelle auf `/dev/ttyUSB_Zigbee` ge&auml;ndert werden:   
`sudo nano /opt/zigbee2mqtt/data/configuration.yaml`
Anpassen oder erg&auml;nzen:   
```   
serial:
  port: /dev/ttyUSB_Zigbee
```   
Wichtig: GENAU zwei Leerzeichen vor dem Wort `port:`   

3. Testen, ob Zigbee2MQTT richtig startet:   
  `cd /opt/zigbee2mqtt`   
  `npm start`   
  Wenn alles passt, erh&auml;lt man die Meldung   
  `z2m: Zigbee2MQTT started!`   
  Beenden des Programms mit &lt;strg&gt;c   
  Falls der MQTT-Server nicht l&auml;uft erh&auml;lt man die Fehlermeldung `error:    z2m: MQTT failed to connect, exiting...`.   
  Beenden mit &lt;strg&gt;c   

## 11.2 Zigbee-Schnittstellenprobleme
Wenn es Probleme mit dem symbolischen Link `/dev/ttyUSB_Zigbee` gibt, kann man auch zwei Konfigurationsdateien mit unterschiedlichen `port:`-Anweisungen anlegen und die gerade g&uuml;ltige Datei nach `/opt/zigbee2mqtt/data/configuration.yaml` kopieren...   
```
cp /opt/zigbee2mqtt/data/configuration.yaml /opt/zigbee2mqtt/data/configz.yaml
cp /opt/zigbee2mqtt/data/configuration.yaml /opt/zigbee2mqtt/data/config0.yaml
cp /opt/zigbee2mqtt/data/configuration.yaml /opt/zigbee2mqtt/data/config1.yaml
```
Dateien bearbeiten:   
`nano /opt/zigbee2mqtt/data/config0.yaml`   
&Auml;ndern:
```
serial:
  port: /dev/ttyUSB0
```

`nano /opt/zigbee2mqtt/data/config1.yaml`   
&Auml;ndern:
```
serial:
  port: /dev/ttyUSB1
```

`nano /opt/zigbee2mqtt/data/configz.yaml`   
&Auml;ndern:
```
serial:
  port: /dev/ttyUSB_Zigbee
```

In diesem Fall muss die `autostart.sh`-Datei entsprechend ge&auml;ndert werden:   
`sudo nano /usr/local/bin/autostart.sh`   
Inhalt:   
```
#!/bin/bash
# autostart.sh - gestartet beim Systemstart
# Karl Hartinger, 11.07.2025

# Zigbee-Variable
ZB_TIMEOUT=8
ZB_SECS_WAITED=0
ZB_CONFIG_DIR="/opt/zigbee2mqtt/data"
ZB_TARGET_CONFIG="$ZIGBEE_CONFIG_DIR/configuration.yaml"


# Farbdefinitionen
YELLOW='\033[01;33m'
RESET='\033[0m'

logfile="/var/log/autostart.log"

# Funktion zum Loggen mit Zeitstempel
log() {
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$logfile"
}

# Skriptstart
echo -e "${YELLOW}"
log "_____autostart.sh______11.07.2025___Karl Hartinger_____"

while [ ! -e /dev/ttyUSB_Zigbee ] && [ $ZB_SECS_WAITED -lt $ZB_TIMEOUT ]; do
  echo "Warte auf /dev/ttyUSB_Zigbee..."
  sleep 1
  SECONDS_WAITED=$((SECONDS_WAITED + 1))
done
if [ ! -e /dev/ttyUSB_Zigbee ]; then
  log "Fehler: /dev/ttyUSB_Zigbee nach $TIMEOUT Sekunden nicht gefunden."
else
  # Ziel des symbolischen Links ermitteln (zB /dev/ttyUSB0)
  link_target=$(readlink -f /dev/ttyUSB_Zigbee)
  log "OK: /dev/ttyUSB_Zigbee zeigt auf $link_target."
  #/usr/local/bin/zigbee-config-switch.sh
fi

# Symbolischen Link pr&uuml;fen
LINK_TARGET=$(readlink -f /dev/ttyUSB_Zigbee)

echo "Symbolischer Link zeigt auf: $LINK_TARGET"

case "$LINK_TARGET" in
    /dev/ttyUSB0) CONFIG_FILE="config0.yaml" ;;
    /dev/ttyUSB1) CONFIG_FILE="config1.yaml" ;;
    *)
        log "Unbekanntes Ger&auml;t: $LINK_TARGET – keine Konfiguration kopiert."
        exit 1
        ;;
esac

if cp "$ZB_CONFIG_DIR/$CONFIG_FILE" "$ZB_TARGET_CONFIG"; then
    log "$CONFIG_FILE wurde kopiert."
else
    log "Fehler beim Kopieren von $CONFIG_FILE"
fi

log "1 Sekunde warten.."
sleep 1

log "MQTT Steuerung starten (m4hControl)"
nohup /usr/local/bin/m4hControl /usr/local/bin/m4h.conf -q >> "$logfile" 2>&1 &

log "5 Sekunden warten..."
sleep 5

log "Zigbee2MQTT starten"
if cd /opt/zigbee2mqtt; then
  sudo -u pi7 nohup npm run start >> "$logfile" 2>&1 &
else
  log "Fehler: Verzeichnis /opt/zigbee2mqtt nicht gefunden!"
fi

cd ~ || log "Fehler: Home-Verzeichnis nicht gefunden"

log "_______________________________________________________"
echo -e "${RESET}"
exit 0
```
---   
## 11.3 Zigbee2mqtt als Service
Falls man Zigbee2mqtt als Service automatisch starten m&ouml;chte (und nicht in der eigenen `autostart.sh`-Datei):   
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

Falls man das Service aktivieren m&ouml;chte:   
```
sudo systemctl daemon-reexec
sudo systemctl enable zigbee2mqtt
sudo systemctl start zigbee2mqtt
```

Kontrolle, ob das Service l&auml;uft:   
```
sudo systemctl status zigbee2mqtt
```

Und falls man das Service deaktivieren m&ouml;chte:   
```
sudo systemctl disable zigbee2mqtt
```
In diesem Fall wird `zigbe2mqtt` beim Systemstart nicht mehr automatisch gestartet.   
Nur Stoppen des Services geht mit   
`sudo systemctl stop zigbee2mqtt`   

[Zum Seitenanfang](#up)   
<a name="x110"></a>   

# 12. Kiosk-Modus
Der "Kiosk-Modus" ist eine Betriebsart von Rechnern bzw. Terminals mit graphischer Anzeige, bei der die Rechte des Users eingeschr&auml;nkt sind und nur bestimmte Aktionen ausgef&uuml;hrt werden k&ouml;nnen.   
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
  Hinzuf&uuml;gen (Bildschirm nie ausschalten, Chromium starten):   
  `@xset s off`   
  `@xset -dpms`   
  `@xset s noblank`   
  `@chromium-browser --noerrdialogs --disable-infobars --kiosk http://localhost`   
   Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

4. RasPi neu starten   
  `sudo reboot`   

[Zum Seitenanfang](#up)   
<a name="x130"></a>   

# 13. Dies und Das
## 13.1 Vorhandene Sicherungsdateien verwenden
Mit Hilfe des Programmes WinSCP kann man vorhandene, auf dem PC gesicherte Dateien zur&uuml;ck auf das RasPi speichern. Dazu startet man WinSCP, gibt die IP-Adresse des RasPi (zB `10.1.1.1`), den Benutzernamen (zB `pi_`) und das Passwort (zB `pass`) ein (Portnummer 22) und meldet sich an.   

Wenn der Zugriff auf gesch&uuml;tzte Verzeichnisse am RasPi verweigert wird, hilft folgendes Work-Around:   
1. Hilfsverzeichnis im Arbeitsverzeichnis des RasPi erstellen, zB mit   
`mkdir /~/temp`   
2. Hineinkopieren der Dateien vom PC ins Hilfsverzeichnis und Weiterkopieren der Dateien mit sudo-Rechten, zB   
```
sudo cp ~/temp/autostart.sh /usr/local/bin
sudo chown root /usr/local/bin/autostart.sh
sudo chmod 777 /usr/local/bin/autostart.sh
```

## 13.2 Periodisches Senden von MQTT-Nachrichten
Das periodische Senden von MQTT-Nachrichten ist im Kapitel [/md/m4h104_RasPi_crontab.md](/md/m4h104_RasPi_crontab) beschrieben.   

## 13.3 Display-Helligkeit einstellen
Beim OS "Debian Bookworm" mit X11 (X.Org) als Display-Server wird die Helligkeit &uuml;ber das Verzeichnis `/sys/class/backlight/...` gesteuert. In diesem befindet sich ein symbolischer Link auf das Display, aus dem man den Namen des Backlight-Devices (zB 10-0045) entnehmen kann:   
`ls -l /sys/class/backlight/`  

Die Einstellung der Helligkeit erfolgt durch Eintrag eines Wertes von 0 bis 255 in die Datei `/sys/class/backlight/10-0045/brightness`, zB durch   
`echo 128 | sudo tee /sys/class/backlight/10-0045/brightness`   

---

## 13.4 Bildschirmschoner abschalten (alte Version)
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