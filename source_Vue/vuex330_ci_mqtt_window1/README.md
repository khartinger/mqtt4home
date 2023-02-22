Last modified: 2023-02-20 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiWindow1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiWindow1` symbol.   
In general:   
* The `CiWindow1` symbol waits for MQTT messages to represent the state of the window (open, closed, locked).   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiWindow1Controller.ts` file.   

# Quick Reference
* Download directory `vuex330_ci_mqtt_CiWindow1`.   
* Start Visual Studio Code (VSC) with TypeScript support installed and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* The CiWindow1 symbol waits for MQTT messages containing information about the window state and possibly the battery state. For simulation, these values can also be entered with the program `mosquitto_pub` in a Windows console:   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/window/-1 -m "{\"contact\":false,\"battery\":90}"`   
  * `mosquitto_pub -h 10.1.1.1 -t z2m/t/window/-1 -m "{\"contact\":true,\"battery\":10}"`   
  * `mosquitto_pub -h 10.1.1.1 -t wifi/t/window/-5/ret/status -m 1`   

The values are displayed accordingly.   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiWindow1` representations. Which attributes cause which representation is described afterwards.     
![CiWindow1 variants](./images/vuex330_all_window1.png "CiWindow1 variants")   
_Fig. 1: Examples for different CiWindow1 representations_.   

## CiWindow1 call
The drawing of a CiWindow1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiWindow1>` and further attributes.   
_Example_:   
```   
<CiWindow1 :x="50" :y="70" sid="window_o" dir="h1"></CiWindow1>
```   

## Positioning of a symbol (:x, :y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiWindow1 to border the left edge and be 20 units away from the top edge:   
![location of a CiWindow1](./images/vuex330_window1_location1.png "location of a CiWindow1")   
_Fig. 2: Location of a CiWindow1 symbol_   

_Note_:   
Remaining space at the bottom: 220 - 20 - 100 = 100 units.   
Remaining space right: 500 - 100 = 400 units   

## ID of a symbol (sid)
The symbol ID (sid) represents the connection of the graphical representation to the controller (see property [id](#id) in chapter "[Attributes defined in the base controller](#id)").   
If `sid=""` is set, no texts and backgrounds will be displayed.   

## Border of a symbol (:border)
Normally CI symbols are 100x100 units in size outside and have a fixed border of 5 units inside. With the CiWall1 symbol this is different:   
In order to make adjacent symbols in a 100 unit grid look continuous, the `CiWindow1` symbols go to the symbol border, so they are exceptionally 100x100 units in size.   
Normally, the border of the `CiWindow1` symbols is not displayed. If you want to display the border exceptionally, there are four ways to do so, which are set via the `:border=` attribute:   

| Parameter | Meaning |   
| ------------- | ------------------------------ |   
| `:border="0"` | No border will be displayed. |   
| `:border="1"` | Only the outer border of the symbol is displayed (= total space required by the symbol). |   
| `:border="2"` | Only the inner border of the symbol is shown (= the drawing area for the symbol). |   
| `:border="3"` | The border of the symbol is displayed in yellow. |   

Default is `:border="0"`.   

## Direction of the CiWindow1 symbol (dir)
The direction of a symbol is specified with the `dir` ("Direction") property.   

The __first letter__ indicates the base direction:   
* `h` or `H`: Horizontal (horizontal, x-direction).   
* `v` or `V`: Vertical (vertical, y-direction)   
* `d` or `D`: Diagonal (inclined at +/- 45°)   

If a lowercase letter is used, the window opening direction is drawn small, if an uppercase letter is used, it is drawn large (see _Fig. 1_).   

The __second letter__ indicates the opening direction, which is the quadrant into which the window opens. According to the 4 quadrants, the numbers 1 to 4 are possible.   

![window_dir_axes](./images/vuex330_window1_dir_axes1.png "window_dir_axes")   
_Fig. 3: Direction labels for symbols in axis direction_   

For diagonal elements, a __third letter__ is required to indicate the `propagation` direction more precisely. The first letter is a `d` (for diagonal), the second letter indicates the diagonal direction for the symbol (`1` to `4`) and the third letter indicates the axis towards which an element opens (`x` or `y`).   

![window_dir_dia](./images/vuex330_window1_dir_dia1.png "window_dir_dia")   
_Fig. 4: Directional labels for symbols in diagonal direction_   

__Examples of designations:__   
`dir="h1" ....` Horizontal symbol whose propagation (window opening) is in the first quadrant (towards "up").   
`dir="v1" ....` Vertical symbol, whose propagation (window opening) is in the first quadrant (to "right").   
`dir="d1x" ...` Diagonal symbol to the right-up, whose propagation (window opening) is in the direction of the x-direction (to "down").   

## Text information in CiWindow1 symbol (lines)
Each `CiWindow1` symbol knows three lines for text output:   
* header   
* line 4   
* footer   

Which texts are actually displayed in the symbol depends on the value of the 'lines=' attribute:    
| parameter | meaning |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Neither header nor line 4 nor footer will be displayed. |   
| `lines="1"`   | The header line is displayed. If the `name` parameter is defined in the controller, this text is displayed, otherwise the `id`. |   
| `lines="2"`   | Header and footer are displayed.   |   

Default is `lines="2"`.   

### Header
1. If the attribute `lines="0"` is set in the symbol, the header will not be displayed.   
2. If the parameter `name` is defined in the controller, this text will be displayed.   
3. If the parameter `name` contains a question mark, it will be replaced by the text `OPEN`, `CLOSE` or `LOCK` depending on the window status.   
4. Otherwise the `id` is displayed.   

### Line 4
1. If the attribute `lines="0"` is set in the symbol, the line 4 will not be displayed.   
2. If the id of the symbol (`sid`) does not match any entry in the list `public window1s: Array<Window1>` (file `CiWindow1Controller.ts`), "`unknown`" is displayed.   
3. If there is a value for the MQTT key `battery`, this value is displayed.   
4. Otherwise, an empty line is output.   

### Footer
1. If the attribute `lines="0"` or `lines="1"` is set in the symbol, the footer will not be displayed.   
2. If the id of the symbol (`sid`) does not match any entry in the list `public window1s: Array<Window1>` (file `CiWindow1Controller.ts`), "`sid`" is displayed.   
3. If the `textF=` attribute is used in the symbol, this text will be displayed.   
4. If the `textFooter` parameter is defined in the controller, this text will be displayed.   
5. If the parameter `textFooter` contains a question mark, it will be replaced by the text `OPEN`, `CLOSE` or `LOCK` depending on the window status.   
6. Otherwise, an empty line is output.   

![miscellaneous footers](./images/vuex330_window1_footer1.png "miscellaneous footers")   
_Fig. 5: Miscellaneous footers_   

__Coding example__:   
```   
    <CiWindow1 :x="50"  :y="70" sid="windowX" dir="h1"></CiWindow1>
    <CiWindow1 :x="150" :y="70" sid="window_o" dir="h1"></CiWindow1>
    <CiWindow1 :x="350" :y="70" sid="window_o" dir="h1" lines="0"></CiWindow1>
    <CiWindow1 :x="50"  :y="170" sid="windowF" dir="h1"></CiWindow1>
    <CiWindow1 :x="150" :y="170" sid="windowF" dir="h1"></CiWindow1>
    <CiWindow1 :x="250" :y="170" sid="windowF" dir="h1"></CiWindow1>
    <CiWindow1 :x="350" :y="170" sid="windowF" dir="h1" textF="textF :)"></CiWindow1>
