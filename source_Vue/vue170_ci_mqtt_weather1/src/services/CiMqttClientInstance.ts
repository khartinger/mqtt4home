// ______CiMqttClientInstance.ts_________________khartinger_____
// 2023-02-07: new
import { CiMqttClient } from './CiMqttClient'
import { ciWeather1Controller } from '@/controller/CiWeather1Controller'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciWeather1Controller)
