Last modified: 2022-02-14 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1><i>RasPi m4hInBlockOut</i>: Filtering or blocking of MQTT messages</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# What is it about?

## These instructions answer the following questions:   
1. [What tools do I need for this project?](#a10)   
2. [How do I use this program?](#a20)   
3. [What are the configuration options?](#a30)   
4. [How do I compile and test the program?](#a40)   
5. [How is the program coded?](#a90)   

<a name="a10"></a>[_top of page_](#up)   

# What tools do I need for this project?
Theoretically all steps could be done on the RasPi, practically using a PC/laptop makes sense. Therefore the following is needed:   

* Hardware: Raspberry Pi as access point (WLAN Raspi11, PW 12345678) with IP 10.1.1.1, running an MQTT broker (e.g. Mosquitto).   
---   
* Hardware: PC or laptop with internet access, browser   
* Software: Terminal program [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop   
---   
* Software: Visual Studio Code ("VSC") prepared for C++ applications.   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) for data&uuml;transfer from PC/laptop to RasPi.   
* Software: The MQTT client programs `mosquitto_sub` and ` mosquitto_pub` (installed on the PC or RasPi) to test the program.   

<a name="a20"></a>[_top of page_](#up)   

# How do I use this program?



[_top of page_](#up)   
