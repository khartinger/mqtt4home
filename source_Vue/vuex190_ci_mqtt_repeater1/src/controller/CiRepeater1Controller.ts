// ______CiRepeater1Controller.ts________________khartinger_____
// 2022-10-19: new
// 2023-02-10: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0f } from '../classes/Geo'

export interface Repeater1 extends IBase {
  // ---------mandatory-----------------------------------------
  iRepeater1State: number;
  type: string;
  lastRepeater1Date: string;
  lastRepeater1Time: string;
  // ---------optional------------------------------------------
  linkquality?: string;
  textFooter?: string;
}

export class CiRepeater1Controller extends CiBaseController {
  public repeater1s: Array<Repeater1> = reactive(
    [
      { // ---repeater1: IKEA-------------------------------------
        id: 'repeater1',
        name: 'Repeat1 IKEA',
        type: 'TRADFRI', // IKEA
        iRepeater1State: -99,
        lastRepeater1Date: 'Date?',
        lastRepeater1Time: 'Time?',
        subTopic: 'z2m/t/repeater/-1 z2m/t/repeater/-1/time',
        pubTopic: ''
      }
    ]
  );

  // --------------state constants------------------------------
  public stateOk = -1;
  public stateLastSeen = -2;
  public stateNotOk = -998;
  public stateNoState = -999;

  // --------------Message for this ci (control/indicator)?-----
  public onMessage (message: Message): void {
    this.repeater1s.forEach(repeater1 => {
      const aSubTopic = repeater1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // --------repeater1 topic found ------------------------
        console.log('onMessage:', message.topic, ', payload', message.payload)
        if (message.topic.includes('time')) {
          // ------brokertime yyyymmdd HHMMSS received?---------
          if (message.payload.length === 15) {
            repeater1.lastRepeater1Date = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            repeater1.lastRepeater1Time = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
            repeater1.iRepeater1State = this.stateOk
          } else {
            repeater1.lastRepeater1Date = geo0f.noDate
            repeater1.lastRepeater1Time = geo0f.noTime
          }
          // ------END brokertime yyyymmdd HHMMSS received?-----
        } else {
          // ------work on different repeater types-------------
          repeater1.iRepeater1State = this.stateNotOk
          repeater1.lastRepeater1Date = geo0f.noDate
          repeater1.lastRepeater1Time = geo0f.noTime
          if (repeater1.type === 'TRADFRI') {
            try {
              const aPayload = JSON.parse(message.payload)
              // {"last_seen":"2022-10-19T17:03:00+02:00","linkquality":72,"update":{"state":"idle"},"update_available":false}
              if (aPayload.last_seen.length > 24) {
                const dt = aPayload.last_seen // "last_seen":"2022-10-19T17:03:00+02:00"
                const date1 = dt.substring(8, 10) + '.' + dt.substring(5, 7) + '.' + dt.substring(2, 4)
                repeater1.lastRepeater1Date = date1
                repeater1.lastRepeater1Time = dt.substring(11, 19)
                const lqiPercent = Math.round(aPayload.linkquality * 100 / 255)
                repeater1.linkquality = ' link ' + `${lqiPercent}` + '% = ' + `${aPayload.linkquality}` + ' lqi'
                repeater1.iRepeater1State = this.stateLastSeen
                // console.log('Repeater:', aPayload.last_seen + ' = ' + date1 + ' ' + dt.substring(11, 19) + ' state=' + repeater1.iRepeater1State)
              }
            } catch (error) {
              repeater1.lastRepeater1Date = 'ERROR'
              repeater1.lastRepeater1Time = 'json'
              console.log('onMessage error=', error)
            }
          }
          if (repeater1.type === 'something_else') {
            try {
              // ...add functionality for another sensor here...
            } catch (error) { }
          }
        } // ------END work on different repeater types----------
      } // --------END repeater1 topic found---------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiRepeaterController: ERROR:', e) })
  }
}

export const ciRepeater1Controller = new CiRepeater1Controller()
