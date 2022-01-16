// ______CiText5Controller.ts___________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'
// import Geo from '@/components/CiBase.vue'
export interface Text5 extends IBase {
  iText5State: number;
  lines: string[];
}

export class CiText5Controller extends CiBaseController {
  public text5s: Array<Text5> = reactive(
    [
      {
        id: 'text5_1',
        name: 'Text5_1',
        iText5State: -1,
        lines: ['Text5_1', 'MQTT-Messages:', 'Next line', 'with \\n', 'This is the last line....'],
        // lines: [],
        subTopic: 'ci/text5/set',
        pubTopic: ''
      }
    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for text5 topic---------------------------
    this.text5s.forEach(text5 => {
      const aSubTopic = text5.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---text5 topic found -------------------------------
        text5.iText5State = 0
        let s1 = message.payload
        s1 = s1.replaceAll('\\\\n', '$^~$') // replace "text" \n
        let anzN = s1.match(/\\n/g)?.length
        if (typeof anzN === 'undefined') anzN = 0
        console.log('Anzahl \\n=', anzN)
        if (anzN < 2) {
          // ---------only (title+) one long line: split it up to lines-
          let iLine = 0
          if (anzN === 1) {
            // ----there is a title-----------------------------
            const aHelp = s1.split('\\n')
            text5.lines[0] = aHelp[0].replaceAll('$^~$', '\\\\n')
            s1 = aHelp[1]
            text5.iText5State = 2
            iLine = 1
          }
          s1 = s1.replaceAll('$^~$', '\\\\n')
          const max = geo0.tmax
          let iStart = 0
          for (let iL = iLine; iL < 5; iL++) {
            let iEnd = iStart + max
            if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
            else {
              if (s1.at(iEnd) === ' ') iEnd++
            }
            text5.lines[iL] = s1.substring(iStart, iEnd)
            iStart = iEnd
          }
          if (text5.iText5State !== 2) text5.iText5State = 1
        } else {
          // ------payload contains \n (next line) commands-----
          s1 = s1.replaceAll('$^~$', '\\\\n')
          text5.lines = s1.split('\\n')
          for (let i = 0; i < text5.lines.length; i++) {
            text5.lines[i] = text5.lines[i].replaceAll('$^~$', '\\n')
          }
          text5.iText5State = 1
        }
      } // END text5 topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciText5Controller = new CiText5Controller()
