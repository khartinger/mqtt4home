// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-28: new
import { CiMqttClient } from './CiMqttClient'
import { ciZ2Mstatus1Controller } from '@/controller/CiZ2Mstatus1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciZ2Mstatus1Controller)
