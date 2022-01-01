// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButtonController } from '@/controller/CiButtonController'
import { ciButton2Controller } from '@/controller/CiButton2Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciButtonController)
ciMqttClientInstance.registerController(ciButton2Controller)
