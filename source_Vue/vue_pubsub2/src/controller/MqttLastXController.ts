// ______MqttLastXController.ts_________________________________
import { Message } from '@/services/MqttClient'
import { messages, addMessage } from '@/store/MessageStore'
import { DeviceController } from './DeviceController'
import { mqttClientInstance } from '@/services/MqttClientInstance'
import { computed, ComputedRef } from 'vue'

export class MqttLastXController extends DeviceController {
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

  public urlSubscribe (): ComputedRef<string> {
    return computed(() => mqttClientInstance.connectUrl())
  }

  public subTopic (): ComputedRef<string> {
    return computed(() => mqttClientInstance.mqttSubscription.topic)
  }
}

export const mqttLastXController = new MqttLastXController()
