// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-29: new
import { CiMqttClient } from './CiMqttClient'
import { ciSocket2Controller } from '@/controller/CiSocket2Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciSocket2Controller)
