Last modified: 2021-08-08   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi as MQTT broker</h1>
<a href="readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h03_RasPiMQTTBroker.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
There are various libraries for using MQTT. In this case, the open source project Mosquitto is used, as it provides a broker, a sending client and a receiving client and runs on both Linux and Windows.   
The goals in detail:   
* Install Mosquitto on the RasPi.
* Install Mosquitto on the PC/laptop.
* Send MQTT messages from the PC and RasPi and display them on both devices.

## Required tools
* Hardware: RasPi with internet access
* Hardware: PC/Laptop with Internet access
 
## Procedure
### Installing the MQTT broker and clients on the RasPi
1. install Mosquitto Server   
   ```
   sudo apt-get install mosquitto
   ```
2. install Mosquitto clients (Publisher and Subscriber)   
   ```
   sudo apt-get install mosquitto-clients
   ```
3. install Mosquitto C library   
   ```
   sudo apt-get install libmosquitto-dev
   ```

### Testing the MQTT installation on the RasPi
To test the installation you need three things:   
1. a running program that forwards messages ("Broker").
2. a message sender ("Publisher").
3. a message receiver ("Subscriber").

This is done as follows:   
* First, the MQTT broker must be started:   
  `sudo /etc/init.d/mosquitto start`   
  `[ ok ] Starting mosquitto (via systemctl): mosquitto.service.`

* Next, a data receiver (subscriber) is set up that listens for messages of type "Test1":   
 `mosquitto_sub -d -t Test1`   
 `Client mosqsub/3152-raspberryp sending CONNECT`   
 `Client mosqsub/3152-raspberryp received CONNACK`   
 `Client mosqsub/3152-raspberryp sending SUBSCRIBE (Mid: 1, Topic: Test1, QoS: 0)`   
 `Client mosqsub/3152-raspberryp received SUBACK`   
 `Subscribed (mid: 1): 0`   

* Sending a message.   
  For this purpose it is necessary to open a second console window (e.g. by pressing &lt;Alt&gt;&lt;F2&gt;) or a second data connection using putty. Enter the following command into this window:   
   `mosquitto_pub -d -r -t Test1 -m 'Hallo vom Publisher!'`   
  `Client mosqpub/3194-raspberryp sending CONNECT`   
  `Client mosqpub/3194-raspberryp received CONNACK`   
  `Client mosqpub/3194-raspberryp sending PUBLISH (d0, q0, r0, m1, 'Test1', ... (20 bytes))`   
  `Client mosqpub/3194-raspberryp sending DISCONNECT`   

As soon as you send the message, it appears in the subscriber window :)   
The option `-d` means "Enable debug messages", i.e. all messages are output. This also causes the PINGREQ message to be displayed every minute. If you want to see only the messages, you have to omit -d ;)   
The `-r` switch causes the last message to be cached in the broker, so it can be "forwarded" to a client if it was just offline.   
   
Detailed documentation on Mosquitto can be found at [http://mosquitto.org/man/](http://mosquitto.org/man/).   

### Mosquitto installation on the PC/laptop.
The installation program for Windows (and other operating systems) can be downloaded from the following page:   
[`https://mosquitto.org/download/`](https://mosquitto.org/download/)


### RasPi: Displaying a message sent from the laptop/PC
1. In a console or putty window, start a subscriber on the RasPi that listens for all messages:   
``mosquitto_sub -t "#" -v```

2. Open a command window ("command prompt") on the PC:   
``cmd.exe``
in the start menu.   
Change to the correct drive and Mosquitto directory:   
```c:```   
```cd /programs/mosquitto``

3. Send message from PC prompt window   
```mosquitto_pub -h 192.168.1.2 -t Test1 -m "Hello from PC!"```   
With the switch `-h` the IP address of the Raspi is specified.   

In the Subscribe window (Putty or RasPi console) the corresponding message appears (possibly after a short time):<br>.
```
pi_@raspi:~ $ mosquitto_sub -t "#" -v
Test1 Hello from the PC!
```

## Helpful information about Mosquitto

* Stop broker   
  `sudo /etc/init.d/mosquitto stop`   
  `[ ok ] Stopping mosquitto (via systemctl): mosquitto.service.`   

* Start broker   
  `sudo /etc/init.d/mosquitto start`   
  `[ ok ] Starting mosquitto (via systemctl): mosquitto.service.`   

* Show all topics and their value ( payload )   
  `mosquitto_sub -t "#" -v`   
  `test1 D1mini message #1971`   
  `button 1`   
  `counter 21`   
  `-t "#"` means show all topics and their subtopics,   
  `-v` means show also the topic.   

* Check if Mosquitto is running.   
  `systemctl status mosquitto.service`   

Normally the MQTT server is set up after installation to start automatically on every reboot. If this is not the case, you can execute the start command for Mosquitto automatically at every system start by adding the line   
`sudo /etc/init.d/mosquitto start &`   
in the autostart file.
(The ampersand & is intentional and means that the execution of the autostart file should continue).

* Deleting a specific message stored on the broker (retain).   
  Sending a message with the corresponding topic and the switches `-n -r -d` (instead of the payload `-m`)   
  _Example_: deleting the message `Test1`.   
  `mosquitto_pub -h 10.1.1.1 -t Test1 -n -r -d`   

&nbsp;   
---   

## Helpful links for MQTT
* Delete __all__ messages stored on the broker (retain):   
[https://community.openhab.org/t/clearing-mqtt-retained-messages/58221](https://community.openhab.org/t/clearing-mqtt-retained-messages/58221)   
```
sudo service mosquitto stop   
sudo rm /var/lib/mosquitto/mosquitto.db   
sudo service mosquitto start   
```
_Note_: All messages are stored in the file `/var/lib/mosquitto/mosquitto.db`.

* Documentation for libmosquitto   
[https://mosquitto.org/api/files/mosquitto-h.html](https://mosquitto.org/api/files/mosquitto-h.html)


