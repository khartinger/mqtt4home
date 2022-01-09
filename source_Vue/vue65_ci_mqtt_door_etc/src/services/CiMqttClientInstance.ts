// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciBlindController } from '@/controller/CiBlindController'
import { ciDoorController } from '@/controller/CiDoorController'
import { ciWindowController } from '@/controller/CiWindowController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciBlindController)
ciMqttClientInstance.registerController(ciDoorController)
ciMqttClientInstance.registerController(ciWindowController)
