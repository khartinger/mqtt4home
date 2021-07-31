Last modified: 2021-07-31   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Basic installation of the Raspberry Pi</h1>
<a href="readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h01_RasPiInstall.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h02_RasPiAccessPoint_e.md">==> RasPi As Access Point</a>
</td></tr></table><hr>

## Targets

* Installation of the operating system "Raspberry Pi OS" on the RasPi.   
The following settings are selected in the project, which should be changed for security reasons:   

|           |                                 |   
| --------- | ------------------------------- |   
| Hostname: | `raspi` (instead of raspberry)  |   
| Username: | `pi_`   (instead of pi)         |   
| Passwort: | `pass`  (instead of raspberry)  |   
   

## Required tools

* Hardware: RasPi (incl. accessories), Micro-SD card reader.   
* Software: [programming software for the operating system ("imager")](https://www.raspberrypi.org/software)
* Software: Terminal program [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop   

## Procedure
### Copy the operating system to the SD card
1. Insert micro SD card (size 8GB or more) into the card reader, connect the card reader to the PC.   
2. Download the utility to write the SD card in Windows.   
 See [http://www.raspberrypi.org/software](http://www.raspberrypi.org/software). Download "`imager_1.6.2.exe`".   
Start the programming program and write the operating system image directly to the micro SD card.   

### First startup of the RasPi
When starting the RasPi for the first time, some settings have to be made. These can also be changed later.   
1. Insert micro SD card into the RasPi.   
2. Connect screen, keyboard, mouse, network cable to the RasPi.   
3. Apply operating voltage: RasPi starts in configuration mode.   
4. Configure operating system "Raspberry Pi OS":   
Select country, enter new password `pass`. Do not select WiFi network at this stage.   
Start software update, restart RasPi afterwards.   

### First settings on the RasPi

* __Enable interfaces__   
Click on [Menu] at the top left of the graphics screen - Settings - Raspberry Pi Configuration - [Interfaces].   

|                            |                  |                   |   
| -------------------------- | ---------------- | ----------------- |   
| Camera:                    | (&nbsp;) Enabled | (*) Disabled      |   
| SSH:                       | (*) Enabled      | (&nbsp;) Disabled |   
| VNC:                       | (&nbsp;) Enabled | (*) Disabled      |   
| SPI:                       | (*) Enabled      | (&nbsp;) Disabled |   
| I2C:                       | (*) Enabled      | (&nbsp;) Disabled |   
| Serial Port:               | (*) Enabled      | (&nbsp;) Disabled |   
| Serial Console:            | (&nbsp;) Enabled | (*) Disabled      |   
| Single Wire Bus:           | (&nbsp;) Enabled | (*) Disabled      |   
| GPIO Remote Access: &nbsp; | (&nbsp;) Enabled | (*) Disabled      |   
Click [OK] but do not restart yet: Click [No].
&nbsp;   
* __Change password and hostname, disable automatic login__.   
On the graphics screen top left click [Menu] - Settings - Raspberry Pi Configuration - [System].   

|                          |                               |       |   
| ------------------------ | ----------------------------- | ----- |   
| Password:                | [Change password.]            |       |   
| _...enter in submenu..._ | Enter new password: `pass`    |       |   
|                          | Confirm new password: `pass` [OK] [OK] |     |   
| Hostname:                | `raspi`                       | &nbsp;       |   
| Boot:                    | ( &nbsp;) To Desktop          | (*) To CLI   |   
| Automatic login:         | ( &nbsp;) Log in as user 'pi' | (*) Disabled |   
Click [OK] and allow the system to restart [OK].   
   

If you are not asked for a <b>reboot</b>, you can trigger the reboot like this:   

|                    |                                                |   
| ------------------ | ---------------------------------------------- |   
| Graphics screen    | Click on [Menu] in the upper left corner of the graphics screen - Logout - [Restart] |   
| From the command line:  | ```sudo reboot``` |   

The RasPi will now start in terminal mode and ask for the user name (`pi`) and password (`pass`).   

The graphics screen can be started at any time by entering the following command line:  
 
```
startx
```

### Prepare remote control
For many tasks it is easier to operate the RasPi from the PC, because then you can copy commands from instructions directly and execute them on the RasPi. A simple program for this is the terminal program [`putty`](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html). After the installation you only need the IP of the RasPi and the user name and password.   
If you connect the RasPi with a LAN cable to the PC, you can display the IP via the command line like this:   
```
ifconfig eth0   
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST> mtu 1500   
      inet 192.168.0.68 netmask 255.255.248.0 broadcast 192.168.0.255   
```
That means the IP address is 192.168.0.68   
If the putty connection is denied despite the correct IP address, then SSH may not be enabled on the interfaces (see above!).   

### Change default user name pi
Source: [https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/](https://jankarres.de/2013/09/raspberry-pi-standard-benutzername-pi-aendern/) [10/24/2017]</a>   
__Note: If user crontabs have already been created for user pi, they will be deleted. (Save!)__   
1. Start Putty (recommended) or switch the RasPi to command line mode (see above), reboot and enter user name (`pi`) and password (`pass`).      
2. Create auxiliary user temp:   
 ```
  sudo adduser --no-create-home temp   
  Enter a new UNIX password: pass   
  Enter the new UNIX password again: pass   
 ```
 Leave all other entries blank. (The default values will be used)
       
3. Grant the user `temp` sudo privileges   
 ```
 sudo visudo
 ```   
 Add at the end of the file:   
 ```temp ALL=(ALL) NOPASSWD: ALL```   
 Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
       
4. Reboot Raspberry Pi (```sudo reboot```) and log in as user `temp`:   
 ```
 raspi login: temp password: pass
 ```
 If you work with putty, you have to restart it too ;)
       
5. Rename user pi to pi_:   
 ```
 sudo usermod --move-home --login pi_ --home /home/pi_ pi
 ```   
 Note: If the English character set is set now: key - for ?, / for -.
        
6. Change the name of the group pi:   
 ```
 sudo groupmod -n pi_ pi
 ```
       
7. Reboot Raspberry Pi (```sudo reboot```) and log in as user `pi_`:   
 ```
 raspi login: pi_ password: pass
 ```
 If you work with putty, you have to restart it too ;)
       
8. Remove the sudo rights from the user `temp`:   
 ```
 sudo visudo
 ```   
 Add a comment character # at the beginning of the line:   
 ```#temp ALL=(ALL) NOPASSWD: ALL```   
 Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
       
9. Delete user `temp`   
 ```
 sudo deluser temp
 ```
       
---   
10. Change username for automatic login:   
 ```
 sudo nano /etc/lightdm/lightdm.conf
 ```   
 Change or add the following lines   
 ```#autologin-user=pi autologin-user=pi_```   
 Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
       
11. Further change:   
 ```
 sudo nano /etc/system/system/autologin@.service
 ```   
 Change or add the following lines   
 ```#ExecStart=-/sbin/agetty --autologin pi --noclear %I $TERM```   
 ```ExecStart=-/sbin/agetty --autologin pi_ --noclear %I $TERM```   
 Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
       
12. From the command line switch on the automatic start of the graphical user interface   
 ```
 sudo raspi-config
 ```   
 
 * The command line configuration menu appears:   
 
  | Raspberry Pi Software Configuration Tool | (raspi-config)            |   
  | ---------------------------------------- | ------------------------- |   
  | 1 System Options         | Configure system settings                 |   
  | 2 Display Options        | Configure display settings                |   
  | 3 Interface Options      | Configure connections to peripherals      |   
  | 4 Performance Options    | Configure performance settings            |   
  | 5 Localisation Options   | Configure language and regional settings  |   
  | 6 Advanced Options       | Configure advanced settings               |   
  | 8 Update                 | Update this tool to the latest version    |   
  | 9 About raspi-config     | Information about this configuration tool |   
  | <Select>                 | <Finish>                                  |   
 Select point 1 by pressing <enter> .   
 
 * Go to point S5 with the cursor key <down> .   
 ```S5 Boot / Auto Login Select boot into desktop or to command line```   
 and select with the <enter> key.   
    
 * Go to item B4 with the cursor key <down>.   
 ```B4 Desktop Autologin Desktop GUI, automatically logged in as 'pi_' user```   
 and select with the <enter>-key.   
    
 * Select <Finish> with the tab key and answer the question "Would you like to reboot now?" with <Yes>.   
 The RasPi boots and restarts the graphic screen automatically.   
    
 Switching on the graphic screen could of course also have been done on the RasPi by entering   
 ```
 startx
 ```   
 and switching on the graphic screen by clicking [Menu] in the upper left corner - Settings - Raspberry Pi Configuration - [System]...
   
## Other setting options
### Switch off screen saver
1. Create new directory with control file   
`sudo mkdir /etc/X11/xorg.conf.d`   
Create control file:   
`sudo nano /etc/X11/xorg.conf.d/01-disable-monitor-off.conf`  
Contents of the file:   
`Section "ServerFlags"`   
`Option "BlankTime" "0"`    
`Option "StandbyTime" "0"`   
`Option "SuspendTime" "0"`   
`Option "OffTime" "0"`   
`EndSection`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
      
2. Create another file:   
`nano ~/.xinitrc`   
Contents of the file:   
`xset s off`   
`xset -dpms`   
`xset s noblank`   
`exec /etc/alternatives/x-session-manager`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x
      
3. Reboot the RasPi   
`sudo reboot`

### Rotate display 180 degrees
If you use a transparent case from Amazon or BetterShopping (ART02314), you also have to rotate the display by 180&deg;.   
This is done simply by an entry in the file `/boot/config.txt`:   
```
sudo nano /boot/config.txt
```   
Add the following line to the end of the file   
```lcd_rotate=2```   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
The entry will not take effect until the next system startup.

### Own autostart file `autostart.sh`
If you want to start your own programs automatically at system startup, it makes sense to collect these commands in an own script "`autostart.sh`". For this the following steps are necessary.   

1. Create autostart file.   
```
sudo nano /usr/local/bin/autostart.sh
```   
Content of the file e.g.   
```
#!/bin/bash   
#...change the color of the font to yellow...   
echo -e "\\033[01;33m"   
printf "_____autostart.sh______26.07.2021_______khartinger_____\n"   
printf "_______________________________________________________\n"   
#...change the color of the font back to white...   
echo -e "\n[00m   
exit 0   
```
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
__*Notes:*__   
  * The file only changes the font color to yellow and prints the messages after `printf`.   
  * Own program calls must be between the two `printf` lines.   
  * Tip: If you do not want to wait for the end of the program when starting a file, there must be an "ampersand" at the end of the call line!   

      
2. Call own autostart file at system start.   
Edit system initialization file:   
```
sudo nano /etc/rc.local
```   
Before `exit 0` insert the following:   
```
#-----Calling a script with my own commands-----   
/usr/local/bin/autostart.sh   
```
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
      
3. Make script executable for all users   
```
sudo chmod 777 /usr/local/bin/autostart.sh
```
   
