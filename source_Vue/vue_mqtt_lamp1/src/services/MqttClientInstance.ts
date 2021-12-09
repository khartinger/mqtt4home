// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { ciLampController } from '@/controller/CiLampController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(ciLampController)
