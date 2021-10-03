// _____DeviceController.ts______________________________________
import { Message, MqttClient } from '@/services/MqttClient'
export abstract class DeviceController {
  protected client: MqttClient | null = null;

  public registerClient (mqttClient: MqttClient): void {
    this.client = mqttClient
  }

  protected async publish (topic: string, payload: string): Promise<void> {
    return this.client?.publish(topic, payload, false)
  }

  public abstract onMessage(message: Message): void;
}