```   

## Stretching of a symbol (:f)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbol (on both sides) so that the placement point remains the same.   
Stretching is done by specifying the `f` parameter, as the following example shows:   
![Stretched CiWindow1](./images/vuex330_window1_stretch1.png "Stretched CiWindow1")   
_Fig. 6: Stretched CiWindow1 symbol_   

The symbol at the bottom left and far right are each stretched by a factor of 2.   

__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiWindow1 :x="100" :y="70"  sid="window_o" dir="h1"></CiWindow1>
  <CiWindow1 :x="100" :y="170" sid="window_o" dir="h1" :f="2"></CiWindow1>
  <CiWindow1 :x="300" :y="110" sid="window_o" dir="v2"></CiWindow1>
  <CiWindow1 :x="400" :y="110" sid="window_o" dir="v2" :f="2"></CiWindow1>
  </svg>
</template>
```   

## Different CiWindow1 shapes (shape)
CiWindow1 symbols come in four forms, selected by the `dir=` and `shape=` attributes:   
### `dir`
* A lowercase letter results in a small arc or rectangle ending at the bottom edge of line 1.   
* An uppercase letter results in an arc or rectangle corresponding to a quarter circle.   
### `shape`
* If you set `shape= "rect"`, a rectangle will be drawn instead of an arc.   
The default is `shape="arc"`.   

## Colors in CiWindow1 symbol (colorH, colorF, colorWall)
The colors in the CiWindow1 symbol can be partially selected or are assigned automatically.   

