// _____DeviceController.ts______________________________________
import { Message, MqttClient } from '@/services/MqttClient'
import type { QoS } from 'mqtt'
export abstract class DeviceController {
  protected client: MqttClient | null = null;

  public registerClient (mqttClient: MqttClient): void {
    this.client = mqttClient
  }

  protected async publish (topic: string, payload: string, retain: boolean, qos: QoS): Promise<void> {
    return this.client?.publish(topic, payload, retain, qos)
  }

  public abstract onMessage(message: Message): void;
}
