// ______CiSmsOutController.ts___________________________________
import { Message } from '@/services/CiMqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

export interface SmsOut extends IBase {
  iSmsOutState: number;
  lines: string[];
}

export class CiSmsOutController extends CiBaseController {
  public smsOuts: Array<SmsOut> = reactive(
    [
      {
        id: 'smsOut_1',
        name: 'SmsOut_1',
        iSmsOutState: -1,
        lines: ['+4368012345678', ' jjjj-mm-dd HH:MM:SS', 'Just for demo: Here', 'would be the last sent', 'SMS text...'],
        subTopic: 'sms/send/ret',
        pubTopic: 'sms/send'
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for smsOut topic----------------------------
    this.smsOuts.forEach(smsOut => {
      const aSubTopic = smsOut.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---smsOut topic found --------------------------------
        smsOut.iSmsOutState = -1
        if (message.payload.includes('SMS sent')) {
          smsOut.iSmsOutState = 3
        }
        if (message.payload.includes('SMS NOT sent')) {
          smsOut.iSmsOutState = 1
        }
        // console.log('smsOut.iSmsOutState=', smsOut.iSmsOutState)
        // ---copy message to vue symbol------------------------
        const pos1 = message.payload.indexOf('(')
        const pos2 = message.payload.indexOf(':', pos1)
        const pos3 = message.payload.indexOf(')')
        if (pos2 < pos1 || pos3 < pos2) smsOut.iSmsOutState = 0
        else {
          const smsphone = message.payload.substring(pos1 + 1, pos2)
          if (smsphone.length > 0) smsOut.lines[0] = smsphone
          // ---split SMS text on the three lines-----------------
          const s1 = message.payload.substring(pos2 + 1, pos3)
          const iLine = 2
          const max = 2 * geo0.tmax
          let iStart = 0
          for (let iL = iLine; iL < 5; iL++) {
            let iEnd = iStart + max
            if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
            else {
              if (s1.at(iEnd) === ' ') iEnd++
            }
            smsOut.lines[iL] = s1.substring(iStart, iEnd)
            iStart = iEnd
          }
        } // END else
      } // END smsOut topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSmsOutController = new CiSmsOutController()
