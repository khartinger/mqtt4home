// ______CiPumpController.ts_____________________2022-01-01_____
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import geo0 from '@/components/CiBase.vue'

export interface Pump extends IBase {
  type: string;
  iPumpState: number;
  sSensor?: string;
  sStatus?: string;
  lastPumpDate: string;
  lastPumpTime: string;
}

export class CiPumpController extends CiBaseController {
  public pumps: Array<Pump> = reactive(
    [
      { // pump 1
        id: 'pump1',
        type: 'd1',
        name: 'Pump_1',
        iPumpState: -1,
        sSensor: '-',
        sStatus: 'NO STATE',
        lastPumpDate: '--.--.--',
        lastPumpTime: '--:--:--',
        subTopic: 'ci/pump/-1/ret/pump ci/pump/-1/ret/pump/time',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for pump topic-----------------------------
    this.pumps.forEach(pump => {
      const aSubTopic = pump.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---pump topic found ---------------------------------
        if (message.topic.includes('time')) {
          if (message.payload.length === 15) {
            pump.lastPumpDate = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            pump.lastPumpTime = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            pump.lastPumpDate = geo0.noDate
            pump.lastPumpTime = geo0.noTime
          }
        } else { // -pump info----------------------------------
          pump.lastPumpDate = geo0.noDate
          pump.lastPumpTime = geo0.noTime
          if (pump.type === 'D1' || pump.type === 'ESP32') {
            const aPayload = JSON.parse(message.payload)
            pump.iPumpState = -1
            if (aPayload.pumpe === 0 || aPayload.pumpe === '0') pump.iPumpState = 0
            if (aPayload.pumpe === 1 || aPayload.pumpe === '1') pump.iPumpState = 1
            pump.sSensor = `${aPayload.sensor}`
            pump.sStatus = aPayload.status
          }
        }
      } // END pump topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciPumpController = new CiPumpController()
