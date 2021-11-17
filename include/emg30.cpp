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
 * direction. The pins are set for output. 
 * See tutorial: https://esp32io.com/tutorials/esp32-dc-motor   
 * ==========================================================================*/
void setupMotCntl()
{
   Log.verboseln("<setupMotCntl> Set up motor control pins");
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 1 speed.", G_MOT1_ENA);
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 1 direction.", G_MOT1_DIR1);
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 1 direction.", G_MOT1_DIR2);
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 2 speed.", G_MOT2_ENB);
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 2 direction.", G_MOT2_DIR3);
   Log.verboseln("<setupMotCntl> ... GPIO pin %d controls motor 2 direction.", G_MOT2_DIR4);
   pinMode(G_MOT1_ENA, OUTPUT); // Set motor1 speed pin to output.
   ledcSetup(PWM_MOT1_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure motor 1 PWM properties.
   ledcAttachPin(G_MOT1_ENA, PWM_MOT1_CHANNEL); // Attach PWM channel to pin connected to enable motor 1.
   pinMode(G_MOT1_DIR1, OUTPUT); // Set motor1 direction1 pin to output.
   pinMode(G_MOT1_DIR2, OUTPUT); // Set motor1 direction2 pin to output.
   pinMode(G_MOT2_ENB, OUTPUT); // Set motor2 speed pin to output.
   ledcSetup(PWM_MOT2_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure motor 2 PWM properties.
   ledcAttachPin(G_MOT2_ENB, PWM_MOT2_CHANNEL); // Attach PWM channel to pin connected to enable motor 2.
   pinMode(G_MOT2_DIR3, OUTPUT); // Set motor2 direction3 pin to output.
   pinMode(G_MOT2_DIR4, OUTPUT); // Set motor2 direction4 pin to output.
} // setupMotCntl()

/**
 * @brief Set the speed of a drive motor. Valid values are 0 to 255.
 * @details Set the duty cycle of the specified motor (left or right). 
 * @param motorNumber specifies the left () or right (1) motor.
 * @param dutyCycle specifies the duty cycle which determines the speed.
 * @return true if the motor is 0 or 1, otherwise returns false.   
 * ==========================================================================*/
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
      case LEFT_MOTOR: 
         ledcWrite(PWM_MOT1_CHANNEL, dutyCycle); // Set motor 1 speed via duty cycle.
         break;
      case RIGHT_MOTOR: 
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
 * ==========================================================================*/
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
         digitalWrite(G_MOT1_DIR1, LOW); 
         digitalWrite(G_MOT1_DIR2, LOW); 
         return true;
      } // if
      if(direction == 1)
      {
         Log.traceln("<setMotorDirection> Forward");
         digitalWrite(G_MOT1_DIR1, HIGH); 
         digitalWrite(G_MOT1_DIR2, LOW);
         return true; 
      } // if
      if(direction == 2)
      {
         Log.traceln("<setMotorDirection> Backward");
         digitalWrite(G_MOT1_DIR1, LOW); 
         digitalWrite(G_MOT1_DIR2, HIGH);
         return true; 
      } // if
   } // if
   if(motorNumber == 1)
   {
      if(direction == 0)
      {
         digitalWrite(G_MOT2_DIR3, LOW); 
         digitalWrite(G_MOT2_DIR4, LOW); 
         return true;
      } // if
      if(direction == 1)
      {
         digitalWrite(G_MOT2_DIR3, HIGH); 
         digitalWrite(G_MOT2_DIR4, LOW);
         return true; 
      } // if
      if(direction == 2)
      {
         digitalWrite(G_MOT2_DIR3, LOW); 
         digitalWrite(G_MOT2_DIR4, HIGH);
         return true; 
      } // if
   } // if
   Log.errorln("<setMotorDirection> Something went wrong. Possibly motor number. Doing nothing.");
   return false;
} // setMotorDirection()

#endif // End of precompiler protected code block