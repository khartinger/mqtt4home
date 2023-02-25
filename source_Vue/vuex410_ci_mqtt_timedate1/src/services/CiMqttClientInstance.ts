// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-01-31: new
import { CiMqttClient } from './CiMqttClient'
import { ciTimeDate1Controller } from '@/controller/CiTimeDate1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciTimeDate1Controller)
