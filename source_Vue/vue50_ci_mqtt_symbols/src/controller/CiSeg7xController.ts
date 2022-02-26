// ______CiSeg7xController.ts___________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Seg7x extends IBase {
  iSeg7xState: number;
  battery: string;
  text5?: string;
}

export class CiSeg7xController extends CiBaseController {
  public seg7xs: Array<Seg7x> = reactive(
    [
      {
        id: 'seg7x_1',
        name: ' ',
        iSeg7xState: -1,
        battery: '-',
        text5: 'text5',
        subTopic: 'ci/seg7x/1/set/seg7',
        pubTopic: 'ci/seg7x/1/set/seg7',
        pubPayload: '-1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.seg7xs.forEach(seg7x => {
      const aSubTopic = seg7x.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---seg7 found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) seg7x.iSeg7xState = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) seg7x.iSeg7xState = 0
          else { seg7x.iSeg7xState = -1 }
        }
        // console.log('CiSeg7xController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSeg7xController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSeg7xController = new CiSeg7xController()
