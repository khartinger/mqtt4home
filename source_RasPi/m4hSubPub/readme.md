# m4hSubPub: Receive and send MQTT messages.
## Purpose of the program
This program uses `m4hBase` as a base to perform the following tasks:
1. after receiving a message, it waits for a certain time and then sends a response message.
2. each incoming and (associated) outgoing message and the waiting time must be defined in the configuration file (default `m4h.conf`) in a section `[subpub]` each.
3. for the outgoing message it can be defined if the retain flag should be set or not (keyword retain, default: false)
4. the program is built in such a way that it can be easily extended to process runtime data (JSON formatted incoming payload or action key). Built-in examples are:
   * Send along the wait time in the payload {"delay": 4000}
   * Sending along the outgoing payload {"text": "new payload out :)"}
   * Send the current time {"do": "brokertime"} or {"do": "time"}
   * Inverted forwarding of the incoming payload 0 or 1 as 1 or 0 {"do": "invert01"}

## Configuration examples
To make the following text easier to read, messages are represented as follows:   
`-t topic -m payload`   

### Example 1: Delayed message forwarding
To the incoming `-t /test/1/get -m name` message, a reply message `-t /test/1/ret/name -m "my name is m4hSubPub"` should be sent after two seconds.   
The entry in the configuration file `m4h.conf` for this looks like this:   
```   
[subpub]
in: /test/1/get name
out: /test/1/ret/name my name is m4hSubPub
delay: 2000
```   
The program automatically splits the first two program lines into __key__ (characters up to the colon i.e. in or out), __topic__ (characters - without spaces - after the colon up to the 1st space) and __payload__ (after the space up to the end of the line).

### Example 2: Forwarding a message after a waiting period has been included
After the arrival of a message `/test/2` the specified time (e.g. 3 seconds) should be waited and a response with topic `/test/2/ret` and included payload (e.g. `waited 3 seconds :)`) should be sent.   
Entry in the configuration file `m4h.conf`:    
```   
[subpub]
# payload e.g. "{\"delay\":3000,\"text\":\"waited 3 seconds :)\"}"
in: /test/2
out: /test/2/ret
```   

### Example 3: Query broker time
To the incoming message `-t /test/3/get -m time` a response message `-t /test/3/ret/time` with date and time is to be sent as payload. Furthermore the message should be stored at the broker (retain: true).   
Entry in the configuration file `m4h.conf`:    
```   
[subpub]
in: /test/3/get time
out: /test/3/ret/time
action: {"do": "brokertime"}
retain: true
```   
Note: `action: {"do": "time"}` returns the time in the german format dd.mm.yyyy HH:MM:SS
