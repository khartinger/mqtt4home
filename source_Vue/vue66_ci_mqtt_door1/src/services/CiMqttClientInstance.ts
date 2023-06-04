// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-06-04 New
import { CiMqttClient } from './CiMqttClient'
import { ciDoor1Controller } from '@/controller/CiDoor1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciDoor1Controller)
