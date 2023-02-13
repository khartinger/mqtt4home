// ______CiLamp1Controller.ts____________________khartinger_____
// 2021-12-28: new
// 2023-01-30: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Lamp1 extends IBase {
  iLamp1State: number;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiLamp1Controller extends CiBaseController {
  public lamps1: Array<Lamp1> = reactive(
    [
      {
        id: 'lamp1',
        name: 'Lamp_1',
        iLamp1State: -99,
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'lamp2',
        name: 'Lamp_2',
        iLamp1State: -99,
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.lamps1.forEach(lamp1 => {
      const aSubTopic = lamp1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---lamp1 topic found --------------------------------
        // console.log('CiLamp1Controller.onMessage payload=', message.payload)
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') lamp1.iLamp1State = -2
          if (message.payload === '1') lamp1.iLamp1State = -1
        }
        if (message.topic.includes('voltage')) {
          lamp1.battery = `${message.payload}` + '%'
        }
        // ---end lamp1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiLamp1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiLamp1Controller: ERROR:', e) })
  }
}

export const ciLamp1Controller = new CiLamp1Controller()
