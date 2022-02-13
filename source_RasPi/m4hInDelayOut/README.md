Last modified: 2022-02-13 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>m4hInDelayOut: Receive and send MQTT messages</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

## Purpose of the program
This program uses `m4hBase` as a base to send out a (different) message with a time delay after the arrival of a certain message.   
The topic of the incoming message must be known.   
### Furthermore
1. Each incoming and (related) outgoing message must be defined in the configuration file (default `m4h.conf`) in a section `[indelayout]` each.   
(Keyword `in` or `out`)   
2. For the outgoing message it can be specified whether the retain flag should be set or not.   
(Keyword `retain`, default: `false`).   
3. For the outgoing message it can be defined whether the same message can be sent several times in a row.   
(keyword `allowsame`, default: `false`).   
4. The delay time can be specified either fixed in the configuration file or dynamically in the payload.   
(Keyword `delay`, default `0`ms)   
5. How the content of the payload should be interpreted can be controlled by the `action` keyword (see below).
6. Certain placeholders are replaced by corresponding values in the outgoing payload:   
```<in> | <text> | <delay> | <value> | <invert> | <brokertime> | <time> ```   
   `<in>` = incoming topic, `<delay>` = delay time in ms, `<brokertime>` = current time in the format "yyyymmdd HHMMSS", `<time>` = current time in the German format "dd.mm.yyy HH:MM:SS".
7. The program is arbitrarily extendable (see example below).   

### Implemented "action:" parameters
* `text ........ ` The payload corresponds to the placeholder `<text>`.   
* `delay ....... ` The payload is the delay time in milliseconds.  
* `delaytext ... ` The payload consists of the delay time (in ms) and the text.   
* `setvalue .... ` The value after this is available as a placeholder. `<value>`.   
* `invert ...... ` The following two values are compared with the incoming payload and the other value is made available as `<invert>`.   
   
### List of keywords in the configuration file
`in | delay | action | out | retain | allowsame`   

&nbsp;
   
## Configuration examples
To make the following text easier to read, messages are represented as follows:   
`-t topic -m payload`   

### Example 1: Delayed forwarding of a message
The incoming message `-t /test/1/get -m name` should be followed by a `-t /test/1/ret/name -m "Program name: m4hInDelayOut (4000 ms)."` reply message after four seconds.   
The entry in the configuration file `m4h.conf` for this looks like this:   
```   
[indelayout]
in: test/1/get name
delay: 4000
out: test/1/ret/name Program name: m4hInDelayOut (<delay> ms).
```   
The program automatically splits the two lines `in:` and `out:` into __key__ (characters up to the colon i.e. in and out respectively), __topic__ (characters - without spaces - after the colon up to the 1st space) and __payload__ (after the space up to the end of the line).

### Example 2: Sending a message with the current time
After the arrival of the message `-t test/1/get -m time` a message with the current time should be sent out in the German format.   
```   
[indelayout]
in: test/1/get time
out: test/1/ret/time time (german format): <time>
```   

### Example 3: Switch-off delay
After a lamp has been switched on by the message `-t test/1/set/lamp -m 1`, it should be switched off by the payload `0` after 5 seconds.   
```   
[indelayout]
in: test/1/set/lamp
delay: 5000
action: setvalue 0
out: test/1/set/lamp <value>
```   

### Example 4: Brokertime
After the arrival of the message `-t getTime -m ?` a message with the current time as "Brokertime" in the format "yyyymmdd HHMMSS" is to be sent out and stored (retain=true).   
```   
[indelayout]
in: getTime ?
out: brokertime <brokertime>
retain: true
```   

### Example 5: Delayed forwarding of a payload under a different topic
The payload of the message `-t test/2/set/text -m "Hello world!"` should be forwarded after 4 seconds under the topic `test/2/ret/text`.   
```   
[indelayout]
in: test/2/set/text
delay: 4000
action: text
out: test/2/ret/text <text>
```   

### Example 6: Delay time as payload
After the arrival of the message `-t test/3/set/delay -m 3000` the message specified in the configuration file should be sent out after the specified time (3000ms = 3 seconds).   
```   
[indelayout]
in: test/3/set/delay
action: delay
out: test/3/delay/ret Payload was delay time (Delay: <delay>ms)
```   

