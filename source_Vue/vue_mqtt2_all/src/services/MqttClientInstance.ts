// ______mqttClientInstance.ts__________________________________
import { MqttClient } from './MqttClient'
import { ciLampController } from '@/controller/CiLampController'
import { ciButtonController } from '@/controller/CiButtonController'
import { ciText5Controller } from '@/controller/CiText5Controller'
import { ciText5LController } from '@/controller/CiText5LController'
import { ciWeatherController } from '@/controller/CiWeatherController'
import { ciMotionController } from '@/controller/CiMotionController'
import { ciPumpController } from '@/controller/CiPumpController'
import { ciSocketController } from '@/controller/CiSocketController'
import { ciDoorController } from '@/controller/CiDoorController'
import { ciBlindController } from '@/controller/CiBlindController'

export const mqttClientInstance = new MqttClient()
mqttClientInstance.registerController(ciLampController)
mqttClientInstance.registerController(ciButtonController)
mqttClientInstance.registerController(ciText5Controller)
mqttClientInstance.registerController(ciText5LController)
mqttClientInstance.registerController(ciWeatherController)
mqttClientInstance.registerController(ciMotionController)
mqttClientInstance.registerController(ciPumpController)
mqttClientInstance.registerController(ciSocketController)
mqttClientInstance.registerController(ciDoorController)
mqttClientInstance.registerController(ciBlindController)