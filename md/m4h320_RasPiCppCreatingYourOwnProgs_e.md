Last modified: 2022-02-12 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>RasPi: How do I create my own C++ utility?</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h320_RasPiCppCreatingYourOwnProgs.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hDemo1">==> Code @ GitHub</a><hr>

# What is it about?
This tutorial shows how to create your own modules or utilities using the template `m4hXxx`.   
A module `C_Demo1.hpp` is created, which is used in the project `m4hDemo1` and shows how to receive and send messages and do periodic tasks in parallel. The [detailed task description](#a20) is below.   

_Procedure_:   
The program is created on the PC in Visual Studio code, then transferred to the RasPi and compiled there.   
(It would also be possible to work with Visual Studio Code directly on the RasPi).   

## This tutorial answers the following questions:   
1. [What things do I need to create an executable utility](#a10).   
2. [What should the demo program be able to do?](#a20)   
3. [How do I prepare the solution?](#a30)
4. [How to fulfill the requirements of the demo program?](#a40)
5. [How do I create and test the m4hDemo1 program?](#a50)
6. [How do I make the program available to all users?](#a60)

<a name="a10"></a>[_top of page_](#up)   

# What things do I need to create an executable utility?
* Hardware: PC or laptop with internet access, browser.   
* Hardware: Raspberry Pi as access point (WLAN Raspi11, PW 12345678) with IP 10.1.1.1, running an MQTT broker (e.g. Mosquitto).   
* Software: [Visual Studio Code ("VSC")](https://code.visualstudio.com/), which is prepared for C++ applications.   
* Software: terminal program [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop.   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) to transfer data from the PC/laptop to the RasPi.   
* Software: The MQTT client programs `mosquitto_sub` and `mosquitto_pub` (installed on PC or RasPi)   

<a name="a20"></a>[_top of page_](#up)   

# What should the demo program be able to do?
* {a} In the configuration file a section `[demo1]` should be defined and a "start value" (key `startvalue`) of `20` should be given.   

* {b} The program shall read the configuration data at startup   
(file `m4h.conf`, section `[demo1]`, key `startvalue`).   

* {c} The program shall display all incoming messages.   

* {d} The program shall respond to the message `-t m4hDemo1/get -m keys` with the message `-t m4hDemo1/ret/keys -m [keys]`.   
   `[keys]` shall be all keys.   

* {e} Starting from a start value (default 30), the program shall count down to 0 every second and then end the program.   

* {f} At the end of the program the message `Thank you for using m4hDemo1!` should be output.   

<a name="a30"></a>[_top of page_](#up)   

# How do I prepare the solution?
The first steps on the RasPi can be done directly - in a console window - or via `putty`. It is assumed that the template `m4hXxx` is present on the RasPi. If this is not the case, the template must first be created on the RasPi or copied up.   

## Preparation 1: Create template m4hXxx on the RasPi
1. using the browser, open the `m4hXxx` directory on GitHub:   
Link: [https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hXxx](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hXxx)   

2. open a console window on the RasPi (or connect to the RasPi on the PC with `putty`).   

3. create the directory `m4hXxx`:   
  `mkdir ~/m4hXxx`   

4. change to the directory `m4hXxx`:   
  `cd ~/m4hXxx`   

5. copy all source codes:   
    1. click on a source code file, e.g. `C_Xxx.hpp`, on GitHub, press [Raw], copy source code (e.g. &lt;ctrl&gt;a &lt;ctrl&gt;c).   
    2. create an empty file on the RasPi for the source code:   
    `nano ./C_Xxx.hpp`.   
    3. paste the source code into `nano` with the right key   
    4. save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
  Repeat these steps for the files `m4h.conf`, `m4hBase.cpp`, `m4hBase.h`, `m4hExtension.hpp` and `m4hMain.cpp`.   

Another way is to download the whole repository from GitHub and transfer the files to the RasPi.   

## Preparation 2: Create project m4hDemo1

1. RasPi: Create a project directory `m4hDemo1`:   
`mkdir ~/m4hDemo1`   

2. RasPi: Change to this directory:   
`cd ~/m4hDemo1`   

3. raspi: copy the template files `m4hXxx` into this directory:   
`cp ~/m4hXxx/*.* ~/m4hDemo1`.   

4. raspi: rename the file C_Xxx.hpp to Demo1.hpp   
`mv C_Xxx.hpp C_Demo1.hpp`   

5. PC: Download the directory `~/m4hDemo1` from RasPi to PC using WinSCP.   

6. PC: Start Visual Studio Code and open the project folder `m4hDemo1` __on the PC__:   
  Menu File - Open Folder...   

7. renaming the dummy designation Xxx   
When renaming the dummy name `Xxx` to `Demo1` the files `C_Cxx.hpp`, `m4hExtension.hpp` and `m4h.conf` MUST be case sensitive, i.e.   
renaming must be done in three steps:   
  Edit menu - Replace to files...   
  __Important__: Activate __`Aa`__ in the search field!   
    1. replace Xxx with demo1 (class name, approx. 33 replacements in 3 files)   
    2. replace xxx with demo1 (global variable, approx. 10 replacements in 3 files)   
    3. replace XXX with DEMO1 (definitions, about 8 replacements in 1 file)   

8. comment out (or delete) lines with `DEMO1_DEMO`, `DEMO1_DEMO_KEY` and `_demo_`.   
    * Edit menu - Search in files... `DEMO1_DEMO_KEY`   
      Comment out 4 occurrences, commenting out (or deleting) the whole if block in the `Demo1::readConfig` method:   
      ```  
        //-----search key---------------------------------------------
        //if(sKey==DEMO1_DEMO_KEY) {
        // _demo_=sVal;
        //}
      ```   
    * Edit menu - Search in files... `_demo_`   
      Comment out 2 occurrences.   

9. testing the changes   
    * Copy the files of the directory `m4hDemo` from the PC to the RasPi (with WinSCP).   
    * Compile the file by typing (in putty or in the console)   
      `g++ m4hMain.cpp m4hBase.cpp -o m4hDemo1 -lmosquitto -lpthread`   
    No errors should be displayed and the file `m4hDemo` should have been created. (Check by typing `ls -lh`)   
    * If the server or broker is running, the program can also be started:  
      `./m4hDemo1`.   
      Quit the program with &lt;ctrl&gt;c.   

---   

<a name="a40"></a>[_top of page_](#up)   

# How are the requirements for the demo program fulfilled?
## 1. Requirement {a}: Add configuration file   
In VSC click on the file `m4h.conf` and enter the following:   
   ```   
   [demo1]
   startvalue: 20
   ```   

## 2. Requirement {b}: Read in configuration data   
Reading in configuration data requires several steps in VSC:   

* C_Demo1.hpp: create constants for the start value:   
   ```   
   #define DEMO1_STARTVALUE_KEY "startvalue".   
   #define DEMO1_STARTVALUE 30
   ```   

* C_Demo1.hpp: Declare property for the start value.   
  Enter it in the class Demo1 under "application specific properties":   
  `int startvalue;`   
  and initialize in the method `setDefaults()`:   
   `startvalue=DEMO1_STARTVALUE;`   
   `keys=std::string(DEMO1_STARTVALUE_KEY);`   

* C_Demo1.hpp: Define method to return the start value.   
  Enter in class `Demo1` under "setter and getter methods":   
  `int getStartvalue() { return startvalue; }`   

* C_Demo1.hpp: Read start value from configuration file (if defined).   
  In the method `readConfig(std::string pfConf)` after the line   
`//-----search key---------------------------------------------`   
at ` // ..ToDo..` insert the following:   
```   
  //-----search key---------------------------------------------
  if(sKey==DEMO1_STARTVALUE_KEY) {
   try{ // string to int
    int temp=std::stoi(sVal);
    startvalue=temp;
   } catch(std::invalid_argument) {};
  }
 } // end for every line in section
 return true;
}
```   

* C_Demo1.hpp: Show start value.   
Add to the `show()` method:   
    ```   
    std::cout<<DEMO1_STARTVALUE_KEY<<" | "<<startvalue<<std::endl;
    ```   

* m4hExtension.hpp: Reading the configuration file   
In the file `m4hExtension.hpp` in the function `f2Init()` there should be the following lines (or else add them):   
    ```
     bRet&g_demo1.readConfig(pfConf); // {a} read config data
     if(g_prt) g_demo1.show(); // show config values Demo1
    ```   
   
## 3. Demand {c}: "Display all messages".   
* C_Demo1.hpp: In the `onMessage()` method add:   
   `std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;`   
* m4hExtension.hpp - Control: In the function `f3OnMessage()` must be the following line:   
  `g_demo1.onMessage(mosq, topic, payload);`   
   
## 4. Request {d}: "Respond to a message".   
C_Demo1.hpp: In the `onMessage()` method add:   
```   
 bool bRet=true;
 if(topic=="m4hDemo1/get" && payload=="keys") {
  std::string sTopic="m4hDemo1/ret/keys";
  std::string sPayload=getKeys();
  int iRet=mosquitto_publish(mosq, NULL,sTopic.c_str(),
   sPayload.length(), sPayload.c_str(), 0, false);
  if(iRet!=0) {
   if(g_prt) fprintf(stderr, "Could not send MQTT message %s. Error=%i\n",sTopic.c_str(),iRet);
   bRet=false;
 }
 return bRet;
}
```   

## 5. Demand {e}: Count down from start value   
In the file `C_Demo1.hpp` in the function `periodic` add:   
```   
 char cSec[16];
 static int iSec=g_demo1.getStartvalue();
 sprintf(cSec," %d ",iSec);
 fputs(cSec, stdout); fflush(stdout);
 iSec--; if(iSec<0) return false;
 return true;
```   

In the file `m4hExtension.hpp` add in the function `f5Periodic` in the while loop:   

```   
  if(zsec%10==0) bDoPeriodic=g_demo1.periodic(mosq);
```   

## 6. Demand {f}: output end message   
C_Demo1.hpp: Add to `onExit()` method:   
`std::cout<<"Thank you for using m4hDemo1!"<<std::endl;`   

<a name="a50"></a>[_top of page_](#up)   

# How do I create and test the m4hDemo1 program?
1. PC: Copy up the project files from the PC to the RasPi with WinSCP.   

2. create the executable file   
`g++ m4hMain.cpp m4hBase.cpp -o m4hDemo1 -lmosquitto -lpthread`   
_Note 1_: The file `m4hDemo1` was created in the directory `~/m4hDemo1`.   
_Note 2_: The command line used to create the executable is always in the file `m4hExtension.hpp` and `C_Xxx.hpp`.   
   

3. Execute the file:   
Input of   
`./m4hDemo1`   
The counter counts down from 20 to 0 and the program is terminated:   

    ```   
     20  
     ...
     19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
    Exit program... Thank you for using m4hDemo1!
    MQTT end message sent.

    Program terminated by application (12.02.2022 20:45:46)
    Beendet
    ```   

4. open a command window ("command prompt") on the PC:   
`cmd.exe`   
in the start menu.   
Change to the correct drive and Mosquitto directory:   
`c:`   
`cd /programs/mosquitto`.   

5. start the program again   
  `./m4hDemo1`   
  and during the counting process enter the following in the command window:   
  `mosquitto_pub -h 10.1.1.1 -t m4hDemo1/get -m keys`.   
  The following message appears in the putty window:   

    ```   
    19  18  17  16  15  | -t m4hDemo1/get -m keys |   
    | -t m4hDemo1/ret/keys -m startvalue |`
    14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
    ```   

---   

# How do I make the program available to all users?

If the file is to be usable by any user and from any directory, the following steps are required:   
1. copy the file into a generally accessible directory:   
`sudo cp ~/m4hDemo1/m4hDemo1 /usr/local/bin`   

2. share the file with all users:   
`sudo chown root /usr/local/bin/m4hDemo1`   
`sudo chmod 777 /usr/local/bin/m4hDemo1`   
`sudo chmod u+s /usr/local/bin/m4hDemo1`   

3. automatically start the file at RasPi startup   
* Open file `/usr/local/bin/autostart.sh`:   
`sudo nano /usr/local/bin/autostart.sh`   
* Insert between the printf statements:   
`/usr/local/bin/m4hDemo1 &`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   

_Note_: The "ampersand" & at the end of the line is intentional and causes the startup file not to wait for the program to exit.   
(see also section "Custom autostart file `autostart.sh`" in [m4h01_RasPiInstall.md](https://github.com/khartinger/mqtt4home/blob/main/m4h01_RasPiInstall.md) )   

[_top of page_](#up)   
