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
      { //
        id: 'window_1',
        name: 'Window_V4',
        iWindowState: -1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/window/1/ret ci/button2/1/status',
        pubTopic: ''
      },
      { // TEST
        id: 'window_2',
        name: 'W_d1x_?',
        iWindowState: -1,
        type: 'D1',
        battery: '10%',
        subTopic: 'ci/window/2/ret ci/button2/1/status',
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
          // console.log('WindowController:onMessage()-zb', window.iWindowState);
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
