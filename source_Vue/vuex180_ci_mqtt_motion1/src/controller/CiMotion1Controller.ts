// ______CiMotion1Controller.ts_________________khartinger_____
// 2022-01-01: new
// 2023-02-09: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Motion1 extends IBase {
  // ---------mandatory-----------------------------------------
  iMotion1State: number;
  type: string;
  lastMotion1Date: string;
  lastMotion1Time: string;
  // ---------optional------------------------------------------
  battery?: string;
  textFooter?: string;
}

export class CiMotion1Controller extends CiBaseController {
  public motion1s: Array<Motion1> = reactive(
    [
      { // ---motion1: IKEA-------------------------------------
        id: 'motion1',
        name: 'Motion1_1',
        type: 'E1745', // IKEA (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-1 z2m/t/motion/-1/time',
        pubTopic: ''
      },
      { // ---motion2: Aqara------------------------------------
        id: 'motion2',
        name: 'Motion1_2',
        type: 'RTCGQ11LM', // Aquara (zigbee)
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        subTopic: 'z2m/t/motion/-2 z2m/t/motion/-2/time',
        pubTopic: ''
      },
      { // ---motion3: just for test...-------------------------
        id: 'motion3',
        name: 'No_batt',
        type: 'noBattery',
        iMotion1State: -99,
        lastMotion1Date: 'Date?',
        lastMotion1Time: 'Time?',
        textFooter: 'show colorF',
        subTopic: 'z2m/t/motion/-3 z2m/t/motion/-3/time',
        pubTopic: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOn = -1;
  public stateOff = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.motion1s.forEach(motion1 => {
      const aSubTopic = motion1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------motion1 topic found ------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            motion1.lastMotion1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            motion1.lastMotion1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            motion1.lastMotion1Date = geo0f.noDate
            motion1.lastMotion1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different motion types---------------
          motion1.iMotion1State = this.stateNotOk
          if (motion1.type === 'RTCGQ11LM' || motion1.type === 'E1745') {
            try {
              const aPayload = JSON.parse(message.payload)
              motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'noBattery') {
            try {
              const aPayload = JSON.parse(message.payload)
              // motion1.battery = `${aPayload.battery}` + '%'
              if (aPayload.occupancy === true) motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === 'true') motion1.iMotion1State = this.stateOn
              if (aPayload.occupancy === false) motion1.iMotion1State = this.stateOff
              if (aPayload.occupancy === 'false') motion1.iMotion1State = this.stateOff
            } catch (error) {
              motion1.lastMotion1Date = 'ERROR'
              motion1.lastMotion1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (motion1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different motion types----------
      } // --------END motion1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiMotionController: ERROR:', e) })
  }
}

export const ciMotion1Controller = new CiMotion1Controller()
