// ______CiTextarea1Controller.ts________________khartinger_____
// 2022-08-14: New
// 2023-02-05: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Textarea1 extends IBase {
  // ---------mandatory-----------------------------------------
  iTextarea1State: number; // 0=no title, 1=name, 2=id, 4=message
  subPayload: string; //      text displayed in textarea
  clkWhenMounted: boolean; // text request on mounting?
  // ---------optional------------------------------------------
  useRawText?: boolean; //    do not wrap text
  textFooter?: string; //     footer text
}

export class CiTextarea1Controller extends CiBaseController {
  public textarea1s: Array<Textarea1> = reactive(
    [
      {
        id: 'textarea1',
        name: 'Textarea1_1',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/1/set ci/textarea1/2/get',
        subPayload: 'Click: publish a text by textarea1 - this is just to make the text longer and longer 1234567890 and once more longer... (END OF TEXT :)',
        pubTopic: 'ci/textarea1/1/get ci/textarea1/2/set',
        pubPayload: 'standard text published by textarea1 :)',
        textFooter: '==Footer=='
      },
      {
        id: 'textarea2',
        name: 'Textarea1_2',
        iTextarea1State: -1, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: true,
        subTopic: 'ci/textarea1/2/set',
        subPayload: 'click sends request ci/textarea1/2/get',
        pubTopic: 'ci/textarea1/2/get',
        pubPayload: 'text'
      },
      {
        id: 'textarea3',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: false,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      },
      {
        id: 'textarea4',
        iTextarea1State: -99, // -1 title=name, -2 title=id, -4 title=payload line 0
        clkWhenMounted: false,
        useRawText: true,
        subTopic: 'ci/textarea1/3/set',
        subPayload: 'If useRawText is not defined or false, words with two, three  or four letters  are not devided.',
        pubTopic: ''
      }

    ]
  );

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for topic textarea1------------------------
    this.textarea1s.forEach(textarea1 => {
      const aSubTopic = textarea1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea1 topic found ----------------------------
        if (textarea1.id === 'textarea1') {
          // -message for id=textarea1--------------------------
          if (message.topic === 'ci/textarea1/2/get') {
            this.publishCi('ci/textarea1/2/set', 'special: requested text from id=textarea1')
          } else textarea1.subPayload = message.payload
        } else {
        // ---message is not for id=textarea1-------------------
          textarea1.subPayload = message.payload
        }
      } // ---END textarea1 topic found-------------------------
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTextarea1Controller: ERROR:', e) })
  }
}

export const ciTextarea1Controller = new CiTextarea1Controller()
