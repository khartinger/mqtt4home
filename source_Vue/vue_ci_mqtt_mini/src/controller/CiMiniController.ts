// ______CiMiniController.ts____________________________________
// ===========[1] import required components====================
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

// ===========[2] interface for properties of the Controller====
export interface Mini extends IBase {
  topicIn: string;
  payloadIn: string;
}

// ===========[3] define the controller class===================
export class CiMiniController extends CiBaseController {
  // ---------[4] public properties of CiMiniController---------
  public mini: Mini = reactive(
    {
      id: 'mini1',
      topicIn: '--no message--',
      payloadIn: '--no message--'
    }
  )

  // ---------[5] public methods of CiMiniController------------
  // _________[6] receive a message_____________________________
  public onMessage (message: Message): void {
    this.mini.topicIn = message.topic
    this.mini.payloadIn = message.payload
  }

  // _________[7] publish a message_____________________________
  public publishCi (topic: string, payload: string): void {
    // console.log('CiMiniController:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

// ===========[8] Provide an object of the controller class=====
export const ciMiniController = new CiMiniController()
