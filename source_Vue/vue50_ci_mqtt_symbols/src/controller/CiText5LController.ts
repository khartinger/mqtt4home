// ______CiText5LLController.ts__________________________________
import { Message } from '@/services/CiMqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

export interface Text5L extends IBase {
  iText5LState: number;
  lines: string[];
}

export class CiText5LController extends CiBaseController {
  public text5Ls: Array<Text5L> = reactive(
    [
      {
        id: 'text5L_1',
        name: 'Text5L_1',
        iText5LState: -1,
        lines: ['Text5L_1', 'MQTT-Messages:', 'Next line', 'with \\n', 'This is the last line....'],
        subTopic: 'ci/text5L/set',
        pubTopic: ''
      }
    ]
  );

  // geo = new Geo(0, 0);

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for text5L topic---------------------------
    this.text5Ls.forEach(text5L => {
      const aSubTopic = text5L.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---text5L topic found -------------------------------
        text5L.iText5LState = 0
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
            text5L.lines[0] = aHelp[0].replaceAll('$^~$', '\\\\n')
            s1 = aHelp[1]
            text5L.iText5LState = 2
            iLine = 1
          }
          s1 = s1.replaceAll('$^~$', '\\\\n')
          const max = 2 * geo0.tmax
          let iStart = 0
          for (let iL = iLine; iL < 5; iL++) {
            let iEnd = iStart + max
            if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
            else {
              if (s1.at(iEnd) === ' ') iEnd++
            }
            text5L.lines[iL] = s1.substring(iStart, iEnd)
            iStart = iEnd
          }
          if (text5L.iText5LState !== 2) text5L.iText5LState = 1
        } else {
          // ------payload contains \n (next line) commands-----
          s1 = s1.replaceAll('$^~$', '\\\\n')
          text5L.lines = s1.split('\\n')
          for (let i = 0; i < text5L.lines.length; i++) {
            text5L.lines[i] = text5L.lines[i].replaceAll('$^~$', '\\n')
          }
          text5L.iText5LState = 1
        }
      } // END text5L topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciText5LController = new CiText5LController()
