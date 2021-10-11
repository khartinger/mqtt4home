Last modified: 2021-10-11   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi as access point</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
### * Create a WLAN with the following properties:   

|                              |                 |   
| ---------------------------- | --------------- |   
| WLAN name:                   | `Raspi11`       |   
| WLAN password: &nbsp;        | `12345678`      |   
| Encryption:                  | `WPA2`          |   
| IP address:                  | `10.1.1.1`      |   
| netmask:                     | `255.255.248.0` |   

### * Connection to a router (or to the Internet) via the LAN port.   

|                   |                 |   
| ----------------- | --------------- |   
| Router IP: &nbsp; | `192.168.1.2`   |   
| Netmask:          | `255.255.248.0` |   
| Gateway:          | `192.168.0.1`   |   

### * Providing Internet access for the WLAN subscribers.   

## Required tools

* Hardware: RasPi
* Software: Internet access for the RasPi
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop

## Variant 1: Using /etc/dhcpcd.conf
As of Raspbian Jessie (2016), the DHCP client daemon (DHCPCD) is enabled, so this is the new way to install an access point on the Raspberry. However, there may be problems with `dnsmasq`, so the older way of installation (using `/etc/network/interfaces`) is also described below.   
The following installation assumes that the basic RasPi installation (as described in [Install Raspberry Pi](m4h01_RasPiInstall.md), for example) has been performed.   
Source: [https://www.elektronik-kompendium.de/sites/raspberry-pi/2002171.htm](https://www.elektronik-kompendium.de/sites/raspberry-pi/2002171.htm) (2021-10-11)   
Source: [https://www.raspberrypi.org/documentation/configuration/wireless/access-point-bridged.md](https://www.raspberrypi.org/documentation/configuration/wireless/access-point-bridged.md) (2021-10-11)

### Installing the required packages
```
 sudo apt-get update
 sudo apt-get upgrade
 sudo apt-get install dnsmasq hostapd
```

### Configure WLAN
If the RasPi should work as an access point, the WLAN adapter _wlan0_ needs a static IP. Normally the WLAN configuration is done by the software "wpa_supplicant", which detects wireless networks and automatically establishes the connection. In this case, wpa_supplicant is not used (entry `nohook wpa_supplicant`).   
A static IP is also selected for the LAN.   

* Set network configuration in `/etc/dhcpcd.conf`:   
   `sudo nano /etc/dhcpcd.conf`   
   Append to the end of the file:
```   
#-----statische IP--------------------
interface eth0
static ip_address=192.168.1.2/21
static routers=192.168.0.1
static domain_name_servers=192.168.0.1 8.8.8.8

interface wlan0
static ip_address=10.1.1.1/21
nohook wpa_supplicant
static routers=192.168.0.1 8.8.8
#static domain_name_servers=192.168.0.1 8.8.8
```   

* Restart DHCP client daemon:   
   `sudo systemctl restart dhcpcd`   

* Check if both network interfaces (eth0, wlan0) are present:   
   `ip l`   
   Both interfaces (eth0 and wlan0) must be displayed.   
   Display of a possible IP address:   
   `ifconfig`   

### Configuring the DHCP and DNS server
The "dnsmasq" daemon ensures that WLAN clients are assigned an IP after logging in and that server name resolution is performed.   

1. save original configuration file and create new configuration file:   
 `sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig`   
 `sudo nano /etc/dnsmasq.conf`   
 Contents of the file   
 ```
#_______/etc/dnsmasq.conf_________________________________
interface=wlan0           # Use interface wlan0
no-dhcp-interface=eth0    # no dhcp for lan
dhcp-range=interface:wlan0,10.1.1.150,10.1.1.199,24h # Assign IP addresses in this
                          # range with a 24 hour lease time
dhcp-option=option:dns-server,10.1.1.1 # DNS
```
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. test the configuration:   
   `dnsmasq --test -C /etc/dnsmasq.conf`   
   Result: _`dnsmasq: syntax check OK.`_   

3. restart the dnsmasq server and display the status:   
   `sudo systemctl restart dnsmasq`   
   `sudo systemctl status dnsmasq`   

4. start the dnsmasq server automatically at system startup:   
   `sudo systemctl enable dnsmasq`   
   _`Synchronizing state of dnsmasq.service with SysV service script with /lib/systemd/systemd-sysv-install`_   
   _`Executing: /lib/systemd/systemd-sysv-install enable dnsmasq`_   

5. __OR__ start the dnsmasq server automatically at system startup:   
   `sudo service dnsmasq start`   
   `sudo update-rc.d dnsmasq enable`   

### Setting up the WLAN AP host
The host access point daemon (hostapd for short) provides the WLAN.
1. create a new configuration file for hostapd:   
`sudo nano /etc/hostapd/hostapd.conf`   
Write the following into it:   
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
Adjust the following values:   
* `ssid ............. ` name of the network.   
* `wpa_passphrase ... ` network key (password, at least 8 characters)   
* `channel .......... ` Transmit channel. Value from 1 to 13 possible   

Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. change read rights to the configuration file,   
   because the password is given in clear text:   
   `sudo chmod 600 /etc/hostapd/hostapd.conf`   

### Test WLAN AP host configuration
* Test the configuration by entering   
`sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf`   
results in   
`Configuration file: /etc/hostapd/hostapd.conf`   
`wlan0: Could not connect to kernel driver`   
`Using interface wlan0 with hwaddr dc:a6:32:e0:2b:f6 and ssid "Raspi11"`   
`wlan0: interface state UNINITIALIZED->ENABLED`   
`wlan0: AP-ENABLED`   
The network "Raspi11" is visible, but you can't log in yet, because you don't get an IP.   
Cancel with &lt;Ctrl&gt; c

* You can get a detailed output by starting the WLAN host in debug mode:   
   `sudo hostapd -dd /etc/hostapd/hostapd.conf`.   
   Cancel with &lt;ctrl&gt;c and search for the following line:   
   _`wlan0: AP-ENABLED`_   

### Start WLAN AP host as daemon in the background
* Enable daemon operation in the configuration file:   
   `sudo nano /etc/default/hostapd`   
   Under the line `#DAEMON_CONF=""` or at the end of the file insert:   
   `RUN_DAEMON=yes`   
   `DAEMON_CONF="/etc/hostapd/hostapd.conf"`   
   Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
* Put WLAN AP host into operation:   
   `sudo systemctl unmask hostapd`   
   `sudo systemctl enable hostapd`   
   `sudo systemctl start hostapd`   
* Display the status of the daemon:   
   `sudo systemctl status hostapd`   
   The following lines should be displayed:   
   _`Loaded: loaded (/lib/system/system/hostapd.service; enabled; ...`_   
   _`Active: active (running) since ...`_   
   _`...`_   
   _`Oct 11 12:47:24 raspi hostapd[3469]: wlan0: AP-ENABLED`_   

### Enable IP4 forwarding
1. enable IP4 forwarding   
`sudo nano /etc/sysctl.conf`   
Remove the # character before the line with   
`net.ipv4.ip_forward=1`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

2. set up network address translation (NAT) between wlan0 and eth0:   
```
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

3. save the NAT rules in the file `/etc/iptables.ipv4.nat`:   
`sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"`   

4. invoke NAT rules at every system startup:   
`sudo nano /etc/rc.local`   
Insert the following lines BEFORE the exit 0 line:   
`iptables-restore < /etc/iptables.ipv4.nat`   

5. reboot Raspberry:   
 `sudo reboot`   

### Test of the access point
__Watch out when restarting `putty`__: The IP address of the RasPi is now `192.168.1.2` (!)   
1. check if the services "hostapd" and "dnsmasq" are running:   
   `sudo systemctl status hostapd`   
   End the display with &lt;strg&gt; c
   `ps ax | grep hostapd`   
   `sudo systemctl status dnsmasq`   
   `ps ax | grep dnsmasq`   

2. in the terminal (or terminal program) test the internet access:   
 `ping 8.8.8 -c 4`   

3. on a laptop/PC that is not connected to the Internet via LAN, select the WLAN network "`Raspi11`" and enter the key "`12345678`".

4. in the Bowser on the laptop, go to the Google website, for example:   
 `http://www.google.com`   
 or in the Windows cmd window   
 `ping 8.8.8.8`   
 enter.

---  
---   

## Variant 2: Use /etc/network/interfaces.
This is the older way to install an access point on the RasPi, because there were often problems with `dnsmasq`. Here the configuration is done via `/etc/network/interfaces`, which automatically disables `dhcpcd`.   
The following installation assumes that the basic RasPi installation (as described in [Install Raspberry Pi](m4h01_RasPiInstall.md)) has been performed.   

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

5. Reboot Raspberry:   
 `sudo reboot`   

### Test of the access point

1. Test the internet access in the terminal (or terminal program):   
 `ping 8.8.8.8 -c 4`   
 Attention when restarting `putty`: The IP address of the RasPi is now `192.168.1.2` (!)   

2. On a laptop/PC that is not connected to the internet via LAN, select the WLAN network `Raspi11` and enter the key `12345678`.

3. in Bowser on the laptop, go to the Google website, for example:   
 `http://www.google.com`   
 or in the Windows cmd window   
 `ping 8.8.8.8`   
 enter.
