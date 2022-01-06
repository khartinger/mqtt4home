// ______mqttClientInstance.ts__________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciBlindController } from '@/controller/CiBlindController'
import { ciButton2Controller } from '@/controller/CiButton2Controller'
import { ciButtonController } from '@/controller/CiButtonController'
import { ciDoorController } from '@/controller/CiDoorController'
import { ciLampController } from '@/controller/CiLampController'
import { ciMotionController } from '@/controller/CiMotionController'
import { ciPumpController } from '@/controller/CiPumpController'
import { ciSocketController } from '@/controller/CiSocketController'

import { ciWindowController } from '@/controller/CiWindowController'

export const ciMqttClientInstance = new CiMqttClient(true)
ciMqttClientInstance.registerController(ciBlindController)
ciMqttClientInstance.registerController(ciButton2Controller)
ciMqttClientInstance.registerController(ciButtonController)
ciMqttClientInstance.registerController(ciDoorController)
ciMqttClientInstance.registerController(ciLampController)
ciMqttClientInstance.registerController(ciMotionController)
ciMqttClientInstance.registerController(ciPumpController)
ciMqttClientInstance.registerController(ciSocketController)

ciMqttClientInstance.registerController(ciWindowController)
