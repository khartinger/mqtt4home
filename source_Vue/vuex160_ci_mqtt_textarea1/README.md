Last modified: 2023-02-24 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiTextarea1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiTextarea1` symbol.   
In general:   
* When the `CiTextarea1` symbol is clicked, an MQTT message is sent out.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiTextarea1Controller.ts` file.   

# Quick Reference
* Download directory `vue150_ci_mqtt_CiTextarea1`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* If you press a CiTextarea1 symbol in the browser, one or more MQTT messages are sent, which can be seen in the console, e.g.   
`ci/textarea1/1/get standard text published by textarea1 :)`   
`ci/textarea1/2/set standard text published by textarea1 :)`   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a ci symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiTextarea1` representations. Which attributes cause which representation is described afterwards.     
![CiTextarea1 variants](./images/vuex160_all_textarea1_1.png "CiTextarea1 variants")   
![CiTextarea1 variants](./images/vuex160_all_textarea1_2.png "CiTextarea1 variants")   
_Fig. 1: Examples for different CiTextarea1 representations_.   

## CiTextarea1 call
The drawing of a CiTextarea1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiTextarea1>` and further attributes.   
_Example_:   
```   
<CiTextarea1 :x="50" :y="80" sid="textarea1" :border="3" lines="0" :fx="2" :fy="1.5" color="white"></CiTextarea1>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiTextarea1 to border the left edge and be 20 units away from the top edge:   
![location of a CiTextarea1](./images/vuex160_textarea1_location1.png "location of a CiTextarea1")   
_Fig. 2: Location of a CiTextarea1 symbol_   

_Note_:   
Remaining space at the bottom: 220 - 20 - 100 = 100 units.   
Remaining space right: 500 - 100 = 400 units   

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

Default is `:border="2"`.   

## Text information in CiTextarea1 symbol (lines)
Each `CiTextarea1` symbol knows three possible areas for text output:   
* header   
* footer   
* drawing area (= text area)   

Which texts of the header and footer actually are displayed in the symbol depends on the value of the 'lines=' attribute:    
| parameter | meaning |   
| ------------- | ------------------------------ |   
| `lines="0"`   | Neither header nor footer will be displayed. |   
| `lines="1"`   | The header line is displayed. If the `name` parameter is defined in the controller, this text is displayed, otherwise the `id`. |   
| `lines="2"`   | Header and footer are displayed.<br>Header: If the parameter `name` is defined in the controller, this text is displayed, otherwise the `id`.<br>Footer: If the `textFooter` parameter is defined in the controller, this text is displayed, otherwise the `id`. |  

Default is `lines="1"`.   

Depending on the number of displayed lines, the size of the drawing area for the CI symbol changes. For the standard symbol with the total size 100x100 applies:   
* `lines="0"`: character area 90x90 units   
* `lines="1"`: drawing area 90x72 units   
* `lines="2"`: drawing area 90x54 units   

## Stretching of a symbol (fx, fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched CiTextarea1](./images/vuex160_CiTextarea1_stretch1.png "Stretched CiTextarea1")   
_Fig. 3: Stretched CiTextarea1 symbol_   

The left two CiTextarea1 symbols are stretched by the factor fx=2 (results in 2*100 = 200 units), the third symbol is stretched by the factor fy=2 and the right symbol is stretched by fx=2 and fy=2, i.e. it is twice as large as the original with a size of 200x200.   

__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiTextarea1 :x="50" :y="70"  sid="textarea1" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="50" :y="170" sid="textarea2" :fx="2" :fy="1"></CiTextarea1>
  <CiTextarea1 :x="250" :y="70" sid="textarea1" :fx="1" :fy="2"></CiTextarea1>
  <CiTextarea1 :x="350" :y="70" sid="textarea2" :fx="2" :fy="2"></CiTextarea1>
  </svg>
</template>
```   

## Color of the CiTextarea1 symbol  (color, colorH, colorF)
With the help of the attribute `color=` the background color of a CiTextarea1 symbol can be chosen. The value can either be an RGB value (red-green-blue value) like `#eedd00` or a text value like `red`, `white` etc..   
If no value is specified, the symbol is transparent, i.e. it has the background color.   
Furthermore, the `colorH=` attribute can be used to specify the background color of the header and `colorF=` to specify the background color of the footer.

