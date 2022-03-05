// ______CiSeg7xController.ts___________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface CiSeg7x extends IBase {
  iSeg7xState: number;
  text5?: string;
  value7x?: string;
}

export class CiSeg7xController extends CiBaseController {
  public ciSeg7xs: Array<CiSeg7x> = reactive(
    [
      {
        id: 'ciSeg7x_1',
        name: '',
        iSeg7xState: -1,
        text5: 'line 5 text...',
        subTopic: 'ci/seg7x/1/set/value',
        pubTopic: 'ci/seg7x/1/ret/value'
        // pubPayload: '?'
      },
      {
        id: 'ciSeg7x_2',
        name: ' ',
        iSeg7xState: -1,
        text5: 'text5',
        subTopic: 'ci/seg7x/2/set/value',
        pubTopic: 'ci/seg7x/2/ret/value'
        // pubPayload: '?'
      },
      {
        id: 'ciSeg7x_3',
        name: ' ',
        iSeg7xState: -1,
        text5: 'text5',
        subTopic: 'ci/seg7x/3/set/value',
        pubTopic: 'ci/seg7x/3/ret/value'
        // pubPayload: '?'
      }

    ]
  );

  public onMessage (message: Message): void {
    this.ciSeg7xs.forEach(ciSeg7x => {
      const aSubTopic = ciSeg7x.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---seg7x found---------------------------------
        ciSeg7x.value7x = message.payload
        console.log('CiSeg7xController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSeg7xController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSeg7xController = new CiSeg7xController()
