Last modified: 2022-02-13 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1><i>RasPi m4hLog2</i>: Writing MQTT messages to files (log files)</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

# What is it about?
The program `m4hLog2` writes MQTT messages to files. Two types of files are created in separate directories:   
1. log file: These files are created separately for each topic and are recreated monthly.   
  They document when, how often and with which payload messages were sent.   
2. load file: Here a file is created for each topic, which contains only the last received topic. The modification date of the file can be used to determine when the topic was last sent. Furthermore, an easy access for other programs (e.g. PHP web pages) is possible.   

The file name for log files consists of the topic + period + year + month + ".log", where spaces in topics are replaced by underscores and slashes by the at sign. Load filenames consist of the topic only.   

_Example_:   
The message `-t z2m/ret/health -m Zigbee2mqtt-health is perfect.` sent in February 2022 is stored as follows:   

Log file:   
* File name: `z2m@ret@health.2202.log`   
* File content: `11.02.22 09:16:16 | z2m@ret@health | Zigbee2mqtt-health is perfect.`   
* Default directory: `./log/`   

Load file:   
* File name: `z2m@ret@health`   
* File content: ` Zigbee2mqtt-health is perfect. `   
* Default directory: `./data/`   

## These instructions answer the following questions:   
1. [What tools do I need for this project?](#a10)   
2. [How do I use this program?](#a20)   
3. [What are the configuration options?](#a30)   
4. [How do I compile and test the program?](#a40)   
5 [How is the program coded?](#a90)   

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
## Standard function
If the program is already compiled in the directory `~/m4hLog2`, you only have to start it by typing the following in putty or a RasPi console:   
`~/m4hLog2/m4hLog2`   
_Note_: For creating the executable see chapter [How to compile and test the program](#a40).

After reading the configuration file (default `./m4h.conf`) the connection to the broker is established:   

```   
Read config file ./m4h.conf: OK
=====[base]===========================
config file         | ./m4h.conf
all keys            | versionin|versionout|mqttstart|mqttend|progend|readconfin|readconfout|addtime
version (in)        | -t m4hLog2/get -m version
version (out)       | -t m4hLog2/ret/version -m 2022-02-13
mqtt @ start (out,*)| -t info/start -m m4hLog2
mqtt @ end (out,*)  | -t info/end__ -m m4hLog2
progend by mqtt (in)| -t m4hLog2/set -m ...end...
reload conf-file(in)| -t m4hbase/set/conf -m ./m4h.conf
reload conf-fil(out)| -t m4hbase/ret/conf -m Read config:
         * add time | true
=====[log2]==============================
config file         | ./m4h.conf
all keys            | pathlog|pathlast|skiplog|timein|timeout
pathlog             | ./log/
pathlast            | ./last/
timein              | m4hLog2/get/time
timeout             | m4hLog2/ret/time
file time format    | %Y%m%d %H%M%S
.....log: topics to skip..............
z2m/bridge/info,z2m/bridge/devices,z2m/bridge/logging
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

From now on the program writes received messages into the files described above.   
End of program with &lt;ctrl&gt;c   

## Query the last date+time value of a message
1. at the PC a terminal window (`cmd.exe`) or putty window or at the RasPi a console &ouml;ffnen change into the Mosquitto directory and start the program to display MQTT messages:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   
2. at the PC change a second terminal window (`cmd.exe`) or putty window or at the RasPi a second console &ouml;ffnen into the Mosquitto directory.   
For testing you can first send a MQTT message, e.g. query the version of the program:   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get -m version`.   
3. in the second console window send the message to query the date and time:   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get/time -m m4hLog2/get`.   
5. in the first console window you can see the following messages:   
  `m4hLog2/get/time m4hLog2/get`.   
  `m4hLog2/ret/time 20220213 120500 | m4hLog2/get`   
  This means: The payload contains the date (`20220213`), the time (`120500`) and the topic (`m4hLog2/get`) for which the values were requested.   

<a name="a30"></a>[_top of page_](#up)   

# What are the configuration options?   

The following keywords are possible:   
`pathlog|pathlast|skiplog|timein|timeout`   

## pathlog
Specification of the directory (path) for the log files (history files).   

_example:_   
`pathlog: ./log/`   

## pathlast
Specify the directory (path) for the load files.   

_example:_   
`pathlast: ./last/`   

## skiplog
Specify all topics for which log files (history) should not be created. Topics are separated by commas.   
The load files will be created for these topics anyway, otherwise it would not be possible to query the last date.   

_example:_   
`skiplog: z2m/bridge/info,z2m/bridge/devices,z2m/bridge/logging`   

## timein
Topic to query the last date+time value of a message. The topic of the time to be queried must be specified as a payload in the query message.   

_Example:_   
`timeIn: m4hLog2/get/time`   
Total query message e.g.   
`mosquitto_pub -h 10.1.1.1 -t m4hLog2/get/time -m info/start`   

## timeout
Topic for sending the response to the last date+time query of a message. As additional value a time format can be specified.   
If no time format is specified, the default format from `C_Log2.hpp` is used:   
`#define LOG2_TIMEFORMAT "%Y%m%d %H%M%S"`   

_examples:_   
`timeOut: m4hLog2/ret/time`   
or   
`timeOut: m4hLog2/ret/time %d.%m.%Y %H:%M:%S`   

<a name="a40"></a>[_top of page_](#up)   

# How do I compile and test the program?
If all files of the project are already in the directory `~/m4hLog2`, you only have to create the executable file by typing the following in putty or a RasPi console:   
`g++ m4hMain.cpp m4hBase.cpp -o m4hLog2 -lmosquitto -lpthread -std=c++17`.   

If this is not the case, compiling the C++ utility is done as described in the manual ["RasPi: What C++ utilities are there and how do I get them to work?"](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30).   
Link address: [https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30)   

To test the program you can use point ["How do I use this program?"](#a20) of this manual.   

<a name="a90"></a>[_top of page_](#up)   

# How is the program coded?   
An overview of the files is given in the following image, where `Xxx` is to be replaced by `Log2` ;)   
![m4hBase files](./images/rpi_m4hBase_files2.png "m4hBase files")   
_Fig. 1: Files for the creation of a C++ helper_.   

For the coding of the file `C_Log2.hpp` see [https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLog2/C_Log2.hpp](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hLog2/C_Log2.hpp)   

[_top of page_](#up)   
