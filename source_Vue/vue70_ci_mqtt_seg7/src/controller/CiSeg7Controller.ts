// ______CiSeg7Controller.ts____________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface CiSeg7 extends IBase {
  iSeg7State: number;
  value7?: string;
  text5?: string;
}

export class CiSeg7Controller extends CiBaseController {
  public ciSeg7s: Array<CiSeg7> = reactive(
    [
      {
        id: 'ciSeg7_1',
        name: 'CiSeg7',
        iSeg7State: -1,
        value7: '8:.',
        text5: 'one digit',
        subTopic: 'ci/seg7/1/set/value',
        pubTopic: 'ci/seg7/1/ret/value',
        pubPayload: '-1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.ciSeg7s.forEach(ciSeg7 => {
      const aSubTopic = ciSeg7.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---ciSeg7 found ---------------------------------
        ciSeg7.value7 = message.payload
        ciSeg7.iSeg7State = 1
        console.log('CiSeg7Controller:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSeg7Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSeg7Controller = new CiSeg7Controller()
