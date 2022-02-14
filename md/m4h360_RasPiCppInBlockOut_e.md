Last modified: 2022-02-14 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1><i>RasPi m4hInBlockOut</i>: Filtering or blocking of MQTT messages</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./m4h360_RasPiCppInBlockOut.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hInBlockOut">==> Code @ GitHub</a><hr>

# What is it about?
This program ensures that certain messages are not sent too often. If a registered message is received, it is checked whether within the last time this message was already received. Only if this is not the case, a given message is sent further.   
Thus the sending of the original message cannot be prevented, but the sending of the forwarded message.   

_Example of a configuration file entry:_   
Messages with the topic 'm4hInBlockOut/test3' are to be converted to messages with the topic 'm4hInBlockOut/test3/forwarded'. The original payload shall be appended with `(Block: <block> sec)`, where `<block>` stands for the blocking time. This should be one minute.   
Solution - entry in the configuration file:   

```   
[inblockout]
in    : m4hInBlockOut/test3
block : 00:01:00
out   : <in>/forwarded <text> (Block: <block> sec)
retain: false
```   

_Note_: The placeholders `<in>` for the incoming topic, `<text>` for the incoming payload and `<block>` for the blocking time (in seconds) are fixed by the program.   

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
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) for data transfer from PC/laptop to RasPi.   
* Software: The MQTT client programs `mosquitto_sub` and ` mosquitto_pub` (installed on the PC or RasPi) to test the program.   

