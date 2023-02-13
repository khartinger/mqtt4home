// ______CiSocket1Controller.ts__________________khartinger_____
// 2022-09-14: new
// 2023-01-29: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Socket1 extends IBase {
  iSocket1State: number;
  type: string;
  battery?: string;
  textCenter?: string;
  textFooter?: string;
}

export class CiSocket1Controller extends CiBaseController {
  public sockets1: Array<Socket1> = reactive(
    [
      {
        id: 'socket1',
        name: 'Socket1_1',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textFooter: '--Footer1--',
        subTopic: 'ci/socket/1',
        pubTopic: 'ci/socket/1/set',
        pubPayload: '{"state": "TOGGLE"}'
      },
      {
        id: 'socket2',
        name: 'Socket1_2',
        iSocket1State: -99,
        type: 'e1603', // e1603=IKEA
        textCenter: 'CENTER',
        textFooter: '==Footer2==',
        subTopic: 'ci/socket/2',
        pubTopic: 'ci/socket/2/set',
        pubPayload: '{"state": "TOGGLE"}'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.sockets1.forEach(socket1 => {
      const aSubTopic = socket1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---socket found -------------------------------------
        // console.log('CiSocket1Controller.onMessage payload=', message.payload)
        if (socket1.type === 'e1603') {
          const aPayload = JSON.parse(message.payload)
          if ((aPayload.state === 'ON') || (aPayload.state === 'on')) socket1.iSocket1State = -1
          else {
            if ((aPayload.state === 'OFF') || (aPayload.state === 'off')) socket1.iSocket1State = -2
            else { socket1.iSocket1State = -9 }
          }
        }
        // ---end socket found----------------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSocket1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSocketController: ERROR:', e) })
  }
}

export const ciSocket1Controller = new CiSocket1Controller()
