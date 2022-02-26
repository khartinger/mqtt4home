// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciSeg7Controller } from '@/controller/CiSeg7Controller'
import { ciSeg7xController } from '@/controller/CiSeg7xController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciSeg7Controller)
ciMqttClientInstance.registerController(ciSeg7xController)
