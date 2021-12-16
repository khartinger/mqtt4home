Last modified: 2021-08-22   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Create your own programs in C++</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h09_RasPiCppCreatingYourOwnProgs.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
* Create own programs using the template `m4hBase`.   
   

## Required tools
* Hardware: RasPi
* Software: Internet access to GitHub.
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop
* Software: Possibly   
   [Visual Studio Code](https://code.visualstudio.com/) and   
   [WinSCP](https://winscp.net/eng/docs/lang:de) for data transfer from the PC/laptop to the RasPi   

## Create your own programs
Creating your own program is to be demonstrated using the example `m4hDemo1`. The easiest way to do this is to prepare the program on the PC in Visual Studio Code, then transfer it to the RasPi and compile it there. (It would also be possible to work with Visual Studio Code directly on the RasPi).    

### Requirements
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
1. RasPi: create a project directory `m4hDemo1`:   
`mkdir ~/m4hDemo1`.   

2. RasPi: change to this directory:   
`cd ~/m4hDemo1`   

3. RasPi: copy the template files (e.g. from `m4hXxx`) into this directory:   
`cp ~/m4hXxx/*.* ~/m4hDemo1`.   

4. RasPi: rename the file C_Xxx.hpp to Demo1.hpp   
`mv C_Xxx.hpp C_Demo1.hpp`   

5. PC: Download the directory `~/m4hDemo1` from RasPi to PC using WinSCP  and   
   open the project folder `m4hDemo1` __on the PC__ with Visual Studio Code (menu File - Open Folder...).   

6. renaming the dummy name Xxx   
Renaming the dummy name `Xxx` to `Demo1` (in the files `C_Cxx.hpp`, `m4hExtension.hpp` and `m4h.conf`) MUST be done case sensitive, i.e. the renaming must be done in three steps:
   1. replace Xxx with demo1 (class name)   
   2. replace xxx with demo1 (global variable)   
   3. replace XXX by DEMO1 (definitions)   
      In Visual Studio Code the replacement is done in the menu   
   `Edit - Replace in files`   
   __Important__: Activate __`Aa`__ in the search field!

7. delete lines with `DEMO1_DEMO_KEY` and `_demo_`.   

---
### Solution to the requirements
8. requirement {a}: add to configuration file   
add the following to the `m4h.conf` file:   
   ```   
   [demo1]
   startvalue: 20
   ```   

9. requirement {b}: read configuration data   
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
   
10. demand {c}: "show all messages"   
* C_Demo1.hpp: In method `onMessage()` add:   
   `std::cout<<" | -t "<<topic<<" -m "<<payload<<" | "<<std::endl;`   
* m4hExtension.hpp - Control: In the function f3OnMessage() must be the following line:   
  `g_demo1.onMessage(mosq, topic, payload);`   
   
11. requirement {d}: "Respond to a message".   
C_Demo1.hpp: In the `onMessage()` method add:   
```   
 if(topic=="m4hDemo1/get" && payload=="keys") {
  publish(mosq, "m4hDemo1/ret/keys", getKeys(), false);
 }
```   

12. demand {e}: Count down from initial value   
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

13. demand {f}: output end message   
C_Demo1.hpp: In the method `onExit()` add:   
`std::cout<<"Thank you for using m4hDemo1!"<<std::endl;`   
   
14. PC: Copy up the project files from the PC to the RasPi using WinSCP.   
---   
&nbsp;
### Creating the utility program

15. creating the executable file   
```g++ m4hMain.cpp m4hBase.cpp -o m4hDemo1 -lmosquitto -lpthread```   
_Note 1_: In the directory `~/m4hDemo1` the file `m4hDemo1` was created.   
_Note 2_: The command line that creates the executable file is always in the `m4hExtension.hpp` and `C_Xxx.hpp` file.   
   

16. testing or executing the file:   
```./m4hDemo1```. 

17. make the file available for all users   
If you want to be able to use the file from any user and from any directory, the following steps are required:   
```sudo cp ~/m4hDemo1/m4hDemo1 /usr/local/bin```   
```sudo chown root /usr/local/bin/m4hDemo1```   
```sudo chmod 777 /usr/local/bin/m4hDemo1```   

18. automatically start the file at RasPi startup   
* Open file /usr/local/bin/autostart.sh &ouml;ff:   
`sudo nano /usr/local/bin/autostart.sh`   
* Insert between the printf statements:   
`/usr/local/bin/m4hDemo1 &`   
Save and exit by &lt;Ctrl&gt;o &lt;Enter&gt; &lt;Ctrl&gt; x   
_Note_: The "ampersand" & at the end of the line is intentional!   
(see also section "Custom autostart file `autostart.sh`" in [m4h01_RasPiInstall.md](https://github.com/khartinger/mqtt4home/blob/main/m4h01_RasPiInstall.md) )
