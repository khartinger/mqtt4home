// ______CiBlindController.ts____________________________________
import { Message } from '@/services/MqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { Geo } from '../components/CiBase.vue'

export interface Blind extends IBase {
  iBlindState: number;
  type?: string;
  battery?: string;
  text5?: string;
}

export class CiBlindController extends CiBaseController {
  public blinds: Array<Blind> = reactive(
    [
      {
        id: 'blindH',
        name: 'Blind_H',
        iBlindState: 1,
        battery: '-',
        subTopic: 'ci/blind/1/ret',
        pubTopic: ''
      },
      {
        id: 'blindV',
        name: 'Blind_V',
        iBlindState: 1,
        battery: '-',
        subTopic: 'ci/blind/2/ret',
        pubTopic: ''
      }
    ]
  );

  geo = new Geo();

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.blinds.forEach(blind => {
      const aSubTopic = blind.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---blind topic found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) blind.iBlindState = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) blind.iBlindState = 0
          else { blind.iBlindState = -1 }
        }
      } // END blind topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciBlindController = new CiBlindController()
