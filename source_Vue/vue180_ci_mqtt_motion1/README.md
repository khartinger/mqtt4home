Last modified: 2023-02-09 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiMotion1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiMotion1` symbol.   
In general:   
* The `CiMotion1` icon waits for MQTT messages and shows the motion state.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiMotion1Controller.ts` file.   

# Quick Reference
* Download directory `vue150_ci_mqtt_CiMotion1`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* The CiMotion1 symbol waits for MQTT messages of the following type, which are e.g. sent by Zigbee sensors or - for simulation - can be entered with the program `mosquitto_pub` in a Windows console:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/motion/-1 -m "{\"battery\":47,\"occupancy\":false}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/motion/-1/time -m "20230209 123344"`   
The values are displayed accordingly.   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a ci symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiMotion1` representations. Which attributes cause which representation is described afterwards.     
![CiMotion1 variants](./images/vue180_all_motion1.png "CiMotion1 variants")   
_Fig. 1: Examples for different CiMotion1 representations_.   

## CiMotion1 call
The drawing of a CiMotion1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiMotion1>` and further attributes.   
_Example_:   
```   
<CiMotion1 :x="50" :y="80" sid="motion1" :border="3" :fx="2" :fy="1.5" colorH="white"></CiMotion1>
```   

## Positioning of a symbol
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiMotion1 to border the left edge and be 20 units away from the top edge:   
![location of a CiMotion1](./images/vue180_motion1_location1.png "location of a CiMotion1")   
_Fig. 2: Location of a CiMotion1 symbol_   

_Note_:   
Remaining space at the bottom: 220 - 20 - 100 = 100 units.   
Remaining space right: 500 - 100 = 400 units   

## Border of a symbol
All CI symbols are normally 100x100 units on the outside and have a fixed margin of 5 units on the inside. So the individual "workspace" of a symbol is 90x90 units.   
The margin makes it possible to arrange symbols directly next to each other in a 100-unit grid without the symbol drawings visually touching each other.   
If you want to display the border of a CI symbol, there are four possibilities for this, which are set via the attribute `:border=`:   

| Parameter | Meaning |   
| ------------- | ------------------------------ |   
| `:border="0"` | No border will be displayed. |   
| `:border="1"` | Only the outer border of the symbol is displayed (= total space required by the symbol). |   
| `:border="2"` | Only the inner border of the symbol is shown (= the drawing area for the symbol). |   
| `:border="3"` | The border of the symbol is displayed in yellow. |   

Default is `:border="2"`.   

## Text information in CiMotion1 symbol
Each `CiMotion1` symbol knows three possible areas for text output:   
* header   
* footer   
* drawing area (= text area)   

### Header
If the parameter `name` is defined in the controller, this text is displayed, otherwise the `id`.

### Footer
1. if the `textFooter` parameter is defined in the controller, this text is displayed.   
2. if the received MQTT message contains an indication of the battery state of charge ("battery") and the value is assigned to the attribute in the `onMessage()` method (file `CiMotion12Controller.ts`), this value is displayed.   
3. if none of the points 1 and 2 is fulfilled, the `id` is displayed.   

### Character area
The character area contains three lines (lines 2 to 4) that indicate the following:   
#### Line 2   
On the left side the symbol of a motion detector `))` and on the right side the state of motion is displayed (`on`, `off` or `?`)   
#### Line 3   
1. if the id of the symbol (`sid`) does not match any entry in the list `public motion1s: Array<Motion1>` (file `CiMotion12Controller.ts`), "`unknown`" is displayed.   
2. if there is a date value, it will be displayed (format `dd.mm.yy`).   
3. otherwise `date?` is displayed.   
#### Line 4   
1. if the id of the symbol (`sid`) does not match any entry in the list `public motion1s: Array<Motion1>` (file `CiMotion12Controller.ts`), "`sid`" is displayed.   
2. if there is a time value, it will be displayed (format `HH:MM:SS`).   
3. Otherwise `Time?` is displayed.   

## Stretching of a symbol
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched CiMotion1](./images/vue180_motion1_stretch1.png "Stretched CiMotion1")   
_Fig. 3: Stretched CiMotion1 symbol_   

The left two CiMotion1 symbols are stretched by the factor fx=2 (results in 2*100 = 200 units), the third symbol is stretched by the factor fy=2 and the right symbol is stretched by fx=2 and fy=2, i.e. it is twice as large as the original with a size of 200x200.   

__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiMotion1 :x="50" :y="70"  sid="motion1" :fx="2" :fy="1"></CiMotion1>
  <CiMotion1 :x="50" :y="170" sid="motion3" :fx="2" :fy="1"></CiMotion1>
  <CiMotion1 :x="250" :y="70" sid="motion1" :fx="1" :fy="2"></CiMotion1>
  <CiMotion1 :x="350" :y="70" sid="motion1" :fx="2" :fy="2"></CiMotion1>
  </svg>
