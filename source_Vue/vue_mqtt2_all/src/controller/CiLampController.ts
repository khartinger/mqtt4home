// ______CiLampController.ts____________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Lamp extends IBase {
  iLampState: number;
  battery: string;
  text5?: string;
}

export class CiLampController extends CiBaseController {
  public lamps: Array<Lamp> = reactive(
    [
      {
        id: 'lamp1',
        name: 'Lamp_1',
        iLampState: -1,
        battery: '-',
        text5: 'ci/lamp/1/ret',
        subTopic: 'ci/lamp/1/ret',
        pubTopic: 'ci/lamp/1/set ci/lamp/9/set',
        pubPayload: '-1'
      },
      {
        id: 'lamp2',
        name: 'Lamp_2',
        iLampState: -1,
        battery: '-',
        text5: 'ci/lamp/2/ret',
        subTopic: 'ci/lamp/2/ret ci/lamp/9/ret',
        pubTopic: 'ci/lamp/2/set ci/lamp/9/set',
        pubPayload: '-1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.lamps.forEach(lamp => {
      const aSubTopic = lamp.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---lamp found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) lamp.iLampState = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) lamp.iLampState = 0
          else { lamp.iLampState = -1 }
        }
        // console.log('CiLampController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiLampController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciLampController = new CiLampController()
