// ______CiBlindController.ts____________________2022-01-08_____
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
export interface Blind extends IBase {
  iBlindState: number;
  type: string;
  iMotorState?: number;
  battery?: string;
  text5?: string;
}

export class CiBlindController extends CiBaseController {
  public blinds: Array<Blind> = reactive(
    [
      {
        id: 'blind_1',
        name: 'Rollo ?',
        iBlindState: -1,
        type: 'D1',
        battery: '-',
        subTopic: 'ci/blind/1/ret ci/blind/1/ret/motor ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_2',
        name: 'Rollo ?',
        iBlindState: -1,
        type: 'D1',
        battery: '-',
        subTopic: 'ci/blind/1/ret ci/blind/x/ret/status',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.blinds.forEach(blind => {
      const aSubTopic = blind.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---blind topic found ---------------------------------
        if (blind.type === 'zb') {
          const aPayload = JSON.parse(message.payload)
          blind.battery = `${aPayload.battery}`
          const doorstate_ = `${aPayload.contact}`
          if (doorstate_ === 'true') blind.iBlindState = 0
          else {
            if (doorstate_ === 'false') blind.iBlindState = 1
            else blind.iBlindState = -1
          }
        }
        if (blind.type === 'D1') {
          // console.log('BlindController:onMessage()-D1: ', `window found ${message.topic}|${message.payload}`)
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'ein')) blind.iBlindState = 1
            else {
              if ((message.payload === '0') || (message.payload === 'aus')) blind.iBlindState = 0
              else { blind.iBlindState = -1 }
            }
          }
          if (message.topic.includes('voltage')) {
            blind.battery = `${message.payload}`
          }
          if (message.topic.includes('motor')) {
            const aPayload = JSON.parse(message.payload)
            if (aPayload === 1) blind.iMotorState = 1
            if (aPayload === 0) blind.iMotorState = 0
            if (aPayload === -1) blind.iMotorState = -1
            if (aPayload.motor === -1) blind.iMotorState = -1
            if (aPayload.motor === 0) blind.iMotorState = 0
            if (aPayload.motor === 1) blind.iMotorState = 1
          }
        }
      } // END blind topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciBlindController = new CiBlindController()
