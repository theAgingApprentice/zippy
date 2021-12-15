[return to main page](https://github.com/theAgingApprentice/zippy/blob/main/README.md)

# Circuit components

The Zippy prototype is made up of the following hardware components:

1. NWDZ V2.0 power supply featuring the LM2596 chip.
2. [Dual H-bridge motor drive controller board](https://handsontec.com/dataspecs/module/L298N%20Motor%20Driver.pdf) featuring the L298N chip.
3. 16x2 I2C LCD with a [Hitachi HD44780 LCD control driver](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf) and a [Mantech LCD2004 I2C interface.](https://www.mantech.co.za/datasheets/products/LCD2004-i2c.pdf).
4. Twidec AC SPDT 1NO Short Metal Hinge Roller Lever Arm Switch Snap Action Button Type 2 Pins Mini Micro Limit Switch KW11-3Z09.
5. KeeYees 4 Channel IIC I2C Logic Level Converter Bi-Directional Module 3.3V to 5V Shifter.
6. EMG30 motors with hall effect sensor encoders.
7. Adafruit Huzzah32 feather dev board and termblock featherwing.
8. Rugged Metal On/Off Switch with Blue LED Ring - 16mm Green On/Off.
9. Rugged Metal Pushbutton - 16mm 6V RGB Momentary.

# Circuit architecture

Zippy's circuit architecture can be divided into five functional groupings. These groupings are mobility, balance, power, interface and processor. 

## Mobility circuit

Zippy has a 12VDC closed loop mobility system made up of:

1. Two EMG30 motors requiring 12 VDC input.
2. One Dual H-Bridge motor driver requiring 5 VDC input.
3. Six GPO pins for motor control.
4. Four GPI pins for motor encoders.
5. One bi-directional line level converter for the motor encoders (5 VDC to 3.3 VDC).

## Balance circuit

Zippy's 2DOF balance circuit is made up of:

1. An MPU6050 requiring 3.3VDC input.
2. Optional front & back bumper switches.
3. An I2C bus.
4. Two GPI pins if there are bumper switches.

## Power circuit

Zippy's power circuit is made up of:

1. One Dewalt 18V adapter.
2. One Dewalt DCB230 battery (20 VDC).
3. One Dewalt DCB112 charger.
4. One On/Off switch rated for 3 amps.
5. A 3.3 VDC output rail.
6. A 5.0 VDC outut rail.
7. A 12 VDC outut rail.

## Control circuit

Zippy's control circuit is made up of:

1. One 16x2 LCD.
2. One Reset switch.
3. One RGB LED.
4. An I2C bus.
5. Three GPO pins.

## Processor circuit

Zippy's processor circuit is made upp of:

1. One ESP32 SOC.
2. One Huzzah32 featherboard screw termiinal

# Circuit wiring

The prototype zippy robot does not have a PCB but rather is a collection of development boards and wiring harnesses. The main boards of the circuit are: 

1. Protoboard 1.
2. Protoboard 2.
3. Dual H-bridge board.
4. Huzzah32 with screw terminal breakout board.
5. An NWDZ V2.0 power board.

The two protoboards act as the wiring hub for everything so how the circuit is wired up is best understood by detailing the wiring connetions to those boards.

## Protoboard 1

The header wiring found on protobard 1 (shown in figure 1) is detailed in the table below.

<figure>
 <img src="https://github.com/theAgingApprentice/zippy/blob/main/img/zippyPCB1.png" alt="Protoboard1">
 <figcaption>Figure 1: Zippy protoboard 1.</figcaption>
</figure>

<table>
  <tr>
    <th>Header Label</th>
    <th>Wire</th>
    <th>Purpose</th>
  </tr>
  <tr>
    <td>1</td>
    <td>W3</td>
    <td>5 VDC power rail from power supply.</td>
  </tr>
  <tr>
    <td>2</td>
    <td>W2</td>
    <td>3.3VDC power rail from power supply.</td>
  </tr>
  <tr>
    <td>3</td>
    <td>W13</td>
    <td>3.3VDC power to PCB 2.</td>
  </tr>
  <tr>
    <td>4</td>
    <td>W12</td>
    <td>5 VDC power to PCB 2.</td>
  </tr>
  <tr>
    <td>5</td>
    <td>W7</td>
    <td>Motor 0 connection.</td>
  </tr>
  <tr>
    <td>6</td>
    <td>W8</td>
    <td>Motor 2 connection.</td>
  </tr>
  <tr>
    <td>7</td>
    <td>W9 & W10</td>
    <td>Motor 0 & motor 1 encoders to Huzzah32.</td>
  </tr>
  <tr>
    <td>8</td>
    <td>W9 & W11</td>
    <td>MPU6050 I2C to Huzzah32.</td>
  </tr>
  <tr>
    <td>9</td>
    <td>W14</td>
    <td>16x2 LCD.</td>
  </tr>
  <tr>
    <td>10</td>
    <td>W19</td>
    <td>20VDC from battery.</td>
  </tr>
  <tr>
    <td>11</td>
    <td>W1</td>
    <td>20VDC to power supply.</td>
  </tr>
  <tr>
    <td>12</td>
    <td>W15 & W16</td>
    <td>From power button & blue LED.</td>
  </tr>
  <tr>
    <td>13</td>
    <td>W17 & W18</td>
    <td>From reset button and RGB LED.</td>
  </tr>
  <tr>
    <td>14</td>
    <td>W26</td>
    <td>RGB LEDs to PCB2.</td>
  </tr>
  <tr>
    <td>15</td>
    <td>W6</td>
    <td>Motor 1 PWM control from H-bridge.</td>
  </tr>
  <tr>
    <td>16</td>
    <td>W5</td>
    <td>Motor 0 PWM control from H-bridge.</td>
  </tr>
  <tr>
    <td>17</td>
    <td>W20</td>
    <td>Reset button to Huzzah32.</td>
  </tr>
</table>

## Protoboard 2

The header wiring found on protobard 2 (shown in figure 2) is detailed in the table below.

<figure>
 <img src="https://github.com/theAgingApprentice/zippy/blob/main/img/zippyPCB2.png" alt="Protoboard2">
 <figcaption>Figure 2: Zippy protoboard 2.</figcaption>
</figure>

<table>
  <tr>
    <th>Header Label</th>
    <th>Wire</th>
    <th>Purpose</th>
  </tr>
  <tr>
    <td>1</td>
    <td>W12</td>
    <td>5 VDC power from protoboard 1.</td>
  </tr>
  <tr>
    <td>2</td>
    <td>W13</td>
    <td>3.3 VDC power from protoboard 1.</td>
  </tr>
  <tr>
    <td>3</td>
    <td>W26</td>
    <td>RGB LED from protoboard 1.</td>
  </tr>
  <tr>
    <td>4</td>
    <td>W21</td>
    <td>From front limit switch.</td>
  </tr>
  <tr>
    <td>5</td>
    <td>W22</td>
    <td>From back limit switch.</td>
  </tr>
  <tr>
    <td>6</td>
    <td>W27</td>
    <td>Front limit switch to Huzzah32.</td>
  </tr>
  <tr>
    <td>7</td>
    <td>W27</td>
    <td>Back limit switch to Huzzah32.</td>
  </tr>
  <tr>
    <td>8</td>
    <td>W24</td>
    <td>Motor 0 and 1 control to H-Bridge.</td>
  </tr>
  <tr>
    <td>9</td>
    <td>W25</td>
    <td>Motor 0 and 1 control to Huzzah32.</td>
  </tr>
  <tr>
    <td>10</td>
    <td>W28</td>
    <td>RGB LED to Huzzah32.</td>
  </tr>
  <tr>
    <td>11</td>
    <td>W29</td>
    <td>5 VDC power to Huzzah32 USB port.</td>
  </tr>
</table>

[return to main page](https://github.com/theAgingApprentice/zippy/blob/main/README.md)
