/******************************************************************************
 Note that the physical pin count starts at the reset button on long pin side 
 of the Huzzah32 development board.
 ******************************************************************************/
#ifndef _ZIPPY_GPIO_PINS_H // Start of conditional preprocessor code that only allows this library to be included once

#define _ZIPPY_GPIO_PINS_H // Preprocessor variable used by above check

#include <huzzah32_gpio_pins.h> // Mapping of ESP32 pins to Huzzah32 development board pins
/*
#define backLimitSwitch PIN_LBL_14 // Back limit switch, physical pin 20 
#define frontLimitSwitch PIN_LBL_32 // Front limit switch, physical pin 19 
#define I2C_BUS0_SDA PIN_LBL_SDA // Serial Data Line (SDA) for I2C bus 0, physical pin 17
#define I2C_BUS0_SCL PIN_LBL_SCL // Serial Clock Line (SCL) for I2C bus 0, physical pin 18
#define I2C_BUS1_SDA PIN_LBL_TX // Serial Data Line (SDA) for I2C bus 1, physical pin 15
#define I2C_BUS1_SCL PIN_LBL_21 // Serial Clock Line (SCL) for I2C bus 1, physical pin 16
#define resetRedLED PIN_LBL_27 // Red LED in reset button, physical pin 23
#define resetBlueLED PIN_LBL_33 // Blue LED in reset button, physical pin 22
#define resetGreenLED PIN_LBL_15 // Green LED in reset button, physical pin 21
*/

const int8_t G_MOT1_SPD = PIN_17_LBL_SDA; // Speed of motor 1, physical pin 17
const int8_t G_MOT1_DIR = PIN_18_LBL_SCL; // Direction of motor 1, physical pin 18
const int8_t G_I2C_BUS0_SDA = PIN_15_LBL_TX; // Serial Data Line (SDA) for I2C bus 0, physical pin 15
const int8_t G_I2C_BUS0_SCL = PIN_16_LBL_21; // Serial Clock Line (SCL) for I2C bus 0, physical pin 16
//const int8_t G_BUTTON_A = PIN_21_LBL_15; // Button A on OLED featherboard, physical pin 21
//const int8_t G_BUTTON_B = PIN_20_LBL_32; // Button B on OLED featherboard, physical pin 20
//const int8_t G_BUTTON_C = PIN_19_LBL_14; // Button C on OLED featherboard, physical pin 19
const int8_t G_RESET_RED_LED = PIN_23_LBL_27; // Red LED in reset button, physical pin 23
const int8_t G_RESET_BLUE_LED = PIN_22_LBL_33; // Blue LED in reset button, physical pin 22
const int8_t G_RESET_GREEN_LED = PIN_21_LBL_15; // Green LED in reset button, physical pin 12

#endif // End of conditional preprocessor code