### Example 7: The incoming payload contains delay time and text
After the arrival of the message `-t test/4 -m "3000 Waited for 3 seconds :)"` the specified text ("Waited for 3 seconds :)") should be published under the topic `test/4/ret` after 3 seconds.
```   
[indelayout]
in: test/4
action: delaytext
out: test/4/ret <text>
```   
The "delaytext" action splits the incoming payload with the help of the first space into a delay time (3000) and the rest as text (in `<text>`).

### Example 8: Inverted retransmission of a payload
The payload of an incoming message (e.g. `-t test/5/invert -m 1`) is compared with the two values stored in the configuration file. If one value matches, the other value is made available in the placeholder `<invert>` and can be forwarded under the topic `test/5/invert/ret`, for example.   
```   
in: test/5/invert
delay: 2000
action: invert 0 1
out: test/5/invert/ret <invert>
retain: false
```   

### Example 9: Repeated sending of a message
If you set the entries of `in:` and `out:` in the configuration file to the same value, the message may be sent endlessly. By default this is not allowed. If you want to intentionally allow endless sending of a message, the keyword `allowsame` must be set to `true`.   
```   
[indelayout]
in: test/6 -1
delay: 2000
out: test/6 -1
allowsame: true
#allowsame: false
```   
Application examples could be: e.g. the regular query of a sensor or a blinking light etc.

## Extension of the program
The extension of the program is to be shown at a simple example:   
The payload is to be interpreted as an integer and the number increased by 1 is to be available as a placeholder `<inc>`. If a limit specified in the configuration file is exceeded, or if the payload is not an integer, `<inc>` is set to `0`.   
The entry in the configuration file should generate an endless counter from 0 to 12 in 3-second intervals under the topic `test/counter`.   

### Solution for the configuration file
```   
[indelayout]
in: test/counter
delay: 3000
action: inc 12
out: <in> <inc>
allowsame: true
```   
### Additions to the file C_InDelayOut.hpp   
   
1. add the following to the existing `#define` statements:   
   ```
   #define IDO_PLAHO_INC        "<inc>"
   #define IDO_ACT_INC          "inc"
   ```   
   _Note_: The definitions of the placeholder ("PLAHO") and the "action" command ("ACT") are just to keep the program clear ;)   

2. in the method `void InDelayOut::threadFunctionDelay()` at the beginning after `std::string s1; // help value` insert a variable ´sInc´ to buffer the `<val>` value:   
   ```   
   std::string sInc="0"; // increment value
   ```   

3. in the method `void InDelayOut::threadFunctionDelay()` insert, for example, after the block `if(actionKey==IDO_ACT_INVERT) {}` the block to increment the payload value:   
   ```   
   if(actionKey==IDO_ACT_INC) 
   {//---------increment payloadIn-------------------------------
    try{
     int iPayloadIn=std::stoi(cpay); // payload in as int
     int iIncmax=std::stoi(actionVal); // config max value as int
     iPayloadIn++; // increment value
     if(iPayloadIn>iIncmax) temp=0; // limit check
     sInc=std::to_string(iPayloadIn); // convert to string
    } catch(std::string& error) { return; }
   }
   ```   

4. in the method `void InDelayOut::threadFunctionDelay()` still __BEFORE__ the line `if(m2d.payloadOut.length()<1) return;` perform the placeholder replacement:   
   ```   
   conf.replaceAll(m2d.payloadOut,IDO_PLAHO_INC,sInc);
   ```   

### Translate and test the program
1. compile the program:   
   `g++ m4hMain.cpp m4hBase.cpp -o m4hInDelayOut -lmosquitto -lpthread
`   
2. start the program:   
   `./m4hInDelayOut`   
   
3. start the counter by e.g. the following MQTT message:   
   `mosquitto_pub -t test/counter -m 10`    
   _Note_: 10 is an example for the start value   
   Result in a second console (with `mosquitto_sub -t "#" -v`):   
   ```   
   info/start m4hInDelayOut (09/06/2021 16:47:10)
   test/counter 10
   test/counter 11
   test/counter 12
   test/counter 0
   test/counter 1
   info/end__ m4hInDelayOut (09/06/2021 16:47:27)
   ```   