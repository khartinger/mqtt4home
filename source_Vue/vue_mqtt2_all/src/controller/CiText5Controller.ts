// ______CiText5Controller.ts___________________________________
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import geo0 from '@/components/CiBase.vue'

export interface Text5 extends IBase {
  iText5State: number;
  lines: string[];
}

export class CiText5Controller extends CiBaseController {
  public text5s: Array<Text5> = reactive(
    [
      {
        id: 'text5',
        name: 'Text5_1',
        iText5State: -1,
        lines: ['Text5_1', 'Zeilenumbruch', 'mit |...'],
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
        let s1 = message.payload
        const i1 = s1.indexOf('\\|')
        const i2 = s1.indexOf('/|')
        if (i1 >= 0 || i2 >= 0) {
          s1 = s1.replaceAll('\\|', '$^~$')
          s1 = s1.replaceAll('/|', '$^~$')
          text5.lines = s1.split('|')
          for (let i = 0; i < text5.lines.length; i++) {
            text5.lines[i] = text5.lines[i].replaceAll('$^~$', '|')
          }
        } else {
          text5.lines = s1.split('|')
        }
        // ---only one long line?-------------------------------
        if (text5.lines.length === 1) {
          // -split one long line up into max. 5 short lines----
          const len = text5.lines[0].length
          // const max = this.geo.tmax
          const max = geo0.tmax
          // console.log('Text5Controller: len=', len + ', max=', max)
          if (len > max) {
            const s0 = text5.lines[0]
            for (let i3 = 0; i3 < 5; i3++) {
              text5.lines[i3] = s0.substr(i3 * max, max)
            }
            text5.iText5State = 1
          }
        }
        // console.log('Text5Controller: text5.lines = ', text5.lines)
      } // END text5 topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciText5Controller = new CiText5Controller()
