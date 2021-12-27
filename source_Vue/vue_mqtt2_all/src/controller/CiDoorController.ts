// ______CiDoorController.ts____________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Door extends IBase {
  iDoorState: number;
  type?: string;
  battery?: string;
  text5?: string;
}

export class CiDoorController extends CiBaseController {
  public doors: Array<Door> = reactive(
    [
      {
        id: 'doorH1',
        name: 'Door_H1',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorh1',
        name: 'Door_h1',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorH2',
        name: 'Door_H2',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorh2',
        name: 'Door_h2',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorH2',
        name: 'Door_H2',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorh2',
        name: 'Door_h2',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorH3',
        name: 'Door_H3',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorh3',
        name: 'Door_h3',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorH4',
        name: 'Door_H4',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorh4',
        name: 'Door_h4',
        iDoorState: 1,
        battery: '-',
        subTopic: 'ci/door/1/ret',
        pubTopic: ''
      },
      {
        id: 'doorV1',
        name: 'Door_V1',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorv1',
        name: 'Door_v1',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorV2',
        name: 'Door_V2',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorv2',
        name: 'Door_v2',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorV3',
        name: 'Door_V3',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorv3',
        name: 'Door_v3',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorV4',
        name: 'Door_V4',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      },
      {
        id: 'doorv4',
        name: 'Door_v4',
        iDoorState: 1,
        battery: '100',
        subTopic: 'ci/door/5/ret',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    this.doors.forEach(door => {
      const aSubTopic = door.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---door topic found ---------------------------------
        if ((message.payload === '1') || (message.payload === 'on')) door.iDoorState = 1
        else {
          if ((message.payload === '0') || (message.payload === 'off')) door.iDoorState = 0
          else { door.iDoorState = -1 }
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
