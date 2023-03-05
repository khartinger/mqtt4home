// ______CiZ2Mstatus1Controller.ts_______________khartinger_____
// 2023-02-28: new
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Z2Mstatus1 extends IBase {
  // ---------mandatory-----------------------------------------
  iZ2MstatusState: number;
  // ---------optional------------------------------------------
  textUpper?: string;
  textLower?: string;
  pubTopic2?: string;
  pubPayload2?: string;
  // Note: default pubPayload means pubPayloadUpper!
}

export class CiZ2Mstatus1Controller extends CiBaseController {
  public z2Mstatus1s: Array<Z2Mstatus1> = reactive(
    [
      {
        id: 'z2mstatus1',
        name: 'Status zigbee2MQTT',
        iZ2MstatusState: -99,
        textUpper: 'get Status Z2M',
        textLower: 'Restart Z2M ',
        subTopic: 'z2m/bridge/response/health_check z2m/bridge/response/restart z2m/bridge/state',
        pubTopic: 'z2m/bridge/request/health_check',
        pubPayload: '',
        pubTopic2: 'z2m/bridge/request/restart',
        pubPayload2: ''
      }
    ]
  );

  // _________state constants___________________________________
  public maskZ2mOnline = 0x20;
  public maskZ2mOnlineValid = 0x10;
  public maskZ2mError = 0x08;
  public maskZ2mHealthy = 0x04;
  public maskZ2mStatusOk = 0x02;
  public maskZ2mStatusValid = 0x01;

  // _________react on incomming MQTT messages__________________
  public onMessage (message: Message): void {
    this.z2Mstatus1s.forEach(z2mstatus1 => {
      const aSubTopic = z2mstatus1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ===z2mstatus1 found==================================
        // health check: topic z2m/bridge/response/health_check-
        if (z2mstatus1.iZ2MstatusState < 0) z2mstatus1.iZ2MstatusState = 0
        if (message.topic.includes('health_check')) {
          try {
            // payload: {"data":{"healthy":true},"status":"ok"}
            // console.log(message.payload)
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusValid
            if (message.payload.includes('"healthy"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"healthy":true')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mHealthy
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mHealthy
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
            if (message.payload.includes('"status"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"status":"ok"')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
          }
        }
        // after restart: topic z2m/bridge/response/restart-----
        if (message.topic.includes('restart')) {
          try {
            // payload: {"data":{},"status":"ok"}
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusValid
            if (message.payload.includes('"status"')) {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mError
              if (message.payload.includes('"status":"ok"')) {
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              } else {
                z2mstatus1.iZ2MstatusState &= ~this.maskZ2mStatusOk
                z2mstatus1.iZ2MstatusState |= this.maskZ2mStatusValid
              }
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState |= this.maskZ2mError
          }
        }
        // after restart: topic z2m/bridge/state----------------
        if (message.topic.includes('state')) {
          try {
            // payload: online|offline
            // console.log(message.payload)
            if (z2mstatus1.iZ2MstatusState < 0) z2mstatus1.iZ2MstatusState = 0
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnlineValid
            if (message.payload === 'online') {
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnline
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnlineValid
            }
            if (message.payload === 'offline') {
              z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnline
              z2mstatus1.iZ2MstatusState |= this.maskZ2mOnlineValid
            }
          } catch (error) {
            z2mstatus1.iZ2MstatusState &= ~this.maskZ2mOnlineValid
          }
        }
      } // ===END of z2mstatus1 found===========================
    })
  }

  // _________publish a MQTT message____________________________
  public publishCi (topic: string, payload: string): void {
    // console.log('CiZ2Mstatus11Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiZ2Mstatus1Controller: ERROR:', e) })
  }
}

export const ciZ2Mstatus1Controller = new CiZ2Mstatus1Controller()
