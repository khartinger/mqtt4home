Last modified: 2023-02-18 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiBlind1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiBlind1` symbol.   
In general:   
* The `CiBlind1` symbol waits for MQTT messages to represent the state of a blind or a blind motor (open, closed, locked; up, down, stop).   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiBlind1Controller.ts` file.   

# Quick Reference
* Download directory `vuex340_ci_mqtt_CiBlind1`.   
* Start Visual Studio Code (VSC) with TypeScript support installed and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* The CiBlind1 symbol waits for MQTT messages containing information about the blind or motor state and possibly the battery state. For simulation, these values can also be entered with the program `mosquitto_pub` in a Windows console:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/blind/-1 -m "{\"contact\":false,\"battery\":90}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/blind/-1 -m "{\"contact\":true,\"battery\":10}"`   
  * `mosquitto_pub -h 10.1.1.1 -t wifi/t/blind/-5/ret/status -m 1`   

The values are displayed accordingly.   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiBlind1` representations. Which attributes cause which representation is described afterwards.     
![CiBlind1 variants](./images/vuex340_all_blind1.png "CiBlind1 variants")   
_Fig. 1: Examples for different CiBlind1 representations_.   

## CiBlind1 call
The drawing of a CiBlind1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiBlind1>` and further attributes.   
_Example_:   
```   
<CiBlind1 :x="50" :y="70" sid="blind_o" dir="h1"></CiBlind1>
```   

## Positioning of a symbol (:x, :y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiBlind1 to border the left edge and be 20 units away from the top edge:   
![location of a CiBlind1](./images/vuex340_blind1_location1.png "location of a CiBlind1")   
_Fig. 2: Location of a CiBlind1 symbol_   

_Note_:   
Remaining space at the bottom: 220 - 20 - 100 = 100 units.   
Remaining space right: 500 - 100 = 400 units   

## ID of a symbol (sid)
The symbol ID (sid) represents the connection of the graphical representation to the controller (see property [id](#id) in chapter "[Attributes defined in the base controller](#id)").

## Border of a symbol (:border)
Normally CI symbols are 100x100 units in size outside and have a fixed border of 5 units inside. With the CiWall1 symbol this is different:   
In order to make adjacent symbols in a 100 unit grid look continuous, the `CiBlind1` symbols go to the symbol border, so they are exceptionally 100x100 units in size.   
Normally, the border of the `CiBlind1` symbols is not displayed. If you want to display the border exceptionally, there are four ways to do so, which are set via the `:border=` attribute:   

| Parameter     | Meaning                        |   
| ------------- | ------------------------------ |   
| `:border="0"` | No border will be displayed.   |   
| `:border="1"` | Only the outer border of the symbol is displayed (= total space required by the symbol). |   
| `:border="2"` | Only the inner border of the symbol is shown (= the drawing area for the symbol). |   
| `:border="3"` | The border of the symbol is displayed in yellow. |   

Default is `:border="0"`.   

## Direction of the CiBlind1 symbol (dir)
The direction of a symbol is specified with the `dir` ("Direction") property.   

The __first letter__ indicates the base direction:   
* `h` or `H`: Horizontal (horizontal, x-direction).   
* `v` or `V`: Vertical (vertical, y-direction)   
* `d` or `D`: Diagonal (inclined at +/- 45°)   

If a lowercase letter is used, the blind direction is drawn small, if an uppercase letter is used, it is drawn large (see _Fig. 1_).   

The __second letter__ indicates the opening direction, which is the quadrant into which the blind opens. According to the 4 quadrants, the numbers 1 to 4 are possible.   

![blind_dir_axes](./images/vuex340_blind1_dir_axes1.png "blind_dir_axes")   
_Fig. 3: Direction labels for symbols in axis direction_   

For diagonal elements, a __third letter__ is required to indicate the `propagation` direction more precisely. The first letter is a `d` (for diagonal), the second letter indicates the diagonal direction for the symbol (`1` to `4`) and the third letter indicates the axis towards which an element opens (`x` or `y`).   

![blind_dir_dia](./images/vuex340_blind1_dir_dia1.png "blind_dir_dia")   
_Fig. 4: Directional labels for symbols in diagonal direction_   

__Examples of designations:__   
`dir="h1" ....` Horizontal symbol whose propagation (blind opening) is in the first quadrant (towards "up").   
`dir="v1" ....` Vertical symbol, whose propagation (blind opening) is in the first quadrant (to "right").   
`dir="d1x" ...` Diagonal symbol to the right-up, whose propagation (blind opening) is in the direction of the x-direction (to "down").   

## Text information in CiBlind1 symbol (lines)
Each `CiBlind1` symbol knows three lines for text output:   
* header   
* line 4   
* footer   

Which texts are actually displayed in the symbol depends on the value of the 'lines=' attribute:    
| parameter | meaning |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Neither header nor footer will be displayed. |   
| `lines="1"`   | The header line is displayed. If the `name` parameter is defined in the controller, this text is displayed, otherwise the `id`. |   
| `lines="2"`   | Header and footer are displayed.   |   

Default is `lines="2"`.   

### Header
1. If the attribute `lines="0"` is set in the symbol, the header will not be displayed.   
2. If the parameter `name` is defined in the controller, this text will be displayed.   
3. If the parameter `name` contains a question mark, it will be replaced by the text `OPEN`, `CLOSE` or `LOCK` depending on the blind status.   
4. Otherwise the `id` is displayed.   

### Line 4
1. If the attribute `lines="0"` is set in the symbol, the line 4 will not be displayed.   
2. If the id of the symbol (`sid`) does not match any entry in the list `public blind1s: Array<Blind1>` (file `CiBlind1Controller.ts`), "`unknown`" is displayed.   
3. If there is a value for the MQTT key `battery`, this value is displayed.   
4. Otherwise, an empty line is output.   

If the blind symbol is drawn in line 4, these texts are displayed in line 2.   

### Footer
1. If the attribute `lines="0"` or `lines="1"` is set in the symbol, the footer will not be displayed.   
2. If the id of the symbol (`sid`) does not match any entry in the list `public blind1s: Array<Blind1>` (file `CiBlind1Controller.ts`), "`sid`" is displayed.   
3. If the `textF=` attribute is used in the symbol, this text will be displayed.   
4. If the `textFooter` parameter is defined in the controller, this text will be displayed.   
5. If the parameter `textFooter` contains a question mark, it will be replaced by the text `OPEN`, `CLOSE` or `LOCK` depending on the blind status.   
6. Otherwise, an empty line is output.   

![miscellaneous footers](./images/vuex340_blind1_footer1.png "miscellaneous footers")   
_Fig. 5: Miscellaneous footers_   

__Coding example__:   
```   
    <CiBlind1 :x="50"  :y="70" sid="blindX" dir="h1"></CiBlind1>
    <CiBlind1 :x="150" :y="70" sid="blind_o" dir="h1"></CiBlind1>
    <CiBlind1 :x="350" :y="70" sid="blind_o" dir="h1" lines="0"></CiBlind1>
    <CiBlind1 :x="50"  :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="150" :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="250" :y="170" sid="blindF" dir="h1"></CiBlind1>
    <CiBlind1 :x="350" :y="170" sid="blindF" dir="h1" textF="textF :)"></CiBlind1>
```   

## Stretching of a symbol (:f)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbol (on both sides) so that the placement point remains the same.   
Stretching is done by specifying the `f` parameter, as the following example shows:   
![Stretched CiBlind1](./images/vuex340_blind1_stretch1.png "Stretched CiBlind1")   
_Fig. 6: Stretched CiBlind1 symbol_   

The symbol at the bottom left and far right are each stretched by a factor of 2.   

__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiBlind1 :x="100" :y="70"  sid="blind_o" dir="h1" :border="3"></CiBlind1>
  <CiBlind1 :x="100" :y="170" sid="blind_o" dir="h1" :f="2"></CiBlind1>
  <CiBlind1 :x="300" :y="110" sid="blind_o" dir="v2"></CiBlind1>
  <CiBlind1 :x="400" :y="110" sid="blind_o" dir="v2" :f="2"></CiBlind1>
  </svg>
</template>
```   

## Different CiBlind1 shapes (shape)
CiBlind1 symbols come in two shapes, selected by the `dir=` attribute:   
* a lowercase letter results in a small rectangle (one text line high).   
* a capital letter results in a large rectangle (two lines high).   

## Colors in CiBlind1 symbol (colorH, colorF, colorMotor)
The colors in the CiBlind1 symbol can be partially selected or are assigned automatically.   

### Automatically set colors
* Incorrect "`sid`", i.e. an object with this "`sid`" does not exist in the controller:   
* The blind area is colored light blue (color "`colorUnknown`" from the file `Geo.ts`) and lines 4 and 5 are colored light red (color "`colorNotOK`" from the file `Geo.ts`).   
* The blind area is colored according to the blind status (variable `iBlind1State`):   
  * blind open: light green (`colorOpen`)   
  * Blind closed: light red (`colorClose`)   
  * Blind locked: red (`colorLock`)   
* If a battery value is smaller than `batteryMin`, line 4 is colored light red (color "`colorNotOK`" from the file `Geo.ts`).   
* The motor area is colored depending on the value of the variable `iMotorState`:   
  * No motor information: wall color (dark red, `colorWall`).
  * Motor in "up" position: yellow "motor" rectangles (`colorOn`).
  * Motor in idle state: dark gray "motor" rectangles (`colorOff2`).
  * Motor in "down" position: golden "motor" rectangles (`colorOn2`).

### Selectable colors
* The `colorH=` attribute can be used to set the background color of the header. The value can be either an RGB value (red-green-blue value), such as `#ffee00` or a text specification such as `red`, `green` etc.   
* With the attribute `colorF=` the background color of the footer can be set (color value see above).   
* With the attribute `colorMotor=` the color of the motor elements (rectangles left and right of the blind area) can be set (color value see above).   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning                                  |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiBlind1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiBlind1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiBlind1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiBlind1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiBlind1Controller.ts`.
The following attributes are mandatory:   
| Property       | Meaning                              |   
| -------------- | ------------------------------------ |   
| `iBlind1State` | Status of the CiBlind1. Use e.g. -99 as default for the start. |   
| `type`         | Sensor type that detects the blind status and sends a corresponding MQTT message. |   

The following attributes are optional and can be used:   
| property      | meaning                              |   
| ------------- | ------------------------------------ |   
| `iMotorState` | Blind motor state. Possible states are `stateMotorDown`, `stateMotorStop` or `stateMotorUp`. Changes the color of the motor (see above).   |   
| `battery`     | Receive the state of charge if a CiBlind1 sensor is operated with battery. Can be displayed in the footer. |   
| `textHeader`  | Text in the header of the symbol.    |   
| `textFooter`  | Specify a fixed text in the footer.  |   

## Example: Coding of a `CiBlind1Controller.ts`.   
Normally, only the values within the `Array<Blind1>` area need to be added.   
If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

In the example below, six blind objects are defined:   
The first three objects `blind_o`, `blind_c` and `blind_l` are used to represent open, closed and locked blinds.   
The objects `blind_bat10` and `blind_bat20` are used to represent a battery value and `blindF` shows the display of a footer value.   

```
// ______CiBlind1Controller.ts___________________khartinger_____
// 2022-01-08: new
// 2023-02-20: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Blind1 extends IBase {
  // ---------mandatory-----------------------------------------
  iBlind1State: number;
  type: string;
  // ---------optional------------------------------------------
  iMotorState?: number;
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiBlind1Controller extends CiBaseController {
  public blind1s: Array<Blind1> = reactive(
    [
      {
        id: 'blind_o',
        type: 'zb',
        iBlind1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/blind/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_c',
        type: 'zb',
        iBlind1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/blind/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_l',
        type: 'zb',
        iBlind1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/blind/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_bat10',
        type: 'zb',
        iBlind1State: -1,
        battery: '10',
        subTopic: 'z2m/t/blind/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_bat20',
        type: 'zb',
        iBlind1State: -1,
        battery: '20',
        subTopic: 'z2m/t/blind/-5',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'blindF',
        name: 'Blind1_Footer',
        type: 'motor',
        iBlind1State: -1,
        iMotorState: -5,
        textFooter: '--Footer1--12345678901234567890',
        subTopic: 'z2m/t/blind/-6  z2m/t/blind/-6/motor',
        pubTopic: '',
        pubPayload: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOpen = -1;
  public stateClose = -2;
  public stateLock = -3;
  public stateMotorDown = -4;
  public stateMotorStop = -5;
  public stateMotorUp = -6;
  public stateNotOk = -998;
  public stateNoState = -999;

  public onMessage (message: Message): void {
    this.blind1s.forEach(blind1 => {
      const aSubTopic = blind1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---blind1 topic found -------------------------------
        if (blind1.type === 'zb') {
        // ---blind type 1--------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            blind1.battery = `${aPayload.battery}`
            const blindstate_ = String(aPayload.contact)
            if (blindstate_ === 'true') blind1.iBlind1State = this.stateClose
            else {
              if (blindstate_ === 'false') blind1.iBlind1State = this.stateOpen
              else blind1.iBlind1State = this.stateNotOk
            }
          } catch (error) { }
        }
        // ---blind type 2--------------------------------------
        if (blind1.type === 'motor') {
          try {
            console.log('type=motor', message.payload)
            const aPayload = JSON.parse(message.payload)
            // ....for example: blind with open/close info......
            const blindstate_ = String(aPayload.contact)
            blind1.iBlind1State = this.stateNotOk
            if (blindstate_ === 'true') blind1.iBlind1State = this.stateClose
            if (blindstate_ === 'false') blind1.iBlind1State = this.stateOpen
            // ....for example: blind with motor movement info..
            const motor_ = aPayload.motor
            blind1.iMotorState = this.stateNotOk
            if (motor_ === -1) blind1.iMotorState = this.stateMotorDown
            if (motor_ === 0) blind1.iMotorState = this.stateMotorStop
            if (motor_ === 1) blind1.iMotorState = this.stateMotorUp
          } catch (error) { }
        }
        // ---blind type 3--------------------------------------
        if (blind1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different blind1 types-----------
        // ---END blind1 topic found ---------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiBlind1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiBlind1Controller: ERROR:', e) })
  }
}

export const ciBlind1Controller = new CiBlind1Controller()
```

[Top of page](#up)
