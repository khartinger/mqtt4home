// ______CimqttClientInstance.ts________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciSocket2Controller } from '@/controller/CiSocket2Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciSocket2Controller)
