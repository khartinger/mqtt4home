######
<table><tr><td><img src="logo/mqtt4home_96.png"></img></td><td>&nbsp;</td><td>
<h1>Raspberry Pi als Access Point</h1>
<a href="liesmich.md">==> Startseite</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint_e.md">==> English version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Ziele
* Erzeugen eines WLANs mit folgenden Eigenschaften:   

|                              |                 |   
| ---------------------------- | --------------- |   
| WLAN-Name:                   | `Raspi11`       |   
| WLAN-Passwort:               | `12345678`      |   
| Verschl&uuml;sselung: &nbsp; | `WPA2`          |   
| IP-Adresse:                  | `10.1.1.1`      |   
| Netmask:                     | `255.255.248.0` |   

* Verbindung zu einem Router (bzw. zum Internet) &uuml;ber den LAN-Anschluss.   

|                   |                 |   
| ----------------- | --------------- |   
| Router-IP: &nbsp; | `192.168.1.2`   |   
| Netmask:          | `255.255.248.0` |   
| Gateway:          | `192.168.0.1`   |   

* Bereitstellung des Internet-Zugangs f&uuml;r die WLAN-Teilnehmer
   
## Ben&ouml;tigte Hilfsmittel

* Hardware: RasPi
* Software: Internetzugang f&uuml;r das RasPi
* Software: Terminal-Programm [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) auf dem PC/Laptop

 
## Vorgangsweise
Die nachfolgende Installation geht davon aus, dass die RasPi-Grundinstallation (wie zB in [Raspberry Pi installieren](m4h01_RasPiInstall.md) beschrieben) durchgef&uuml;hrt wurde. In neueren RasPi-Installationen erfolgt die Interface-Konfiguration &uuml;ber das Service `dhcdcd`, allerdings gibt es da Probleme mit `dnsmasq`. Aus diesem Grund erfolgt die Konfiguration wie bisher &uuml;ber `/etc/network/interfaces`, wodurch `dhcpcd` automatisch deaktiviert wird.   
Quelle: Die Installation ist zB auf der Seite [https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/](https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/) 
beschrieben.   

### Installation der erforderlichen Pakete
```
 sudo apt-get update<br>
 sudo apt-get upgrade<br>
 sudo apt-get install dnsmasq hostapd<br>
```

### Statische IP f&uuml;r Interface wlan0
Da das Interface `wlan0` eine statische Adresse ben&ouml;tigt, darf man nicht &uuml;ber das WLAN mit dem Raspi verbunden sein. Daher Tastatur + Bildschirm oder LAN + putty verwenden!   

* eth0 und wlan0 mit einer statischen IP versehen:   
`sudo nano /etc/network/interfaces`   
Am Ende der Datei folgendes einf&uuml;gen:   
```
#source-directory /etc/network/interfaces.d<br>
<br>
auto eth0<br>
#iface eth0 inet dhcp<br>
iface eth0 inet static<br>
address   192.168.1.2<br>
netmask   255.255.248.0<br>
network   192.168.0.0<br>
broadcast 192.168.0.255<br>
gateway   192.168.0.1<br>
<br>
auto wlan0<br>
allow-hotplug wlan0<br>
iface wlan0 inet static<br>
address   10.1.1.1<br>
netmask   255.255.248.0<br>
network   10.1.1.0<br>
broadcast 10.1.1.255<br>
#wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf<br>
```
Durch das # Zeichen (Doppelkreuz, Nummernzeichen oder hash) wird die wpa-conf-Zeile unwirksam gemacht (Kommentar).<br>
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x

* In der Konfigurationsdatei zum Service `dhcpcd` einen eventuellen Eintrag f&uuml;r `eth0` und `wlan0` auskommentieren:   
`sudo nano /etc/dhcpcd.conf`
Zeichen # setzen:   
```
#denyinterfaces wlan0
#-----statische IP--------------------<br>
#interface eth0<br>
#static ip_address=192.168.0.111/21<br>
#static routers=192.168.0.1<br>
#static domain_name_servers=192.168.0.1<br>
```
Speichern und beenden durch &lt;Strg&gt;o &lt;Enter&gt; &lt;Strg&gt; x   

### Access-Point-Software `hostapd` konfigurieren
1. Neue Konfigurationsdatei f&uuml;r hostapd erstellen:   
`sudo nano /etc/hostapd/hostapd.conf`   
Folgendes hineinschreiben:   
```
# name of the WiFi interface we want to use<br>
interface=wlan0<br>
# Use the nl80211 driver with the brcmfmac driver<br>
driver=nl80211<br>
# This is the name of the network (change to your values!)<br>
<b>ssid=Raspi11</b><br>
# Use the 2.4GHz band<br>
hw_mode=g<br>
# Use channel 6<br>
channel=6<br>
# Enable 802.11n<br>
ieee80211n=1<br>
# Enable WMM<br>
wmm_enabled=1<br>
# Enable 40MHz channels with 20ns guard interval<br>
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]<br>
# Accept all MAC addresses<br>
macaddr_acl=0<br>
# Use WPA authentication<br>
auth_algs=1<br>
# Require clients to know the network name<br>
ignore_broadcast_ssid=0<br>
# Use WPA2<br>
wpa=2<br>
# Use a pre-shared key<br>
wpa_key_mgmt=WPA-PSK<br>
# Network password (passphrase)<br>
<b>wpa_passphrase=12345678</b><br>
# Use AES, instead of TKIP<br>
rsn_pairwise=CCMP<br>
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

4.Bei Raspian-Versionen ab M&auml;rz 2019 eventuell zuerst hostapd freischalten:   
```
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
```

### DHCP- und DNS-Server dnsmasq konfigurieren
1. Original-Konfigurationsdatei sichern und neue Konfigurationsdatei anlegen:   
 `sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig´   
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
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE<br>
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT<br>
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

3. Abspeichern der NAT-Regeln in der Datei `/etc/iptables.ipv4.nat`:   
`sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"`   

4. Aufruf der NAT-Regeln bei jedem Systemstart:   
`sudo nano /etc/rc.local`   
VOR der Zeile exit 0 folgende Zeilen einf&uuml;gen:   
`iptables-restore < /etc/iptables.ipv4.nat`   

### Test des Access-Points
1. Raspberry neu starten:   
 `sudo reboot`   

2. Im Terminal (oder Terminal-Programm) den Internetzugang testen:   
 `ping 8.8.8.8 -c 4`   
 Achtung beim Neustart von `putty`: Die IP-Adresse des RasPi ist nun `192.168.1.2` (!)   

3. Auf einem Laptop/PC, der nicht &uuml;ber LAN mit dem Internet verbunden ist, das WLAN-Netzwerk "`Raspi11`"  ausw&auml;hlen und den Schl&uuml;ssel "`12345678`" eingeben.

4. Im Bowser auf dem Laptop zB die Webseite von Google aufrufen:   
 `http://www.google.com`   
 oder im Windows-cmd-Fenster   
 `ping 8.8.8.8`   
 eingeben.
