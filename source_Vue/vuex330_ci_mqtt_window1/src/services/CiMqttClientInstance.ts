// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-20: new
import { CiMqttClient } from './CiMqttClient'
import { ciWindow1Controller } from '@/controller/CiWindow1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciWindow1Controller)
