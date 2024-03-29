Last modified: 2023-01-28 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiButton2 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiButton2` symbol.   
In general:   
* If you click on the upper half of the `CiButton2` symbol, a first MQTT message is sent out (e.g. "On").   
* If you click on the lower half of the `CiButton2` symbol, another MQTT message is sent out (e.g. "Off").   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiButton2Controller.ts` file.   

# Quick Reference
* Download directory `vuex120_ci_mqtt_button2`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* If you press the upper half of the button in the browser, one or more MQTT messages are sent, which can be seen in the console, e.g.   
`ci/lamp/1/set/lamp 1`   
`ci/lamp/2/set/lamp 1`   
* If you press the lower half of the button in the browser, one or more MQTT messages are sent, which can be seen in the console, e.g.   
`ci/lamp/1/set/lamp 0`   
`ci/lamp/2/set/lamp 0`   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiButton2` representations. Which attributes cause which representation is described afterwards.     
![Button variants](./images/vuex120_all_buttons2.png "Button variants")   
_Fig. 1: Examples for different CiButton2 representations_.   

## Button call
The drawing of a button is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiButton1>` and further attributes.   
_Example_:   
```   
<CiButton2 :x="50" :y="80" sid="button1" :border="3" lines="0" :fx="2" :fy="1.5" shape="round_1right_2left"></CiButton2>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 300 150">`   
then the location specification `:x="50" :y="60"` causes the button to border the left edge and be 10 units away from the top edge:   
![location of a button](./images/vuex120_button2_location1.png "location of a button")   
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

## Text information in CiButton2 symbol (lines)
Each `CiButton2` symbol knows four possible areas for text output:   
* header   
* footer   
* Upper Label   
* Lower Label   

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

The caption texts at the top and bottom of the button symbol are a special case. The display of these texts is independent of the value of the `lines` parameter. If values for the `textUpper` and `textUpper` parameters are defined in the `CiButton2Controller.ts` file, these texts are displayed in any case.   

## Stretching of a symbol (fx, fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched Button](./images/vuex120_button2_stretch1.png "Stretched Button")   
_Fig. 3: Stretched button_   
The button is stretched by a factor of 2 in the x-direction (results in 2*100 = 200 units) and by 1.4 in the y-direction (results in 140 units).   
__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiButton2 :x="50" :y="60" sid="button1" :fx="2" :fy="1.4"></CiButton2>
</svg>
</template>
```   

## Different CiButton2 shapes (shape)
CiButton2 symbols come in four shapes, which are selected by the `shape=` attribute:   
| value | meaning |   
| ----------- | ---------------------------------------- |   
| `"circle"` | Circular button which will be adjusted to the smaller side length of the drawing area. |   
| `"round"` | Oval button that stretches the circle to fill the drawing area.   |   
| `"rect"` | Rectangular button with rounded corners (1/10 of a circle radius) |    
| `"square"` | Square button with rounded corners, stretched to fit the smaller side length of the drawing area.    |   

Default is `shape="circle"`.   

## Additional symbols inside the button
The symbol `CiButton2` can contain an equilateral triangle in the upper and lower area each with the tip pointing up, down, left or right. This is achieved by specifying the following for the `shape` attribute:   
* Upper symbol: "1up", "1down", "1left" or "1right".   
* Lower symbol: "2up", "2down", "2left" or "2right".   
An underscore should be used to separate the values.   
_Examples_:   
* `shape="1up_2down"` creates a circular button (= default) with an "arrow" (triangle) up in the upper area and an "arrow" down in the lower area.   
* `shape="square_1left_2right"` creates a square button with an "arrow" to the left in the upper area and an "arrow" to the right in the lower area.   
![Additional symbols](./images/vuex120_button2_additional1.png "Additional symbols")   
_Fig. 4: Additional icons in the button_   

## Color of the button (colorup, colordn)
With the help of the attributes `colorup=` and `colordn=` the color of the upper and lower `CiButton2` symbol can be selected. The value can either be an RGB value (red-green-blue value) like `#ffee00` or a text value like `red`, `green` etc..   
If no value is given, the color is light blue (according to the constant `colorUnknown` from the file `Geo.ts`).   
The `CiButton2` colors can also be changed over received MQTT messages, which can be controlled over the variable `iButtonState`. For this the coding of the methods `onMessage` (in the file `CiButton1Controller.ts`) and `colorUpper` or `colorLower` (in the file `CiButton2.vue`) must be adapted.   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property | Meaning |   
| ------------ | ---------------------------------------- |   
| `id` | Should normally be a unique name of a button.    |   
| `subTopic` | Messages or MQTT topics the button "listens" to (can also be empty ''). |   
| `pubTopic` | MQTT topics that the button sends when clicked. |   

