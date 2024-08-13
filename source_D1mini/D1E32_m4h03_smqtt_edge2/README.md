<a href="./LIESMICH.md">==> Deutsche Version</a>   
Last update: August 13, 2024 <a name="up"></a>   
<h1>ESP32: Power measurement through pulse counting and MQTT info</h1>   

# Aim
This program for a D1mini or ESP32 realises a power measurement by pulse counting, displays the power on a 0.66 inch OLED and regularly sends MQTT messages with the measured power.   

Meters from the French company Sagemcom, such as the S210 (single-phase) or TR210-D-r (three-phase), are used in Austria to measure the electrical energy consumption of households. These meters have an 
LCD display and two LEDs that indicate the active and reactive energy by impulse (1000 pulses/kWh or 1000 pulses/kVArh). The first LED can be used for power measurement by pulse counting.   

## Overview
* [Required hardware](#a10)   
* [Quick guide](#a20)   
* [Power calculation](#a30)   
* [DIY board "NE555-Schmitt-Trigger"](#a40)   
* [Programme details](#a50)   

<a name="a10"></a>   

# Required hardware
1. D1 mini or ESP32 D1 mini   
2. 1x [DIY board D1_NE555 Schmitt trigger](#a40) with light-sensitive resistor (LDR) or photo LED (photo transistor)   
3. 1x OLED display with SSD1306 controller (e.g. 0.66" display with 64x48 pixels)   

![ESP32 power measurement](./images/300_D1_NE555_ST_power.png "ESP32 power measurement")   
_Figure 1: ESP32 with D1_NE555 Schmitt trigger and 0.66" OLED shield_   

[Top of page](#up)
<a name="a20"></a>   

# Quick guide
## Preparing the hardware

1. solder the socket connectors to the D1 mini or ESP32 D1 mini processor board.   
2. assemble the NE555-Schmitt-Trigger DIY board according to the instructions [(see below)](#a40).   
3. plug the NE555-Schmitt-Trigger self-assembly board and 0.66" OLED shield onto the processor board.   
4. connect the light sensor (LDR or photo LED).   
5. connect the processor board to the USB port of the PC/laptop.   

## Preparation of the software
1. download this project and open it in Visual Studio Code (with PlatformIO installed).   

2. in the programme `D1E32_m4h03_smqtt_edge2.ino` select the correct `#define` line for the selected processor (comment out other lines), e.g:   
```   
#define D1MINI 1 // ESP8266 D1mini +pro
//#define ESP32D1 2 // ESP32 D1mini
```   

3. select the processor in the footer by clicking on `env:` (`env:d1_mini` or `env:wemos_d1_mini32`)

4. enter the WLAN data in the following line:   
```   
SimpleMqtt client(String("Raspi11"),String("12345678"),String("10.1.1.1"),String(TOPIC_BASE));
```   
  * Parameter 1: Name of the WLAN (WiFi)   
  * Parameter 2: Password of the WLAN (WiFi)   
  * Parameter 3: IP of the MQTT broker   
  * Parameter 4: Basic topic for MQTT communication   

5. compile the project and load it onto the processor (right arrow symbol `->`). The processor starts the programme automatically.   

After starting the programme, the D1 mini/ESP32 searches for the specified WLAN and logs on to the MQTT broker.   

The processor now periodically sends the power values.   

If you want to know the current status of the power measurement, you can request this with the following message:   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt`.   
Response e.g:
`test/watt/ret/watt 123`   

__assumptions__:   
* The Mosquitto clients are installed on the computer.   
* The base topic is `test/watt`.   

[Top of page](#up)
<a name="a30"></a>   

# Calculation of the power
## Assumption
Meter constant z = 1000 pulses/kWh = 1 pulse/Wh   

## Calculation of the instantaneous power P<sub>1</sub>
The instantaneous power P<sub>1</sub> is the power resulting from the time &Delta;t<sub>1</sub> between two pulses:   
z = 1 Pulse/Wh   
z = 1 Pulse / (3600 Ws)   
3600 = 1 / (P1*t1)   
P<sub>1</sub> = 3600 / &Delta;t<sub>1</sub> &nbsp; &nbsp; . . . . . with P<sub>1</sub> in watt and &Delta;t<sub>1</sub> in seconds.   

_Calculation examples:_   

| Time between two pulses | Power | Current @ 230V |   
| ----------------------- | ----- | -------------- |   
|   0,1 s        |   36 kW | 156,52 A |   
|   0,2 s        |   18 kW |  78,26 A |   
|   0,5 s        | 7200 W  |  31,30 A |   
|   1 s          | 3600 W  |  15,65 A |   
|   2 s          | 1800 W  |   7,83 A |   
|   5 s          |  720 W  |   3,13 A |   
|  10 s          |  360 W  |   1,57 A |   
|  20 s          |  180 W  |   0,78 A |   
|  30 s          |  120 W  |   0,52 A |   
|  60 s (1 min)  |   60 W  |   0,26 A |   
| 100 s          |   36 W  |   0,16 A |   
| 200 s          |   18 W  |   0,08 A |   
| 300 s (5 min)  |   12 W  |   0,05 A |   
| 600 s (10 min) |    6 W  |   0,03 A |   
| 900 s (15 min) |    4 W  |   0,02 A |   

As you can see, the time between two pulses is very long for low powers, and for a power of P1 = 0 W the time is even "infinite".   

## Average power for a period &Delta;t<sub>n</sub>
If you want to determine the (average) power for a fixed period &Delta;t<sub>n</sub>, you count the pulses n in this period and then divide by this time:   
P<sub>n</sub> = 3600 * n / &Delta;t<sub>n</sub> &nbsp; &nbsp; . . . . . with P<sub>n</sub> in watt and &Delta;t<sub>n</sub> in seconds.   

## MQTT messages for power measurement
MQTT requests can be used to query the instantaneous and average power:    
__Example__: Querying the instantaneous power   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt`   
Answer e.g:   
`test/watt/ret/watt 123`   

__Example__: Querying the average power   
`mosquitto_pub -h 10.1.1.1 -t test/watt/get -m watt_`   
Answer e.g:   
`test/watt/ret/watt 123`   
_Note_: The answer is given with `/ret/watt` and not `/ret/watt_`.

In addition, the average power is automatically sent every 5 minutes:   
`test/watt/ret/watt5 123`   
_Note_: The message is sent with `/ret/watt5`.

[Top of page](#up)
<a name="a40"></a>   

# DIY board "NE555-Schmitt-Trigger"
The DIY board "NE555-Schmitt-Trigger" is used to sharpen the output signal of the sensor (LDR). This is useful because the power calculation software works with edge detection.   

![D1_NE555_ST circuit](./images/300_D1_NE555_ST_circuit.png "D1_NE555_ST circuit")   
_Figure 2: Circuit of the D1_NE555_ST shield (board)_   

_Components:_   
* 1x NE555 with 8-pin socket   
* 1x LDR (light-sensitive resistor) with 2 k&Omega; at medium brightness   
* 1x small trimmer (upright) R1 with 10 k&Omega;   
* 1x capacitor C1 with 10 nF   
* 1x capacitor C2 with 100 nF   
* 1x 2-pole pin header J1, 90° angled   
* Low-current LED D1 (3mm, 2mA) and series resistor 1 k&Omega;   

According to the data sheet, the output voltage of the LM555 is 3.3V at a supply voltage of 5V (measured 3.6V), so that the output voltage can be applied directly to the input pin of the processor.   
The switching thresholds of the Schmitt trigger are 1/3 and 2/3 of the supply voltage, i.e. approx. 1.7V and 3.3V. With the help of the trimmer R1 
the switching times can be adapted to the ambient brightness.   
The low-current LED D1 (2mA) with series resistor R2 is used to indicate the switching status and can also be omitted.   
The two capacitors C1 and C2 prevent the circuit from oscillating at high frequency.   
If the LDR has a higher impedance, a 50 k&Omega; trimmer must be used.   

The circuit can be built on a D1 mini protoboard.   

![D1_NE555_ST Protoboard](./images/300_D1_NE555_ST_shield.png "D1_NE555_ST Protoboard")   
_Figure 3: Structure of the D1_NE555_ST shield on a D1 mini Protoboard_   

[Top of page](#up)
<a name="a50"></a>   

# Program details

## Development environment
The program was created in Visual Studio Code and PlatformIO.   

## Program sections
The program consists of six sections.   

### 1. definitions, declarations and global variables
Hardware-specific things are defined here, depending on whether a D1 mini or ESP32 is used, e.g. whether the LED lights up when the control pin is on 0 (D1 mini) or 1 (ESP32).   
This is followed by definitions for the OLED screen.   

### 2. interrupt functions
This section contains definitions and global variables for use in the interrupt routines as well as the interrupt programs themselves.   
The function `io_ISR` is called on every rising edge at D5, `myTimer_ISR` every 5 minutes.   
The function `startMyTimerInterrupt()` starts the timer, whereby the coding depends on the processor used.   

### 3. MQTT functions
This area creates a SimpleMqtt object and defines the reaction of the program to all MQTT messages.    

### 4. display functions
The two functions `showLine(int line_, String text_)` and `refreshScreen()` simplify the control of the OLED display.   
With `refreshScreen()`, the OLED screen is cleared and redrawn every five minutes, which prevents screen contents from burning in.   

### 5. further functions
The function `getsWatt5()` calculates the average power of the last 5 minutes and returns the value as a string.   

### 6. standard functions
This section contains the standard functions `setup()` and `loop()`.   

[Top of page](#up)
