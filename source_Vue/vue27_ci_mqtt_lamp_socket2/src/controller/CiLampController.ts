// ______CiLampController.ts_____________________khartinger_____
// 2021-12-28: new
// 2022-09-14: add .catch((e) ..) to this.publish
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Lamp extends IBase {
  iLampState: number;
  battery?: string;
  text5?: string;
}

export class CiLampController extends CiBaseController {
  public lamps: Array<Lamp> = reactive(
    [
      {
        id: 'lamp1',
        name: 'Lamp_1',
        iLampState: -1,
        text5: 'Text_5',
        // subTopic: 'ci/lamp/1/ret/lamp',
        subTopic: 'ci/lamp/1/set/lamp',
        pubTopic: 'ci/lamp/1/ret/lamp return_lamp_state...',
        pubPayload: '1'
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.lamps.forEach(lamp => {
      const aSubTopic = lamp.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---lamp topic found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) lamp.iLampState = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) lamp.iLampState = 0
          else { lamp.iLampState = -1 }
        }
        // ---just for DEMO: send answer message----------------
        const aPubTopic = lamp.pubTopic.split(' ')
        aPubTopic.forEach((topic) => {
          this.publishCi(topic, lamp.iLampState.toString())
        })
        // ---end DEMO------------------------------------------
        // console.log('CiLampController:onMessage: message.payload=', message.payload)
      } // END lamp topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiLampController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiLampController: ERROR:', e) })
  }
}

export const ciLampController = new CiLampController()
