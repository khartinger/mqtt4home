Last modified: 2023-03-05 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiZ2Mstatus1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiZ2Mstatus1` symbol.   
In general:   
* The CiZ2Mstatus1 icon is used to query the status of a Zigbee2MQTT bridge (e.g. Sonoff Zigbee 3.0 USB Dongle Plus model ZBDongle-P).   
* If you click on the upper half of the CiZ2Mstatus1 symbol, the status of the Zigbee2MQTT bridge ("health check") is queried.   
* Clicking on the lower half of the CiZ2Mstatus1 icon sends a restart MQTT message to the Zigbee2MQTT bridge.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiZ2Mstatus1Controller.ts` file.   

# Quick Reference
* Download directory `vuex510_ci_mqtt_z2mstatus1`.   
* Start Visual Studio Code (VSC) with Vue 3 + TypeScript support and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality without a Zigbee2MQTT bridge, you have to run a MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* The CiZ2mStatus1 icon waits for MQTT messages containing information about the Zigbee2MQTT bridge. For simulation, different response values can also be entered with the program `mosquitto_pub` in a Windows console:   
* `mosquitto_pub -h 10.1.1.1 -t z2m/bridge/state -m offline`   
* `mosquitto_pub -h 10.1.1.1 -t z2m/bridge/state -m online`   
* `mosquitto_pub -h 10.1.1.1 -t z2m/bridge/response/restart -m "{\"data\":{},\"status\":\"ok\"}"`   
* `mosquitto_pub -h 10.1.1.1 -t z2m/bridge/response/restart -m "{\"data\":{},\"status\":\"error\"}"`   
* `mosquitto_pub -h 10.1.1.1 -t z2m/bridge/response/health_check -m "{\"data\":{\"healthy\":true},\"status\":\"ok\"}"`   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with different `CiZ2Mstatus1` sizes. Which attributes cause which display is described afterwards.     
![CiZ2Mstatus1 variants](./images/vuex510_all_z2mstatus1.png "CiZ2Mstatus1 variants")   
_Fig. 1: Examples for different CiZ2Mstatus1 representations_.   

## Creating the CiZ2Mstatus1 symbol
The drawing of a CiZ2Mstatus1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiZ2Mstatus11>` and further attributes.   
_Example_:   
```   
<CiZ2Mstatus1 :x="50" :y="70"  sid="z2mstatus1" :fx="1.5" :fy="1" colorH="white" colorF="white" :border="3"></CiZ2Mstatus1>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiZ2Mstatus1 symbol to border the left edge and be 20 units away from the top edge:   
![location of a CiZ2Mstatus1 symbol](./images/vuex510_z2mstatus1_location1.png "location of a CiZ2Mstatus1 symbol")   
_Fig. 2: Location of a CiZ2Mstatus1 symbol_   

_Note_:   
Remaining space at the bottom: 220 - 20 - 100 = 100 units.   
Remaining space right: 500 - 100 = 400 units   

