// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-07: new
import { CiMqttClient } from './CiMqttClient'
import { ciMotion1Controller } from '@/controller/CiMotion1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciMotion1Controller)
