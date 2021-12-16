# RasPi: Reacting to specific keys in (JSON) payloads
Program: m4hPayload, Version 2021-08-30   
[--> Deutsche Version](./LIESMICH.md "Deutsche Version")   

## Purpose of the program
The program `m4hPayload` serves as a basis for a C++ program, that   
* on finding a certain key in the payload of a message   
* can send a MQTT response message.   

_Example_: If the value of the key "`battery`" falls below a certain value __of any topic__ (!), a warning message is sent.   

### Basic structure of the program
The program `m4hPayload` is structured in such a way that   

* the processing of the key only in the method   
   `bool Payload::editPayload(Payload1 &p1, std::string sValue)`   
   of the file `C_Payload.hpp` has to be added (as C++ code) and   

* details about the processing of a key are in the configuration file (e.g. `m4h.conf`).   

This division makes it possible to have to make subsequent adjustments only in the configuration file.   

## Example: Adding a "frost warning".
The program 'm4hPayload' is to be supplemented by the following functionality:   

If the temperature of a sensor falls below a value of 3 degrees Celsius, the following message is to be sent:   
`-t info/frost_warning -m <in> (<value>degree)`   
where `<in>` should be replaced by the topic of the temperature sensor and `<value>` by the current temperature value.   

_Note_: By changing the topic to `send/sms ...` the sending of an SMS could also be achieved...   

### Solution part 1: Entry in the configuration file   
In the configuration file in a section `[payload]` three 
information must be specified:   
* `key: ` The key to search for in the (JSON formatted) payload ( here `temperature`).
* `if: ` The condition to be met. This consists of 3 parts:   
   1. comparison character (here `<`), 
   2. value with which the incoming value is to be compared (here `3`) and
   3. text, which should be output, if the condition is fulfilled   
   (here "`<in> (<value> *C)`", where `<in>` is replaced by the topic and `<value>` by the temperature value automatically).
* `out: ` the topic for the outgoing message (here `info/frost_warning`).   
_Note_: The payload is at the `if:`.   

Result:   
```   
[payload]
key:    temperature
if:     <3 <in> (<value> *C)
out:    info/frost_warning
```   
   
### Solution Part 2: C++ Code   
The following C++ code must be added to the file `C_Payload.hpp` in the method 
 `bool Payload::editPayload(Payload1 &p1, std::string sValue)` before the line   
 ` //=========== add your code here... ======================`   
 must be inserted:   
 ```   
 //===========process payload key "temperature"=================
 if(p1.key=="temperature")
 {//----------skip given topics---------------------------------
  for(int ii=0; ii<p1.vSkip.size(); ii++) {
   if(p1.m2.topicIn==p1.vSkip.at(ii)) return false;
  }
  //----------process condition---------------------------------
  try { //----string to double----------------------------------
   double dValIn=0, dValConf=0;        // temperature as double
   double temp=std::stod(sValue);      // convert incomming value
   dValIn=temp;                        // store value
   temp=std::stod(p1.ifVal);           // convert config value
   dValConf=temp;                      // store value
   //---------process condition---------------------------------
   switch(p1.ifCond) {                 // type of comparison chars
    case 1:                            // 1 means '<' in config
     if(dValIn<dValConf) {             // compare temperatures
      sPay+=p1.ifText;                 // payload = if text
      bRet=true;                       // yes, send warning msg
     }                                 // end if(dValIn<dValConf)
     break;                            // end of case 1
    default: break;                    // all other comp chars
   }                                   // end switch
  } catch(std::invalid_argument) {};   // do nothing on error
 } // end process payload key "temperature"
```   

__*Explanation*__:   
Properties:   
* `p1` is an object of class `Payload1` which contains all information of current message and current section.   
* `sValue` is the current value to the key (`temperature`).   

Code:   
* `if(p1.key=="temperature"){}` represents the block in which the key `temperature` is processed.   
* In the for loop, all topics listed in the configuration file in the section under `skip:` are compared with the current topic. If the topic matches, processing is aborted (`return false;`).
* In the try-catch block, first the two temperature values are converted to double numbers. If an error occurs, the program jumps to catch and the following code is not executed.
* The `switch(p1.ifCond)` block distinguishes between the different comparison characters, assigning an integer to each character when reading the configuration file:   
1 means `<`, 2 `<=`, 3 `=`, 4 `!=`, 5 `>=`, and 6 `>`.   
* In `case 1:` the two temperatures are compared. If the temperature is too small, the if text is appended to the payload and `bRet=true;` allows sending the response message.   


### Test
Generate the executable file by entering   
`g++ m4hMain.cpp m4hBase.cpp -o m4hPayload -lmosquitto -lpthread`   
After starting the program by   
`pi_@raspi:~/m4hPayload $ ./m4hPayload`   
you get the following output:   
```   
Read config file ./m4h.conf: OK
-----[base]---------------------------
config file         | ./m4h.conf
section name        | base
version (in)        | -t m4hPayload/get -m version
version (out)       | -t m4hPayload/ret/version -m 2021-08-30
mqtt @ start (out,*)| -t info/start -m m4hPayload
mqtt @ end (out,*)  | -t info/end__ -m m4hPayload
progend by mqtt (in)| -t m4hPayload/set -m -end-
         * add time | true
-----[payload]------------------------
config file         | ./m4h.conf
all keys            | in|skip|key|if|else|out|retain|<in>|<value>
.....Payloads to check................
topic-in: (all) | skip: (none)
     key: temperature | if <3 <in> (<value> *C) else  | OUT: -t info/frost_warning -m
Try to connect to mosquitto...
Connected: Waiting for topics...
```   

Send a message, e.g.   
`mosquitto_pub -t zigbee/sensor1 -m "{\"battery\":100,\"humidity\":54.17,\"linkquality\":36,\"pressure\":974.4,\"temperature\":2.32,\"voltage\":3035}"`   
you get the following MQTT messages:   
```   
zigbee/sensor1 {"battery":100,"humidity":54.17,"linkquality":36,"pressure":974.4,"temperature":2.32,"voltage":3035}
info/frost_warning zigbee/sensor1 (2.32 *C)
```   
_Note_: The payload of e.g. the temperature sensor TS0201 (TuYa Temperature & humidity sensor with display) is structured as follows:   
`{"battery":100,"humidity":54.17,"linkquality":36,"pressure":974.4,"temperature":22.32,"voltage":3035}`   