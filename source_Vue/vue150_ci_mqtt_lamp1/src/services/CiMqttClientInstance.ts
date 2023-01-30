// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-30: new
import { CiMqttClient } from './CiMqttClient'
import { ciLamp1Controller } from '@/controller/CiLamp1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciLamp1Controller)
