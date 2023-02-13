// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-31: new
import { CiMqttClient } from './CiMqttClient'
import { ciTextarea1Controller } from '@/controller/CiTextarea1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciTextarea1Controller)
