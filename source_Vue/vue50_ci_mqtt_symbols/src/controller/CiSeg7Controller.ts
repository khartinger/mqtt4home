// ______CiSeg7Controller.ts____________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Seg7 extends IBase {
  iSeg7State: number;
  battery: string;
  text5?: string;
}

export class CiSeg7Controller extends CiBaseController {
  public seg7s: Array<Seg7> = reactive(
    [
      {
        id: 'seg7_1',
        name: ' ',
        iSeg7State: -1,
        battery: '-',
        text5: 'text5',
        subTopic: 'ci/seg7/1/set/seg7',
        pubTopic: 'ci/seg7/1/set/seg7',
        pubPayload: '-1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.seg7s.forEach(seg7 => {
      const aSubTopic = seg7.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---seg7 found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) seg7.iSeg7State = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) seg7.iSeg7State = 0
          else { seg7.iSeg7State = -1 }
        }
        // console.log('CiSeg7Controller:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSeg7Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSeg7Controller = new CiSeg7Controller()
