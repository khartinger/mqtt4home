// ______CiTextareaController.ts___________________khartinger_____
// 2022-08-14: New
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'
// import CiTextarea from '@/components/CiTextarea.vue'

export interface Textarea extends IBase {
  iTextareaState: number; // 0=no title, 1=name, 2=id, 4=message
  payload: string;
}

export class CiTextareaController extends CiBaseController {
  public textareas: Array<Textarea> = reactive(
    [
      {
        id: 'textarea_1',
        name: 'textarea',
        iTextareaState: 4,
        payload: '***Textarea***\nThis is Line2\nLine3: ÄqÖpÜg---6789|\n123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|',
        subTopic: 'ci/textarea/set',
        pubTopic: '',
        pubPayload: ''
      }
    ]
  );
  /*
  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for textarea topic---------------------------
    this.textareas.forEach(textarea => {
      const aSubTopic = textarea.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea topic found -----------------------------
        textarea.payload = message.payload
        textarea.newPayload = true
      } // END textarea topic found
    })
  }
  */

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
    // -------search for textarea topic---------------------------
    this.textareas.forEach(textarea => {
      const aSubTopic = textarea.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea topic found -----------------------------
        textarea.payload = message.payload
        /*
        let s1 = message.payload
        s1 = s1.replaceAll('\\n', '^~') // replace "text" \n
        let aPay: string[] = []
        aPay = s1.split('^~')
        if (aPay === null) {
          const aPay: string[] = []
          aPay[0] = s1
        }
        let iPay = 0 //                index of payload "lines"
        const numPay = aPay.length
        // console.log('Anzahl \\n=', numPay)
        // console.log('Payload: ', aPay)
        // ---should line 1 be a title?-------------------------
        let iLine = 0 //               index of lines to display
        if (textarea.iTextareaState === 1) { // title = name
          if (textarea.name) textarea.lines[iLine++] = textarea.name
          else textarea.lines[iLine++] = ''
        }
        if (textarea.iTextareaState === 2) { // title = id
          textarea.lines[iLine++] = textarea.id
        }
        if (textarea.iTextareaState === 4) { // title = payload line 0
          textarea.lines[iLine++] = aPay[iPay++]
        }
        // ---convert payload lines into display lines----------
        let aHelp: string[] = []
        for (let i1 = iPay; i1 < numPay; i1++) {
          if (!aPay[iPay]) break
          aHelp = this.splitToArray(aPay[iPay++], maxCharPerLine)
          // console.log('aHelp: ', aHelp)
          const lenHelp = aHelp.length
          // -transfer help lines to display lines--------------
          for (let i2 = 0; i2 < lenHelp; i2++) {
            textarea.lines[iLine++] = aHelp[i2]
            if (iLine > maxLines) { // finished. End for loops
              i2 = lenHelp
              i1 = numPay
            }
          }
        }
      */
      } // END textarea topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTextareaController: ERROR:', e) })
  }
}

export const ciTextareaController = new CiTextareaController()
