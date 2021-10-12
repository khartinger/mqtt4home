Last modified: 2021-10-12   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Sending MQTT messages regularly</h1>
<a href="../readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h104_RasPi_crontab.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Target
* Sending MQTT messages at specific times.

## Required tools
* Hardware: RasPi
* Hardware: PC or laptop with internet access, browser
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop

## Realization
For periodic execution of actions the operating system Linux provides the service `crontab`, which can start scripts or programs automatically.   
It therefore makes sense to use this service to send MQTT messages.   
A lot of information about `crontab` can be found on the Internet, e.g.   
* [https://wiki.ubuntuusers.de/Cron/ (2021-10-12)](https://wiki.ubuntuusers.de/Cron/)   
* [https://www.stetic.com/developer/cronjob-linux-tutorial-und-crontab-syntax/ (2021-10-12)](https://www.stetic.com/developer/cronjob-linux-tutorial-und-crontab-syntax/)   
* [https://crontab.guru/ (2021-10-12)](https://crontab.guru/)   

### "Blind" Example
* The opening of a blind shall be started every day at 07:10 by the MQTT command `rollo/motor` with the payload `1`.   
* At 19:30 the blind shall be lowered (same topic, payload `-1`).   
* One minute after the start of an action the motor shall be stopped again (payload `0`).   

Assumption: MQTT commands are sent with the program `mosquitto_pub`.   
### Solution
* Start crontab (parameter e = edit):   
   `crontab -e`
* Enter the following commands into the table:   
```   
10 7 * * mosquitto_pub -t rollo/motor -m 1
11 7 * * mosquitto_pub -t rollo/motor -m 0
30 19 * * mosquitto_pub -t rollo/motor -m -1
31 19 * * mosquitto_pub -t rollo/motor -m 0
```   
* Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x