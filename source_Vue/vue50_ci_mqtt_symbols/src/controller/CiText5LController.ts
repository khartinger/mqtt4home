// ______CiText5LController.ts___________________khartinger_____
// 2022-08-11
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

export interface Text5L extends IBase {
  iText5LState: number; // 0=no title, 1=name, 2=id, 4=message
  lines: string[];
}

export class CiText5LController extends CiBaseController {
  public text5Ls: Array<Text5L> = reactive(
    [
      {
        id: 'text5_1',
        name: 'Text5_1',
        iText5State: 1,
        lines: ['Text5_1', 'MQTT-Messages:', 'Next line', 'with \\n', 'This is the last line....'],
        // lines: [],
        subTopic: 'ci/text5/set',
        pubTopic: ''
      }
    ]
  );

  // ----split a string to a array of strings with max len------
  // s1: given string
  // numCharPerLine: max number of char per element in a1
  public splitToArray (s1: string, numCharPerLine: number): Array<string> {
    const a1: string[] = []
    let i1 = 0 //            Index of char in s1
    let iStart = 0 //        Index of start char in s1 of this line
    let ia1 = 0 //           Index of line in a1
    // console.log('splitToArray s1: ', s1)
    // console.log('splitToArray numCharPerLine: ', numCharPerLine)
    const lens1 = s1.length
    for (i1 = 0; i1 < lens1; i1++) {
      let iEnd = iStart + numCharPerLine
      if (iEnd > lens1) {
        iEnd = lens1
        i1 = lens1 // end of for loop
      }
      if (s1.at(iEnd - 2) === ' ' && s1.at(iEnd) !== ' ') iEnd = iEnd - 1
      else {
        if (s1.at(iEnd) === ' ') iEnd++
      }
      a1[ia1++] = s1.substring(iStart, iEnd)
      iStart = iEnd
    }
    // console.log('splitToArray a1: ', a1)
    return a1
  }

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for text5L topic---------------------------
    this.text5Ls.forEach(text5L => {
      const aSubTopic = text5L.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---text5L topic found -------------------------------
        const maxCharPerLine = 2 * geo0.tmax
        const maxLines = 5 // maximum of display lines
        for (let iL = 0; iL < maxLines; iL++) text5L.lines[iL] = ' '
        // ...split payload into parts ("payload lines")........
        let s1 = message.payload
        s1 = s1.replaceAll('\\n', '^~') // replace "text" \n
        let aPay: string[] = []
        aPay = s1.split('^~')
        if (aPay === null) {
          let aPay: string[] = []
          aPay[0] = s1
        }
        let iPay = 0 //                index of payload "lines"
        const numPay = aPay.length
        // console.log('Anzahl \\n=', numPay)
        // console.log('Payload: ', aPay)
        // ---should line 1 be a title?-------------------------
        let iLine = 0 //               index of lines to display
        if (text5L.iText5LState === 1) { // title = name
          if (text5L.name) text5L.lines[iLine++] = text5L.name
          else text5L.lines[iLine++] = ''
        }
        if (text5L.iText5LState === 2) { // title = id
          text5L.lines[iLine++] = text5L.id
        }
        if (text5L.iText5LState === 4) { // title = payload line 0
          text5L.lines[iLine++] = aPay[iPay++]
        }
        // ---convert payload lines into display lines----------
        let aHelp: string[] = []
        for (let i1 = iLine; i1 < maxLines; i1++) {
          if (!aPay[iPay]) break
          aHelp = this.splitToArray(aPay[iPay++], maxCharPerLine)
          // console.log('aHelp: ', aHelp)
          const lenHelp = aHelp.length
          // -transfer help lines to display lines--------------
          for (let i2 = 0; i2 < lenHelp; i2++) {
            text5L.lines[iLine++] = aHelp[i2]
            if (iLine > maxLines) { // finished. End for loops
              i2 = lenHelp
              i1 = maxLines
            }
          }
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
