// ______CiPump1Controller.ts____________________khartinger_____
// 2022-10-19: new
// 2023-02-15: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Pump1 extends IBase {
  // ---------mandatory-----------------------------------------
  iPump1State: number;
  type: string;
  lastPump1Date: string;
  lastPump1Time: string;
  // ---------optional------------------------------------------
  sSensor?: string;
  sStatus?: string;
  textFooter?: string;
}

export class CiPump1Controller extends CiBaseController {
  public pump1s: Array<Pump1> = reactive(
    [
      { // ---pump1---------------------------------------------
        id: 'pump1',
        name: 'Pump 1',
        type: 'ESP32',
        iPump1State: -99,
        sSensor: '--',
        sStatus: 'Status?',
        lastPump1Date: 'Date?',
        lastPump1Time: 'Time?',
        subTopic: 'z2m/t/pump/-1 z2m/t/pump/-1/time',
        pubTopic: ''
      }
    ]
  );

  // --------------state constants------------------------------
  //  public stateOk = -1;
  public stateOn = -1;
  public stateOff = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.pump1s.forEach(pump1 => {
      const aSubTopic = pump1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------pump1 topic found ---------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            pump1.lastPump1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            pump1.lastPump1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
            // pump1.iPump1State = this.stateOk
          } else {
            pump1.lastPump1Date = geo0f.noDate
            pump1.lastPump1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different pump types-----------------
          pump1.iPump1State = this.stateNotOk
          if (pump1.type === 'D1' || pump1.type === 'ESP32') {
            try {
              const aPayload = JSON.parse(message.payload)
              if (aPayload.pump === 0 || aPayload.pump === '0') pump1.iPump1State = this.stateOff
              if (aPayload.pump === 1 || aPayload.pump === '1') pump1.iPump1State = this.stateOn
              pump1.sSensor = `${aPayload.sensor}`
              pump1.sStatus = `${aPayload.status}`
            } catch (error) {
              pump1.iPump1State = this.stateNotOk
              pump1.lastPump1Date = 'ERROR'
              pump1.lastPump1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (pump1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different repeater types----------
      } // --------END pump1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiRepeaterController: ERROR:', e) })
  }
}

export const ciPump1Controller = new CiPump1Controller()
