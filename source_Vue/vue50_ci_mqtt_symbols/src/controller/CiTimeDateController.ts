// ______CiTimeDateLController.ts__________________________________
import { Message } from '@/services/CiMqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

export interface TimeDate extends IBase {
  iTimeDateState: number;
}

export class CiTimeDateController extends CiBaseController {
  public timeDates: Array<TimeDate> = reactive(
    [
      {
        id: 'timeDate_1',
        name: 'TimeDate_1',
        iTimeDateState: -1,
        subTopic: 'ci/timeDate/set',
        pubTopic: ''
      }
    ]
  );

  // geo = new Geo(0, 0);

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for timeDate topic---------------------------
    this.timeDates.forEach(timeDate => {
      const aSubTopic = timeDate.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---timeDate topic found -------------------------------
        // ..ToDo..
      } // END timeDate topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciTimeDateController = new CiTimeDateController()
