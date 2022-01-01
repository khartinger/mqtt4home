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
        id: 'button_1',
        name: 'button/1',
        iButtonState: 0,
        battery: '-',
        shape: 'rect',
        color: '#CC8888',
        text3: 'OFF',
        text5: 'ci/lamp/+/ret',
        subTopic: '',
        pubTopic: 'ci/button/1 ci/lamp/1/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'button_2',
        name: 'button/2',
        iButtonState: 0,
        battery: '-',
        shape: 'round',
        subTopic: '',
        pubTopic: 'ci/button/2 ci/lamp/1/set/lamp',
        pubPayload: '1'
      },
      {
        id: 'button_3',
        name: 'button/3',
        iButtonState: -9,
        battery: '-',
        shape: 'up',
        text3: 'UP',
        text5: '',
        subTopic: '',
        pubTopic: 'ci/button/3',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button_4',
        name: 'button/4',
        iButtonState: -9,
        battery: '-',
        shape: 'down_circle',
        text3: 'DOWN',
        text5: '',
        subTopic: '',
        pubTopic: 'ci/button/4',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
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
