// ______CiWeatherController.ts_________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Weather extends IBase {
  type: string;
  iWeatherState: number;
  temperature?: string;
  humidity?: string;
  pressure?: string;
  brightness?: string;
  battery?: string;
  text5?: string;
}

export class CiWeatherController extends CiBaseController {
  public weathers: Array<Weather> = reactive(
    [
      { // weather 1
        id: 'weather_1',
        type: 'zb',
        name: 'Weather1_zb',
        iWeatherState: -1,
        battery: '100%',
        subTopic: 'z2m/weather/1/ret',
        pubTopic: ''
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
        if (weather.type === 'zb') {
          try {
            const aPayload = JSON.parse(message.payload)
            weather.temperature = aPayload.temperature.toFixed(1) + '°C'
            weather.humidity = aPayload.humidity.toFixed(1) + '%'
            weather.battery = aPayload.battery + '%'
            weather.iWeatherState = 0
          } catch (error) { }
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
