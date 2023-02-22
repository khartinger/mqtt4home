// ______CiDoor1Controller.ts____________________khartinger_____
// 2022-01-08: new
// 2023-02-18: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Door1 extends IBase {
  // ---------mandatory-----------------------------------------
  iDoor1State: number;
  type: string;
  // ---------optional------------------------------------------
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiDoor1Controller extends CiBaseController {
  public door1s: Array<Door1> = reactive(
    [
      {
        id: 'door_o',
        type: 'zb',
        iDoor1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/door/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_c',
        type: 'zb',
        iDoor1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/door/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_l',
        type: 'zb',
        iDoor1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/door/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_bat10',
        type: 'D1',
        iDoor1State: -1,
        battery: '10',
        subTopic: 'z2m/t/door/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'door_bat20',
        type: 'D1',
        iDoor1State: -1,
        battery: '20',
        subTopic: 'wifi/t/door/-5/ret/status wifi/t/door/-5/ret/voltage',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'doorF',
        name: 'Door1_2D1',
        type: 'D1',
        iDoor1State: -1,
        textFooter: '--Footer1--',
        subTopic: 'wifi/t/door/-6/ret/status wifi/t/door/-6/ret/voltage',
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
    this.door1s.forEach(door1 => {
      const aSubTopic = door1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---door1 topic found --------------------------------
        if (door1.type === 'zb') {
        // ---door type 1---------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            door1.battery = `${aPayload.battery}`
            const doorstate_ = String(aPayload.contact)
            if (doorstate_ === 'true') door1.iDoor1State = this.stateClose
            else {
              if (doorstate_ === 'false') door1.iDoor1State = this.stateOpen
              else door1.iDoor1State = this.stateNotOk
            }
          } catch (error) { }
        }
        if (door1.type === 'D1') {
        // ---door type 2---------------------------------------
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'on')) door1.iDoor1State = this.stateOpen
            else {
              if ((message.payload === '0') || (message.payload === 'off')) door1.iDoor1State = this.stateClose
              else {
                if (message.payload === '-1') door1.iDoor1State = this.stateLock
                else door1.iDoor1State = this.stateNotOk
              }
            }
          }
          if (message.topic.includes('voltage')) {
            door1.battery = `${message.payload}`
          }
        }
        // ---door type 3---------------------------------------
        if (door1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different door1 types------------
        // ---END door1 topic found ----------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiDoor1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiDoor1Controller: ERROR:', e) })
  }
}

export const ciDoor1Controller = new CiDoor1Controller()
