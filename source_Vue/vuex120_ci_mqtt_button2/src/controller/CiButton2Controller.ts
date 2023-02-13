// ______CiButton2Controller.ts__________________khartinger_____
// 2023-01-28: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Button2 extends IBase {
  iButton2State: number;
  type?: string;
  battery?: string;
  textUpper?: string;
  textLower?: string;
  textFooter?: string;
  pubPayloadLower?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiButton2Controller extends CiBaseController {
  public buttons2: Array<Button2> = reactive(
    [
      {
        id: 'button1',
        name: 'Button2_1',
        iButton2State: -99,
        // textUpper: 'ON',
        // textLower: 'OFF',
        textFooter: '--Footer1--',
        subTopic: 'ci/lamp/1/ret ci/lamp/2/ret',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: '1',
        pubPayloadLower: '0'
      },
      {
        id: 'button2',
        name: 'Button2_2',
        iButton2State: -99,
        textUpper: 'ON',
        textLower: 'OFF',
        textFooter: '==Footer2==',
        subTopic: 'ci/lamp/2/set/lamp',
        pubTopic: 'ci/lamp/1/set/lamp ci/lamp/2/set/lamp',
        pubPayload: 'ON',
        pubPayloadLower: 'OFF'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.buttons2.forEach(button2 => {
      const aSubTopic = button2.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---button found ---------------------------------
        // console.log('CiButton1Controller.onMessage payload=', message.payload)
        if (message.topic.includes('ci/lamp/2/set/lamp')) {
          if (message.payload === '0') button2.iButton2State = -2
          if (message.payload === '1') button2.iButton2State = -1
        }
        if (message.topic.includes('voltage')) {
          button2.battery = `${message.payload}` + '%'
        }
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiButton1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiButtonController: ERROR:', e) })
  }
}

export const ciButton2Controller = new CiButton2Controller()
