Last modified: 2021-08-19   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Template for MQTT programs in C++</h1>
<a href="readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h07_RasPiCppBase.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* With the help of the template `m4hBase` a simple C++ program shall be created and tested, which displays all incoming MQTT messages.   
* The purpose of the individual parts of the C++ template `m4hBase` is to be explained.   

## Required tools
* Hardware: RasPi
* Software: Internet access to GitHub.
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop
* Software: Possibly [WinSCP](https://winscp.net/eng/docs/lang:de) for data transfer from the PC/laptop to the RasPi   
## Overview
In order to make systems more transparent, they should have a modular structure. Modular means that a certain function is realized by exactly one module and modules can be added or omitted if necessary.   
In this tutorial, the template `m4hBase` is used to create C++ programs that perform exactly one task and can be used like operating system calls.   

## What can the template `m4hBase` do?
1. read settings from m4h.conf configuration file.   
2. possibility to specify another configuration file   
   (when starting the program on the command line).   
3. answering a query about the program version.   
   Default for the request: Topic "m4hBase/get", Payload "version".   
   Default for the answer: Topic "m4hBase/ret/version", Payload "2021-08-15".   
4. sending (or not sending) an MQTT message at program start and/or program end.   
5. use of the global objects g_base, g_prt, g_mosq
6. possibility to terminate the program by an MQTT message defined in the configuration file (key "progend" in the configuration file).
7. exit the program with &lt;ctrl&gt;c.

   
## Structure of the C++ template `m4hBase`.
 The template consists of three fixed files (always stay the same) and (at least) two files that need to be edited:   
### Fixed files
* __`m4hMain.cpp`__ (main program)   
This file contains the main program with the calls of all functions that have to be defined in the extension file. Other tasks are starting MQTT, the &lt;ctrl&gt;c handling, ending the program etc.
* __`m4hBase.h`__ and __`m4hBase.cpp`__ (basic classes)   
   In these two files the following classes are included:   
   * `Message` and `Message2` for MQTT message storage,   
   * `Conf` for loading the configuration file and editing the entries,   
   * `M4hBase` to provide basic functionalities   
     (version, MQTT message at program start and/or end, ...).   
   Furthermore, the files contain a global object `g_base`, which can be used to access the methods of the `M4hBase` class.

### Files that must be edited
* __`m4hExtension.hpp`__.   
The file contains the five functions that are used to implement the user function.   
* __`m4h.conf`__ (configuration file)   
The configuration file can be used to control the behavior of the programs. The structure of the file consists of sections (in square brackets[]) and key:value pairs.   
   
For many programs it is also useful to implement additional functionality in a "working class".

## Editing the extension file `m4hExtension.hpp`
In the file `m4hExtension.hpp` the following five functions must be defined:   
* `void f1PrintHelptext() { }`   
* `void f2Init(std::string pfConf) { }`   
* `void f3OnMessage(struct mosquitto *mosq, std::string topic, std::string payload) { }`   
* `void f4OnExit(struct mosquitto *mosq, int reason) { }`   
* `void f5Periodic(struct mosquitto *mosq) { }`   

For the basic "display incoming MQTT messages" application, the file may look like this:   
```   
//_____m4hExtension.hpp__________________________khartinger_____
// ...
#include "mosquitto.h"            // mosquitto_* functions
#include "m4hBase.h"              // m4h basic functions

//-------global values------------------------------------------
extern bool g_prt;                     //true=printf,false=quiet
void terminate_program(int reason);    // myMqttPeriodicExec

//_______main: print this help text_____________________________
void f1PrintHelptext()
{
 fprintf(stdout, "\nUsage  : m4hBase [-h | -q | pf.conf]\n");
 fprintf(stdout, "         -h ........ print this help text\n");
 fprintf(stdout, "         -q ........ no output to stdout, stderr\n");
 fprintf(stdout, "         pf.conf ... path+filename of config.file (default m4hLocal.conf)\n");
 fprintf(stdout, "Purpose: (1) Demo for m4hBase\n");
 fprintf(stdout, "         (2) Program shows all incomming messages.\n");
 fprintf(stdout, "Author : Karl Hartinger\n");
 fprintf(stdout, "Version: 2021-08-15");
 fprintf(stdout, "Needs  : sudo apt-get install libmosquitto-dev\n\n");
 fprintf(stdout, "Exit program by pressing <ctrl>c\n");
}

//_______init extension_________________________________________
// pfConf...path and filename of config file
void f2Init(std::string pfConf)
{
}

//_______react to further mqtt messages_________________________
void f3OnMessage(struct mosquitto *mosq, 
 std::string topic, std::string payload)
{
 //------print all received messages----------------------------
 std::cout<<topic<<" | "<<payload<<std::endl;
}

//_______Possibility for cleanup before end of program__________
void f4OnExit(struct mosquitto *mosq, int reason)
{
}

//_______for periodic actions (a parallel thread)_______________
void f5Periodic(struct mosquitto *mosq)
{
 bool bDoPeriodic=true;                // do "endless"
 int  iEnd=4;                          // reason for end
 while(bDoPeriodic) //-----"endless"----------------------------
 { //...Do something...
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 };
 terminate_program(iEnd);
}
```   
_Listing 1: Example for m4hExtension.hpp_   
   

---   
## Creation of the command line program `m4hBase`
### Preparation
1. create a directory for the program code on the RasPi:   
```mkdir ~/m4hBase```

2. change to this directory:   
```cd ~/m4hBase```
### Copy project files
3. download the source code of the project files.   
The source code of the files is located at [https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBase](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hBase).   
4. copy the files to the project directory `m4hBase`.   
(From PC to RasPi e.g. with `WinSCP`)   

### Create all files with `nano`.
Alternatively you can create the files with the text editor `nano` and copy the source code file by file into it:   
* Start putty and connect to the RasPi.
* Change to the project directory   
  ```cd ~/m4hBase```
* Click a source file, e.g. `m4h.conf`, on GitHub, press [Raw], copy source code
* On the RasPi type:
```nano ./m4h.conf```   
* Paste the source code into `nano` with the right button
* Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

Repeat the procedure from the 3rd point for the files `m4hBase.cpp`, `m4hBase.h`, `m4hExtension.hpp` and `m4hMain.cpp`.   

### Compile source code
5. The compilation is done with the following command:   
```g++ m4hMain.cpp m4hBase.cpp -o m4hBase -lmosquitto -lpthread```   
In the directory `~/m4hBase` the file `m4hBase` was created.   

_Note_: For the compilation to succeed, the Mosquitto library must be installed:   
```sudo apt-get install libmosquitto-dev```   
 (see also [https://github.com/khartinger/mqtt4home/blob/main/m4h03_RasPiMQTTBroker.md](https://github.com/khartinger/mqtt4home/blob/main/m4h03_RasPiMQTTBroker.md) )   
   
&nbsp;   

## Test of the command line program `m4hBase`.
To test the program you need a putty window and a PC prompt window:
1. run the newly created program in the console or a putty window:   
```./m4hBase```  
Result:   
```   
Read config file ./m4h.conf: OK
-----[base]---------------------------
config file         | ./m4h.conf
section name        | base
version (in)        | -t m4hBase/get -m version
version (out)       | -t m4hBase/ret/version -m 2021-08-15
mqtt @ start (out,*)| -t info/start -m m4hBase -r
mqtt @ end (out,*)  | -t info/end -m m4hBase -r
progend by mqtt (in)| -t m4hBase/set -m ...end...
         * add time | true
Try to connect to mosquitto...
Connected: Waiting for topics...
info/start | m4hBase (15.08.2021 19:29:10)
```   
2. open a command window ("command prompt") on the PC:   
``cmd.exe``
in the start menu.   
Change to the correct drive and Mosquitto directory:   
```c:```   
```cd /programs/mosquitto```

3. send message from PC prompt window   
```mosquitto_pub -h 10.1.1.1 -t Test1 -m "Hello from PC!"```   
Use the switch `-h` to specify the IP address of the Raspi.   

4. in the console or the putty window appears (maybe after a short time) the corresponding message:<br>
``Test1 | Hello from PC``   

If one terminates the program with &lt;ctrl&gt;c, one receives the following messages:   
```
^C
Exit program... MQTT end message sent.
info/end | m4hBase (15.08.2021 19:32:19)

Program terminated by <ctrl>c (15.08.2021 19:32:19)
Beendet
```
