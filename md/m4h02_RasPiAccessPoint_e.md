Last modified: 2021-07-31   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi as access point</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Create a WLAN with the following properties:   

|                              |                 |   
| ---------------------------- | --------------- |   
| WLAN name:                   | `Raspi11`       |   
| WLAN password: &nbsp;        | `12345678`      |   
| Encryption:                  | `WPA2`          |   
| IP address:                  | `10.1.1.1`      |   
| netmask:                     | `255.255.248.0` |   

* Connection to a router (or to the Internet) via the LAN port.   

|                   |                 |   
| ----------------- | --------------- |   
| Router IP: &nbsp; | `192.168.1.2`   |   
| Netmask:          | `255.255.248.0` |   
| Gateway:          | `192.168.0.1`   |   

* Providing Internet access for the WLAN subscribers.   

## Required tools

* Hardware: RasPi
* Software: Internet access for the RasPi
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop

## Procedure
The following installation assumes that the basic RasPi installation (as described in [Install Raspberry Pi](m4h01_RasPiInstall.md)) has been done. In newer RasPi installations the interface configuration is done by the service `dhcdcd`, but there are problems with `dnsmasq`. For this reason the configuration takes place as before over `/etc/network/interfaces`, whereby `dhcpcd` is deactivated automatically.   
Source: The installation is described e.g. on the page [https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/](https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/). 
described.   

### Installing the required packages
```
 sudo apt-get update
 sudo apt-get upgrade
 sudo apt-get install dnsmasq hostapd
```

### Static IP for interface wlan0
Because the interface `wlan0` needs a static address, you must not be connected to the Raspi via WLAN. Therefore use keyboard + screen or LAN + putty!   

* give eth0 and wlan0 a static IP:   
`sudo nano /etc/network/interfaces`.   
At the end of the file insert the following:   
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
The # sign (double cross, number sign or hash) will invalidate the wpa-conf line (comment).
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x

* In the configuration file for the service `dhcpcd` comment out any entry for `eth0` and `wlan0`:   
`sudo nano /etc/dhcpcd.conf`
Set character #:   
```
#denyinterfaces wlan0
#-----statische IP--------------------
#interface eth0
#static ip_address=192.168.0.111/21
#static routers=192.168.0.1
#static domain_name_servers=192.168.0.1
```
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

### Configure access point software `hostapd
1. Create a new configuration file for hostapd:   
`sudo nano /etc/hostapd/hostapd.conf`   
Write the following into it:   
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
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. Test of the configuration   
Input of   
`sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf`   
results in   
`Configuration file: /etc/hostapd/hostapd.conf`   
`wlan0: Could not connect to kernel driver`   
`Using interface wlan0 with hwaddr dc:a6:32:e0:2b:f6 and ssid "Raspi11"`   
`wlan0: interface state UNINITIALIZED->ENABLED`   
`wlan0: AP-ENABLED`   
The network "Raspi11" is visible, but you can't log in yet, because you don't get an IP.   
Cancel with &lt;Ctrl&gt; c

3. Make configuration file for hostapd available at startup:   
`sudo nano /etc/default/hostapd`   
Insert under the line `#DAEMON_CONF=""`:   
`DAEMON_CONF="/etc/hostapd/hostapd.conf"`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

4. For Raspian versions from March 2019, possibly enable hostapd first:   
```
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
```

### Configure DHCP and DNS server dnsmasq.
1. Save original configuration file and create new configuration file:   
 `sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig`   
 `sudo nano /etc/dnsmasq.conf`   
File content:   
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
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. Start the dnsmasq server automatically at system startup:   
`sudo service dnsmasq start`   
`sudo update-rc.d dnsmasq enable`   

### Enable IP4 forwarding
1. Enable IP4 forwarding   
`sudo nano /etc/sysctl.conf`   
Remove the # character before the line with   
`net.ipv4.ip_forward=1`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. Set up network address translation (NAT) between wlan0 and eth0:   
```
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

3. Save the NAT rules in the `/etc/iptables.ipv4.nat` file:   
`sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"`   

4. Call the NAT rules at every system start:   
`sudo nano /etc/rc.local`   
Insert the following lines BEFORE the exit 0 line:   
`iptables-restore < /etc/iptables.ipv4.nat`   

### Test of the access point
1. Reboot Raspberry:   
 `sudo reboot`   

2. Test the internet access in the terminal (or terminal program):   
 `ping 8.8.8.8 -c 4`   
 Attention when restarting `putty`: The IP address of the RasPi is now `192.168.1.2` (!)   

3. On a laptop/PC that is not connected to the internet via LAN, select the WLAN network `Raspi11` and enter the key `12345678`.

4. in Bowser on the laptop, go to the Google website, for example:   
 `http://www.google.com`   
 or in the Windows cmd window   
 `ping 8.8.8.8`   
 enter.

