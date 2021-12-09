// ______CiButton2Controller.ts_________________________________
import { Message } from '@/services/MqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button2 extends IBase {
  iButton2State: number;
  battery?: string;
  textOn: string;
  textOff: string;
  text5?: string;
}

export class CiButton2Controller extends CiBaseController {
  public button2s: Array<Button2> = reactive(
    [
      {
        id: 'button2_1',
        name: 'button on/off',
        iButton2State: -9,
        battery: '-',
        textOn: 'ON',
        textOff: 'OFF',
        text5: 'ci/lamp/+/ret',
        subTopic: 'ci/lamp/1/ret',
        pubTopic: 'ci/lamp/1/set',
        // pubTopic: 'ci/lamp/1/set ci/lamp/2/set',
        pubPayload: '0'
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