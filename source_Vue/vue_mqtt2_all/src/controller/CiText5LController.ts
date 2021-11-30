// ______CiText5LLController.ts__________________________________
import { Message } from '@/services/MqttClient'
import { reactive } from 'vue'
import { CiBaseController, IBase } from './CiBaseController'
import { Geo } from '../components/CiBase.vue'

export interface Text5L extends IBase {
  iText5LState: number;
  lines: string[];
}

export class CiText5LController extends CiBaseController {
  public text5Ls: Array<Text5L> = reactive(
    [
      {
        id: 'text5L',
        name: 'Text5L_1',
        iText5LState: -1,
        lines: [],
        subTopic: 'ci/text5L/set'
      }
    ]
  );

  geo = new Geo();

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for text5L topic---------------------------
    this.text5Ls.forEach(text5L => {
      const aSubTopic = text5L.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---text5L topic found -------------------------------
        let s1 = message.payload
        const i1 = s1.indexOf('\\|')
        const i2 = s1.indexOf('/|')
        if (i1 >= 0 || i2 >= 0) {
          s1 = s1.replaceAll('\\|', '$^~$')
          s1 = s1.replaceAll('/|', '$^~$')
          text5L.lines = s1.split('|')
          for (let i = 0; i < text5L.lines.length; i++) {
            text5L.lines[i] = text5L.lines[i].replaceAll('$^~$', '|')
          }
        } else {
          text5L.lines = s1.split('|')
        }
        // ---only one long line?-------------------------------
        if (text5L.lines.length === 1) {
          // -split one long line up into max. 5 short lines----
          const len = text5L.lines[0].length
          const charmax = 2 * this.geo.tmax + 2
          // console.log('Text5LController: len=', len + ', max=', max)
          if (len > charmax) {
            const s0 = text5L.lines[0]
            for (let i3 = 0; i3 < 5; i3++) {
              text5L.lines[i3] = s0.substr(i3 * charmax, charmax)
            }
            text5L.iText5LState = 1
          }
        }
        // console.log('Text5LController: text5L.lines = ', text5L.lines)
      } // END text5L topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0)
  }
}

export const ciText5LController = new CiText5LController()
