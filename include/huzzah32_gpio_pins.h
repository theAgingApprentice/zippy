/*******************************************************************************
 * @file huzzah32_gpio_pins.h  
 *******************************************************************************/
#ifndef _HUZZAH32_GPIO_PINS_H // Start of precompiler check to avoid dupicate inclusion of this code block.
   #define _HUZZAH32_GPIO_PINS_H // Preprocessor variable used by above check

#include <Arduino.h> // Required for variable type declarations. Arduino Core for ESP32. Comes with Platform.io

/** 
 * @brief Map Adafruit Huzzah32 devboard pins.
 * @details This file defines constants representing each of the preipheral 
 * GPIO pins of the ESP32 which are exposed on the Huzzah32 development board.
 * 
 * # Board Overview
 * The Adafruit Huzzah32 dev board has 28 pins of which 21 are usable to us. 
 * In order for the numbering system used in this file to make sense orient the 
 * board so that the component side is facing up and the USB connector is
 * pointed away from you. This results in the 16 pin header being on the left 
 * and the 12 pin header on the right. Start counting physical pins from the 
 * upper left corner. This is pin 1.
 * 
 * # Constant Names
 * The variable names work as follows: 
 * PIN_<physical pin number>_<silk screen label>.
 * The variable values are the GPIO numbers used in code to reference pins.
 * Physical pins 1 through 4 are not used by our code, nor are physical pins
 * 26 through 28 (see pin comments below for details).
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// Variables used to reference the pins in the left 16 pin socket.
///////////////////////////////////////////////////////////////////////////////
// Physical pin 1 is labeled RST button
// Physical pin 2 is labeled 3V
// Physical pin 3 is labeled NC
// Physical pin 4 is labeled GND
const int8_t PIN_5_LBL_A0 = 26; // Physical pin 5. Input and output.
const int8_t PIN_6_LBL_A1 = 25; // Physical pin 6. Input and output.
const int8_t PIN_7_LBL_A2 = 34; // Physical pin 7. Input only. No pullup/down resistor.
const int8_t PIN_8_LBL_A3 = 39; // Physical pin 8. Input only. No pullup/down resistor.
const int8_t PIN_9_LBL_A4 = 36; // Physical pin 9. Input only. No pullup/down resistor.
const int8_t PIN_10_LBL_A5 = 4; // Physical pin 10. Input and output.
const int8_t PIN_11_LBL_SCK = 5; // Physical pin 11. Input and output. PWM signal at boot.
const int8_t PIN_12_LBL_MO = 18; // Physical pin 12. Input and output.
const int8_t PIN_13_LBL_MI = 19; // Physical pin 13. Input and output.
const int8_t PIN_14_LBL_RX = 16; // Physical pin 14. Input and output.
const int8_t PIN_15_LBL_TX = 17; // Physical pin 15. Input and output.
const int8_t PIN_16_LBL_21 = 21; // Physical pin 16. Input and output.
///////////////////////////////////////////////////////////////////////////////
// Variables used to reference the pins in the right 12 pin socket.
///////////////////////////////////////////////////////////////////////////////
const int8_t PIN_17_LBL_SDA = 23; // Physical pin 17. Input and output. 
const int8_t PIN_18_LBL_SCL = 22; // Physical pin 18. Input and output.
const int8_t PIN_19_LBL_14 = 14; // Physical pin 19. Input and output.
const int8_t PIN_20_LBL_32 = 32; // Physical pin 20. Input and output.
const int8_t PIN_21_LBL_15 = 15; // Physical pin 21. Input and output.
const int8_t PIN_22_LBL_33 = 33; // Physical pin 22. Input and output.
const int8_t PIN_23_LBL_27 = 27; // Physical pin 23. Input and output.
const int8_t PIN_24_LBL_12 = 12; // Physical pin 24. Output only.
const int8_t PIN_25_LBL_13 = 13; // Physical pin 25. Input and output.
// Physical pin 26 is labeled USB
// Physical pin 27 is labeled EN
// Physical pin 28 is labeled BAT

#endif // End of conditional preprocessor code