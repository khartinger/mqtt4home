// ______CiSocketController.ts__________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Socket extends IBase {
  type: string;
  iSocketState: number;
  battery?: string;
  text5?: string;
}

export class CiSocketController extends CiBaseController {
  public sockets: Array<Socket> = reactive(
    [
      {
        type: 'e1603', // e1603=IKEA
        id: 'socket_1',
        name: '',
        iSocketState: -9, // 0x9999FF,
        battery: '-',
        text5: 'ci/socket/1',
        subTopic: 'ci/socket/1/set',
        pubTopic: 'ci/socket/1',
        pubPayload: '0'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.sockets.forEach(socket => {
      const aSubTopic = socket.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---socket found ---------------------------------
        if (socket.type === 'e1603') {
          const aPayload = JSON.parse(message.payload)
          if ((aPayload.state === 'ON') || (aPayload.state === 'on')) socket.iSocketState = -1
          else {
            if ((aPayload.state === 'OFF') || (aPayload.state === 'off')) socket.iSocketState = -2
            else { socket.iSocketState = -9 }
          }
        }
        // console.log('CiSocketController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSocketController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSocketController = new CiSocketController()