</template>
```   

## Color of the CiMotion1 symbol
### Color of the drawing area
The drawing area is colored depending on the motion state:   
| state | color name | color |   
| ------- | ------------------- | ------------ |   
| `on` | `colorMotion` | yellow |   
| `off` | `colorNoMotion` | light gray |   
| `?` `geof.colorUnknown` | medium blue |   

If an error occurs, the background color is set to light red. (Color "`colorNotOK`" from the file `Geo.ts`)   

### Header color   
With the help of the attribute `colorH=` the background color of the header of a CiMotion1 symbol can be set. The value can either be an RGB value (red-green-blue value), such as `#eedd00` or a text specification such as `red`, `white` etc..   
If no color is selected, the header is transparent.   

### Footer color   
* In case of an error the background color is set to light red. (Color "`colorNotOK`" from the file `Geo.ts`).   
* If a "`battery`"-value is present, this value is compared with the `batteryMin`-value (=15). If the `battery` value is larger, the footer line is colored green (`colorOk`) otherwise light red (`colorNotOk`). If an error occurs, the color medium blue is chosen (`colorUnknown`).   
* If a color value was specified with `colorF=`, this color will be used.   
* Otherwise the background is transparent.

# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning                                  |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiMotion1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiMotion1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiMotion1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiMotion1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiMotion1Controller.ts`.
The following attributes are mandatory:
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `iMotion1State` | State of the CiMotion1 symbol. For this purpose the constants `stateOn`, `stateOff`, `stateNotOk` and `stateNoState` are defined in the file `CiMotion1Controller.ts`.   |   
| `type` | Type of the CiMotion1 symbol. Tells something about what data is expected from the motion detector.   |   
| `lastMotion1Date` | Date when the last motion message arrived.<br>Can be set to `Date?` or `--.--.--` when filling for a specific symbol, for example.   |   
| `lastMotion1Time` | Time when the last motion message arrived.<br>Can be set to `Time?` or `--:--` when filling in for a specific symbol, for example.   |   

The following attributes are optional and can be used if required:   
| property       | meaning                              |   
| -------------- | ------------------------------------ |   
| `battery`      | String with the state of charge of a battery. Displayed in line 5.   |   
| `textFooter`   | Specification of a fixed text in the footer line. |   

## Example: Coding of the `CiMotion1Controller.ts`.   
Normally, only the values within the `Array<CiMotion1>` area need to be added.   
In the example below three CiMotion1 symbols are defined, where the third symbol does not evaluate a possible 'battery' value. The symbol is only used to show the color of the footer.   
In the method `public onMessage (message: Message): void` the strings for "`lastMotion1Date`" and "`lastMotion1Time`" are used to display the text "`ERROR json`" in case of an error.   

If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

```
// ______CiMotion1Controller.ts_________________khartinger_____
// 2022-01-01: new
// 2023-02-09: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Motion1 extends IBase {
  // ---------mandatory-----------------------------------------
  iMotion1State: number;
  type: string;
  lastMotion1Date: string;
  lastMotion1Time: string;
  // ---------optional------------------------------------------
  battery?: string;
  textFooter?: string;
}

export class CiMotion1Controller extends CiBaseController {
  public motion1s: Array<Motion1> = reactive(
    [
      { // ---motion1: IKEA-------------------------------------
        id: 'motion1',
        name: 'Motion1_1',
        type: 'E1745', // IKEA (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-1 z2m/t/motion/-1/time',
        pubTopic: ''
      },
      { // ---motion2: Aqara------------------------------------
        id: 'motion2',
        name: 'Motion1_2',
        type: 'RTCGQ11LM', // Aquara (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-2 z2m/t/motion/-2/time',
        pubTopic: ''
      },
      { // ---motion3: just for test...-------------------------
        id: 'motion3',
        name: 'No_batt',
        type: 'noBattery',
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        textFooter: 'show colorF',
        subTopic: 'z2m/t/motion/-3 z2m/t/motion/-3/time',
        pubTopic: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOn = -1;
  public stateOff = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.motion1s.forEach(motion1 => {
      const aSubTopic = motion1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------motion1 topic found ------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            motion1.lastMotion1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            motion1.lastMotion1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            motion1.lastMotion1Date = geo0f.noDate
            motion1.lastMotion1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different motion types---------------
          motion1.iMotion1State = this.stateNotOk
          if (motion1.type === 'RTCGQ11LM' || motion1.type === 'E1745') {
            try {
              const aPayload = JSON.parse(message.payload)
              motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'noBattery') {
            try {
              const aPayload = JSON.parse(message.payload)
              // motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different motion types----------
      } // --------END motion1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiMotionController: ERROR:', e) })
  }
}

export const ciMotion1Controller = new CiMotion1Controller()
```

[Top of page](#up)
