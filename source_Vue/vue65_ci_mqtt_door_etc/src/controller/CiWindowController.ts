// ______CiWindowController.ts___________________2022-01-08_____
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Window extends IBase {
  iWindowState: number;
  type: string;
  battery?: string;
  text5?: string;
}

export class CiWindowController extends CiBaseController {
  public windows: Array<Window> = reactive(
    [
      {
        id: 'window_h1',
        name: 'Window_h1',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/11/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_H1',
        name: 'Window_H1',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/12/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_h2',
        name: 'Window_h2',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/13/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_H2',
        name: 'Window_H2',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/14/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_h3',
        name: 'Window_h3',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/15/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_H3',
        name: 'Window_H3',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/16/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_h4',
        name: 'Window_h4',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/17/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_H4',
        name: 'Window_H4',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/18/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'window_v1',
        name: 'Window_v1',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/2x/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_V1',
        name: 'Window_V1',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/2x/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_d1x',
        name: 'Window_d1x',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/2x/ret/status ci/window/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'window_D1x',
        name: 'Window_D1x',
        iWindowState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/2x/ret/status ci/window/x/ret/status',
        pubTopic: ''
      }

    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.windows.forEach(window => {
      const aSubTopic = window.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---window topic found ---------------------------------
        if (window.type === 'zb') {
          const aPayload = JSON.parse(message.payload)
          window.battery = `${aPayload.battery}`
          const windowstate_ = String(aPayload.contact)
          if (windowstate_ === 'true') window.iWindowState = 0
          else {
            if (windowstate_ === 'false') window.iWindowState = 1
            else window.iWindowState = -1
          }
          // console.log('WindowController:onMessage()-zb', window.iWindowState)
        }
        if (window.type === 'D1') {
          // console.log('WindowController:onMessage()-D1: ', `window found ${message.topic}|${message.payload}`)
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'ein')) window.iWindowState = 1
            else {
              if ((message.payload === '0') || (message.payload === 'aus')) window.iWindowState = 0
              else { window.iWindowState = -1 }
            }
          }
          if (message.topic.includes('voltage')) {
            window.battery = `${message.payload}`
          }
        }
      } // END window topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciWindowController = new CiWindowController()
