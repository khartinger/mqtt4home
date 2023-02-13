Last modified: 2023-02-09 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>Vue: Using the CiWeather1 symbol</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

# Target
Getting to know all the ways to display and use a `CiWeather1` symbol.   
In general:   
* The `CiWeather1` icon waits for MQTT messages and displays certain values.   
* All properties concerning the graphical representation of the symbol are set in the `<template>` area of the representation file (e.g. in `CiMain.vue`).   
* All non-graphical properties, such as ID, name, labels and MQTT functionality, are defined in the `CiWeather1Controller.ts` file.   

# Quick Reference
* Download directory `vue150_ci_mqtt_CiWeather1`.   
* Start Visual Studio Code (VSC) and open the folder of this Vue application.   
* In VSC open the terminal and type the following:   
`npm run serve`   
* Start the browser (e.g. Google Chrome) and call the page `localhost:8080`.   

In the browser the symbols shown in _Fig. 1_ (see below) are displayed.   

If you want to test the MQTT functionality, you have to run an MQTT broker (e.g. Mosquitto) on a computer (or Raspberry Pi) with the IP address `10.1.1.1`.   
* Connect the development computer to the network of the broker server. (For WLAN e.g. SSID `Raspi11`, password `12345678`)   
* Start Windows console (`cmd.exe`), change to the mosquitto directory and enter the following:   
`mosquitto_sub -h 10.1.1.1 -t "#" -v`   
* The CiWeather1 symbol waits for MQTT messages of the following type, which are e.g. sent by Zigbee sensors or - for simulation - can be entered with the program `mosquitto_pub` in a Windows console:   
`mosquitto_pub -h 10.1.1.1 -t z2m/t/wetter/-1 -m "{\"battery\":100,\"humidity\":36,\"temperature\":16.0}"`   
`mosquitto_pub -h 10.1.1.1 -t z2m/t/wetter/-1/time -m "20230208 125558"`   
The values are displayed accordingly.   

Note: A detailed description of individual steps can be found in other chapters of the MQTT4HOME project.

## Graphic representation of a ci symbol
## General
The coding of the graphical representation of a CI symbol `CiXxx` is done in a file with a name like `CiXxx.vue`. These files are then used by the actual project file (e.g. `CiMain.vue`) to display the web page.   
The following image shows the output of this Vue application with the different `CiWeather1` representations. Which attributes cause which representation is described afterwards.     
![CiWeather1 variants](./images/vuex170_all_weather1.png "CiWeather1 variants")   
_Fig. 1: Examples for different CiWeather1 representations_.   

## CiWeather1 call
The drawing of a CiWeather1 symbol is done in the `<template>` area of the presentation file (e.g. in `CiMain.vue`) by the tag `<CiWeather1>` and further attributes.   
_Example_:   
```   
<CiWeather1 :x="50" :y="80" sid="weather1" :border="3" :fx="2" :fy="1.5" color="white"></CiWeather1>
```   

## Positioning of a symbol (x, y)
Normally CI symbols are 100x100 units in size and the center point (50/50) is used to indicate the position (placement point). If the drawing area is defined e.g. by   
`<svg width="100%" viewBox="0 0 500 220">`   
then the location specification `:x="50" :y="70"` causes the CiWeather1 to border the left edge and be 20 units away from the top edge:   
![location of a CiWeather1](./images/vuex170_weather1_location1.png "location of a CiWeather1")   
_Fig. 2: Location of a CiWeather1 symbol_   

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

## Text information in CiWeather1 symbol
Each `CiWeather1` symbol knows three possible areas for text output:   
* header   
* footer   
* drawing area (= text area)   

### Header
If the parameter `name` is defined in the controller, this text is displayed, otherwise the `id`.

### Footer
1. if the `textFooter` parameter is defined in the controller, this text is displayed.   
2. if the received MQTT message contains an indication of the battery state of charge ("battery") and the value is assigned to the attribute in the `onMessage()` method (file `CiWeather12Controller.ts`), the current time (HH:MM) and this value are displayed.   
3. if there is only one value for the time, this value will be displayed (HH:MM:SS).   
4. if none of the points 1 to 3 is fulfilled, the `id` is displayed.