<a name="a20"></a>[_top of page_](#up)   

# How do I use this program?
If the program is already compiled in the directory `~/m4hInBlockOut`, it only has to be started by typing the following into putty or a RasPi console:   
`~/m4hInBlockOut/m4hInBlockOut`   
Note: For creating the executable file see chapter [How to compile and test the program](#a40).

After reading the configuration file (default `./m4h.conf`) the connection to the broker is established:   

```   
Read config file ./m4h.conf: OK
=====[base]===========================
config file         | ./m4h.conf
all keys            | versionin|versionout|mqttstart|mqttend|progend|readconfin|readconfout|addtime
version (in)        | -t m4hInBlockOut/get -m version
version (out)       | -t m4hInBlockOut/ret/version -m 2022-02-14
mqtt @ start (out,*)| -t info/start -m m4hInBlockOut
mqtt @ end (out,*)  | -t info/end__ -m m4hInBlockOut
progend by mqtt (in)| -t m4hInBlockOut/set/end -m ..true..
reload conf-file(in)| -t m4hInBlockOut/set/conf -m ./m4h.conf
reload conf-fil(out)| -t m4hInBlockOut/ret/conf -m Reload Config file:
         * add time | true
=====[inblockout]==============================
config file         | ./m4h.conf
all keys            | in|block|action|out|retain
.....inblockout messages..............
3 messages:
 IN: -t m4hInBlockOut/test1 -m payload_must_fit | block 10s | OUT: -t <in>/ret -m New message to fixed payload :) | (no action)
 IN: -t m4hInBlockOut/test2 -m  | block 10s | OUT: -t <in> -m <text> (sent by inblockout!) | (no action)
 IN: -t m4hInBlockOut/test3 -m  | block 60s | OUT: -t m4hInBlockOut/ret/test3 -m <text> (Block: <block> sec) | (no action)
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

Messages with the topic `m4hInBlockOut/test1` and the payload `payload_must_fit` now only generate a message with the topic `m4hInBlockOut/test1/ret` and the payload `New message to fixed payload :)` every 10 seconds.   
The same applies to the messages `m4hInBlockOut/test2` and `m4hInBlockOut/test3`, where the content of the incoming payload is arbitrary.   

End of program with &lt;ctrl&gt;c   

<a name="a30"></a>[_top of page_](#up)   

# What configuration options are available?   
For each message to be controlled, a separate `[inblockout]` section must be created in the configuration file (e.g. `m4h.conf`).   

The following keys are possible:   
`in|block|action|out|retain`   
For each `[inblockout]` entry at least the keys `in:` and `out:` must be specified!   

## in
Topic of the message to be monitored.   
If a payload is specified, it must also match.   

_Example:_   
`in : m4hInBlockOut/test1 payload_must_fit`   
or   
`in : m4hInBlockOut/test2`   

## block
Specification of the blocking time in the format `HH:MM:SS` (with `HH`=hours, `MM`=minutes and `SS`=seconds).   
The following limits are preset in the program:   
Minimum blocking time: 1 second   
Maximum blocking time: 30 days   
Blocking time if no value is specified in the configuration file: 6 hours   

```   
#define IBO_BLOCK_SEC_DEFAULT 21600         // 21600s = 6h
#define IBO_BLOCK_SEC_MIN     1             // 1s = 1s
#define IBO_BLOCK_SEC_MAX     2592000       // 2592000 = 30d
```   

_Example for a time of 10 seconds:_  
`block : 00:00:10`   

## action
Currently there are no actions defined.   

## out
Topic and payload for sending a message if the incoming message was not blocked.   

When composing the topic, the placeholder `<in>` can be used, which stands for the incoming topic.   

When composing the payload, the placeholders `<in>`, `<text>` and `<block>` can be used, where `<in>` stands for the incoming topic, `<text>` for the incoming payload and `<block>` for the blocking time.   

Topic and payload must be separated by a space.   
_example:_   
`out : <in>/ret New_payload`   

## retain
Specify whether the retain flag should be set for the outgoing message. Default is no.   
Possible values for the retain value are `true` and `false`.   

Example   
`retain: true`   

<a name="a40"></a>[_top of page_](#up)   

# How do I compile and test the program?
If all files of the project are already in the directory `~/m4hInBlockOut`, you only have to create the executable file by typing the following into putty or a RasPi console:   
`g++ m4hMain.cpp m4hBase.cpp -o m4hInBlockOut -lmosquitto -lpthread -std=c++17`.   

If this is not the case, the compilation of the C++ utility is done as in the manual ["RasPi: Which C++ utilities are there and how do I get them to work?"](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30).   
Link address: [https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30](https://github.com/khartinger/mqtt4home/blob/main/md/m4h310_RasPiCppDemos.md/#a30)   

## How do I test this program?
### First test
1. at the PC open a terminal window (`cmd.exe`) or putty window or at the RasPi open a console and start the program to display MQTT messages:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   

2. start the program e.g. from a Putty window   
  `~/m4hInBlockOut/m4hInBlockOut`   

3. open a second terminal window (`cmd.exe`) or putty window on the PC or a second console on the RasPi and send a MQTT message:   
  `mosquitto_pub -h 10.1.1.1 -t m4hInBlockOut/test1 -m payload_must_fit`   
  Two messages appear in the first terminal window:   
  `m4hInBlockOut/test1 payload_must_fit`   
  `m4hInBlockOut/test1/ret New message to fixed payload :)`   
  The Putty window displays the following message:   
  `IN: -t m4hInBlockOut/test1 -m payload_must_fit ==> NOT blocked => OUT: -t m4hInBlockOut/test1/ret -m New message to fixed payload :)`   

4. if you send the message again (or more than once) within 10 seconds, only the first   
  only the first message appears in the first terminal window:   
  `m4hInBlockOut/test1 payload_must_fit`.   
  In the Putty window the following message appears:   
  `Incoming message "m4hInBlockOut/test1" blocked!`   

### Other tests
Sending the message   
`mosquitto_pub -h 10.1.1.1 -t m4hInBlockOut/test2 -m ***Test2***`   
produces two outputs in the putty window:   
* `IN: -t m4hInBlockOut/test2 -m ***Test2*** ==> NOT blocked => OUT: -t m4hInBlockOut/test2 -m ***Test2*** (sent by inblockout!)`   
* `Incoming message "m4hInBlockOut/test2" blocked!`   

and in the first terminal window:   
`m4hInBlockOut/test2 ***Test2***`   
`m4hInBlockOut/test2 ***Test2*** (sent by inblockout!)`   
The reason for the blocking message is that the outgoing topic is identical to the incoming topic and the message sent by itself is also received again ;)   

---   

Sending the message three times   
`mosquitto_pub -h 10.1.1.1 -t m4hInBlockOut/test3 -m ***Test3***`   
generates the output in the putty window:   
`IN: -t m4hInBlockOut/test3 -m **test3*** ==> NOT blocked => OUT: -t m4hInBlockOut/ret/test3 -m **test3*** (Block: 60 sec)`   
`Incoming message "m4hInBlockOut/test3" blocked!`   
`Incoming message "m4hInBlockOut/test3" blocked!`   

and in the first terminal window:   
`m4hInBlockOut/test3 **test3***`   
`m4hInBlockOut/ret/test3 **test3*** (Block: 60 sec)`   
`m4hInBlockOut/test3 **test3***`   
`m4hInBlockOut/test3 **test3***`   

<a name="a90"></a>[_top of page_](#up)   

# How is the program coded?   
An overview of the files is given in the following image, where `Xxx` is to be replaced by `InBlockOut` ;)   
![m4hBase files](./images/rpi_m4hBase_files2.png "m4hBase files")   
_Fig. 1: Files for creating a C++ helper_.   

For the coding of the file `C_InBlockOut.hpp` see [https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hInBlockOut/C_InBlockOut.hpp](https://github.com/khartinger/mqtt4home/blob/main/source_RasPi/m4hInBlockOut/C_InBlockOut.hpp)   

[_top of page_](#up)   
