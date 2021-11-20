#ifndef emg30_cpp // Start precompiler code block. 
   #define emg30_cpp // Precompiler macro to prevent duplicate inclusions.

/*
 * Compiler substitution macros.
 ******************************************************************************/
// #define // Put define statements here.


/*
 * Included libraries.
 ******************************************************************************/
#include <main.h> // Header file for all libraries needed by this program.

/**
 * @brief Set up the motor driver wires.
 * @details Initialize the GPIO pins that control the drive motor's speed and
 * direction. These pins are set for output. Also initialize the GPIO pins that
 * monitor the two hall effect encoders per motor which are 90 degrees out of 
 * phase from each other. Wire harness for the EMG30 motors is as follows:
 * 
 * # Wiring Chart
 * 
 * | Wire colour | Purpose              |
 * |-------------|----------------------|
 * | Purple (1)  | Hall Sensor B Vout   | 
 * | Blue (2)    | Hall sensor A Vout   |
 * | Green (3)   | Hall sensor ground   |
 * | Brown (4)   | Hall sensor Vcc      |
 * | Red (5)     | + Motor              |
 * | Black (6)   | - Motor              |
 * 
 * # Specification
 * 
 * | Property        | Value    |
 * |-----------------|----------|
 * | Rated voltage   | 12v      | 
 * | Rated torque    | 1.5kg/cm | 
 * | Rated speed     | 170rpm   | 
 * | Rated current   | 530mA    | 
 * | No load speed   | 216rpm   | 
 * | No load current | 150mA    | 
 * | Stall Current   | 2.5A     | 
 * | Rated output    | 4.22W    | 
 * 
 * Measured Shaft Speed when used off-load with a 12v supply is:
 * 
 * - Minimum Speed = 1.5rpm
 * - Maximum Speed = 200rpm 
 *  
 * # Encoders
 * 
 * The encoder counts per output shaft turn value depends on how the encoders 
 * are used. The documentation claims 360 counts indicating 1 count per 
 * degree but experimentation produces the following proof table: 
 *
 * | Encoder A   | EncoderB    | Resulting cnt |
 * | ISR setting | ISR setting | per full turn |
 * |-------------|-------------|---------------|
 * | RISE        | RISE        |               | 
 * | RISE        | FALL        |               | 
 * | RISE        | BOTH        |               | 
 * | FALL        | RISE        |               | 
 * | FALL        | FALL        |               | 
 * | FALL        | BOTH        |               | 
 * | BOTH        | RISE        |               | 
 * | BOTH        | FALL        |               | 
 * | BOTH        | BOTH        |               | 
 * 
 * When the motor shaft is turning clockwise (CW), channel A is leading, and 
 * when the motor shaft is turning counter clockwise (CCW), channel B is 
 * leading. So what you need to do in your interrupt service routines is read
 * both channels and compare them. In the ISR for channel A, if the signals 
 * are different, then you know that the motor is turning CW. If the signals 
 * are the same then you know that the shaft is turning CCW. In the ISR for 
 * channel B, if the signals are different then you know that the shaft is 
 * turning CCW. If the signals are different then u know that the shaft is 
 * turning CW.
 *  
 * # Additional information
 * 
 * - Tutorial: https://esp32io.com/tutorials/esp32-dc-motor   
 * - Spec sheet: https://www.robot-electronics.co.uk/htm/emg30.htm
 * 
 * @param null
 * @return null
 ******************************************************************************/
