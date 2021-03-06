/*******************************************************************************
 * @file zippy_gpio_pins.h  
 *******************************************************************************/

#ifndef _ZIPPY_GPIO_PINS_H // Start of precompiler check to avoid dupicate inclusion of this code block.
   #define _ZIPPY_GPIO_PINS_H // Preprocessor variable used by above check.

#include <huzzah32_gpio_pins.h> // Huzzah32 development board pins.
/** 
 * @brief Map application GPIO pins.
 * @details This file defines constants that denote how the preipheral 
 * GPIO pins of the ESP32 are used in this application. These constants are
 * mapped back to the constants defined in the development bard specific
 * file included above.
 ******************************************************************************/
const int8_t G_MOT0_ENA = PIN_10_LBL_A5; // PWM speed of motor 0. Physical pin 10.
const int8_t G_MOT0_ENCA = PIN_6_LBL_A1; // Motor 0 hall effect A. Physical pin 20.
const int8_t G_MOT0_ENCB = PIN_5_LBL_A0; // Motor 0 hall effect B. Physical pin 7.
const int8_t G_MOT0_DIR0 = PIN_24_LBL_12; // Direction of motor 0. Physical pin 24. MTDI boot strap pin. Do not pull high at boot but can pull low at boot (10K resistor).
const int8_t G_MOT0_DIR1 = PIN_25_LBL_13; // Direction of motor 0. Physical pin 25.

const int8_t G_MOT1_ENA = PIN_11_LBL_SCK; // PWM speed of motor 1. Physical pin 11.
const int8_t G_MOT1_ENCA = PIN_7_LBL_A2; // Motor 1 hall effect A. Physical pin 5.
const int8_t G_MOT1_ENCB = PIN_20_LBL_32; // Motor 1 hall effect B. Physical pin 6.
const int8_t G_MOT1_DIR0 = PIN_17_LBL_SDA; // Direction of motor 1. Physical pin 17.
const int8_t G_MOT1_DIR1 = PIN_18_LBL_SCL; // Direction of motor 1. Physical pin 18.

const int8_t G_I2C_BUS0_SDA = PIN_15_LBL_TX; // Serial Data Line (SDA) for I2C bus 0. Physical pin 15.
const int8_t G_I2C_BUS0_SCL = PIN_16_LBL_21; // Serial Clock Line (SCL) for I2C bus 0. Physical pin 16.

const int8_t G_BACK_BUMPER = PIN_19_LBL_14; // Back limit switch. Physical pin 19.  
const int8_t G_FRONT_BUMPER = PIN_12_LBL_MO; // Front limit switch. Physical pin 12.  

const int8_t G_RESET_RED_LED = PIN_23_LBL_27; // Red LED in reset button. Physical pin 23.
const int8_t G_RESET_GREEN_LED = PIN_21_LBL_15; // Green LED in reset button. Physical pin 21.
const int8_t G_RESET_BLUE_LED = PIN_22_LBL_33; // Blue LED in reset button. Physical pin 22.

#endif // End of conditional preprocessor code.