## Text size (:textsize)
The default font size is such that there is room for five lines in the 100x100 standard symbol [line height = (100 - 2x5 margin) / 5 = 18 units). The `textsize` parameter can be used to change the font size:   
| parameter | meaning |   
| ------------------- | ---------------------------------------- |   
| `:textsize="1.5"` | font size is 1.5 lines high. This results e.g. in two lines in the character area with `lines="2"` |   
| `:textsize="2"` | font height is 2 lines high. This results e.g. in two lines with `lines="1"` |   
| `:textsize="3"` | font height is 3 lines high. This results in e.g. one line with `lines="2"` |   

![font sizes](./images/vuex160_textarea1_textsize1.png "font-sizes")   
_Fig. 4: Different font sizes in the character area_


<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning                                  |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiTextarea1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiTextarea1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiTextarea1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiTextarea1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiTextarea1Controller.ts`.
The following attributes are mandatory:
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `iTextarea1State` | State of the CiTextarea1 symbol. Specifies what should be displayed in the header: 0=No title, 1=Name, 2=Id, 4=First line of the message.<br>If the value is positive, this will be used as the color for the background.<br>If you don't need any of these options, you should set the value to e.g. -99. |   
| `subPayload`     | String variable for the content of the text area. |   
| `clkWhenMounted` | `true` means that a click on the symbol is simulated when the symbol is mounted. This e.g. sends out an MQTT message that can request a text etc.. |   

The following attributes are optional and can be used if required:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `useRawText` | If this value is set to `true`, the contents of the variable `subPayload` will not be changed when displayed. <br>Otherwise words with two, three or four letters are not separated if possible and spaces at the beginning and end of a line are removed. |   
| `textFooter` | Specification of a fixed text in the footer line. |   

## Example: Coding of the `CiTextarea1Controller.ts`.   
Normally, only the values within the `Array<CiTextarea1>` area need to be added.   
In the example below four CiTextarea1 symbols are defined, where the symbols `textarea3` and `textarea4` differ only in how the text should be displayed (with or without text adjustments according to the value of the `useRawText` attribute).

If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

As an example, the code below contains a special behavior of symbols with id `textarea1`:   
* On receiving a topic `ci/textarea1/2/get` the symbol sends out a message with the topic `ci/textarea1/2/set` and the payload `special: requested text from id=textarea1`.   

In all other cases the received payload is displayed according to the settings in the symbol.

```
// ______CiTextarea1Controller.ts________________khartinger_____
// 2022-08-14: New
// 2023-02-05: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Textarea1 extends IBase {
  iTextarea1State: number; // 0=no title, 1=name, 2=id, 4=message
  subPayload: string; //      text displayed in textarea
  clkWhenMounted: boolean; // text request on mounting?
  useRawText?: boolean; //    do not wrap text
  textFooter?: string; //     footer text
}

export class CiTextarea1Controller extends CiBaseController {
  public textarea1s: Array<Textarea1> = reactive(
    [
      {
        id: 'textarea1',
        name: 'Textarea1_1',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/1/set ci/textarea1/2/get',
        subPayload: 'click: publish a text by textarea1 - this is just to make the text longer and longer 1234567890 and once more longer...',
        pubTopic: 'ci/textarea1/1/get ci/textarea1/2/set',
        pubPayload: 'standard text published by textarea1 :)',
        textFooter: '==Footer=='
      },
      {
        id: 'textarea2',
        name: 'Textarea1_2',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: true,
        subTopic: 'ci/textarea1/2/set',
        subPayload: 'click sends request ci/textarea1/2/get',
        pubTopic: 'ci/textarea1/2/get',
        pubPayload: 'text'
      },
      {
        id: 'textarea3',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      },
      {
        id: 'textarea4',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: true,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      }

    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for topic textarea1------------------------
    this.textarea1s.forEach(textarea1 => {
      const aSubTopic = textarea1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea1 topic found ----------------------------
        if (textarea1.id === 'textarea1') {
          // -message for id=textarea1--------------------------
          if (message.topic === 'ci/textarea1/2/get') {
            this.publishCi('ci/textarea1/2/set', 'special: requested text from id=textarea1')
          } else textarea1.subPayload = message.payload
        } else {
        // ---message is not for id=textarea1-------------------
          textarea1.subPayload = message.payload
        }
      } // ---END textarea1 topic found-------------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTextarea1Controller: ERROR:', e) })
  }
}

export const ciTextarea1Controller = new CiTextarea1Controller()
```

[Top of page](#up)
