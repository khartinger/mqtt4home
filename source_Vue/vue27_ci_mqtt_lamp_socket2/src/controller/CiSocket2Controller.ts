// ______CiSocket2Controller.ts__________________khartinger_____
// 2022-08-07: new
// 2022-09-14: add .catch((e) ..) to this.publish
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Socket extends IBase {
  type: string;
  iSocketState: number;
  battery?: string;
  text5?: string;
  pubPayloadOn?: string;
}

export class CiSocket2Controller extends CiBaseController {
  public sockets: Array<Socket> = reactive(
    [
      {
        type: 'test', // delock11826, e1603=IKEA
        id: 'socket2',
        name: 's2',
        iSocketState: -9, // 0x9999FF,
        battery: '-',
        text5: 'text5',
        subTopic: 'ci/lamp/1/ret/lamp',
        pubTopic: 'ci/lamp/1/set/lamp',
        pubPayload: '0',
        pubPayloadOn: '1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.sockets.forEach(socket => {
      const aSubTopic = socket.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---socket found ---------------------------------
        if (socket.type === 'test') {
          if ((message.payload === '1') || (message.payload === 'on')) socket.iSocketState = -1
          else {
            if ((message.payload === '0') || (message.payload === 'off')) socket.iSocketState = -2
            else { socket.iSocketState = -9 }
          }
        }
        if (socket.type === 'delock11826') {
          const aPayload = JSON.parse(message.payload)
          if ((aPayload.POWER === 'ON') || (aPayload.POWER === 'on')) socket.iSocketState = -1
          else {
            if ((aPayload.POWER === 'OFF') || (aPayload.POWER === 'off')) socket.iSocketState = -2
            else { socket.iSocketState = -9 }
          }
        }
        // console.log('CiSocketController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSocketController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSocket2Controller: ERROR:', e) })
  }
}

export const ciSocket2Controller = new CiSocket2Controller()
