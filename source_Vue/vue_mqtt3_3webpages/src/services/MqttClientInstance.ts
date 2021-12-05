// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButton2Controller } from '@/controller/CiButton2Controller'
import { mqttLastXController } from '@/controller/MqttLastXController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(ciLampController)
mqttClientInstance.registerController(ciButton2Controller)
mqttClientInstance.registerController(mqttLastXController)