## ID of a symbol (sid)
The symbol ID (sid) represents the connection of the graphical representation to the controller (see property [id](#id) in chapter "[Attributes defined in the base controller](#id)").

## Border of a symbol (:border)
All CI symbols are normally 100x100 units on the outside and have a fixed margin of 5 units on the inside. So the individual "workspace" of a symbol is 90x90 units.   
The margin makes it possible to arrange symbols directly next to each other in a 100-unit grid without the symbol drawings visually touching each other.   
If you want to display the border of a CI symbol, there are four possibilities for this, which are set via the attribute `:border=`:   

| Parameter | Meaning |   
| ------------- | ------------------------------ |   
| `:border="0"` | No border will be displayed. |   
| `:border="1"` | Only the outer border of the symbol is displayed (= total space required by the symbol). |   
| `:border="2"` | Only the inner border of the symbol is shown (= the drawing area for the symbol). |   
| `:border="3"` | The border of the symbol is displayed in yellow. |   

Default is `:border="1"`.   

## Text information in CiZ2Mstatus1 symbol
Each `CiZ2Mstatus1` symbol knows four areas for text output:   
* header   
* footer   
* Upper button   
* Lower button   

It applies: 
* The header contains the symbol name (or `sid`) defined in the controller.   
* The footer contains the feedback of the Zigbee2MQTT bridge.   
* The labels of the buttons are defined in the controller.   

## Stretching of a symbol (:fx, :fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched symbols](./images/vuex510_z2mstatus1_stretch1.png "Stretched symbols")   
_Fig. 3: Stretched `CiZ2Mstatus1` symbol_   

## Color of a CiZ2Mstatus1 symbol (colorH, colorF, colorA1, colorA2)
The attributes `colorH=` and `colorF=` can be used to select the background color of the header and footer (default: none = transparent). The value can either be an RGB value (red-green-blue value) like `#ffee00` or a text value like `red`, `green` etc..   
The color of the buttons can be selected using the attributes `colorA1=` and `colorA2=`. If you don't set any color, they will show the state of the data transmission from the Zigbee2MQTT bridge.   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiZ2Mstatus1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiZ2Mstatus1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiZ2Mstatus1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiZ2Mstatus1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload sent when the upper button of the CiZ2Mstatus1 symbol is pressed. |   

## Attributes defined in `CiZ2Mstatus1Controller.ts`.
The following attributes are mandatory:   
| Property          | Meaning                              |   
| ----------------- | ------------------------------------ |   
| `iZ2MstatusState` | Status of the CiZ2Mstatus1 symbol. Use e.g. 0 or -99 as default for the start. |   

The following attributes are optional and can be used:   
| property | meaning |   
| ------------ | ------------------------------------ |   
| `textUpper` | Label of the upper button.     |   
| `textLower` | Label of the lower button.     |   
| `pubTopic2` | Topic that will be sent when pressing the lower button of the CiZ2Mstatus1 symbol. |   
| `pubPayload2` | Payload that will be sent when the lower button of the CiZ2Mstatus1 symbol is pressed. |   

## Example: Coding of the `CiZ2Mstatus1Controller.ts`.   
Normally only the values inside the `Array<Z2Mstatus1>` area need to be added.   

If you want to implement additional MQTT functionality when receiving messages, the code in the method `public onMessage (message: Message): void` can be extended accordingly.   

```
// ______CiZ2Mstatus1Controller.ts_______________khartinger_____
// 2023-02-28: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Z2Mstatus1 extends IBase {
  // ---------mandatory-----------------------------------------
  iZ2MstatusState: number;
  // ---------optional------------------------------------------
  textUpper?: string;
  textLower?: string;
  pubTopic2?: string;
  pubPayload2?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiZ2Mstatus1Controller extends CiBaseController {
  public z2Mstatus1s: Array<Z2Mstatus1> = reactive(
    [
      {
        id: 'z2mstatus1',
        name: 'Status zigbee2MQTT',
        iZ2MstatusState: -99,
        textUpper: 'get Status Z2M',
        textLower: 'Restart Z2M ',
        subTopic: 'z2m/bridge/response/health_check z2m/bridge/response/restart z2m/bridge/state',
        pubTopic: 'z2m/bridge/request/health_check',
        pubPayload: '',
        pubTopic2: 'z2m/bridge/request/restart',
        pubPayload2: ''
      }
    ]
  );

  // _________state constants___________________________________
  public maskZ2mOnline = 0x20;
  public maskZ2mOnlineValid = 0x10;
  public maskZ2mError = 0x08;
  public maskZ2mHealthy = 0x04;
  public maskZ2mStatusOk = 0x02;
  public maskZ2mStatusValid = 0x01;

  // _________react on incomming MQTT messages__________________
  public onMessage (message: Message): void {
    this.z2Mstatus1s.forEach(z2mstatus1 => {
      const aSubTopic = z2mstatus1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ===z2mstatus1 found==================================
        // health check: topic z2m/bridge/response/health_check-
        if (z2mstatus1.iZ2MstatusState < 0) z2mstatus1.iZ2MstatusState = 0
        if (message.topic.includes('health_check')) {
          try {
            // payload: {"data":{"healthy":true},"status":"ok"}
            // console.log(message.payload)
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusValid
            if (message.payload.includes('"healthy"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"healthy":true')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mHealthy
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mHealthy
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
            if (message.payload.includes('"status"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"status":"ok"')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
          }
        }
        // after restart: topic z2m/bridge/response/restart-----
        if (message.topic.includes('restart')) {
          try {
            // payload: {"data":{},"status":"ok"}
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusValid
            if (message.payload.includes('"status"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"status":"ok"')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
          }
        }
        // after restart: topic z2m/bridge/state----------------
        if (message.topic.includes('state')) {
          try {
            // payload: online|offline
            // console.log(message.payload)
            if (z2mstatus1.iZ2MstatusState < 0) z2mstatus1.iZ2MstatusState = 0
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnlineValid
            if (message.payload === 'online') {
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnline
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnlineValid
            }
            if (message.payload === 'offline') {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnline
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnlineValid
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnlineValid
          }
        }
      } // ===END of z2mstatus1 found===========================
    })
  }

  // _________publish a MQTT message____________________________
  public publishCi (topic: string, payload: string): void {
    // console.log('CiZ2Mstatus11Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiZ2Mstatus1Controller: ERROR:', e) })
  }
}

export const ciZ2Mstatus1Controller = new CiZ2Mstatus1Controller()
```

[Top of page](#up)
