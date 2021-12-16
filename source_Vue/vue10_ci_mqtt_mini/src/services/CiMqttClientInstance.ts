// ______CiMqttClientInstance.ts________________________________
import { CiMqttClient } from './CiMqttClient'
import { ciMiniController } from '@/controller/CiMiniController'

// -----------mqtt client: true=connect and subscribe at start--
export const ciMqttClientInstance = new CiMqttClient(true)

// -----------register all controllers to send/receive messages-
ciMqttClientInstance.registerController(ciMiniController)
