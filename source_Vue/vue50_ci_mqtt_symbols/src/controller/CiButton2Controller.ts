// ______CiButton2Controller.ts__________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button2 extends IBase {
  iButton2State: number;
  battery: string;
  textOn: string;
  textOff: string;
  pubTopicOff: string;
  pubPayloadOff: string;
  shape?: string;
  color?: string;
  text5?: string;
}

export class CiButton2Controller extends CiBaseController {
  public button2s: Array<Button2> = reactive(
    [
      {
        id: 'button2_1',
        name: 'button2/1',
        iButton2State: 0,
        battery: '-',
        textOn: 'ON',
        textOff: 'OFF',
        shape: 'rect',
        color: '#E2B007',
        subTopic: 'ci/button2/1',
        pubTopic: 'ci/lamp/1/set/lamp',
        pubPayload: '1',
        pubTopicOff: 'ci/lamp/1/set/lamp',
        pubPayloadOff: '0'
      },
      {
        id: 'button2_2',
        name: 'button2_2',
        iButton2State: 0,
        battery: '-',
        textOn: 'ON',
        textOff: 'OFF',
        shape: 'rect',
        color: '#D4AF37',
        // subTopic: 'ci/lamp/21/ret',
        subTopic: '',
        pubTopic: 'ci/lamp/21/ret',
        pubPayload: '1',
        pubTopicOff: 'ci/lamp/21/ret',
        pubPayloadOff: '0'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.button2s.forEach(button2 => {
      const aSubTopic = button2.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button2 found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) button2.iButton2State = -1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) button2.iButton2State = -2
          else { button2.iButton2State = -9 }
        }
        // console.log('CiButton2Controller:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton2Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciButton2Controller = new CiButton2Controller()
