Last modified: 2023-01-27 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiButton1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiButton1` symbol.   
In general:   
* When the `CiButton1` symbol is clicked, an MQTT message is sent out.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiButton1Controller.ts` file.   

# Quick Reference
* Download directory `vuex110_ci_mqtt_button1`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* If you press a button in the browser, one or more MQTT messages are sent, which can be seen in the console, e.g.   
`ci/lamp/1/set/lamp 0`   
`ci/lamp/2/set/lamp 0`   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiButton1` representations. Which attributes cause which representation is described afterwards.     
![Button variants](./images/vuex110_all_buttons1.png "Button variants")   
_Fig. 1: Examples for different button representations_.   

## Button call
The drawing of a button is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiButton1>` and further attributes.   
_Example_:   
```   
<CiButton1 :x="50" :y="80" sid="button1" :border="3" lines="0" :fx="2" :fy="1.5" shape="round_right"></CiButton1>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 300 150">`   
then the location specification `:x="50" :y="60"` causes the button to border the left edge and be 10 units away from the top edge:   
![location of a button](./images/vuex110_button_location1.png "location of a button")   
_Fig. 2: Location of a button_   

_Note_:   
Remaining space at the bottom: 150 - 10 - 100 = 40 units.   
Remaining space right: 300 - 100 = 200 units   

## ID of a symbol (sid)
The symbol ID (sid) represents the connection of the graphical representation to the controller (see property [id](#id) in chapter "[Attributes defined in the base controller](#id)").

## Border of a symbol (border)
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

## Text information in CiButton1 symbol (lines)
Each `CiButton1` symbol knows three possible areas for text output:   
* header   
* footer   
* Middle line   

Which texts are actually displayed in the symbol depends on the value of the 'lines=' attribute:    
| parameter | meaning |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Neither header nor footer will be displayed. |   
| `lines="1"`   | The header line is displayed. If the `name` parameter is defined in the controller, this text is displayed, otherwise the `id`. |   
| `lines="2"`   | Header and footer are displayed.<br>Header: If the parameter `name` is defined in the controller, this text is displayed, otherwise the `id`.<br>Footer: If the `textFooter` parameter is defined in the controller, this text is displayed, otherwise the value of `battery` or, if `battery` is not defined, the `id`. |  

Default is `lines="1"`.   

Depending on the number of displayed lines, the size of the drawing area for the CI symbol changes. For the standard symbol with the total size 100x100 applies:   
* `lines="0"`: character area 90x90 units   
* `lines="1"`: drawing area 90x72 units   
* `lines="2"`: drawing area 90x54 units   

The text in the center of the button symbol is a special case. The display of this text is independent of the value of the `lines` parameter. If the `textCenter` parameter is defined, this text will be displayed in the center of the symbol in any case.   

## Stretching of a symbol (fx, fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched Button](./images/vuex110_button_stretch1.png "Stretched Button")   
_Fig. 3: Stretched button_   
The button is stretched by a factor of 2 in the x-direction (results in 2*100 = 200 units) and by 1.4 in the y-direction (results in 140 units).   
__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiButton1 :x="50" :y="60" sid="button1" :fx="2" :fy="1.4"></CiButton1>
</svg>
</template>
```   

## Different CiButton1 shapes (shape)
CiButton1 symbols come in four shapes, which are selected by the `shape=` attribute:   
| value | meaning |   
| ----------- | ---------------------------------------- |   
| `"circle"` | Circular button which will be adjusted to the smaller side length of the drawing area. |   
| `"round"` | Oval button that stretches the circle to fill the drawing area.   |   
| `"rect"` | Rectangular button with rounded corners (1/10 of a circle radius) |    
| `"square"` | Square button with rounded corners, stretched to fit the smaller side length of the drawing area.    |   

Default is `shape="circle"`.   

## Additional symbols inside the button
CiButtons can contain an equilateral triangle inside with its tip pointing up, down, left or right. This is achieved by specifying "up", "down", "left" or "right" in the `shape` attribute. When combined with a button shape specification, an underscore should be used.   
_Examples_:   
* `shape="up"` creates a circular button (= default) with "arrow" (triangle) pointing up.   
* `shape="square_down"` creates a square button with "arrow" (triangle) down.   

## Color of the button (color)
The `color=` attribute can be used to select the color of a CiButton1. The value can be either an RGB value (red-green-blue value), such as `#ffee00` or a text specification such as `red`, `green` etc..   
If no value is given, the color is light blue (according to the constant `colorUnknown` from the file `Geo.ts`).   
The CiButton1 color can also be changed via received MQTT messages, which can be controlled by the variable `iButtonState`. For this purpose the coding of the methods `onMessage` (in the file `CiButton1Controller.ts`) and `colorButton` (in the file `CiButton1.vue`) must be adapted.   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a button.    |   
| `subTopic`   | Messages or MQTT topics the button "listens" to (can also be empty '').  |   
| `pubTopic`   | MQTT topics that the button sends when clicked. |   

The following attributes are optional:   
| property     | meaning |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a button. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiButton1Controller.ts`.
The following attributes are mandatory:   
| Property       | Meaning |   
| -------------- | ------------------------------------ |   
| `iButtonState` | Status of the button. Use e.g. -99 as default for the start. |   

The following attributes are optional and can be used:   
| property     | meaning |   
| ------------ | ------------------------------------ |   
| `battery`    | Receive the state of charge if a pushbutton is operated with battery. Can be displayed in the footer. |   
| `textCenter` | Text in the center of the symbol, e.g. "up", "down", "left", "right" etc. |   
| `textFooter` | Specify a fixed text in the footer. |   

## Example: Coding of the `CiButton1Controller.ts`.   
Normally, only the values within the `Array<Button1>` area need to be added.   
If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   
As an example, the code below contains a response to the topic `ci/lamp/2/set/lamp`. If a MQTT server is running on a computer or Raspberry Pi with the IP 10.1.1.1, pressing a button on the bottom right (at "Centertext") causes, among other things, the sending of a message with the topic `ci/lamp/2/set/lamp` and the payload `1`. This is received by buttons with the ID `button2` and the button is colored "yellow" ("on"). 
Pressing the other buttons with ID `button1` send out a message with the topic `ci/lamp/2/set/lamp` and the payload `0`, which leads to coloring of the lower buttons to "gray" ("off").   

```
// ______CiButtonController.ts___________________khartinger_____
// 2022-09-14: add .catch((e) ..) to this.publish
// 2023-01-25: add textCenter, textFooter | remove shape, color
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button1 extends IBase {
  iButtonState: number;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiButton1Controller extends CiBaseController {
  public buttons1: Array<Button1> = reactive(
    [
      {
        id: 'button1',
        name: 'Button_1',
        iButtonState: -99,
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'button2',
        name: 'Button_2',
        iButtonState: -99,
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons1.forEach(button1 => {
      const aSubTopic = button1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (button1.id.substring(0, 7) === 'lamp1') {
          const aPayload = JSON.parse(message.payload)
          button1.iButtonState = 10 + aPayload.motor
        }
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button1.iButtonState = -2
          if (message.payload === '1') button1.iButtonState = -1
        }
        if (message.topic.includes('voltage')) {
          button1.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton1Controller = new CiButton1Controller()
```

[Top of page](#up)
