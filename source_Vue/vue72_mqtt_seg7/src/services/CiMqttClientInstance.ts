// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { seg7MainMqttController } from '@/controller/Seg7MainMqttController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(seg7MainMqttController)
