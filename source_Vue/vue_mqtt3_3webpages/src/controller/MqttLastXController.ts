// ______MqttLastXController.ts_________________________________
import { Message } from '@/services/MqttClient'
import { mqttClientInstance } from '@/services/MqttClientInstance'
import { messages, addMessage } from '@/store/MessageStore'
import { computed, ComputedRef } from 'vue'
import { CiBaseController } from './CiBaseController'

export class MqttLastXController extends CiBaseController {
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
    return computed(() => mqttClientInstance.mqttState.connected && mqttClientInstance.mqttSubscription.subscribed)
  }
}

export const mqttLastXController = new MqttLastXController()
