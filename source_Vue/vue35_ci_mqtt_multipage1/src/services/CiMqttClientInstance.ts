// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButton2Controller } from '@/controller/CiButton2Controller'
import { mqttLastXController } from '@/controller/MqttLastXController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciButton2Controller)
ciMqttClientInstance.registerController(mqttLastXController)
