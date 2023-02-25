// ______CiTimeDate1Controller.ts________________khartinger_____
// 2023-02-25: New

import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface TimeDate1 extends IBase {
  // ---------mandatory-----------------------------------------
  iTimeDate1State: number; //
  // ---------optional------------------------------------------
  textFooter?: string;
}

export class CiTimeDate1Controller extends CiBaseController {
  public timedate1s: Array<TimeDate1> = reactive(
    [
      {
        id: 'timedate1',
        name: 'TimeDate_1',
        iTimeDate1State: -1,
        subTopic: 'ci/timeDate/set',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for topic timedate1------------------------
    this.timedate1s.forEach(timedate1 => {
      const aSubTopic = timedate1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---timedate1 topic found ----------------------------
        if (timedate1.id === 'something_else') {
          try {
            // ...add functionality for symbol here...
          } catch (error) { }
        }
      } // ---END timedate1 topic found-------------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTimeDate1Controller: ERROR:', e) })
  }
}

export const ciTimeDate1Controller = new CiTimeDate1Controller()
