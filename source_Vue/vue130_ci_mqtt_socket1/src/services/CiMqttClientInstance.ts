// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-24: new
import { CiMqttClient } from './CiMqttClient'
import { ciSocket1Controller } from '@/controller/CiSocket1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciSocket1Controller)