### Character area
The character area contains three lines (lines 2 to 4) that indicate the following:   
#### Line 2   
1. if the id of the symbol (`sid`) does not match any entry in the list `public weather1s: Array<Weather1>` (file `CiWeather12Controller.ts`), "`ERROR`" is displayed.   
2. if there is a temperature value, it will be displayed.   
3. otherwise nothing is displayed (empty line).   
#### Line 3   
1. if the id of the symbol (`sid`) does not match any entry in the list `public weather1s: Array<Weather1>` (file `CiWeather12Controller.ts`), "`unknown`" is displayed.   
2. if there is a humidity value, it will be displayed.   
3. otherwise nothing is displayed (empty line).   
#### Line 4   
1. if the id of the symbol (`sid`) does not match any entry in the list `public weather1s: Array<Weather1>` (file `CiWeather12Controller.ts`), "`sid`" is displayed.   
2. if there is a pressure value, it will be displayed.   
3. if there is a brightness value, it will be displayed.   
4. if there is a value for the date, it will be displayed (DD.MM.YY).   
5. Otherwise nothing is displayed (empty line).   

## Stretching of a symbol (fx, fy)
By default, all symbols are 100x100 units in size. However, it is possible to stretch the symbols in x and in y direction. The placement point remains the same.   
Stretching is done by specifying the parameters `fx` and `fy`, as the following example shows:   
![Stretched CiWeather1](./images/vuex170_weather1_stretch1.png "Stretched CiWeather1")   
_Fig. 3: Stretched CiWeather1 symbol_   

The left two CiWeather1 symbols are stretched by the factor fx=2 (results in 2*100 = 200 units), the third symbol is stretched by the factor fy=2 and the right symbol is stretched by fx=2 and fy=2, i.e. it is twice as large as the original with a size of 200x200.   

__Coding example__:   
```   
<template>
  <svg width="100%" viewBox="0 0 500 220">
  <rect class="ciBackground" x="0" y="0" width="500" height="220" />
  <text x="0" y="15" class="ciFont1">viewbox 500x220</text>
  <CiWeather1 :x="50" :y="70"  sid="weather1" :fx="2" :fy="1"></CiWeather1>
  <CiWeather1 :x="50" :y="170" sid="weather3" :fx="2" :fy="1"></CiWeather1>
  <CiWeather1 :x="250" :y="70" sid="weather1" :fx="1" :fy="2"></CiWeather1>
  <CiWeather1 :x="350" :y="70" sid="weather1" :fx="2" :fy="2"></CiWeather1>
  </svg>
</template>
```   

## Color of the CiWeather1 symbol (color, colorH, colorF)
### Color of the drawing area
With the help of the attribute `color=` the background color of a CiWeather1 symbol can be chosen. The value can either be an RGB value (red-green-blue value) like `#eedd00` or a text value like `red`, `white` etc..   
* If an error occurs, the background color is set to light red. (Color "`colorNotOK`" from the file `Geo.ts`).   
* Without error - and if a color value was specified - this color is used.   
* Otherwise the background is transparent.   

### Header color   
The background of the header is either specified with `colorH=` or it is transparent.   

### Footer color   
* In case of an error, the background color is set to light red. (color "`colorNotOK`" from the `Geo.ts` file).   
* If a "`battery`" value is present, this value is compared with the `batteryMin` value (=15). If the "`battery`"-value is greater, the footer will be colored green (`colorOk`) otherwise light red (`colorNotOk`). If an error occurs, the color will be middle blue (`colorUnknown`).   
* If a color value is specified, this color is used.   
* Otherwise the background is transparent.

<a name="id"></a>   
# Non-graphical attributes
## Attributes defined in the base controller.
Attributes of CI symbols that have nothing to do with the graphical representation of the symbol are defined in "`xxxController.ts`" files.   
The following attributes are defined in the base controller (`CiBaseController.ts` file) and must be mandatory in all derived controllers:   
| Property     | Meaning                                  |   
| ------------ | ---------------------------------------- |   
| `id`         | Should normally be a unique name of a CiWeather1 symbol.    |   
| `subTopic`   | Messages or MQTT topics the CiWeather1 symbol "listens" to (can also be empty ''). |   
| `pubTopic`   | MQTT topics that the CiWeather1 symbol sends when clicked. |   

The following attributes are optional:   
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `name`       | Name of a CiWeather1 symbol. Can be displayed in the header of the icon. |   
| `pubPayload` | Payload to be used when sending a message. |   


## Attributes defined in `CiWeather1Controller.ts`.
The following attributes are mandatory:
| property     | meaning                              |   
| ------------ | ------------------------------------ |   
| `iWeather1State` | State of the CiWeather1 symbol. For this the constants `stateOk`, `stateNotOk` and `stateNoState` are defined in `CiWeather1Controller.ts`.   |   
| `type` | Type of Weather1 symbol. Tells something about what data is expected, e.g. `json_THB`: A json string containing the keys `temperature`, `humidity` and `battery` is expected.   |   
| `lastWeather1Date` | Date when the last weather message arrived.<br>Can be set to `Date?` or `--.--.--` when filling for a specific symbol, for example.   |   
| `lastWeather1Time` | Time when the last weather message arrived.<br>Can be set to `Time?` or `--:--` when filling in for a specific symbol, for example.   |   

