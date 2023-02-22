// ______CiWindow1Controller.ts__________________khartinger_____
// 2022-01-08: new
// 2023-02-20: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Window1 extends IBase {
  // ---------mandatory-----------------------------------------
  iWindow1State: number;
  type: string;
  // ---------optional------------------------------------------
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiWindow1Controller extends CiBaseController {
  public window1s: Array<Window1> = reactive(
    [
      {
        id: 'window_o',
        type: 'zb',
        iWindow1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/window/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_c',
        type: 'zb',
        iWindow1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/window/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_l',
        type: 'zb',
        iWindow1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/window/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_bat10',
        type: 'D1',
        iWindow1State: -1,
        battery: '10',
        subTopic: 'z2m/t/window/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'window_bat20',
        type: 'D1',
        iWindow1State: -1,
        battery: '20',
        subTopic: 'wifi/t/window/-5/ret/status wifi/t/window/-5/ret/voltage',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'windowF',
        name: 'Window1_2D1',
        type: 'D1',
        iWindow1State: -1,
        textFooter: '--Footer1--1234567890ABCDE',
        subTopic: 'wifi/t/window/-6/ret/status wifi/t/window/-6/ret/voltage',
        pubTopic: '',
        pubPayload: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOpen = -1;
  public stateClose = -2;
  public stateLock = -3;
  public stateNotOk = -998;
  public stateNoState = -999;

  public onMessage (message: Message): void {
    this.window1s.forEach(window1 => {
      const aSubTopic = window1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---window1 topic found --------------------------------
        if (window1.type === 'zb') {
        // ---window type 1---------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            window1.battery = `${aPayload.battery}`
            const windowstate_ = String(aPayload.contact)
            if (windowstate_ === 'true') window1.iWindow1State = this.stateClose
            else {
              if (windowstate_ === 'false') window1.iWindow1State = this.stateOpen
              else window1.iWindow1State = this.stateNotOk
            }
          } catch (error) { }
        }
        if (window1.type === 'D1') {
        // ---window type 2---------------------------------------
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'on')) window1.iWindow1State = this.stateOpen
            else {
              if ((message.payload === '0') || (message.payload === 'off')) window1.iWindow1State = this.stateClose
              else {
                if (message.payload === '-1') window1.iWindow1State = this.stateLock
                else window1.iWindow1State = this.stateNotOk
              }
            }
          }
          if (message.topic.includes('voltage')) {
            window1.battery = `${message.payload}`
          }
        }
        // ---window type 3---------------------------------------
        if (window1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different window1 types------------
        // ---END window1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiWindow1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiWindow1Controller: ERROR:', e) })
  }
}

export const ciWindow1Controller = new CiWindow1Controller()
