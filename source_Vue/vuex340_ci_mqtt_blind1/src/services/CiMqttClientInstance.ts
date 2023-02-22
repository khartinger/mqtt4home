// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-16: new
import { CiMqttClient } from './CiMqttClient'
import { ciBlind1Controller } from '@/controller/CiBlind1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciBlind1Controller)
