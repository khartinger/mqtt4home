// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-10: new
import { CiMqttClient } from './CiMqttClient'
import { ciRepeater1Controller } from '@/controller/CiRepeater1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciRepeater1Controller)
