Last modified: 2022-02-03 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>Raspberry Pi: Time monitoring of MQTT sensors (Watchdog) in C++</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h350_RasPiCppWatchdog.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<a href="https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hWdog">==> Code @ GitHub</a><hr>

# What is it about?
Many sensors send measured values and then go into a sleep mode to save energy. Battery-powered sensors eventually run out of energy and can no longer send anything, or they lose the network connection.   
The `m4hWdog` ("Watchdog") project helps to detect this failure mode. It sends a MQTT message when a sensor has stopped sending a message within a given time.   
In a configuration file it is defined within which time span a message from a certain topic has to arrive.   

## This guide answers the following questions:   
1. [What things do I need for this project?](#a10)   
2. [How do I use this program?](#a20)   
3. [How do I compile and test the program m4hWdog?](#a30)   
4. [How is the program m4hWdog coded?](#a90)   

<a name="a10"></a>[_Top of page_](#up)   
# What things do I need for this project?
* Hardware: PC or laptop with internet access, browser.   
* Hardware: Raspberry Pi as access point (WLAN Raspi11, PW 12345678) with the IP 10.1.1.1, running a MQTT broker (e.g. Mosquitto)   
* Software: Visual Studio Code ("VSC"), which is prepared for C++ applications.   
* Software: Terminal program [__*putty*__](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop.   
* Software: [__*WinSCP*__](https://winscp.net/eng/docs/lang:de) for data transfer from PC/laptop to RasPi.   
* Software: The MQTT client programs `mosquitto_sub` and `mosquitto_pub` (installed on PC or RasPi)   

<a name="a20"></a>[_Top of page_](#up)   
# How do I use this program?   
All sensors to be monitored must be entered in the configuration file (e.g. `m4h.conf`) in the section `[wdog]`.   
For each sensor a line must be created with the key "`in:`" and the value Topic + space + maximum response time in the format `[HHHH]H:MM:SS`, where the number of hours can be any value between 0 and 63660 (= 10 * 365,25 * 24 = 10 years).   

* _Example_:   
  The sensor `m4h/button_2` must be pressed at least every 10 seconds.   
  The following entry is required in the configuration file:   
  `[wdog]`   
  `in: m4h/button_2 00:00:10`   

The key `out` can be used to specify under which topic a warning should be sent.   
* _Example_:   
  If the sensor "`<in>`" exceeds the watchdog time, the message "Sensor &lt;in&gt; missing!" should be sent under the topic "m4hWdog/attention".   
  _Solution_:   
  `out: m4hWdog/attention sensor <in> missing!`   
  The `<in>` placeholder is replaced by the corresponding sensor name.   
  The key `out` applies to all `in` lines.   

To use the program, just create an executable file and start it (see the following chapter).   

<a name="a30"></a>[_Top of page_](#up)   
# How do I compile and test the program m4hWdog?
The creation of the executable file is the same as described in the chapter [Raspberry Pi: Helpful single programs in C++](m4h310_RasPiCppDemos.md) as an example:   
1. Create a project directory `mkdir ~/m4hWdog`   
2. Change to the project directory `cd ~/m4hWdog`   
3. Copy the project files from GitHub to the project directory   
4. Create the executable file `m4hWdog`.  
  `g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread`   
5. Start the program   
  `~/m4hWdog/m4hWdog`   

Exit the program e.g. with the key combination &lt;Ctrl&gt;c   

## Test of the program
1. Open a terminal window (`cmd.exe`) or putty window at the PC or a console at the RasPi and start the program to display MQTT messages:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   

2. Start the program e.g. from a putty window   
  `~/m4hWdog/m4hWdog`   

3. If you do nothing, the following output appears in the putty window every 10 seconds:   
`ERROR! Watchdog timeout m4h/button_2`.   
Furthermore, the following MQTT message is displayed in the first terminal window every 10 seconds:   
`m4hWdog/attention sensor m4h/button_2 missing!`   

4. If you press the button `m4h/button_2` within the 10 seconds, the message appears in the Putty window:   
`Update secLast: m4h/button_2`   
and in the first terminal window the message of the button.   

5. If you don't have a button available, you can open a second terminal window (`cmd.exe`) or putty window on the PC or a second console on the RasPi and simulate a button message:   
`mosquitto_pub -h 10.1.1.1 -t m4h/button_2 -m anything`.   
With each message the watchdog timer is restarted.   

<a name="a90"></a>[_Top of page_](#up)   
# How is the m4hWdog program coded?   
## Creating the project
1. Windows: create the project folder `m4hWdog`   
2. Windows: copy the files of the template `m4hWdog` into the folder `m4hWdog`   
3. Start Visual Studio Code (VSC)   
4. Open the project folder in VSC   
   Menu File - Open folder... In the dialog change to the folder and press [Select Folder]   
5. Change the file name `C_Wdog.hpp` to `C_Wdog.hpp`   
   In the Explorer window, right-click on `C_Wdog.hpp`, select "Rename" and enter `C_Wdog.hpp`.   
6. In all files rename `XXX`, `Xxx` and `xxx` to `WDOG`, `Wdog` and `wdog`.   
   Edit menu - Replace to files   
   __IMPORTANT__: In the search input field activate the symbol "Aa", so that the upper/lower case is considered!   
   Approx. 23 replacements in 2 files or 35 replacements in 4 files and 9 replacements in 4 files are performed.   
7. Adjust formattings stc. (if desired)   
   * Since the new identifiers `Wdog` have one letter more than `Xxx`, the columns for comments are also shifted one place...   
   * Fix date to the current date (file `C_Wdog.hpp`, `m4hExtension.hpp`, `m4h.conf`)   
8. Save all files   
   Menu File - Save All

The project can already be transferred to the RasPi and tested there:   
* Start RasPi, connect PC with RasPi via WLAN.   
* Transfer the directory `m4hWdog` from the PC to the RasPi (with _WinSCP_)   
* Access the RasPi with _Putty_ (User `pi_`, PW `pass`) and change to the project folder:   
  `cd ~/m4hWdog`   
* _Putty_: Create executable program:   
  `g++ m4hMain.cpp m4hBase.cpp -o m4hWdog -lmosquitto -lpthread`   
* _Putty_: Run the program:   
  `./m4hWdog`   
* PC: Open terminal window (`cmd.exe`) and run the program to display MQTT messages:   
  `mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* PC: Open a second terminal window (`cmd.exe`) and send a MQTT message:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/get -m version`   
  In the first terminal window the messages appear   
  `m4hWdog/get version`   
  `m4hWdog/ret/version 2022-02-03`   
* PC: Exit the program using MQTT command:   
  `mosquitto_pub -h 10.1.1.1 -t m4hWdog/set -m ...end...`   
  In the first terminal window appear the messages   
  `m4hWdog/set ...end...`   
  `info/end__ m4hWdog (03.02.2022 18:13:19)`   
  or in _Putty_ the messages appear   
  `Exit program... MQTT end message sent.`   
  `Program terminated by MQTT (03.02.2022 18:13:19)`   
  `Finished`   

## Additional class "WdogIn1" for sensor data
Since for each sensor to be monitored   
* a topic,   
* the watchdog duration and   
* the time of the last call    

must be stored, a separate class `WdogIn1` is defined for this. For simplicity all properties are defined as `public`.   

## Define constant
To be able to use the program as flexible as possible, a constant and a variable is defined in the file `C_Wdog.hpp` for each key and each value of the configuration file:   
```   
#define  WDOG_OUT_KEY        "out"
#define  WDOG_OUT_TOPIC      "m4hWdog/attention"
#define  WDOG_OUT_PAYLOAD    "Sensor <in> missing!"
#define  WDOG_IN_KEY         "in"
```   
_IMPORTANT:_ Keys may only contain lower case letters!   

In the class `Wdog` the associated properties are defined:   
```   
 std::string wdog_out_key;             // topic out key
 std::string wdog_out_topic;           // topic out
 std::string wdog_out_payload;         // value out
 std::string wdog_in_key;              // topic in key
 std::vector<WdogIn1>vIn;              // topic in values
```   

Additionally, constants for the topic-in placeholder ("`<in>`") and the minimum and maximum value for the watchdog time are defined.   

In general, the "demo" entries in the `C_Wdog.hpp` file are not needed and can be commented out or deleted.   

## Set default values (setDefaults)
In the method `setDefaults()` the default values are assigned to the variables and the string `keys` is formed, which can display all keys.   
```   
 wdog_out_key     = WDOG_OUT_KEY;      // topic out key
 wdog_out_topic   = WDOG_OUT_TOPIC;    // topic out value
 wdog_out_payload = WDOG_OUT_PAYLOAD;  // payload out
 wdog_in_key      = WDOG_IN_KEY;       // topic out
 vIn.clear();                          // topic in values
 keys=std::string(WDOG_OUT_KEY);
 keys+="|"+std::string(WDOG_IN_KEY);
```   

## Reading the configuration file (readConfig)   
In the method `readConfig()` all lines of the section `[wdog]` of the configuration file are already read into the vector `v1` in the given code.   

Now the evaluation of the lines must be completed:   

* Each line is decomposed into its key and value (using the colon). Leading or trailing blanks are removed.   
* The value in the line `out:` is decomposed into topic and text (by the first blank).   
  If the text is missing, the default value is used.   
* Each line `in:` is split into topic and time text sHMS (by the first space character). Then the string `sHMS` is split into hour, minute and second, the strings are converted to numbers and the number of seconds is calculated.   
  Topic and time are converted into a `WdogIn1` object and pushed into the vector `vIn`.   
  If the time is missing or incorrect, the topic is ignored.   

## Display of the read data (show)   
The display of the read data is done in the usual way:   
```   
-----[wdog]------------------------------
config file         | ./m4h.conf
all keys            | out|in
out                 | -t m4hWdog/attention -m Sensor <in> missing!
-----in----------------------------------
m4h/button_2: 10sec | test/t3723: 3723sec | test/t259200: 259200sec |
```   

Coding:   
```   
 std::cout<<wdog_out_key<<"                 | -t "<<wdog_out_topic<<" -m "<<wdog_out_payload<<std::endl;
 std::cout<<"-----in----------------------------------"<<std::endl;
 for(int i=0; i<vIn.size(); i++) {
  std::cout<<vIn.at(i).topicIn<<": "<<vIn.at(i).secDiff<<"sec | ";
 }
 std::cout<<std::endl;
```   

## Reaction to incoming messages (onMessage).
If a message arrives, the topic is compared with all stored topics. If it is found, the time of the last call is set to the current time and the method is exited.   

Coding:   

```   
//_______act on messages..._____________________________________
bool Wdog::onMessage(struct mosquitto *mosq, std::string topic, std::string payload)
{
 int num=vIn.size();
 time_t now;
 time(&now);                           // get sec since 1.1.1970
 //------for all sensors----------------------------------------
 for(int i=0; i<num; i++){
  if(topic==vIn.at(i).topicIn) 
  {//----topic for watchdog found-------------------------------
   vIn.at(i).secLast=now;              // update sensor
   if(g_prt) std::cout<<"Update secLast: "<<vIn.at(i).topicIn<<std::endl;
   return true;
  }
 }
 return false;
}
```   

## Periodic watchdog control
For the periodic control, whether the maximum watchdog time was exceeded, the method `periodic` is defined. This must be called e.g. every second by the function `f5Periodic` of the file `m4hExtension.hpp`.   

* First the current time is determined ( `now`)   
* In a loop it is checked for each sensor whether the maximum watchdog time has been reached or exceeded.   
* If this is the case, the time of the last call is updated, the output payload is generated and the MQTT message is sent.   

[_Top of page_](#up)   