void setupMotCntl()
{
   Log.verboseln("<setupMotCntl> Set up motor control pins");
   Log.verboseln("<setupMotCntl> ... Motor 1");
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 0 speed.", G_MOT0_ENA);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 0 direction.", G_MOT0_DIR0);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 0 direction.", G_MOT0_DIR1);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d monitors motor 0 encoder A.", G_MOT0_ENCA);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d monitors motor 0 encoder B.", G_MOT0_ENCB);
   Log.verboseln("<setupMotCntl> ... Motor 2");
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 1 speed.", G_MOT1_ENA);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 1 direction.", G_MOT1_DIR0);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d controls motor 1 direction.", G_MOT1_DIR1);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d monitors motor 1 encoder A.", G_MOT1_ENCA);
   Log.verboseln("<setupMotCntl> ...... GPIO pin %d monitors motor 1 encoder B.", G_MOT1_ENCB);
   // Initialize GPIO pins for motor 0
   pinMode(G_MOT0_ENA, OUTPUT); // Set motor 0 speed pin to output.
   ledcSetup(PWM_MOT1_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Motor 0 PWM.
   ledcAttachPin(G_MOT0_ENA, PWM_MOT1_CHANNEL); // Attach PWM channel to GPIO.
   pinMode(G_MOT0_DIR0, OUTPUT); // Set motor 0 direction1 pin to output.
   pinMode(G_MOT0_DIR1, OUTPUT); // Set motor 0 direction2 pin to output.
   pinMode(G_MOT0_ENCA, INPUT_PULLUP); // Hall effect A, motor 0. Input, weak pullup. 
   pinMode(G_MOT0_ENCB, INPUT_PULLUP); // Hall effect B, motor 0. Input, weak pullup. 
   // Initialize GPIO pins for motor 1
   pinMode(G_MOT1_ENA, OUTPUT); // Set motor 1 speed pin to output.
   ledcSetup(PWM_MOT2_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Motor 1 PWM.
   ledcAttachPin(G_MOT1_ENA, PWM_MOT2_CHANNEL); // Attach PWM channel to GPIO.
   pinMode(G_MOT1_DIR0, OUTPUT); // Set motor 1 direction3 pin to output.
   pinMode(G_MOT1_DIR1, OUTPUT); // Set motor 1 direction4 pin to output.
   pinMode(G_MOT1_ENCA, INPUT_PULLUP); // Hall effect A, motor 1. Input, weak pullup. 
   pinMode(G_MOT1_ENCB, INPUT_PULLUP); // Hall effect B, motor 1. Input, weak pullup. 
   // Initialize encoder interrupts
   attachInterrupt(G_MOT0_ENCA, isrMot0EncA, FALLING); // ISR for Motor 0 encoderA.
   attachInterrupt(G_MOT0_ENCB, isrMot0EncB, FALLING); // ISR for Motor 0 encoderB. 
   attachInterrupt(G_MOT1_ENCA, isrMot1EncA, FALLING); // ISR for Motor 1 encoderA.
   attachInterrupt(G_MOT1_ENCB, isrMot1EncB, FALLING); // ISR for Motor 1 encoderB. 
} // setupMotCntl()

/**
 * @brief Interrupt service routine (ISR) for for motor 1 encoder A.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the motor 1 hall effect sensor A changes. When turning clockwise (CW), 
 * encoder A is leading, and when turning counter clockwise (CCW), encoder B is 
 * leading. So we have an ISR for both encoders. that behave as follows:
 * 
 * # Encoder A ISR
 *
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning clockwise.
 * - If the signals are the same, then the shaft is turning counter clockwise. 
 * 
 * # Encoder B ISR 
 * 
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning counter clockwise.
 * - If the signals are the same, then the shaft is turning clock wise. 
 * 
 * # Running in IRAM
 * 
 * Note that this code is loaded into IRAM (rather than the usual DRAM) using 
 * the IRAM_ATTR attribute in its declaration. This means that it is accessed 
 * via the instruction bus for fast and consistent execution times.
 *  
 * @param null.
 * @return null.   
 ******************************************************************************/
void IRAM_ATTR isrMot0EncA() 
{
   portENTER_CRITICAL(&motorEncoderMutex);
   m0LastMillis = m0CurrMillis;
   m0CurrMillis = millis();
   if(digitalRead(G_MOT0_ENCA) == digitalRead(G_MOT0_ENCB)) // CCW.
   {
      m0CurrCnt --;
      m0Dir = mtrBwd;
   } // if
   else // CW.
   {
      m0CurrCnt ++;
      m0Dir = mtrFwd;
   } // else
//   m0TestCnt ++;
   portEXIT_CRITICAL(&motorEncoderMutex);
} // isrMot0EncA() 

/**
 * @brief Interrupt service routine (ISR) for for motor 1 encoder B.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the motor 1 hall effect sensor A changes. When turning clockwise (CW), 
 * encoder A is leading, and when turning counter clockwise (CCW), encoder B is 
 * leading. So we have an ISR for both encoders. that behave as follows:
 * 
 * # Encoder A ISR
 *
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning clockwise.
 * - If the signals are the same, then the shaft is turning counter clockwise. 
 * 
 * # Encoder B ISR 
 * 
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning counter clockwise.
 * - If the signals are the same, then the shaft is turning clock wise. 
 * 
 * # Running in IRAM
 * 
 * Note that this code is loaded into IRAM (rather than the usual DRAM) using 
 * the IRAM_ATTR attribute in its declaration. This means that it is accessed 
 * via the instruction bus for fast and consistent execution times.
 *  
 * @param null.
 * @return null.   
 ******************************************************************************/
void IRAM_ATTR isrMot0EncB() 
{
   portENTER_CRITICAL(&motorEncoderMutex);
   m0LastMillis = m0CurrMillis;
   m0CurrMillis = millis();
   if(digitalRead(G_MOT0_ENCA) == digitalRead(G_MOT0_ENCB)) // CW.
   {
      m0CurrCnt ++;
      m0Dir = mtrFwd;
   } // if
   else // CCW.
   {
      m0CurrCnt --;
      m0Dir = mtrBwd;
   } // else
//   m0TestCnt ++;
   portEXIT_CRITICAL(&motorEncoderMutex);
} // isrMot0EncB() 

/**
 * @brief Interrupt service routine (ISR) for for motor 2 encoder A.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the motor 2 hall effect sensor A changes. When turning clockwise (CW), 
 * encoder A is leading, and when turning counter clockwise (CCW), encoder B is 
 * leading. So we have an ISR for both encoders. that behave as follows:
 * 
 * # Encoder A ISR
 *
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning clockwise.
 * - If the signals are the same, then the shaft is turning counter clockwise. 
 * 
 * # Encoder B ISR 
 * 
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning counter clockwise.
 * - If the signals are the same, then the shaft is turning clock wise. 
 * 
 * # Running in IRAM
 * 
 * Note that this code is loaded into IRAM (rather than the usual DRAM) using 
 * the IRAM_ATTR attribute in its declaration. This means that it is accessed 
 * via the instruction bus for fast and consistent execution times.
 *  
 * @param null.
 * @return null.   
 ******************************************************************************/
void IRAM_ATTR isrMot1EncA() 
{
   portENTER_CRITICAL(&motorEncoderMutex);
   m1LastMillis = m1CurrMillis;
   m1CurrMillis = millis();
   if(digitalRead(G_MOT1_ENCA) == digitalRead(G_MOT1_ENCB)) // CCW.
   {
      m1CurrCnt --;
      m1Dir = mtrBwd;
   } // if
   else // CW.
   {
      m1CurrCnt ++;
      m1Dir = mtrFwd;
   } // else
//   m1TestCnt ++;
   portEXIT_CRITICAL(&motorEncoderMutex);
} // isrMot1EncA() 

/**
 * @brief Interrupt service routine (ISR) for for motor 2 encoder B.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the motor 2 hall effect sensor A changes. When turning clockwise (CW), 
 * encoder A is leading, and when turning counter clockwise (CCW), encoder B is 
 * leading. So we have an ISR for both encoders. that behave as follows:
 * 
 * # Encoder A ISR
 *
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning clockwise.
 * - If the signals are the same, then the shaft is turning counter clockwise. 
 * 
 * # Encoder B ISR 
 * 
 * - Read both encoders and compare them. 
 * - If the signals are different, then the shaft is turning counter clockwise.
 * - If the signals are the same, then the shaft is turning clock wise. 
 * 
 * # Running in IRAM
 * 
 * Note that this code is loaded into IRAM (rather than the usual DRAM) using 
 * the IRAM_ATTR attribute in its declaration. This means that it is accessed 
 * via the instruction bus for fast and consistent execution times.
 *  
 * @param null.
 * @return null.   
 ******************************************************************************/
void IRAM_ATTR isrMot1EncB() 
{
   portENTER_CRITICAL(&motorEncoderMutex);
   m1LastMillis = m1CurrMillis;
   m1CurrMillis = millis();
   if(digitalRead(G_MOT1_ENCA) == digitalRead(G_MOT1_ENCB)) // CW.
   {
      m1CurrCnt ++;
      m1Dir = mtrFwd;
   } // if
   else // CCW.
   {
      m1CurrCnt --;
      m1Dir = mtrBwd;
   } // else
//   m1TestCnt ++;
   portEXIT_CRITICAL(&motorEncoderMutex);
} // isrMot1EncB() 

/**
 * @brief Set the speed of a drive motor. Valid values are 0 to 255.
 * @details Set the duty cycle of the specified motor (left or right). 
 * @param motorNumber specifies the left () or right (1) motor.
 * @param dutyCycle specifies the duty cycle which determines the speed.
 * @return true if the motor is 0 or 1, otherwise returns false.   
 ******************************************************************************/
bool setMotorSpeed(uint8_t motorNumber, uint32_t dutyCycle)
{
   Log.verboseln("<setMotorSpeed> Set the duty cycle of motor %u to %u.", motorNumber, dutyCycle);
   if(dutyCycle > 255)
   {
         Log.errorln("<setMotorSpeed> Invalid duty cycle. Must be from 0 through 255 only.");
         return false;
   } // if
   switch(motorNumber)
   {
      case MOTOR_0: 
         ledcWrite(PWM_MOT1_CHANNEL, dutyCycle); // Set motor 1 speed via duty cycle.
         break;
      case MOTOR_1: 
         ledcWrite(PWM_MOT2_CHANNEL, dutyCycle); // Set motor 2 speed via duty cycle.
         break;
      default:
         Log.errorln("<setMotorSpeed> Invalid motor index. Must be either 0 or 1. Ignoring command.");
         return false;
         break;
   } // switch
   return true;
} // setMotorSpeed()

/**
 * @brief Set the direction of a drive motor.
 * @details Direction is controlled by two enable pins. The proof table is:
 * | In1  | In2  | Direction      |
 * |------|------|----------------|
 * | LOW  | LOW  | Stop           |
 * | HIGH | HIGH | Stop           | 
 * | HIGH | LOW  | Clockwise      | 
 * | LOW  | HIGH | Anti-clockwise | 
 * @param motorNumber specifies the left () or right (1) motor.
 * @param direction specifies the rotation of the motor 0 stop, 1 forward, 
 * 2 backward.
 * @return true if the motor is 0 or 1, otherwise returns false.   
 ******************************************************************************/
bool setMotorDirection(uint8_t motorNumber, uint8_t direction)
{
   Log.verboseln("<setMotorDirection> Set the direction of motor %u to %u.", motorNumber, direction);
   if(direction < 0 && direction > 2)
   {
      Log.errorln("<setMotorDirection> Invalid motor direction. Valid values are 0 (stop), 1 (forward) and 2 (backward).");
      return false;
   } // if
   if(motorNumber == 0)
   {
      if(direction == 0)
      {
         Log.traceln("<setMotorDirection> Stop");
         digitalWrite(G_MOT0_DIR0, LOW); 
         digitalWrite(G_MOT0_DIR1, LOW); 
         return true;
      } // if
      if(direction == 1)
      {
         Log.traceln("<setMotorDirection> Forward");
         digitalWrite(G_MOT0_DIR0, HIGH); 
         digitalWrite(G_MOT0_DIR1, LOW);
         return true; 
      } // if
      if(direction == 2)
      {
         Log.traceln("<setMotorDirection> Backward");
         digitalWrite(G_MOT0_DIR0, LOW); 
         digitalWrite(G_MOT0_DIR1, HIGH);
         return true; 
      } // if
   } // if
   if(motorNumber == 1)
   {
      if(direction == 0)
      {
         digitalWrite(G_MOT1_DIR0, LOW); 
         digitalWrite(G_MOT1_DIR1, LOW); 
         return true;
      } // if
      if(direction == 1)
      {
         digitalWrite(G_MOT1_DIR0, HIGH); 
         digitalWrite(G_MOT1_DIR1, LOW);
         return true; 
      } // if
      if(direction == 2)
      {
         digitalWrite(G_MOT1_DIR0, LOW); 
         digitalWrite(G_MOT1_DIR1, HIGH);
         return true; 
      } // if
   } // if
   Log.errorln("<setMotorDirection> Something went wrong. Possibly motor number. Doing nothing.");
   return false;
} // setMotorDirection()

/**
 * @brief Check to see what the motor encoders are doing.
 * @param null.
 * @return null
 ******************************************************************************/
void checkMotorEncoders()
{
   if(m0LastCnt != m0CurrCnt)
   {
      portENTER_CRITICAL(&motorEncoderMutex);   
      m0LastCnt = m0CurrCnt;
      portEXIT_CRITICAL(&motorEncoderMutex);
      Log.verboseln("<checkMotorEncoders> Motor 0 cnt = %l, direction = %T.", m0CurrCnt, m0Dir);   
   } // if   
   if(m1LastCnt != m1CurrCnt)
   {
      portENTER_CRITICAL(&motorEncoderMutex);   
      m1LastCnt = m1CurrCnt;
      portEXIT_CRITICAL(&motorEncoderMutex);
      Log.verboseln("<checkMotorEncoders> Motor 1 cnt = %l, direction = %T.", m1CurrCnt, m1Dir);
   } // if
//   Log.verboseln("<loop> Right cnt = %u, left cnt = %u.", m1CurrCnt, m0CurrCnt);
//   Log.verboseln("<loop> Right cnt = %u, left cnt = %u.", m1TestCnt, m0TestCnt);
} // checkMotorEncoders()

#endif // End of precompiler protected code block