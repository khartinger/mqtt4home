// ______CiTextareaController.ts___________________khartinger_____
// 2022-08-14: New
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'
import { geo0 } from '@/components/CiBase.vue'

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

  // ---------Message for this ci (control/indicator)?----------
  public onMessage (message: Message): void {
    // -------search for textarea topic---------------------------
    this.textareas.forEach(textarea => {
      const aSubTopic = textarea.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---textarea topic found -----------------------------
        textarea.payload = message.payload
      } // END textarea topic found
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('publish:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiTextareaController: ERROR:', e) })
  }
}

export const ciTextareaController = new CiTextareaController()