### Automatically set colors
* Incorrect "`sid`", i.e. an object with this "`sid`" does not exist in the controller:   
* The window area is colored light blue (color "`colorUnknown`" from the file `Geo.ts`) and lines 4 and 5 are colored light red (color "`colorNotOK`" from the file `Geo.ts`).   
* The window area is colored according to the window status (variable `iWindow1State`):   
  * Window open: light green (`colorOpen`)   
  * Window closed: light red (`colorClose`)   
  * Window locked: red (`colorLock`)   
* If a battery value is smaller than `batteryMin`, line 4 is colored light red (color "`colorNotOK`" from the file `Geo.ts`).   

### Selectable colors
* The `colorH=` attribute can be used to set the background color of the header. The value can be either an RGB value (red-green-blue value), such as `#ffee00` or a text specification such as `red`, `green` etc.   
* With the attribute `colorF=` the background color of the footer can be set (color value see above).   
* With the attribute `colorWall=` the color of the wall elements (rectangles left and right of the window area) can be set (color value see above).   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning                                  |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiWindow1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiWindow1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiWindow1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiWindow1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiWindow1Controller.ts`.
The following attributes are mandatory:   
| Property       | Meaning                              |   
| -------------- | ------------------------------------ |   
| `iWindow1State`  | Status of the CiWindow1. Use e.g. -99 as default for the start. |   
| `type`         | Sensor type that detects the window status and sends a corresponding MQTT message. |   

The following attributes are optional and can be used:   
| property | meaning |   
| ------------ | ------------------------------------ |   
| `battery` | Receive the state of charge if a CiWindow1 sensor is operated with battery. Can be displayed in line 4. |   
| `textHeader` | Text in the header of the symbol.    |   
| `textFooter` | Specify a fixed text in the footer.  |   

## Example: Coding of a `CiWindow1Controller.ts`.   
Normally, only the values within the `Array<Window1>` area need to be added.   
If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

In the example below, six window objects are defined:   
The first three objects `window_o`, `window_c` and `window_l` are used to represent open, closed and locked windows.   
The objects `window_bat10` and `window_bat20` are used to represent a battery value and `windowF` shows the display of a footer value.   

```
// ______CiWindow1Controller.ts____________________khartinger_____
// 2022-01-08: new
// 2023-02-18: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Window1 extends IBase {
  // ---------mandatory-----------------------------------------
  iWindow1State: number;
  type: string;
  // ---------optional------------------------------------------
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiWindow1Controller extends CiBaseController {
  public window1s: Array<Window1> = reactive(
    [
      {
        id: 'window_o',
        type: 'zb',
        iWindow1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/window/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_c',
        type: 'zb',
        iWindow1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/window/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_l',
        type: 'zb',
        iWindow1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/window/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_bat10',
        type: 'D1',
        iWindow1State: -1,
        battery: '10',
        subTopic: 'z2m/t/window/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_bat20',
        type: 'D1',
        iWindow1State: -1,
        battery: '20',
        subTopic: 'wifi/t/window/-5/ret/status wifi/t/window/-5/ret/voltage',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'windowF',
        name: 'Window1_2D1',
        type: 'D1',
        iWindow1State: -99,
        textFooter: '--Footer1--',
        subTopic: 'wifi/t/window/-6/ret/status wifi/t/window/-6/ret/voltage',
        pubTopic: '',
        pubPayload: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOpen = -1;
  public stateClose = -2;
  public stateLock = -3;
  public stateNotOk = -998;
  public stateNoState = -999;

  public onMessage (message: Message): void {
    this.window1s.forEach(window1 => {
      const aSubTopic = window1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---window1 topic found --------------------------------
        if (window1.type === 'zb') {
        // ---window type 1---------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            window1.battery = `${aPayload.battery}`
            const windowstate_ = String(aPayload.contact)
            if (windowstate_ === 'true') window1.iWindow1State = this.stateClose
            else {
              if (windowstate_ === 'false') window1.iWindow1State = this.stateOpen
              else window1.iWindow1State = this.stateNotOk
            }
          } catch (error) { }
        }
        if (window1.type === 'D1') {
        // ---window type 2---------------------------------------
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'on')) window1.iWindow1State = this.stateOpen
            else {
              if ((message.payload === '0') || (message.payload === 'off')) window1.iWindow1State = this.stateClose
              else {
                if (message.payload === '-1') window1.iWindow1State = this.stateLock
                else window1.iWindow1State = this.stateNotOk
              }
            }
          }
          if (message.topic.includes('voltage')) {
            window1.battery = `${message.payload}`
          }
        }
        // ---window type 3---------------------------------------
        if (window1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different window1 types------------
        // ---END window1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiWindow1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiWindow1Controller: ERROR:', e) })
  }
}

export const ciWindow1Controller = new CiWindow1Controller()
```

[Top of page](#up)
