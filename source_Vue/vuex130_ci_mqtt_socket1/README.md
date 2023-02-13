Last modified: 2023-01-29 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiSocket1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiSocket1` symbol.   
In general:   
* When the `CiSocket1` symbol is clicked, an MQTT message is sent out.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiSocket1Controller.ts` file.   

# Quick Reference
* Download directory `vuex130_ci_mqtt_socket1`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* If you press the symbol in the browser, a MQTT message is sent, which can be seen in the console, e.g.   
`ci/socket/1/set {"state": "TOGGLE"}`   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a CI-symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiSocket1` representations. Which attributes cause which representation is described afterwards.     
![Button variants](./images/vuex130_all_socket1.png "Button variants")   
_Fig. 1: Examples for different socket1 representations_.   

## Socket1 call
The drawing of a socket1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiSocket1>` and further attributes.   
_Example_:   
```   
<CiSocket1 :x="50" :y="80" sid="socket1" :border="3" lines="0" :fx="2" :fy="1.5"></CiSocket1>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 300 150">`   
then the location specification `:x="50" :y="70"` causes the socket1 symbol to border the left edge and be 20 units away from the top edge:   
![location of a socket1 symbol](./images/vuex130_socket_location1.png "location of a socket1 symbol")   
_Fig. 2: Location of a socket1 symbol_   

_Note_:   
Remaining space at the bottom: 150 - 20 - 100 = 30 units.   
Remaining space right: 300 - 100 = 200 units   

## ID of a symbol (sid)
The symbol ID (sid) represents the connection of the graphical representation to the controller (see property [id](#id) in chapter "[Attributes defined in the base controller](#id)").

## Border of a CI-symbol (border)
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

## Text information in CiSocket1 symbol (lines)
Each `CiSocket1` symbol knows three possible areas for text output:   
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

The text in the center of the socket1 symbol is a special case. The display of this text is independent of the value of the `lines` parameter. If the `textCenter` parameter is defined, this text will be displayed in any case slightly above the center of the symbol.   

## Stretching of a symbol (fx, fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched socket1 symbol](./images/vuex130_socket1_stretch1.png "Stretched socket1 symbol")   
_Fig. 3: Stretched socket1 symbol_   
The left symbol is stretched by a factor of 2 in the x-direction (results in 2*100 = 200 units) and by 1.3 in the y-direction (results in 130 units).   
__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 300 150">
  <rect class="ciBackground" x="0" y="0" width="300" height="150" />
  <CiSocket1 :x="50" :y="70" sid="socket1" :fx="2" :fy="1.3"></CiSocket1>
</svg>
</template>
```   

## Color of the socket1 symbol (color)
The `color=` attribute can be used to select the color of a CiSocket1 symbol. The value can be either an RGB value (red-green-blue value), such as `#ffee00` or a text specification such as `red`, `green` etc..   
If no value is given, the color is light blue (according to the constant `colorUnknown` from the file `Geo.ts`).   
The CiSocket1 color can also be changed via received MQTT messages, which can be controlled by the variable `iButtonState`. For this purpose the coding of the methods `onMessage` (in the file `CiSocket1Controller.ts`) and `colorButton` (in the file `CiSocket1.vue`) must be adapted.   

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a socket1 symbol.    |   
| `subTopic` | Messages or MQTT topics the socket1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the socket1 symbol sends when clicked. |   

The following attributes are optional:   
| Property     | Meaning |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a socket1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   

## Attributes defined in `CiSocket1Controller.ts`.
The following attributes are mandatory:   
| Property        | Meaning |   
| --------------- | ------------------------------------ |   
| `iSocket1State` | Status of the socket1 symbol. Use e.g. -99 as default for the start. |   
| `type`          | Type of the socket2 element. Depending on the hardware type, received MQTT messages can be responded to accordingly. |   

The following attributes are optional and can be used:   
| Property     | Meaning |   
| ------------ | ------------------------------------ |   
| `battery`    | Receive the state of charge if a socket1 symbol is operated with battery. Can be displayed in the footer. |   
| `textCenter` | Text in the center of the symbol. |   
| `textFooter` | Specify a fixed text in the footer. |   

## Example: Coding of the `CiSocket1Controller.ts`.   
Normally, only the values within the `Array<Socket1>` area need to be added.   
If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

As an example, the code below contains a response to responses from a (Zigbee) wireless socket of type e1603 (IKEA). The payload is JSON formatted and the key `state` contains the switching state of the socket ("ON" or "OFF"). This information sets the value of `iSocketState` to -1 or -2 and the socket1 symbol is colored accordingly (see method `colorSocket1` in the file CiSocket1.vue).   

```
// ______CiSocket1Controller.ts__________________khartinger_____
// 2022-09-14: new
// 2023-01-29: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Socket1 extends IBase {
  iSocket1State: number;
  type: string;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiSocket1Controller extends CiBaseController {
  public sockets1: Array<Socket1> = reactive(
    [
      {
        id: 'socket1',
        name: 'Socket1_1',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textFooter: '--Footer1--',
        subTopic: 'ci/socket/1',
        pubTopic: 'ci/socket/1/set',
        pubPayload: '{"state": "TOGGLE"}'
      },
      {
        id: 'socket2',
        name: 'Socket1_2',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/socket/2',
        pubTopic: 'ci/socket/2/set',
        pubPayload: '{"state": "TOGGLE"}'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.sockets1.forEach(socket1 => {
      const aSubTopic = socket1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---socket found -------------------------------------
        // console.log('CiSocket1Controller.onMessage payload=', message.payload)
        if (socket1.type === 'e1603') {
          const aPayload = JSON.parse(message.payload)
          if ((aPayload.state === 'ON') || (aPayload.state === 'on')) socket1.iSocket1State = -1
          else {
            if ((aPayload.state === 'OFF') || (aPayload.state === 'off')) socket1.iSocket1State = -2
            else { socket1.iSocket1State = -9 }
          }
        }
        // ---end socket found----------------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSocket1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSocketController: ERROR:', e) })
  }
}

export const ciSocket1Controller = new CiSocket1Controller()
```

[Top of page](#up)
