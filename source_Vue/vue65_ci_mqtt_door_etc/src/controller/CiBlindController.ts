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
        id: 'blind_h1',
        name: 'blind_h1',
        iBlindState: 1,
        type: 'D1',
        // iMotorState: -9,
        text5: '?',
        subTopic: 'ci/blind/11/ret ci/blind/11/ret/motor ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_H1',
        name: 'blind_H1',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/12/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_h2',
        name: 'blind_h2',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/13/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_H2',
        name: 'blind_H2',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/14/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_h3',
        name: 'blind_h3',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/15/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_H3',
        name: 'blind_H3',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/16/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_h4',
        name: 'blind_h4',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/17/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_H4',
        name: 'blind_H4',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/18/ret ci/blind/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'blind_v1',
        name: 'blind_v1',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/21/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_V1',
        name: 'blind_V1',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/22/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_v2',
        name: 'blind_v2',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/23/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_V2',
        name: 'blind_V2',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/24/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_v3',
        name: 'blind_v3',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/25/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_V3',
        name: 'blind_V3',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/26/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_v4',
        name: 'blind_v4',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/27/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_V4',
        name: 'blind_V4',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/28/ret ci/blind/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'blind_d1x',
        name: 'blind_d1x',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/31/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_d1y',
        name: 'blind_d1y',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/32/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_d2x',
        name: 'blind_d2x',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/33/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_d2y',
        name: 'blind_d2y',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/34/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_D3x',
        name: 'blind_D3x',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/35/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_D3y',
        name: 'blind_D3y',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/36/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_D4x',
        name: 'blind_D4x',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/37/ret ci/blind/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'blind_D4y',
        name: 'blind_D4y',
        iBlindState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/blind/38/ret ci/blind/x/ret/status',
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
