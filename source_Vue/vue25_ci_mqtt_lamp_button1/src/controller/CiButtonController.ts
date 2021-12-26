// ______CiButtonController.ts__________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button extends IBase {
  iButtonState: number;
  battery: string;
  shape?: string;
  color?: string;
  text3?: string;
  text5?: string;
}

export class CiButtonController extends CiBaseController {
  public buttons: Array<Button> = reactive(
    [
      {
        id: 'button1',
        name: 'turn lamp off',
        iButtonState: 0,
        shape: 'round',
        color: '#555500',
        battery: '-',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons.forEach(button => {
      const aSubTopic = button.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) button.iButtonState = -1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) button.iButtonState = -2
          else { button.iButtonState = -9 }
        }
        // console.log('CiButtonController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButtonController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciButtonController = new CiButtonController()
