Last modified: 2021-08-19   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Helpful single programs in C++</h1>
<a href="readme.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
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
On GitHub in the directory [mqtt4home/source_RasPi](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi) some C++ examples are available for the following tasks:   
* Sending the current broker time when a certain message has been received (`m4hBrokertime`).   
* logging all MQTT messages to files (`m4hLogM`)   
* logging of all MQTT messages to files and additional saving of the last message to a separate file (for faster processing) (`m4hLog2`)   
* Send and receive SMS with MQTT messages (`m4hSms`)   
* Template for creating own applications (`m4hXxx`)   
   

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

## Create your own programs
### Requirements
Creating your own program is to be demonstrated using the example `m4hDemo1`:   
* {a} In the configuration file a section `[demo1]` is to be defined and a "start value" (key `startvalue`) of `20` is to be given.
can.   
* {b} The program shall read the configuration data at startup   
(file `m4h.conf`, key `startvalue`).   
* {c} The program shall display all incoming messages.   
* {d} The program shall respond to the message `-t m4hDemo1/get -m keys` with the message `-t m4hDemo1/ret/keys -m [keys]`.   
   `[keys]` shall be all keys.   
* {e} The program shall count down to 0 every second starting from a start value (default 30) and then end the program.   
* {f} At the end of the program the message `Thank you for using m4hDemo1!` is to be output.

### Preparing the solution   
1. create a project directory `m4hDemo1`:   
`mkdir ~/m4hDemo1`.   

2. change to this directory:   
`cd ~/m4hDemo1`   

3. copy the template files (e.g. from `m4hXxx`) into this directory:   
`cp ~/m4hXxx/*.* ~/m4hDemo1`.   

4. rename the file C_Xxx.hpp to Demo1.hpp   
`mv C_Xxx.hpp C_Demo1.hpp`   

5. renaming the dummy name Xxx   
When renaming the dummy name `Xxx` to `Demo1` the files `C_Cxx.hpp`, `m4hExtension.hpp` and `m4h.conf` MUST be case sensitive, i.e. the renaming must be done in three steps:
   1. replace Xxx with demo1 (class name)
   2. replace xxx with demo1 (global variable)
   3. replace XXX by DEMO1 (definitions)

6. delete lines with `DEMO1_DEMO_KEY` and `_demo_`.   

---
### Solution to the requirements
7. requirement {a}: add to configuration file   
add the following to the `m4h.conf` file:   
   ```   
   [demo1]
   startvalue: 20
   ```   

8. requirement {b}: read configuration data   
Reading in the configuration data requires several steps:   
* C_Demo1.hpp: create constants f&uuml;r the startvalue:   
   ```   
   #define DEMO1_STARTVALUE_KEY "startvalue"   
   #define DEMO1_STARTVALUE 30
   ```   

* C_Demo1.hpp: declare property for start value   
  `int startvalue;`   
  and initialize it in the `setDefaults()` method:   
   `startvalue=DEMO1_STARTVALUE;`
   `keys=std::string(DEMO1_STARTVALUE_KEY);`

* C_Demo1.hpp: Define method to return the start value:   
  `int getStartvalue() { return startvalue; }`   

* C_Demo1.hpp: Read start value from configuration file (if defined).   In the method `readConfig(std::string pfConf)` after the line   
` if(v1.size()<1) return false; // no valid lines read`   
insert the following:   
```   
 //------for every line in section------------------------------
 bool bRet=false;
 for(int i=0; i<v1.size(); i++) {
  //-----get key and value--------------------------------------
  std::string sKey="", sVal="";
  std::string s1=v1.at(i);
  if(!conf.split2String(s1, sKey, sVal, ':')) continue;
  conf.delExtBlank(sKey);
  conf.delExtBlank(sVal);
  conf.strToLower(sKey);
  //-----search key---------------------------------------------
  if(sKey==DEMO1_STARTVALUE_KEY) {
   try{ // string to int
    int temp=std::stoi(sVal);
    startvalue=temp;
    bRet=true;
   } catch(std::invalid_argument) {};
  }
 }
 return bRet;
}
```   

* C_Demo1.hpp: Show start value.   
In the `show()` method add:   
```   
std::cout<<DEMO1_STARTVALUE_KEY<<" | "<<startvalue<<std::endl;
```   

* m4hExtension.hpp: Reading the configuration file   
In the file `m4hExtension.hpp` in the function `f2Init()` add the following lines:   
```
 g_demo1.readConfig(pfConf); // {a} read config data
 if(g_prt) g_demo1.show(); // show config values Demo1
```   
   
9. demand {c}: "show all messages"   
* C_Demo1.hpp: In method `onMessage()` add:   
   `std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;`   
* m4hExtension.hpp - Control: In the function f3OnMessage() must be the following line:   
  `g_demo1.onMessage(mosq, topic, payload);`   
   
10. requirement {d}: "Respond to a message".   
C_Demo1.hpp: In the `onMessage()` method add:   
```   
 if(topic=="m4hDemo1/get" && payload=="keys") {
  publish(mosq, "m4hDemo1/ret/keys", getKeys(), false);
 }
```   

11. demand {e}: Count down from initial value   
In the file `m4hExtension.hpp` in the function `f5Periodic` add:   
```   
 int iSec=g_demo1.getStartvalue();
 char cSec[16];
 //------count down iSec and exit program on 0------------------
 while(iSec>0) //-----"endless"---------------------------------
 { 
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  sprintf(cSec," %d ",iSec);
  fputs(cSec, stdout); fflush(stdout);
  iSec--;
 };
 printf(" 0 ");
 terminate_program(iEnd);
}
```   

12. demand {f}: output end message   
C_Demo1.hpp: In the method `onExit()` add:   
`std::cout<<"Thank you for using m4hDemo1!"<<std::endl;`   
