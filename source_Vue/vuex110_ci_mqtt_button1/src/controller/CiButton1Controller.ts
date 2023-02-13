// ______CiButton1Controller.ts__________________khartinger_____
// 2022-09-14: add .catch((e) ..) to this.publish
// 2023-01-25: add textCenter, textFooter | remove shape, color
// 2023-01-30: change at CiBase (add Geo.ts)

import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button1 extends IBase {
  iButtonState: number;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiButton1Controller extends CiBaseController {
  public buttons1: Array<Button1> = reactive(
    [
      {
        id: 'button1',
        name: 'Button_1',
        iButtonState: -99,
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        // pubTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '0'
      },
      {
        id: 'button2',
        name: 'Button_2',
        iButtonState: -99,
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons1.forEach(button1 => {
      const aSubTopic = button1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (button1.id.substring(0, 7) === 'lamp1') {
          const aPayload = JSON.parse(message.payload)
          button1.iButtonState = 10 + aPayload.motor
        }
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button1.iButtonState = -2
          if (message.payload === '1') button1.iButtonState = -1
        }
        if (message.topic.includes('voltage')) {
          button1.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton1Controller = new CiButton1Controller()
