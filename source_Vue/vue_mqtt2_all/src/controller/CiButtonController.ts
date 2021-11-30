// ______CiButtonController.ts__________________________________
import { Message } from '@/services/MqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button extends IBase {
  iButtonState: number;
  battery: string;
  text5?: string;
}

export class CiButtonController extends CiBaseController {
  public buttons: Array<Button> = reactive(
    [
      {
        id: 'button1',
        name: 'turn lamp off',
        iButtonState: 0x555500,
        battery: '-',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button2',
        name: 'Button2',
        iButtonState: -9,
        battery: '-',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/2/ret ci/lamp/9/ret',
        pubTopic: 'ci/lamp/2/ret ci/lamp/9/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
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
