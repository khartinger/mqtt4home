// ______CiMotionController.ts__________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import geo0 from '@/components/CiBase.vue'

export interface Motion extends IBase {
  type: string;
  iMotionState: number;
  lastMotionDate: string;
  lastMotionTime: string;
  battery?: string;
  text5?: string;
}

export class CiMotionController extends CiBaseController {
  public motions: Array<Motion> = reactive(
    [
      { // motion 1
        id: 'motion_1',
        type: 'RTCGQ11LM', // 'E1745'
        name: 'Motion_1',
        iMotionState: -1,
        battery: '-',
        lastMotionDate: '--.--.--',
        lastMotionTime: '--:--:--',
        subTopic: 'ci/motion/-1 ci/motion/-1/time',
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
        if (message.topic.includes('time')) {
          if (message.payload.length === 15) {
            motion.lastMotionDate = message.payload.substring(6, 8) + '.' + message.payload.substring(4, 6) + '.' + message.payload.substring(2, 4)
            motion.lastMotionTime = message.payload.substring(9, 11) + ':' + message.payload.substring(11, 13) + ':' + message.payload.substring(13, 15)
          } else {
            motion.lastMotionDate = geo0.noDate
            motion.lastMotionTime = geo0.noTime
          }
        } else { // -motion info--------------------------------
          motion.lastMotionDate = geo0.noDate
          motion.lastMotionTime = geo0.noTime
          if (motion.type === 'RTCGQ11LM' || motion.type === 'E1745') {
            const aPayload = JSON.parse(message.payload)
            motion.iMotionState = -1
            motion.battery = `${aPayload.battery}`
            if (aPayload.occupancy === true) motion.iMotionState = 1
            if (aPayload.occupancy === 'true') motion.iMotionState = 1
            if (aPayload.occupancy === false) motion.iMotionState = 0
            if (aPayload.occupancy === 'false') motion.iMotionState = 0
          }
          // if (motion.type === 'D1') {
          // }
        }
      } // END motion topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciMotionController = new CiMotionController()
