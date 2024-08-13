// ______CiBaseController.ts_____________________khartinger_____
// 2022-01-01: new
// 2023-09-02: change import Qos
import { Message, CiMqttClient } from '@/services/CiMqttClient'
import { QoS } from 'mqtt-packet'

export interface IBase {
  id: string;
  name?: string;
  subTopic: string;
  pubTopic: string;
  pubPayload?: string;
}

export abstract class CiBaseController {
  protected client: CiMqttClient | null = null

  public registerClient (mqttClient: CiMqttClient): void {
    this.client = mqttClient
  }

  protected async publish (topic: string, payload: string, retain: boolean, qos: QoS): Promise<void> {
    return this.client?.publish(topic, payload, retain, qos)
  }

  public abstract onMessage(message: Message): void;
}
