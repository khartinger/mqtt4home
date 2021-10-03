// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { mqttLastXController } from '@/controller/MqttLastXController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(mqttLastXController)
