// ______CiDoorController.ts_____________________2022-01-08_____
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Door extends IBase {
  iDoorState: number;
  type: string;
  battery?: string;
  text5?: string;
}

export class CiDoorController extends CiBaseController {
  public doors: Array<Door> = reactive(
    [
      {
        id: 'door_h1',
        name: 'Door_h1',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/11/ret/status ci/door/11/ret/voltage ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_H1',
        name: 'Door_H1',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/12/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_h2',
        name: 'Door_h2',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/13/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_H2',
        name: 'Door_H2',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/14/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_h3',
        name: 'Door_h3',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/15/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_H3',
        name: 'Door_H3',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/16/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_h4',
        name: 'Door_h4',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/17/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_H4',
        name: 'Door_H4',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/18/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'door_v1',
        name: 'Door_v1',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/21/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_V1',
        name: 'Door_V1',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/22/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_v2',
        name: 'Door_v2',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/23/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_V2',
        name: 'Door_V2',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/24/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_v3',
        name: 'Door_v3',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/25/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_V3',
        name: 'Door_V3',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/26/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_v4',
        name: 'Door_v4',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/27/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_V4',
        name: 'Door_V4',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/28/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'door_d1x',
        name: 'Door_d1x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/31/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D1x',
        name: 'Door_D1x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/32/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d1y',
        name: 'Door_d1y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/33/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D1y',
        name: 'Door_D1y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/34/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d2x',
        name: 'Door_d2x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/35/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D2x',
        name: 'Door_D2x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/36/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d2y',
        name: 'Door_d2y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/37/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D2y',
        name: 'Door_D2y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/38/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },

      {
        id: 'door_d3x',
        name: 'Door_d3x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/41/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D3x',
        name: 'Door_D3x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/42/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d3y',
        name: 'Door_d3y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/43/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D3y',
        name: 'Door_D3y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/44/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d4x',
        name: 'Door_d4x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/45/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D4x',
        name: 'Door_D4x',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/46/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_d4y',
        name: 'Door_d4y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/47/ret/status ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_D4y',
        name: 'Door_D4y',
        iDoorState: 1,
        type: 'D1',
        text5: '?',
        subTopic: 'ci/door/48/ret/status  ci/door/48/ret/voltage ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'door_batt',
        name: 'door_batt',
        iDoorState: 1,
        type: 'D1',
        battery: '10%',
        subTopic: 'ci/door/batt/ret/status  ci/door/batt/ret/voltage ci/door/x/ret/status',
        pubTopic: ''
      }

    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    console.log('DoorController:onMessage(): ', `door found ${message.topic}|${message.payload}`)
    this.doors.forEach(door => {
      const aSubTopic = door.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---door topic found ---------------------------------
        if (door.type === 'zb') {
          const aPayload = JSON.parse(message.payload)
          door.battery = `${aPayload.battery}`
          const doorstate_ = String(aPayload.contact)
          if (doorstate_ === 'true') door.iDoorState = 0
          else {
            if (doorstate_ === 'false') door.iDoorState = 1
            else door.iDoorState = -1
          }
          // console.log('DoorController:onMessage()-zb', door.iDoorState);
        }
        if (door.type === 'D1') {
          // console.log('DoorController:onMessage()-D1: ', `door found ${message.topic}|${message.payload}`)
          if (message.topic.includes('status')) {
            if ((message.payload === '1') || (message.payload === 'ein')) door.iDoorState = 1
            else {
              if ((message.payload === '0') || (message.payload === 'aus')) door.iDoorState = 0
              else { door.iDoorState = -1 }
            }
          }
          if (message.topic.includes('voltage')) {
            door.battery = `${message.payload}`
          }
        }
      } // END door topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciDoorController = new CiDoorController()
