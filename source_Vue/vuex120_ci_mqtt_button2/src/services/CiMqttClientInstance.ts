// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-27: new
import { CiMqttClient } from './CiMqttClient'
import { ciButton2Controller } from '@/controller/CiButton2Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciButton2Controller)
