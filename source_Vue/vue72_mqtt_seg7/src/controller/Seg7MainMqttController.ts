// ______Seg7MainMqttController.ts______________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Seg7Mqtt extends IBase {
  value7x: string;
}

export class Seg7MainMqttController extends CiBaseController {
  public seg7Mqtts: Array<Seg7Mqtt> = reactive(
    [
      {
        id: 'seg7MainMqtt_1',
        name: ' ',
        value7x: '1:23.4',
        subTopic: 'seg7/1/set/value',
        pubTopic: 'seg7/1/ret/value',
        pubPayload: '-1'
      }
    ]
  );

  public onMessage (message: Message): void {
    this.seg7Mqtts.forEach(seg7Mqtt => {
      const aSubTopic = seg7Mqtt.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---seg7x found --------------------------------------
        seg7Mqtt.value7x = message.payload
        console.log('Seg7MainMqttController:onMessage: message.payload=', message.payload)
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiSeg7xController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const seg7MainMqttController = new Seg7MainMqttController()