The following attributes are optional and can be used if required:   
| property       | meaning                              |   
| -------------- | ------------------------------------ |   
| `temperature`  | String with the temperature value or any other text for line 2 of the symbol.   |   
| `humidity`     | String with the humidity value or any other text for line 3 of the symbol.   |   
| `pressure`     | String with the pressure value or any other text for line 4 of the symbol.   |   
| `brightness`   | String with the brightness value or any other text for line 4 of the symbol.   |   
| `battery`      | String with the state of charge of a battery. Displayed in line 5.   |   
| `textFooter`   | Specification of a fixed text in the footer line. |   

## Example: Coding of the `CiWeather1Controller.ts`.   
Normally, only the values within the `Array<CiWeather1>` area need to be added.   
In the example below two CiWeather1 symbols are defined, where the second symbol 
does not evaluate a possible 'battery' value. The symbol is only used to show the color of the footer (if measured values are available).   
In the method `public onMessage (message: Message): void` the strings for "`temperature`", "`humidity`" and "`brightness`" (or "`pressure`") are used to display the text "`ERROR in JSON payload`" in case of an error.

If one wants to implement additional MQTT functionality when receiving messages, the code in the `public onMessage (message: Message): void` method can be supplemented accordingly.   

```
// ______CiWeather1Controller.ts_________________khartinger_____
// 2022-01-01: new
// 2023-02-09: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Weather1 extends IBase {
  // ---------mandatory-----------------------------------------
  iWeather1State: number;
  type: string;
  lastWeather1Date: string;
  lastWeather1Time: string;
  // ---------optional------------------------------------------
  temperature?: string;
  humidity?: string;
  pressure?: string;
  brightness?: string;
  battery?: string;
  textFooter?: string;
}

export class CiWeather1Controller extends CiBaseController {
  public weather1s: Array<Weather1> = reactive(
    [
      { // ---weather1------------------------------------------
        id: 'weather1',
        name: 'Weather1_1',
        type: 'json_THB', // json-keys: temperature, humidity, battery
        iWeather1State: -99,
        lastWeather1Date: 'Date?',
        lastWeather1Time: 'Time?',
        subTopic: 'z2m/t/weather/-1 z2m/t/weather/-1/time',
        pubTopic: ''
      },
      { // ---weather2: no battery------------------------------
        id: 'weather2',
        name: 'Weather1_2',
        type: 'json_TH', // json-keys: temperature, humidity
        temperature: 'No values',
        humidity: 'available',
        iWeather1State: -99,
        lastWeather1Date: 'Date?',
        lastWeather1Time: 'Time?',
        subTopic: 'z2m/t/weather/-2 z2m/t/weather/-2/time',
        pubTopic: ''
      }
    ]
  );

  // --------------state constants------------------------------
  public stateOk = -1;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.weather1s.forEach(weather1 => {
      const aSubTopic = weather1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------weather1 topic found ------------------------
        console.log('onMessage:', message.topic)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            weather1.lastWeather1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            weather1.lastWeather1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            weather1.lastWeather1Date = geo0f.noDate
            weather1.lastWeather1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different weather types--------------
          weather1.iWeather1State = this.stateNotOk
          if (weather1.type === 'json_THB') {
            try {
              const aPayload = JSON.parse(message.payload)
              weather1.temperature = aPayload.temperature.toFixed(1) + '°C'
              weather1.humidity = aPayload.humidity.toFixed(1) + '%'
              weather1.battery = aPayload.battery + '%'
              // weather1.brightness = aPayload.brightness.toFixed(0) + ' lx'
              weather1.iWeather1State = this.stateOk
            } catch (error) {
              weather1.temperature = 'ERROR' //  = line 2
              weather1.humidity = 'in JSON' //   = line 3
              weather1.brightness = 'payload' // = line 4
              console.log('onMessage error=', error)
            }
          }
          if (weather1.type === 'json_TH') {
            try {
              const aPayload = JSON.parse(message.payload)
              weather1.temperature = aPayload.temperature.toFixed(1) + '°C'
              weather1.humidity = aPayload.humidity.toFixed(1) + '%'
              // weather1.battery = aPayload.battery + '%'
              weather1.iWeather1State = this.stateOk
            } catch (error) {
              weather1.temperature = 'ERROR' //  = line 2
              weather1.humidity = 'in JSON' //   = line 3
              weather1.pressure = 'payload' // = line 4
              console.log('onMessage error=', error)
            }
          }
          if (weather1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different weather types----------
      } // --------END weather1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiWeatherController: ERROR:', e) })
  }
}

export const ciWeather1Controller = new CiWeather1Controller()
```

[Top of page](#up)
