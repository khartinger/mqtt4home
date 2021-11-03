// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButtonController } from '@/controller/CiButtonController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(ciLampController)
mqttClientInstance.registerController(ciButtonController)
