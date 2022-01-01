// ______CiMotionController.ts__________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import geo0 from '@/components/CiBase.vue'

export interface Motion extends IBase {
  type: string;
  iMotionState: number;
  battery?: string;
  text5?: string;
  lastMotionDate: string;
  lastMotionTime: string;
  subTopicTime: string;
}

export class CiMotionController extends CiBaseController {
  public motions: Array<Motion> = reactive(
    [
      { // motion 1
        id: 'motion1',
        type: 'zb',
        name: 'Motion_1',
        iMotionState: -1,
        battery: '-',
        lastMotionDate: '--.--.--',
        lastMotionTime: '--:--:--',
        subTopic: 'ci/motion/-1',
        subTopicTime: 'ci/motion/-1/time',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for motion topic---------------------------
    this.motions.forEach(motion => {
      const aSubTopic = motion.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---motion topic found -------------------------------
        if (motion.type === 'd1' || motion.type === 'zb') {
          const aPayload = JSON.parse(message.payload)
          motion.iMotionState = 0
          if (aPayload.occupancy === 'true') motion.iMotionState = 1
          if (aPayload.occupancy === true) motion.iMotionState = 1
          if (aPayload.battery) motion.battery = aPayload.battery + '%'
          motion.lastMotionDate = geo0.noDate
          motion.lastMotionTime = geo0.noTime
        }
      } // END motion topic found
    })
    // -------search for time topic-----------------------------
    this.motions.forEach(motion => {
      const aSubTopic = motion.subTopicTime.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---motion topic time found --------------------------
        const s1 = message.payload
        if (s1.length === 15) {
          motion.lastMotionDate = s1.substr(6, 2) + '.' + s1.substr(4, 2) + '.' + s1.substr(0, 4)
          motion.lastMotionTime = s1.substr(9, 2) + ':' + s1.substr(11, 2) + ':' + s1.substr(13, 2)
          // console.log('CiMotionController:onMessage: ', motion.lastMotionDate + ' ' + motion.lastMotionTime)
        }
      } // END motion topic time found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciMotionController = new CiMotionController()
