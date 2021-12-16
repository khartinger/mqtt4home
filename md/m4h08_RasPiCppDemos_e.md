Last modified: 2021-09-06   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Helpful single programs in C++</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h08_RasPiCppDemos.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Be able to compile, run and test existing stand-alone programs in C++.   
* Create own programs using the template `m4hBase`.   
   

## Required tools
* Hardware: RasPi
* Software: Internet access to GitHub.
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop
* Software: Possibly [WinSCP](https://winscp.net/eng/docs/lang:de) for data transfer from the PC/laptop to the RasPi   

## Overview
There are some C++ examples available on GitHub in the directory [mqtt4home/source_RasPi](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi) for the following tasks:   
* [`m4hBrokertime`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBrokertime): Sending the current broker time when a given message has been received.   
* `m4hLogM`: logging all MQTT messages to files   
* `m4hLog2`: logging of all MQTT messages in files and additional saving of the last payload in a separate file (for faster processing)    
* [`m4hInDelayOut`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hInDelayOut): After receiving a message it waits for a specified time and then sends a reply message.   
* `m4hSms`: Send and receive SMS with MQTT messages   
* [`m4hFindSimModule`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hFindSimModule): Searching the interfaces of a RasPi for a SIM module.   
* [`m4hXxx`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hXxx): Template to create own applications   
   
## Example: Creating a utility program
The procedure to create a program will be shown with the example `m4hBrokertime`. The work can be done directly on the RasPi or via `putty`.   

1. creating the project directory on the RasPi   
``mkdir ~/m4hBrokertime``   

2. change to this directory   
```cd ~/m4hBrokertime``

3. copy the project files from GitHub   
(Files: `C_Brokertime.hpp`, `m4h.conf`, `mh4Base.cpp`, `m4hBase.h`, `m4hExtension.hpp`, `m4hMain.cpp`)

4. create the executable file   
```g++ m4hMain.cpp m4hBase.cpp -o m4hBrokertime -lmosquitto -lpthread``   
_Note 1_: In the directory `~/m4hBrokertime` the file `m4hBrokertime` was created.   
_Note 2_: The command line that creates the executable file is always in the file `m4hExtension.hpp` and `C_Xxx.hpp`.   
   
5. testing or executing the file:   
``./m4hBrokertime`` 

6. make the file available for all users   
If you want to be able to use the file from any user and from any directory, the following steps are required:   
```sudo cp ~/m4hBrokertime/m4hBrokertime /usr/local/bin``   
```sudo chown root /usr/local/bin/m4hBrokertime``   
```sudo chmod 777 /usr/local/bin/m4hBrokertime```   

7. automatically start the file at RasPi startup   
* Open file /usr/local/bin/autostart.sh &ouml;ff:   
`sudo nano /usr/local/bin/autostart.sh`   
* Insert between the printf statements:   
`/usr/local/bin/m4hBrokertime &`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
(see also section "Custom autostart file `autostart.sh`" in [m4h01_RasPiInstall.md](https://github.com/khartinger/mqtt4home/blob/main/m4h01_RasPiInstall.md) )
