// ______CiWeatherController.ts_________________________________
import { Message } from '@/services/MqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'

export interface Weather extends IBase {
  type: string;
  iWeatherState: number;
  temperature?: string;
  humidity?: string;
  pressure?: string;
  brightness?: string;
  battery?: string;
}

export class CiWeatherController extends CiBaseController {
  public weathers: Array<Weather> = reactive(
    [
      { // weather 1
        id: 'weather1',
        type: 'zb',
        name: 'Weather1_zb',
        iWeatherState: -1,
        battery: '100',
        subTopic: 'zb/weather/1/ret'
      },
      { // weather 2
        id: 'weather2',
        type: 'd1i',
        name: 'Weather2_d1i',
        iWeatherState: -1,
        battery: '-',
        subTopic: 'ci/weather/2/ret'
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.weathers.forEach(weather => {
      const aSubTopic = weather.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---weather topic found ------------------------------
        weather.iWeatherState = 1
        if (weather.type === 'd1i') {
          const aPayload = JSON.parse(message.payload)
          weather.temperature = aPayload.Ti.toFixed(1) + '°C'
          weather.humidity = aPayload.Hi.toFixed(1) + '%'
          weather.pressure = aPayload.pi.toFixed(0) + 'pa'
          weather.brightness = aPayload.Li.toFixed(0) + 'lx'
          weather.battery = aPayload.battery + '%'
          weather.iWeatherState = 0
        }
        if (weather.type === 'zb') {
          const aPayload = JSON.parse(message.payload)
          weather.temperature = aPayload.temperature.toFixed(1) + '°C'
          weather.humidity = aPayload.humidity.toFixed(1) + '%'
          weather.battery = aPayload.battery + '%'
          weather.iWeatherState = 0
        }
      } // END weather topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciWeatherController = new CiWeatherController()
