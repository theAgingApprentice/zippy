#ifndef limitSwitch_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define limitSwitch_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.

portMUX_TYPE frontSwitchChangeMutex = portMUX_INITIALIZER_UNLOCKED; // Spinlock mutex.
portMUX_TYPE backSwitchChangeMutex = portMUX_INITIALIZER_UNLOCKED; // Spinlock mutex.
bool frontSwitchChanged = false;
bool backSwitchChanged = false;

/**
 * @brief Interrupt service routine (ISR) for for front limit switch.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the front bumper changes meaning that it has either been pressed or released.
 * The switch uses on internal pull-down resitor to prevent it being a flaoting
 * input pin.
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
void IRAM_ATTR isrFrontSwitchChanged() 
{
   portENTER_CRITICAL(&frontSwitchChangeMutex);   
   frontSwitchChanged = true;
   portEXIT_CRITICAL(&frontSwitchChangeMutex);   
} // isrFrontSwitchChanged() 

/**
 * @brief Interrupt service routine (ISR) for for back limit switch.
 * @details This code runs every time the value of the GPIO pin connected to 
 * the back bumper changes meaning that it has either been pressed or released.
 * The switch uses on internal pull-down resitor to prevent it being a flaoting
 * input pin.
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
void IRAM_ATTR isrBackSwitchChanged() 
{
   portENTER_CRITICAL(&backSwitchChangeMutex);   
   backSwitchChanged = true;
   portEXIT_CRITICAL(&backSwitchChangeMutex);    
} // isrBackSwitchChanged() 

/**
 * @brief Set up the limit switches used to detect when the robot falls over.
 * @details The robot has front and  back limit switches that contact the 
 * ground when the robot leans too far forward or backward. This routine
 * initializes the GPIO pins connected to those limit switches as input and 
 * configures the pins with a weak pullup resistor. 
 * 
 * # ISRs on ESP32
 * 
 * https://deepbluembedded.com/esp32-external-interrupts-pins-arduino-examples/
 * @param null.
 * @return null.   
 *   
 ******************************************************************************/
void setupLimitSwitches()
{
   Log.traceln("<setupLimitSwitches> Set weak pullup resistor for GPIO pin %d (forward limit switch) and %d (backward limit switch).", G_FRONT_BUMPER, G_BACK_BUMPER);
   pinMode(G_FRONT_BUMPER, INPUT_PULLDOWN); // Front bumper input with pulldown.
   pinMode(G_BACK_BUMPER, INPUT_PULLDOWN); // Back bumper input with pulldown.
   attachInterrupt(G_FRONT_BUMPER, isrFrontSwitchChanged, CHANGE); // ISR for front bumper.
   attachInterrupt(G_BACK_BUMPER, isrBackSwitchChanged, CHANGE); // ISR for back bumper.
} // setupLimitSwitches()

/**
 * @brief Check to see if the specified switch is contacting the ground.
 ******************************************************************************/
void checkLimitSwitches()
{
   if(frontSwitchChanged)
   {
      Log.verboseln("<checkLimitSwitches> Front limit switch changed.");
      frontSwitchChanged = false;
//      saveRgbColour();
//      setStdRgbColour(PINK);
   } // if
   if(backSwitchChanged)
   {
      Log.verboseln("<checkLimitSwitches> Back limit switch changed.");
      portENTER_CRITICAL(&backSwitchChangeMutex);   
      backSwitchChanged = false;
      portEXIT_CRITICAL(&backSwitchChangeMutex); 
//      saveRgbColour();
//      setStdRgbColour(AQUA);
   } // if
} // checkLimitSwitches()

#endif // End of precompiler protected code block