The following attributes are optional:   
| property | meaning |   
| ------------ | ------------------------------------ |   
| `name` | Name of a button. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload sent when the upper half of the button is pressed. |   


## Attributes defined in `CiButton2Controller.ts`.
The following attributes are mandatory:   
| Property | Meaning |   
| -------------- | ------------------------------------ |   
| `iButtonState` | Status of the button. Use e.g. -99 as default for the start. |   

The following attributes are optional and can be used:   
| property | meaning |   
| ------------ | ------------------------------------ |   
| `battery` | Receive the state of charge if a button is operated with battery. Can be displayed in the footer. |   
| `textUpper` | Text in the upper half of the symbol, e.g. "up", "down", "left", "right" etc. |   
| `textLower` | Text in the lower half of the symbol, e.g. "up", "down", "left", "right" etc. |   
| `textFooter` | Specify a fixed text in the footer. |   
| `pubPayloadLower` | Payload that will be sent when the lower half of the button is pressed. |  

## Example: Coding of the `CiButton2Controller.ts`.   
Normally only the values inside the `Array<Button2>` area need to be added.   
If you want to implement additional MQTT functionality when receiving messages, the code in the method `public onMessage (message: Message): void` can be extended accordingly.   

As an example, the code below contains a response to the topic `ci/lamp/2/set/lamp`. If an MQTT server is running on a computer or Raspberry Pi with the IP 10.1.1.1, then pressing
* of the upper half of a button with the id `button1` causes a sending of messages with the topic `ci/lamp/1/set/lamp` as well as `ci/lamp/2/set/lamp` and the payload `1`. The second message is received from buttons with the ID `button2` and the upper half of the button is colored "yellow" and the lower half "gray". 
* the lower half of a button with the id `button1` a sending a message with the topic `ci/lamp/1/set/lamp` as well as `ci/lamp/2/set/lamp` and the payload `0`. The second message is received by buttons with the ID `button2` and the upper half of the button is colored "gray" and the lower half "yellow".   

```
// ______CiButton2Controller.ts__________________khartinger_____
// 2023-01-28: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button2 extends IBase {
  iButtonState: number;
  battery?: string;
  textUpper?: string;
  textLower?: string;
  textFooter?: string;
  pubPayloadLower?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiButton2Controller extends CiBaseController {
  public buttons2: Array<Button2> = reactive(
    [
      {
        id: 'button1',
        name: 'Button2_1',
        iButtonState: -99,
        // textUpper: 'ON',
        // textLower: 'OFF',
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1',
        pubPayloadLower: '0'
      },
      {
        id: 'button2',
        name: 'Button2_2',
        iButtonState: -99,
        textUpper: 'ON',
        textLower: 'OFF',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: 'ON',
        pubPayloadLower: 'OFF'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons2.forEach(button2 => {
      const aSubTopic = button2.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button2.iButtonState = -2
          if (message.payload === '1') button2.iButtonState = -1
        }
        if (message.topic.includes('voltage')) {
          button2.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton2Controller = new CiButton2Controller()
```

[Top of page](#up)
