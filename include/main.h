/*******************************************************************************
 * @file main.h  
 *******************************************************************************/
#ifndef main_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define main_h // Preprocessor variable used by above check.

/*******************************************************************************
 * @section mainIncludes Included libraries.
 *******************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with PlatformIO.
#include <aaChip.h> // Core (CPU) details that the code running on.
#include <aaNetwork.h> // Wifi functions. 
#include <aaWebService.h> // Realtime web-based network config and OTA code updates.
#include <aaFlash.h> // Use Flash memory to store values that persist past reboot.
#include <aaMqtt.h> // Use MQTT for remote management and monitoring.
#include <known_networks.h> // String arrays of known Access Points and their passwords.
#include <Wire.h> // Required for I2C communication.
#include <Adafruit_PWMServoDriver.h> // https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log.
#include <zippy_gpio_pins.h> // GPIO pin uses
#include <SPI.h> 
#include <LiquidCrystal_I2C.h> // https://github.com/tonykambo/LiquidCrystal_I2C

/*******************************************************************************
 * @section mainVars Global variable definitions.
 * @details In order to get rid of "error: 'xxx' was not declared in this scope"
 * compiler errors list a function from any of the include files here. 
 *******************************************************************************/
// Config related variables
const char* HOST_NAME_PREFIX = "Zippy"; // Prefix for our unique network name.
aaChip appCpu; // Access information about the ESP32 application microprocessor (Core1).
aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
bool networkConnected = false;
bool mqttBrokerConnected = false;
bool lcdConnected = false;
bool mobilityStatus = false;
int8_t displayPage = 1;
// MQTT related variables.
aaFlash flash; // Non-volatile memory management. 
aaMqtt mqtt; // Publish and subscribe to MQTT broker. 
IPAddress brokerIP; // IP address of the MQTT broker.
char uniqueName[HOST_NAME_SIZE]; // Character array that holds unique name for Wifi network purposes. 
char *uniqueNamePtr = &uniqueName[0]; // Pointer to first address position of unique name character array.
char healthTopicTree[50] = ""; // Char array to hold full health topic tree name.
char helpTopicTree[50] = ""; // Char array to hold full health topic tree name.
String result[2] = {"false","true"}; // Provide english lables for true and flase return codes.
// RGB OLED related variabes.
const uint8_t numColoursSupported = 9; // Number of colours LED can be set to.
const bool commonAnode = true; // Set to true if RGB LED has a common anode, false for common cathode.
const uint8_t RED = 0; // Reset button LED red. 
const uint8_t GREEN = 1; // Reset button LED green. 
const uint8_t BLUE = 2; // Reset button LED blue. 
const uint8_t YELLOW = 3; // Reset button LED YELLOW. 
const uint8_t ORANGE = 4; // Reset button LED PINK.
const uint8_t PINK = 5; // Reset button LED PINK.
const uint8_t AQUA = 6; // Reset button LED aqua. 
const uint8_t WHITE = 7; // Reset button LED white. 
const uint8_t BLACK = 8; // Reset button LED white. 
const double_t PWM_FREQ = 500; // 500Hz.
const uint8_t PWM_RESOLUTION = 8; // ESP32 can go up to 8 bit PWM resolution.
#define PWM_RED_CHANNEL 0 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 0 for RGB red.
#define PWM_GREEN_CHANNEL 1 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 1 for RGB green.
#define PWM_BLUE_CHANNEL 2 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 2 for RGB blue.
#define PWM_MOT1_CHANNEL 3 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 3 for motor 1 speed.  
#define PWM_MOT2_CHANNEL 4 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 4 for motor 2 speed.  
const uint8_t MOTOR_0 = 0; // Refer to motor 0 (left?).
const uint8_t MOTOR_1 = 1; // Refer to motor 1 (right?).
typedef struct
{
   String name; // Name associated with colour properties
   uint32_t redDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
   uint32_t greenDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
   uint32_t blueDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
}struct_Colour; 
struct_Colour statusColour[numColoursSupported]; // Array of colours.
struct_Colour memColour; // Used to switch back RGB LED colour when it is temporarily changed.
// I2C related variables.
#define I2C_BUS0_SPEED 400000 // Define speed of I2C bus 2. Note 400KHz is the upper speed limit for ESP32 I2C
#define MPU6050_I2C_ADD 0x68 // GY521 I2C address.
#define leftOLED_I2C_ADD 0x3D // OLED used for robot's left eye I2C adddress.
#define rightOLED_I2C_ADD 0x3C // OLED used for robot' right eye I2C address.
#define dcMotorController 0xB0 >> 1 // Wire Library only uses 7 bit addresses so you need to shift address one bit to the right.
#define LCD16x2 0x3F // Liquid Crystal Display.
// Terminal related variables.
unsigned long serialBaudRate = 115200; // Serial terminal baud rate.
// Local web server related variables.
bool isWebServer; // True is web server running.
const char* WEB_APP_TITLE = "Zippy"; // App name for web page titles.
aaWebService localWebService(WEB_APP_TITLE); // Webserver hosted by microcontroller.
// Drive motor related variables.
long M0TestCnt = 0;
long m0CurrCnt = 0; // Used by ISR.
long m0LastCnt = 0; // Used by main loop.
long m0CurrMillis = 0; // Used to calculate speed.
long m0LastMillis = 0; // Used to calcullate speed.
bool m0Dir; // Track direction left motor shaft is rotating.
long m1TestCnt = 0;
long m1CurrCnt = 0; // Used by ISR.
long m1LastCnt = 0; // Used by main loop.
long m1CurrMillis = 0; // Used to calculate speed.
long m1LastMillis = 0; // Used to calculate speed.
bool m1Dir; // Track direction right motor shaft is rotating.
const bool mtrFwd = true; // Constant for forward rotation.
const bool mtrBwd = false; // Constant for backward rotation.
// ISR related variables
portMUX_TYPE motorEncoderMutex = portMUX_INITIALIZER_UNLOCKED; // Spinlock mutex.

