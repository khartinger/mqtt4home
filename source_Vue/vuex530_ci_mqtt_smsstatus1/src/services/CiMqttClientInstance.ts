// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-28: new
import { CiMqttClient } from './CiMqttClient'
import { ciSmsStatus1Controller } from '@/controller/CiSmsStatus1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciSmsStatus1Controller)
