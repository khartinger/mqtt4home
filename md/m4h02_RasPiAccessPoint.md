Letzte &Auml;nderung: 11.10.2021   
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi als Access Point</h1>
<a href="../liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Ziele
### * Erzeugen eines WLANs mit folgenden Eigenschaften:   

|                              |                 |   
| ---------------------------- | --------------- |   
| WLAN-Name:                   | `Raspi11`       |   
| WLAN-Passwort:               | `12345678`      |   
| Verschl&uuml;sselung: &nbsp; | `WPA2`          |   
| IP-Adresse:                  | `10.1.1.1`      |   
| Netmask:                     | `255.255.248.0` |   

### * Verbindung zu einem Router (bzw. zum Internet) &uuml;ber den LAN-Anschluss.   

|                   |                 |   
| ----------------- | --------------- |   
| Router-IP: &nbsp; | `192.168.1.2`   |   
| Netmask:          | `255.255.248.0` |   
| Gateway:          | `192.168.0.1`   |   

### * Bereitstellung des Internet-Zugangs f&uuml;r die WLAN-Teilnehmer.
   
## Ben&ouml;tigte Hilfsmittel

* Hardware: RasPi
* Software: Internetzugang f&uuml;r das RasPi
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop

## Variante 1: Verwendung von /etc/dhcpcd.conf
Ab Raspbian Jessie (2016) ist der DHCP Client Daemon (DHCPCD) aktiviert und somit ist dies die neue Art, einen Access-Point auf dem Raspberry zu installieren. Es kann allerdings Probleme mit `dnsmasq` geben, daher ist die &auml;ltere Art der Installation (mit `/etc/network/interfaces`) ebenfalls weiter unten beschrieben.   
Die nachfolgende Installation geht davon aus, dass die RasPi-Grundinstallation (wie zB in [Raspberry Pi installieren](m4h01_RasPiInstall.md) beschrieben) durchgef&uuml;hrt wurde.   
Quelle: [https://www.elektronik-kompendium.de/sites/raspberry-pi/2002171.htm](https://www.elektronik-kompendium.de/sites/raspberry-pi/2002171.htm) (11.10.2021)   
Quelle: [https://www.raspberrypi.org/documentation/configuration/wireless/access-point-bridged.md](https://www.raspberrypi.org/documentation/configuration/wireless/access-point-bridged.md) (11.10.2021)

### Installation der erforderlichen Pakete
```
 sudo apt-get update
 sudo apt-get upgrade
 sudo apt-get install dnsmasq hostapd
```

### WLAN konfigurieren
Soll das RasPi als Access Point arbeiten, ben&ouml;tigt der WLAN-Adapter _wlan0_ eine statische IP. Normalerweise ist f&uuml;r die WLAN-Konfiguration die Software "wpa_supplicant" zust&auml;ndig, die Funknetze erkennt und automatisch die Verbindung herstellt. In diesem Fall wird wpa_supplicant nicht verwendet (Eintrag `nohook wpa_supplicant`).   
F&uuml;r das LAN wird ebenfalls eine statische IP gew&auml;hlt.   

* Netzwerk-Konfiguration in `/etc/dhcpcd.conf` festlegen:   
   `sudo nano /etc/dhcpcd.conf`   
   Am Ende der Datei anh&auml;ngen:
```   
#-----statische IP--------------------
interface eth0
static ip_address=192.168.1.2/21
static routers=192.168.0.1
static domain_name_servers=192.168.0.1 8.8.8.8

interface wlan0
static ip_address=10.1.1.1/21
nohook wpa_supplicant
static routers=192.168.0.1 8.8.8.8
#static domain_name_servers=192.168.0.1 8.8.8.8
```   

* DHCP Client Daemon neu starten:   
   `sudo systemctl restart dhcpcd`   

* Kontrolle, ob beide Netzwerkschnittstellen (eth0, wlan0) vorhanden sind:   
   `ip l`   
   Es m&uuml;ssen beide Schnittstellen (eth0 und wlan0) angezeigt werden.   
   Anzeige einer etwaigen IP-Adresse:   
   `ifconfig`   

### DHCP- und DNS-Server konfigurieren
Der "dnsmasq"-Daemon sorgt daf&uuml;r, dass WLAN-Clients nach der Anmeldung eine IP bekommen und die Server-Namensaufl&ouml;sung durchgef&uuml;hrt wird.   

1. Original-Konfigurationsdatei sichern und neue Konfigurationsdatei anlegen:   
 `sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig`   
 `sudo nano /etc/dnsmasq.conf`   
 Inhalt der Datei   
 ```
#_______/etc/dnsmasq.conf_________________________________
interface=wlan0           # Use interface wlan0
no-dhcp-interface=eth0    # no dhcp for lan
dhcp-range=interface:wlan0,10.1.1.150,10.1.1.199,24h # Assign IP addresses in this
                          # range with a 24 hour lease time
dhcp-option=option:dns-Server,10.1.1.1 # DNS
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Test der Konfiguration:   
   `dnsmasq --test -C /etc/dnsmasq.conf`   
   Ergebnis: _`dnsmasq: Syntaxpr&uuml;fung OK.`_   

3. Den dnsmasq Server neu starten und den Status anzeigen:   
   `sudo systemctl restart dnsmasq`   
   `sudo systemctl status dnsmasq`   

4. Den dnsmasq Server beim Systemstart automatisch starten:   
   `sudo systemctl enable dnsmasq`   
   _`Synchronizing state of dnsmasq.service with SysV service script with /lib/systemd/systemd-sysv-install.`_   
   _`Executing: /lib/systemd/systemd-sysv-install enable dnsmasq`_   

5. __ODER__ den dnsmasq Server beim Systemstart automatisch starten:   
   `sudo service dnsmasq start`   
   `sudo update-rc.d dnsmasq enable`   

### WLAN-AP-Host einrichten
Der Host Access Point Daemon (kurz "hostapd") stellt das WLAN zur Verf&uuml;gung.
1. Neue Konfigurationsdatei f&uuml;r hostapd erstellen:   
`sudo nano /etc/hostapd/hostapd.conf`   
Folgendes hineinschreiben:   
```
# ________WLAN router____________________________________________
# name of the WiFi interface we want to use
interface=wlan0
# Use the nl80211 driver with the brcmfmac driver
driver=nl80211

# _______WLAN configuration_____________________________________
# This is the name of the network (change to your values!)
ssid=Raspi11
# Use the 2.4GHz band
hw_mode=g
# Use channel 6
channel=6
# Enable 802.11n and 802.11d
ieee80211n=1
# ieee80211d=1
# country_code=DE
# Enable WMM
wmm_enabled=1
# Enable 40MHz channels with 20ns guard interval
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]
# Accept all MAC addresses
macaddr_acl=0

# _______WLAN encryption________________________________________
# Use WPA authentication
auth_algs=1
# Require clients to know the network name
ignore_broadcast_ssid=0
# Use WPA2
wpa=2
# Use a pre-shared key
wpa_key_mgmt=WPA-PSK
# Network password (passphrase)
wpa_passphrase=12345678
# Use AES, instead of TKIP
rsn_pairwise=CCMP
```
Folgende Werte anpassen:   
* `ssid ............. ` Name des Netzwerkes   
* `wpa_passphrase ... ` Netzwerk-Schl&uuml;ssel (Passwort, mindestens 8 Zeichen)   
* `channel .......... ` Sendekanal. Wert von 1 bis 13 m&ouml;glich   

Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Leserechte auf die Konfigurationsdatei &auml;ndern,   
   da das Passwort im Klartext angegeben ist:   
   `sudo chmod 600 /etc/hostapd/hostapd.conf`   

### WLAN-AP-Host-Konfiguration testen
* Test der Konfiguration durch Eingabe von   
`sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf`   
ergibt   
`Configuration file: /etc/hostapd/hostapd.conf`   
`wlan0: Could not connect to kernel driver`   
`Using interface wlan0 with hwaddr dc:a6:32:e0:2b:f6 and ssid "Raspi11"`   
`wlan0: interface state UNINITIALIZED->ENABLED`   
`wlan0: AP-ENABLED`   
Das Netzwerk "Raspi11" ist zwar zu sehen, man kann sich aber noch nicht einloggen, da man keine IP erh&auml;lt.   
Abbrechen mit &lt;Strg&gt; c

* Eine ausf&uuml;hrliche Ausgabe erh&auml;lt man durch Start des WLAN-Hosts im Debug-Modus:   
   `sudo hostapd -dd /etc/hostapd/hostapd.conf`   
   Abbrechen mit &lt;strg&gt;c und nach folgender Zeile suchen:   
   _`wlan0: AP-ENABLED`_   

### WLAN-AP-Host als Daemon im Hintergrund starten
* Daemon-Betrieb in der Konfigurationsdatei aktivieren:   
   `sudo nano /etc/default/hostapd`   
   Unter der Zeile `#DAEMON_CONF=""` oder am Ende der Datei einf&uuml;gen:   
   `RUN_DAEMON=yes`   
   `DAEMON_CONF="/etc/hostapd/hostapd.conf"`   
   Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x
* WLAN-AP-Host in Betrieb nehmen:   
   `sudo systemctl unmask hostapd`   
   `sudo systemctl enable hostapd`   
   `sudo systemctl start hostapd`   
* Status des Daemons anzeigen:   
   `sudo systemctl status hostapd`   
   Folgende Zeilen sollten angezeigt werden:   
   _`Loaded: loaded (/lib/systemd/system/hostapd.service; enabled; ...`_   
   _`Active: active (running) since ...`_   
   _`...`_   
   _`Okt 11 12:47:24 raspi hostapd[3469]: wlan0: AP-ENABLED`_   

### IP4 forwarding erm&ouml;glichen
1. IP4 forwarding einschalten   
`sudo nano /etc/sysctl.conf`   
Entfernen des # Zeichens vor der Zeile mit   
`net.ipv4.ip_forward=1`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Netzwerkadress&uuml;bersetzung (NAT) zwischen wlan0 und eth0 einrichten:   
```
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

3. Abspeichern der NAT-Regeln in der Datei `/etc/iptables.ipv4.nat`:   
`sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"`   

4. Aufruf der NAT-Regeln bei jedem Systemstart:   
`sudo nano /etc/rc.local`   
VOR der Zeile exit 0 folgende Zeilen einf&uuml;gen:   
`iptables-restore < /etc/iptables.ipv4.nat`   

5. Raspberry neu starten:   
 `sudo reboot`   

### Test des Access-Points
__Achtung beim Neustart von `putty`__: Die IP-Adresse des RasPi ist nun `192.168.1.2` (!)   
1. Kontrolle, ob die Dienste "hostapd" und "dnsmasq" laufen:   
   `sudo systemctl status hostapd`   
   Ende der Anzeige mit &lt;strg&gt; c
   `ps ax | grep hostapd`   
   `sudo systemctl status dnsmasq`   
   `ps ax | grep dnsmasq`   

2. Im Terminal (oder Terminal-Programm) den Internetzugang testen:   
 `ping 8.8.8.8 -c 4`   

3. Auf einem Laptop/PC, der nicht &uuml;ber LAN mit dem Internet verbunden ist, das WLAN-Netzwerk "`Raspi11`"  ausw&auml;hlen und den Schl&uuml;ssel "`12345678`" eingeben.

4. Im Bowser auf dem Laptop zB die Webseite von Google aufrufen:   
 `http://www.google.com`   
 oder im Windows-cmd-Fenster   
 `ping 8.8.8.8`   
 eingeben.

---  
---   

## Variante 2: Verwendung von /etc/network/interfaces
Dies ist die &auml;ltere Art, einen Access-Point auf dem RasPi zu installieren, da es oft Probleme mit `dnsmasq` gab. Hier erfolgt die Konfiguration &uuml;ber `/etc/network/interfaces`, wodurch `dhcpcd` automatisch deaktiviert wird.   
Die nachfolgende Installation geht davon aus, dass die RasPi-Grundinstallation (wie zB in [Raspberry Pi installieren](m4h01_RasPiInstall.md) beschrieben) durchgef&uuml;hrt wurde.   

### Installation der erforderlichen Pakete
```
 sudo apt-get update
 sudo apt-get upgrade
 sudo apt-get install dnsmasq hostapd
```

### Statische IP f&uuml;r Interface wlan0
Da das Interface `wlan0` eine statische Adresse ben&ouml;tigt, darf man nicht &uuml;ber das WLAN mit dem Raspi verbunden sein. Daher Tastatur + Bildschirm oder LAN + putty verwenden!   

* eth0 und wlan0 mit einer statischen IP versehen:   
`sudo nano /etc/network/interfaces`   
Am Ende der Datei folgendes einf&uuml;gen:   
```
#source-directory /etc/network/interfaces.d

auto eth0
#iface eth0 inet dhcp
iface eth0 inet static
address   192.168.1.2
netmask   255.255.248.0
network   192.168.0.0
broadcast 192.168.0.255
gateway   192.168.0.1

auto wlan0
allow-hotplug wlan0
iface wlan0 inet static
address   10.1.1.1
netmask   255.255.248.0
network   10.1.1.0
broadcast 10.1.1.255
#wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
```
Durch das # Zeichen (Doppelkreuz, Nummernzeichen oder hash) wird die wpa-conf-Zeile unwirksam gemacht (Kommentar).
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x

* In der Konfigurationsdatei zum Service `dhcpcd` einen eventuellen Eintrag f&uuml;r `eth0` und `wlan0` auskommentieren:   
`sudo nano /etc/dhcpcd.conf`
Zeichen # setzen:   
```
#denyinterfaces wlan0
#-----statische IP--------------------
#interface eth0
#static ip_address=192.168.0.111/21
#static routers=192.168.0.1
#static domain_name_servers=192.168.0.1
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

### Access-Point-Software `hostapd` konfigurieren
1. Neue Konfigurationsdatei f&uuml;r hostapd erstellen:   
`sudo nano /etc/hostapd/hostapd.conf`   
Folgendes hineinschreiben:   
```
# name of the WiFi interface we want to use
interface=wlan0
# Use the nl80211 driver with the brcmfmac driver
driver=nl80211
# This is the name of the network (change to your values!)
ssid=Raspi11
# Use the 2.4GHz band
hw_mode=g
# Use channel 6
channel=6
# Enable 802.11n
ieee80211n=1
# Enable WMM
wmm_enabled=1
# Enable 40MHz channels with 20ns guard interval
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]
# Accept all MAC addresses
macaddr_acl=0
# Use WPA authentication
auth_algs=1
# Require clients to know the network name
ignore_broadcast_ssid=0
# Use WPA2
wpa=2
# Use a pre-shared key
wpa_key_mgmt=WPA-PSK
# Network password (passphrase)
wpa_passphrase=12345678
# Use AES, instead of TKIP
rsn_pairwise=CCMP
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Test der Konfiguration   
Eingabe von   
`sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf`   
ergibt   
`Configuration file: /etc/hostapd/hostapd.conf`   
`wlan0: Could not connect to kernel driver`   
`Using interface wlan0 with hwaddr dc:a6:32:e0:2b:f6 and ssid "Raspi11"`   
`wlan0: interface state UNINITIALIZED->ENABLED`   
`wlan0: AP-ENABLED`   
Das Netzwerk "Raspi11" ist zwar zu sehen, man kann sich aber noch nicht einloggen, da man keine IP erh&auml;lt.   
Abbrechen mit &lt;Strg&gt; c