/************************************************************************************
 * @section funcDeclare Declare functions found in the include files.
 * @details In order to get rid of "error: 'xxx' was not declared in this scope"
 * compiler errors list a function from any of the include files here. 
 ************************************************************************************/
// Config related functions
void showCfgDetails();
void checkBoot();
// MQTT related functions
bool connectToMqttBroker(aaNetwork &);
boolean isValidNumber(String);
uint32_t convertStrToUint32_t(String);
uint8_t convertStrToUint8_t(String);
int8_t convertStrToInt8_t(String);
bool checkNumArg(int8_t, String *);
bool processCmd(String);
IPAddress getMqttBrokerIP();
void checkMqtt();
// RGB LED related functions.
void createPredefinedColours();
void saveRgbColour();
void loadRgbColour();
void setCustRgbColour(uint32_t, uint32_t, uint32_t);
void setStdRgbColour(uint8_t);
void setupStatusLed();
// I2C related functions.
void identifyDevice(int);
void scanBus0();
void scanBus1();
// Define servoLegs related functions.
void startPositionLegs(int8_t, int8_t);
void initServos();
void initLegs();
bool moveLeg(int8_t, int8_t, float, float, float);
void setupMobility();  
// Define terminal related functions.
void setupSerial();
// Define local web server related functions.
void monitorWebServer();
void startWebServer();
// LCD related functions
void startLcd();
void displaySplashScreen();
void displayBootScreen();
void placeTextHcentre(String, int8_t); 
// Drive motor related functions.
void setupMotCntl();
bool setMotorSpeed(uint8_t, uint32_t); 
bool setMotorDirection(uint8_t, uint8_t);
void IRAM_ATTR isrMot0EncA();
void IRAM_ATTR isrMot0EncB();
void IRAM_ATTR isrMot1EncA();
void IRAM_ATTR isrMot1EncB();
void checkMotorEncoders();

/*******************************************************************************
 * @section codeModules Functions put into files according to function.
 * @details Order functions here in a way that ensures that variables get 
 * declared in one function prior to being referenced by another function. 
 *******************************************************************************/
#include <huzzah32_gpio_pins.h> // Map pins on Adafruit Huzzah32 dev board to friendly names.
#include <zippy_gpio_pins.h> // Map Zippy specific pin naming to generic development board pin names. 
#include <terminal.cpp> // Serial port management.
#include <configDetails.cpp> // Show the environment details of this application.
#include <web.cpp> // Manage locally hosted web service. 
#include <rgbLed.cpp> // Control status LEDs.
#include <network.cpp> // Control networking activities.
#include <mqttBroker.cpp> // Establish connect to the the MQTT broker.
#include <i2c.cpp> // Scan I2C buses to see what devices are present.
#include <lcd.cpp> // Control LCD.
#include <limitSwitch.cpp> // Limit switches used to detect robot falling over.
#include <emg30.cpp> // Control drive motors.

/************************************************************************************
 * @section mainDeclare Declare functions in main.cpp.
 ************************************************************************************/
void setup(); // Arduino mandatory function #1. Runs once at boot. 
void loop(); // Arduino mandatory function #2. Runs continually.

#endif // End of precompiler protected code block