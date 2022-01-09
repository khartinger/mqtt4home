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
        id: 'doorh1',
        name: '?',
        iDoorState: -1,
        type: 'D1',
        text5: 'doorh1',
        subTopic: 'ci/door/1/ret ci/door/x/ret/status',
        pubTopic: ''
      },
      {
        id: 'doorD4x',
        name: 'Door_D4x',
        iDoorState: -1,
        type: 'D1',
        battery: '100%',
        subTopic: 'ci/door/2/ret ci/door/x/ret/status',
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
          // console.log('DoorController:onMessage()-D1: ', `door found ${message.topic}|${message.payload}`);
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