3. Konfigurationsdatei f&uuml;r hostapd beim Start verf&uuml;gbar machen:   
`sudo nano /etc/default/hostapd`   
Unter der Zeile `#DAEMON_CONF=""` einf&uuml;gen:   
`DAEMON_CONF="/etc/hostapd/hostapd.conf"`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x

4. Bei Raspian-Versionen ab M&auml;rz 2019 eventuell zuerst hostapd freischalten:   
```
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
```

### DHCP- und DNS-Server dnsmasq konfigurieren
1. Original-Konfigurationsdatei sichern und neue Konfigurationsdatei anlegen:   
 `sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig`   
 `sudo nano /etc/dnsmasq.conf`   
 Inhalt der Datei   
 ```
interface=wlan0           # Use interface wlan0
listen-address=10.1.1.1   # Explicitly specify the address to listen on
bind-interfaces           # Bind to the interface to make sure
                          # we aren't sending things elsewhere
server=8.8.8.8            # Forward DNS requests to Google DNS
domain-needed             # Don't forward short names
bogus-priv                # Never forward addresses in
                          # the non-routed address spaces.
dhcp-range=10.1.1.150,10.1.1.199,24h # Assign IP addresses in this
                          # range with a 24 hour lease time
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Den dnsmasq Server beim Systemstart automatisch starten:   
`sudo service dnsmasq start`   
`sudo update-rc.d dnsmasq enable`   

### IP4 forwarding erm&ouml;glichen
1. IP4 forwarding einschalten   
`sudo nano /etc/sysctl.conf`   
Entfernen des # Zeichens vor der Zeile mit   
`net.ipv4.ip_forward=1`   
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

2. Netzwerkadress&uuml;bersetzung (NAT) zwischen wlan0 und eth0 einrichten:   
```
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

3. Abspeichern der NAT-Regeln in der Datei `/etc/iptables.ipv4.nat`:   
`sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"`   

4. Aufruf der NAT-Regeln bei jedem Systemstart:   
`sudo nano /etc/rc.local`   
VOR der Zeile exit 0 folgende Zeilen einf&uuml;gen:   
`iptables-restore < /etc/iptables.ipv4.nat`   

5. Raspberry neu starten:   
 `sudo reboot`   

### Test des Access-Points

1. Im Terminal (oder Terminal-Programm) den Internetzugang testen:   
 `ping 8.8.8.8 -c 4`   
 Achtung beim Neustart von `putty`: Die IP-Adresse des RasPi ist nun `192.168.1.2` (!)   

2. Auf einem Laptop/PC, der nicht &uuml;ber LAN mit dem Internet verbunden ist, das WLAN-Netzwerk "`Raspi11`"  ausw&auml;hlen und den Schl&uuml;ssel "`12345678`" eingeben.

3. Im Bowser auf dem Laptop zB die Webseite von Google aufrufen:   
 `http://www.google.com`   
 oder im Windows-cmd-Fenster   
 `ping 8.8.8.8`   
 eingeben.
