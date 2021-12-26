// ______CiMqttLastXController.ts_______________________________
import { Message } from '@/services/CiMqttClient'
import { ciMqttClientInstance } from '@/services/CiMqttClientInstance'
import { messages, addMessage } from '@/store/CiMessageStore'
import { computed, ComputedRef } from 'vue'
import { CiBaseController } from './CiBaseController'

export class CiMqttLastXController extends CiBaseController {
  private messages;
  private addMessage;

  constructor () {
    super()
    this.messages = messages
    this.addMessage = addMessage
  }

  public onMessage (message: Message): void {
    this.addMessage(message)
    console.log('MqttLastXController.ts', 'Received ' + message.topic)
  }

  public isSubscribe (): ComputedRef<boolean> {
    return computed(() => ciMqttClientInstance.mqttState.connected && ciMqttClientInstance.mqttSubscription.subscribed)
  }
}

export const ciMqttLastXController = new CiMqttLastXController()
