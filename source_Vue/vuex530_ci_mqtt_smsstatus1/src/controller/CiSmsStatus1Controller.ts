// ______CiSmsStatus1Controller.ts_______________khartinger_____
// 2023-03-15: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface SmsStatus1 extends IBase {
  // ---------mandatory-----------------------------------------
  iSmsStatusState: number;
  // ---------optional------------------------------------------
  textUpper?: string;
  textMiddle?: string;
  textLower?: string;
  textFooter?: string;
  pubTopic2?: string;
  pubPayload2?: string;
  pubTopic3?: string;
  pubPayload3?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiSmsStatus1Controller extends CiBaseController {
  public z2Mstatus1s: Array<SmsStatus1> = reactive(
    [
      {
        id: 'smsstatus1',
        name: 'Status GSM Modem',
        iSmsStatusState: -99,
        textUpper: 'Modem Status',
        textMiddle: 'SIM Status',
        textLower: 'GUTHABEN',
        subTopic: 'sms/cmd/ret/module sms/cmd/ret/sim sms/cmd/ret/credit',
        pubTopic: 'sms/cmd/get',
        pubPayload: 'module',
        pubTopic2: 'sms/cmd/get',
        pubPayload2: 'sim',
        pubTopic3: 'sms/cmd/get',
        pubPayload3: 'credit'
      }
    ]
  );

  // _________state constants___________________________________
  public maskSmsModule = 0x01;
  public maskSmsSim = 0x02;
  public maskSmsCredit = 0x04;
  public maskSmsValidModule = 0x10;
  public maskSmsValidSim = 0x20;
  public maskSmsValidCredit = 0x40;
  public maskSmsValidAny = 0x70;

  // _________react on incomming MQTT messages__________________
  public onMessage (message: Message): void {
    this.z2Mstatus1s.forEach(smsstatus1 => {
      const aSubTopic = smsstatus1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ========smsstatus1 found=============================
        if (smsstatus1.iSmsStatusState < 0) smsstatus1.iSmsStatusState = 0
        if (message.topic.includes('/ret/module')) {
          // ------module state---------------------------------
          try {
            // payload: {"device":"/dev/ttyUSB_Modem","module":true,"product":"SIM808 R14.18"}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.module) {
              smsstatus1.textFooter = aPayload.device
              smsstatus1.iSmsStatusState |= this.maskSmsModule
            } else {
              smsstatus1.textFooter = 'No device'
              smsstatus1.iSmsStatusState &= ~this.maskSmsModule
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidModule
            console.log('i_=', smsstatus1.iSmsStatusState, ' -t ', message.topic, 'pay: ', aPayload)
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidModule
          }
        }
        if (message.topic.includes('/ret/sim')) {
          // ------sim state------------------------------------
          try {
            // payload: {"provider":"T-Mobile A","sim":true,"result":"^~+COPS: 0,0,'T-Mobile A'^~^~OK^~"}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.sim) {
              smsstatus1.textFooter = aPayload.provider
              smsstatus1.iSmsStatusState |= this.maskSmsSim
            } else {
              smsstatus1.textFooter = 'No provider'
              smsstatus1.iSmsStatusState &= ~this.maskSmsSim
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidSim
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidSim
          }
        }
        if (message.topic.includes('/ret/credit')) {
          // ------sim state------------------------------------
          try {
            // payload: {"euro":33.25}
            const aPayload = JSON.parse(message.payload)
            if (aPayload.euro >= 0) {
              smsstatus1.textFooter = `${aPayload.euro}`
              smsstatus1.iSmsStatusState |= this.maskSmsCredit
            } else {
              smsstatus1.textFooter = 'No Info'
              smsstatus1.iSmsStatusState &= ~this.maskSmsCredit
            }
            smsstatus1.iSmsStatusState |= this.maskSmsValidCredit
          } catch (error) {
            smsstatus1.iSmsStatusState &= ~this.maskSmsValidCredit
          }
        }
      } // ===END of smsstatus1 found===========================
    })
  }

  // _________publish a MQTT message____________________________
  public publishCi (topic: string, payload: string): void {
    // console.log('CiSmsStatus11Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiSmsStatus1Controller: ERROR:', e) })
  }
}

export const ciSmsStatus1Controller = new CiSmsStatus1Controller()
