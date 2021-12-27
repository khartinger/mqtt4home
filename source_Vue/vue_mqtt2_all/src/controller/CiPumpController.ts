// ______CiPumpController.ts____________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { Geo } from '@/components/CiBase.vue'

export interface Pump extends IBase {
  type: string;
  iPumpState: number;
  sSensor?: string;
  sStatus?: string;
  lastPumpDate: string;
  lastPumpTime: string;
  subTopicTime: string;
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
        subTopic: 'ci/pump/-1/ret/pump',
        subTopicTime: 'ci/pump/-1/ret/pump/time',
        pubTopic: ''
      }
    ]
  );

  geo2 = new Geo(0, 0);

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for pump topic---------------------------
    this.pumps.forEach(pump => {
      const aSubTopic = pump.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---pump topic found -------------------------------
        if (pump.type === 'd1' || pump.type === 'esp32') {
          const aPayload = JSON.parse(message.payload)
          pump.iPumpState = -1
          if (aPayload.pump === '0') pump.iPumpState = 0
          if (aPayload.pump === '1') pump.iPumpState = 1
          pump.sSensor = aPayload.sensor
          pump.sStatus = aPayload.status
          const geo3 = new Geo(0, 0)
          pump.lastPumpDate = geo3.noDate
          pump.lastPumpTime = geo3.noTime
        }
      } // END pump topic found
    })
    // -------search for time topic-----------------------------
    this.pumps.forEach(pump => {
      const aSubTopic = pump.subTopicTime.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---pump topic time found --------------------------
        const s1 = message.payload
        if (s1.length === 15) {
          pump.lastPumpDate = s1.substr(6, 2) + '.' + s1.substr(4, 2) + '.' + s1.substr(0, 4)
          pump.lastPumpTime = s1.substr(9, 2) + ':' + s1.substr(11, 2) + ':' + s1.substr(13, 2)
          // console.log('CiPumpController:onMessage: ', pump.lastPumpDate + ' ' + pump.lastPumpTime)
        }
      } // END pump topic time found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciPumpController = new CiPumpController()
