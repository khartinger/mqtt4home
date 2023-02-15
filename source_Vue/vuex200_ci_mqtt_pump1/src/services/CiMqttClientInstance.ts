// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-10: new
import { CiMqttClient } from './CiMqttClient'
import { ciPump1Controller } from '@/controller/CiPump1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciPump1Controller)
