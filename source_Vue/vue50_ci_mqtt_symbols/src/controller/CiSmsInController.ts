// ______CiSmsInController.ts___________________________________
import { Message } from '@/services/CiMqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

export interface SmsIn extends IBase {
  iSmsInState: number;
  lines: string[];
}

export class CiSmsInController extends CiBaseController {
  public smsIns: Array<SmsIn> = reactive(
    [
      {
        id: 'smsIn_1',
        name: 'SmsIn_1',
        iSmsInState: -1,
        lines: ['+4368012345678', ' jjjj-mm-dd HH:MM:SS', 'Just for demo: Here', 'would be the received', 'SMS text...'],
        subTopic: 'sms/received',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for smsIn topic----------------------------
    this.smsIns.forEach(smsIn => {
      const aSubTopic = smsIn.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---smsIn topic found --------------------------------
        smsIn.iSmsInState = 1
        let s1 = message.payload
        s1 = s1.replaceAll('\\\\n', '$^~$') // replace "text" \n
        let anzN = s1.match(/\\n/g)?.length
        if (typeof anzN === 'undefined') anzN = 0
        // console.log('Anzahl \\n=', anzN)
        let iLine = 2 // index for line array
        smsIn.lines[0] = ''
        smsIn.lines[1] = ''
        // ---1x \n: Unusual -> title + blank line + rest-------
        if (anzN === 1) {
          const aHelp = s1.split('\\n')
          smsIn.lines[0] = aHelp[0].replaceAll('$^~$', '\\\\n')
          s1 = aHelp[1] // sms text
          iLine = 2
          smsIn.iSmsInState = 2
        }
        // ---2x \n: Normal case -> phone number, date, sms text
        if (anzN === 2) {
          const aHelp = s1.split('\\n')
          smsIn.lines[0] = aHelp[0].replaceAll('$^~$', '\\\\n')
          smsIn.lines[1] = ' ' + aHelp[1].replaceAll('$^~$', '\\\\n')
          s1 = aHelp[2] // sms text
          iLine = 2
          smsIn.iSmsInState = 3
        }
        // ---split SMS text on the three lines-----------------
        s1 = s1.replaceAll('$^~$', '\\\\n')
        const max = 2 * geo0.tmax
        let iStart = 0
        for (let iL = iLine; iL < 5; iL++) {
          let iEnd = iStart + max
          if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
          else {
            if (s1.at(iEnd) === ' ') iEnd++
          }
          smsIn.lines[iL] = s1.substring(iStart, iEnd)
          iStart = iEnd
        }
      } // END smsIn topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciSmsInController = new CiSmsInController()
