// ______CiWeather1Controller.ts_________________khartinger_____
// 2022-01-01: new
// 2022-09-14: add .catch((e) ..) to this.publish
// 2023-01-02: import { geo0 } changed
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
        // console.log('onMessage:', message.topic)
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
