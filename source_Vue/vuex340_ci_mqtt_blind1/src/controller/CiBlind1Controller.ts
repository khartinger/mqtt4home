// ______CiBlind1Controller.ts___________________khartinger_____
// 2022-01-08: new
// 2023-02-20: change at CiBase (add Geo.ts)
import { reactive } from 'vue'
import { Message } from '@/services/CiMqttClient'
import { CiBaseController, IBase } from './CiBaseController'

export interface Blind1 extends IBase {
  // ---------mandatory-----------------------------------------
  iBlind1State: number;
  type: string;
  // ---------optional------------------------------------------
  iMotorState?: number;
  battery?: string;
  textHeader?: string;
  textFooter?: string;
}

export class CiBlind1Controller extends CiBaseController {
  public blind1s: Array<Blind1> = reactive(
    [
      {
        id: 'blind_o',
        type: 'zb',
        iBlind1State: -1, // -1=open, -2=close, -3=lock
        textFooter: '?',
        subTopic: 'z2m/t/blind/-1',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_c',
        type: 'zb',
        iBlind1State: -2,
        textFooter: '?',
        subTopic: 'z2m/t/blind/-2',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_l',
        type: 'zb',
        iBlind1State: -3,
        textFooter: '?',
        subTopic: 'z2m/t/blind/-3',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_bat10',
        type: 'zb',
        iBlind1State: -1,
        battery: '10',
        subTopic: 'z2m/t/blind/-4',
        pubTopic: '',
        pubPayload: '0'
      },
      {
        id: 'blind_bat20',
        type: 'zb',
        iBlind1State: -1,
        battery: '20',
        subTopic: 'z2m/t/blind/-5',
        pubTopic: '',
        pubPayload: '0'
      },

      {
        id: 'blindF',
        name: 'Blind1_Footer',
        type: 'motor',
        iBlind1State: -1,
        iMotorState: -5,
        textFooter: '--Footer1--12345678901234567890',
        subTopic: 'z2m/t/blind/-6  z2m/t/blind/-6/motor',
        pubTopic: '',
        pubPayload: ''
      }

    ]
  );

  // --------------state constants------------------------------
  public stateOpen = -1;
  public stateClose = -2;
  public stateLock = -3;
  public stateMotorDown = -4;
  public stateMotorStop = -5;
  public stateMotorUp = -6;
  public stateNotOk = -998;
  public stateNoState = -999;

  public onMessage (message: Message): void {
    this.blind1s.forEach(blind1 => {
      const aSubTopic = blind1.subTopic.split(' ')
      if (aSubTopic.includes(message.topic)) {
        // ---blind1 topic found -------------------------------
        if (blind1.type === 'zb') {
        // ---blind type 1--------------------------------------
          try {
            const aPayload = JSON.parse(message.payload)
            blind1.battery = `${aPayload.battery}`
            const blindstate_ = String(aPayload.contact)
            if (blindstate_ === 'true') blind1.iBlind1State = this.stateClose
            else {
              if (blindstate_ === 'false') blind1.iBlind1State = this.stateOpen
              else blind1.iBlind1State = this.stateNotOk
            }
          } catch (error) { }
        }
        // ---blind type 2--------------------------------------
        if (blind1.type === 'motor') {
          try {
            console.log('type=motor', message.payload)
            const aPayload = JSON.parse(message.payload)
            // ....for example: blind with open/close info......
            const blindstate_ = String(aPayload.contact)
            blind1.iBlind1State = this.stateNotOk
            if (blindstate_ === 'true') blind1.iBlind1State = this.stateClose
            if (blindstate_ === 'false') blind1.iBlind1State = this.stateOpen
            // ....for example: blind with motor movement info..
            const motor_ = aPayload.motor
            blind1.iMotorState = this.stateNotOk
            if (motor_ === -1) blind1.iMotorState = this.stateMotorDown
            if (motor_ === 0) blind1.iMotorState = this.stateMotorStop
            if (motor_ === 1) blind1.iMotorState = this.stateMotorUp
          } catch (error) { }
        }
        // ---blind type 3--------------------------------------
        if (blind1.type === 'something_else') {
          try {
            // ...add functionality for another sensor here...
          } catch (error) { }
        } // ------END work on different blind1 types-----------
        // ---END blind1 topic found ---------------------------
      }
    })
  }

  public publishCi (topic: string, payload: string): void {
    // console.log('CiBlind1Controller:publishCi:', '-t ' + topic + ' -m ' + payload)
    this.publish(topic, payload, false, 0).catch((e) => { console.error('CiBlind1Controller: ERROR:', e) })
  }
}

export const ciBlind1Controller = new CiBlind1Controller()
