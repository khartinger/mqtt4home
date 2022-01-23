Last modified: 2022-01-23   
<table><tr><td><img src="logo/mqtt4home_96.png"></td><td>&nbsp;</td><td>
<h1>Raspberry Pi: Sending and receiving SMS via MQTT in C++</h1>
<a href="../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="m4h10_RasPiCppSms.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table><hr>

## Targets
If you want to receive an SMS in certain situations in home automation or control things via SMS, the program `m4hSms` presented here is very helpful for this:   
* The program `m4hSms` converts SMS to MQTT messages and vice versa.   
* Many properties of the program can be set in the configuration file (`m4h.conf`).   
* With a little C++ knowledge the program can easily be adapted to own needs.   

## Required tools (status August 2021)
* Hardware: RasPi
* Hardware: SIM808 module GSM with GPS antenna for 3G 4G SIM card e.g. from [AliExpress](https://de.aliexpress.com/item/1005002384541464.html?spm=a2g0s.9042311.0.0.5c824c4dqUu43E) or [Amazon](https://www.amazon.de/dp/B09CM8TSX9/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B09CM8TSX9&pd_rd_w=yNPjf&pf_rd_p=4f2ceb27-95e9-46ab-8808-db390b56ec01&pd_rd_wg=Muvvr&pf_rd_r=3NTH9ZQRZNE3VBZKD1YV&pd_rd_r=58ca39f9-b9f0-40b5-9b14-7f5d8a37ce1d&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzN1A5OTA0NUNBTTA2JmVuY3J5cHRlZElkPUEwMzcxNzc3MlkxMFpaUTBTRjhYMSZlbmNyeXB0ZWRBZElkPUEwNzU2MTYzMjRJSlNTREJMTjVHSiZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* Hardware: Breadboard jumper wires female to female 4pin e.g. from [Amazon](https://www.amazon.de/Female-Female-Male-Female-Male-Male-Steckbr%C3%BCcken-Drahtbr%C3%BCcken-bunt/dp/B01EV70C78/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Breadboard+Jumper+Dr%C3%A4hte&qid=1629911840&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzRVMzTE5FRDJTWjJSJmVuY3J5cHRlZElkPUEwODQwNzA5MUZGVVRTNVE1WDBBTiZlbmNyeXB0ZWRBZElkPUExMDAwNjg1MUFSNzFTUlhGM0hSOSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)   
* Hardware: DSD TECH USB to TTL Serial Adapter Converter SH-U09C with FTDI FT232RL e.g. from [Amazon](https://www.amazon.de/gp/product/B07BBPX8B8/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1) or DSD TECH USB to TTL Serial Adapter Converter SH-U09C5 e.g. from [Amazon](https://www.amazon.de/DSD-TECH-SH-U09C5-Konverterkabel-Unterst%C3%BCtzung/dp/B07WX2DSVB/ref=sr_1_10?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=DSD+TECH+USB+zu+TTL+Seriell+Adapter+Konverter&qid=1629910857&s=computers&sr=1-10)   
* Hardware: USB extension cable USB A male to A female e.g. [PIPIKA USB 3.0 extension from Amazon](https://www.amazon.de/Verl%C3%A4ngerung-Verl%C3%A4ngerungskabel-Superschnelle-Vergoldeten-Kartenleseger%C3%A4t/dp/B08BHWJLLS/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=USB+verl%C3%A4ngerungskabel+A-Buchse+1m&qid=1629911382&smid=A3PTYKYXFT73EG&sr=8-4)   
* Hardware: SIM card e.g. [in Austria HOT from Hofer/Aldi](https://www.hot.at/tarife.html?gclid=CjwKCAjw1JeJBhB9EiwAV612y80Gd6MxAkyvJRJ2BAeTtinSp9OnIBslTwBgd_B-iJANhbW7v5TQSBoCckgQAvD_BwE)  with tariff "HoT flex-our basic tariff without fixed costs".   
---   
* Software: Internet access to GitHub.
* Software: Terminal program [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) on the PC/laptop
* Software: Possibly   
   [Visual Studio Code](https://code.visualstudio.com/) and   
   [WinSCP](https://winscp.net/eng/docs/lang:de) for data transfer from the PC/laptop to the RasPi   

## Description of the hardware
At least a SIM module and a SIM card are required to send and receive SMS. The SIM module can either
* be connected to the pins of the RasPi (pin 2-5V, pin 6-GND, pin 8-TxD0 and pin 10-RxD0), where a level adjustment 5V/3V3 is required, or   
* can be connected to the RasPi via a USB-Serial-Adapter.   

The connection via a USB-Serial-Adapter is more expensive (because of the costs for the adapter), but easier.   

![Module SIM808 with USB-Serial-Adapter](./images/210825_SIM808_480b.png "Module SIM808 with USB-Serial-Adapter")   
_Fig. 1: USB-Serial-Adapter SH-U09C, module SIM808 and GSM antenna (from left to right)_   
   
The jumper on the USB-Serial-Adapter must be set to 5V, because the supply voltage of the SIM module must be between 5 and 18V. (3.3V is too low).  

When connecting the SIM module to the USB-Serial-Adapter or RasPi you have to take care that the pins TxD are connected to RxD and RxD to TxD ("cross out" the data lines).   

Furthermore, a SIM card is required, whereby a prepaid card is sufficient for many applications, where no fixed costs are incurred. The SIM card is inserted into the SIM module (see Figure 1, bottom left of the module).   

If you have plugged the SIM module into the RasPi, you can use the program [`
m4hFindSimModule`](https://github.com/khartinger/mqtt4home/tree/main/source_RasPi/m4hFindSimModule.) to find out as which "device" the module can be addressed:   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
So in this example the device name is `/dev/ttyUSB0`.   
_Note_: The creation of the program `m4hFindSimModule` is done with   
`g++ m4hFindSimModule.cpp -o m4hFindSimModule`.   

&nbsp;   
## Creation of the program m4hSms
The creation of the executable file is done the same way as it was described in the chapter [Raspberry Pi: Helpful single programs in C++](m4h08_RasPiCppDemos.md) as an example:   
1. create a project directory `mkdir ~/m4hSms`.   
2. change to the project directory `cd ~/m4hSms`   
3. copy the project files from GitHub to the project directory   
4. create the executable `m4hSms` file  
   `g++ m4hMain.cpp m4hBase.cpp ./modem/C_X232.cpp ./modem/C_Gsm.cpp -o m4hSms -lmosquitto -lpthread`   

## Description of configuration options
The configuration of the program m4hSms is done with the configuration file `m4h.conf`. The following entries are required in the section `[sms]`:   
* `device:` Interface to which the SIM module is connected. Possible values are `ttyS0` for the serial interface or `ttyUSB0`, `ttyUSB1` etc. for the USB interface.   
* `from: ` List of phone numbers from which SMS may be received.   
   (phone numbers separated by commas, numbers start with +...).
* `to: ` List of phone numbers to which SMS may be sent.   
   (phone numbers separated by commas, numbers start with +...).
* `sub: ` Topic for sending an SMS. The payload must contain the phone number, a space and then the desired SMS text.   
* `pub: ` Topic under which incoming SMS will be sent, if no topic was specified in the SMS.   
   Structure of an SMS with given topic: `-t topic -m payload`.   
   `-t ` stands for topic, `-m ` stands for payload. If another `-r` follows, the retain flag is set.   
---   
The following entries are optional:   
* `smsStart: ` Send an SMS to the specified phone number when the program `m4hSms` has been started, e.g. `smsStart: +43..... program m4hSms started!`   
* `smsEnd: ` Sending SMS to the given phone number when the program `m4hSms` was finished, e.g. `smsEnd: +43..... program m4hSms finished!`   
* `cmdversion:` defines an SMS text which causes a reply SMS with the version number of the program `m4hSms`.   
* `cmdend:` defines an SMS text that causes the termination of the program `m4hSms`.   
* `cmdreload:` defines an SMS text that causes the reloading of the configuration file `m4h.conf`.   
* `cmdcredit:` defines an SMS text that causes the credit to be queried from the provider and the amount (in Euro) to be sent back as a reply SMS.   

   
_Note_: All keys like `device`, `from`, `to` etc. are defined in the source code of the file `C_Sms.hpp` with   
`#define SMS_..._KEY ...`   
and can easily be adapted to your own needs.   

### Detailed example configuration:
```   
________m4h.conf________________________________khartinger_____
# Configuration file for mqtt4home
# 2021-08-27

[base]
versionIn:  m4hSms/get version
versionOut: m4hSms/ret/version 2021-08-27
mqttStart:  info/start m4hSms
mqttEnd:    info/end__ m4hSms
ProgEnd:    m4hSms/set -end-
addTime:    true

[sms]
# (serial) interface for sim module (ttyS0, ttyUSB0, ttyUSB1)
#device: /dev/ttyS0
device: /dev/ttyUSB0
# authorisized phone numbers
from: 6700,+43.....
to: 6700,+43.....
# mqtt base topics for sending and receiving
# sub-topic with payload num txt --> sms
# --> sms send result published under pub
# sms (cmd, plain)          --> publish with topic pub
# sms (-t topic -m payload) --> publish topic payload
sub: sms/send
pub: sms/send/ret
smsStart: +43..... program m4hSms started!
smsEnd:   +43..... program m4hSms finished!
# sms or mqtt commands to which the program responds
cmdversion: -version-
cmdend:     -end-
cmdreload:  -reload-
cmdcredit:  -credit-
#netid:      T-Mobile A
#netphone:   6700
#nettext:    GUTHABEN
```   
_Note_: Lines starting with # are comment lines and will be ignored.   
