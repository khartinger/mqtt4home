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
        name: 'turn lamp off',
        iButtonState: 0,
        battery: '-',
        shape: 'rect',
        color: '#CC8888',
        text3: 'OFF',
        text5: 'ci/lamp/+/ret',
        subTopic: '',
        // subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button_2',
        name: 'Button2',
        iButtonState: 0,
        battery: '-',
        shape: 'round',
        color: '#777700',
        subTopic: '',
        pubTopic: 'ci/lamp/2/ret',
        pubPayload: '1'
      },
      {
        id: 'button_3',
        name: 'Button3',
        iButtonState: 0,
        battery: '-',
        color: '#DDFFCC',
        text3: 'ON',
        subTopic: '',
        // subTopic: 'ci/lamp/2/ret ci/lamp/9/ret',
        pubTopic: 'ci/lamp/2/ret ci/lamp/9/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '1'
      },
      {
        id: 'button_up',
        name: 'ButtonUp',
        iButtonState: -9,
        battery: '-',
        shape: 'up',
        text3: 'UP',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/2/ret',
        pubTopic: 'ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button_down',
        name: 'ButtonDown',
        iButtonState: -9,
        battery: '-',
        shape: 'down_circle',
        text3: 'DOWN',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/2/ret',
        pubTopic: 'ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button_left',
        name: 'ButtonLeft',
        iButtonState: -9,
        battery: '-',
        shape: 'left',
        text3: 'LEFT',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/2/ret',
        pubTopic: 'ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
      },
      {
        id: 'button_right',
        name: 'ButtonRight',
        iButtonState: -9,
        battery: '-',
        shape: 'right',
        text3: 'RIGHT',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/2/ret',
        pubTopic: 'ci/lamp/2/ret',
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
