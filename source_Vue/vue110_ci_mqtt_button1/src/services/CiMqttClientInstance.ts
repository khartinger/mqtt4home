// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-24: new
import { CiMqttClient } from './CiMqttClient'
import { ciButton1Controller } from '@/controller/CiButton1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciButton1Controller)
