/******************************************************************************
 * @file configDetails.cpp  
 * @brief Manage configuration details.
 * @details Handles the LCD menu system for displaying information about Zippy.
 * Also logs full MPU details and sets the RGB LED based on boot status.
 ******************************************************************************/
#ifndef showCfgDetails_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.

#define showCfgDetails_cpp // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.

int8_t _SSIDIndex = 0; // Contains the SSID index number from the known list of APs.

/**
 * @brief Collect an average WiFi signal strength. 
 * @param int8_t Number of datapoints to use to create average. 
 * @return long Average signal strength of AP connection in decibels (db).
 ******************************************************************************/
long rfSignalStrength(int8_t dataPoints)
{
   long rssi = 0;
   long averageRSSI = 0;
   for(int i=0; i < dataPoints; i++)
   {
      rssi += WiFi.RSSI();
      delay(20);
   } //for
   averageRSSI = rssi / dataPoints;
   return averageRSSI;
} // rfSignalStrength()

/**
 * @brief Format uint32 number with commas.
 * @details Format a uint32_t (32 bits) number into a string in the format
 * "23,854,972". The provided buffer must be at least 14 bytes long. The number 
 * will be right-adjusted in the buffer. Returns a pointer to the first digit.
 * @param val is the number to be convereted.
 * @param startBuffer is a pointer to the start of the conversion buffer.
 * @return pointer to first digit.
 ******************************************************************************/
char * int32toa(uint32_t val, char* startBuffer)
{
   char *endBuffer = startBuffer + 13;
   *endBuffer = '\0';
   do 
   {
      if ((endBuffer - startBuffer) % 4 == 2)
      {
         *--endBuffer = ',';
      } // if
      *--endBuffer = '0' + val % 10;
      val /= 10;
   } while(val);
   return endBuffer;
} // int32toa()

/**
 * @brief Provide human readable Wifi encryption method.
 * @details Wifi encryptions options are:
 * 1. OPEN
 * 2. WEP
 * 3. WPA_PSK
 * 4. WPA2_PSK
 * 5. WPA_WPA2_PSK
 * 6. WPA2_ENTERPRISE
 * @param wifi_auth_mode_t Wifi encryption type code.
 * @return const char* Encryption type in one word. 
 ******************************************************************************/
const char* translateEncryptionType(wifi_auth_mode_t encryptionType)
{
   switch (encryptionType)
   {
      case (WIFI_AUTH_OPEN): return "Open";
      case (WIFI_AUTH_WEP): return "WEP";
      case (WIFI_AUTH_WPA_PSK): return "WPA_PSK";
      case (WIFI_AUTH_WPA2_PSK): return "WPA2_PSK";
      case (WIFI_AUTH_WPA_WPA2_PSK): return "WPA_WPA2_PSK";
      case (WIFI_AUTH_WPA2_ENTERPRISE): return "WPA2_ENTERPRISE";
      default: return "UNKNOWN";
   } //switch
} // translateEncryptionType()

/**
 * @brief Return human readable assessment of signal strength.
 * @param int16_t Signal strength as measured in decibels (db). 
 * @return const char* Assessment of signal quality in one or two words.
 ******************************************************************************/
const char* evalSignal(int16_t signalStrength)
{
   if(signalStrength <= unusable) return "Unusable";
   if(signalStrength <= notGood) return "Not good";
   if(signalStrength <= okay) return "Okay";
   if(signalStrength <= veryGood) return "Very Good";
   return "Amazing";
} // evalSignal()

/**
 * @brief Initialize Bluetooth.
 * @details The Bluetooth address consists of six integers. In this function
 * we concatinate these 6 integers as two-digit hex char* values separate by 
 * an additional 1 colon character. This results in a 15 character long 
 * address in a character array. 
 * @param null.
 * @return null.
 ******************************************************************************/
void btAddress(char* targetArray) 
{
   const uint8_t* point = esp_bt_dev_get_address(); // Retrieve address.
   strcpy(targetArray, ""); // Clear array.
   for (int i = 0; i < 6; i++) // Loop through address one integer at a time.
   {
      char str[3]; // Build address 3 characters at a time.
      sprintf(str, "%02X", (int)point[i]); // Convert integer into a 2-digit hex value.
      strcat(targetArray, str); // Concatinate each hex value into the target array.
      if(i < 5) // Insert colon every 3 characters for the firt 5 groups. 
      {
         strcat(targetArray, ":"); // Concatinate a colon into the target array.
      } // if
   } // for
} // btAddress()

/**
 * @brief Initialize Bluetooth system.
 * @details Initializing the Bluetooth system involves the following 3 steps:
 * 1. Initialize the contrller.
 * 2. Initialize Bluedroid.
 * 3. Enable Bluedroid.
 * 
 * # Esp32 Bluetooth Architecture
 * Bluetooth is a wireless technology standard for exchanging data over short 
 * distances, with advantages including robustness, low power consumption and 
 * low cost. The Bluetooth system can be divided into two different categories: 
 * Classic Bluetooth and Bluetooth Low Energy (BLE). ESP32 supports dual-mode 
 * Bluetooth, meaning that both Classic Bluetooth and BLE are supported by 
 * ESP32. Basically, the Bluetooth protocol stack is split into two parts: a 
 * “controller stack” and a “host stack”. The controller stack contains the 
 * PHY, Baseband, Link Controller, Link Manager, Device Manager, HCI and 
 * other modules, and is used for the hardware interface management and link 
 * management. The host stack contains L2CAP, SMP, SDP, ATT, GATT, GAP and 
 * various profiles, and functions as an interface to the application layer, 
 * thus facilitating the application layer to access the Bluetooth system. The 
 * Bluetooth Host can be implemented on the same device as the Controller, or 
 * on different devices. Both approaches are supported by ESP32.
 * 
 * # Bluedroid
 * The BLUEDROID Bluetooth Stack communicates with Bluetooth Controller over 
 * VHCI (Virtual Host Controller Interface) and at the same time provides APIs 
 * for user application. Bluetooth Profiles determine the functions of each 
 * layer of the Bluetooth from PHY to L2CAP while the Bluetooth Protocols 
 * define message formats and procedures for data transport, link control etc.
 * 
 * The following is a list of Classic Bluetooth Profiles and Protocols 
 * supported by BLUEDROID Bluetooth Stack of ESP32.
 * 
 * ## Classic Bluetooth Profiles
 * 
 * 1. GAP
 * 2. A2DP (SNK)
 * 3. AVRCP (CT)
 * 
 * ## Classic Bluetooth Protocols
 * L2CAP
 * SDP
 * AVDTP
 * AVCTP
 * 
 * The communication between ESP32’s Processor and Bluetooth Controller is 
 * based on Serial Interface. 
 * 
 * @param null.
 * @return null.
 ******************************************************************************/
/*
bool initBluetooth()
{
   if (!btStart()) 
   {
      Log.verboseln("Failed to initialize controller");
      return false;
   } // if
   if (esp_bluedroid_init() != ESP_OK) 
   {
      Log.verboseln("Failed to initialize bluedroid");
      return false;
   } // if
   if (esp_bluedroid_enable() != ESP_OK) 
   {
      Log.verboseln("Failed to enable bluedroid");
      return false;
   } //  if
   return true;
} // initBluetooth()
*/

/**
 * @brief Translates a flash memory mode code to a human readable string.
 * @details The ESP32 MCU supports the following FLASH memory modes:
 * 0. FM_QIO - SPI host uses the "Quad I/O Fast Read" command (EBh). Four SPI 
 * pins are used to write the flash address part of the command, and to read 
 * flash data out. Therefore these phases need a quarter the clock cycles 
 * compared to standard SPI.
 * 1. FM_QOUT - SPI host uses the "Quad Output Fast Read" command (6Bh). Four
 * SPI pins are used to read the flash data out. Slightly slower than QIO, 
 * because the address is written via the single MOSI data pin.
 * 2. FM_DIO - SPI host uses the "Dual I/O Fast Read" command (BBh). Two SPI 
 * pins are used to write the flash address part of the command, and to read 
 * flash data out. Therefore these phases need half the clock cycles compared 
 * to standard SPI.
 * 3. FM_DOUT - SPI host uses the "Dual Output Fast Read" command (3Bh). Two 
 * SPI pins are used to read flash data out. Slightly slower than DIO, because 
 * the address is written via the single MOSI data pin. 
 * 4. FM_FAST_READ - Have not found definition.
 * 5. FM_SLOW_READ - Have not found definition.
 * 255. FM_UNKNOWN - Unknown mode. 
 * @param null.
 * @return null.
 ******************************************************************************/
void transFlashModeCode(char& details)
{
   switch(ESP.getFlashChipMode())
   {
      case FM_QIO: strcpy(&details, "0 (FM_QIO) - Quad I/O Fast Read. Address written via four data pins."); break;
      case FM_QOUT: strcpy(&details, "1 (FM_QOUT) - Quad I/O Fast Read. Address written via the single MOSI data pin."); break;
      case FM_DIO: strcpy(&details, "2 (FM_DIO) - Dual I/O Fast Read. Address written via two SPI pins."); break;
      case FM_DOUT: strcpy(&details, "3 (FM_DIO) - Dual I/O Fast Read. Address written via single MOSI data pin."); break;
      case FM_FAST_READ: strcpy(&details, "4 (FM_FAST_READ) - Unknown details."); break;
      case FM_SLOW_READ: strcpy(&details, "5 (FM_SLOW_READ) - Unknown details."); break;
      default: strcpy(&details, "UNKNOWN MODE."); break; 
   } // switch()
} // transFlashModeCode()

/**
 * @brief Sends details about the host micro controller to the log.
 * @param null.
 * @return null.
 ******************************************************************************/
void logSubsystemDetails()
{
   const uint32_t _STATIC_DATA_SIZE = ESP.getSketchSize() + ESP.getFreeSketchSpace();
   const uint32_t _SRAM_SIZE = _STATIC_DATA_SIZE + ESP.getHeapSize() + uxTaskGetStackHighWaterMark(NULL);
   int8_t _BUFFER_SIZE = 14; // Size of buffer to hold formatted uint32_t numbers.
   char _buffer[_BUFFER_SIZE]; // Buffer to hold formatted uint32_t numbers. 
   const int8_t _DETAIL_SIZE = 80; // Size of buffer holding details about memory.
   char _details[_DETAIL_SIZE]; // Text version of flash memory mode.
   wifi_auth_mode_t encryption = WiFi.encryptionType(_SSIDIndex);
   int8_t _dataReadings = 10; // Number of data readings to average to determine Wifi signal strength.
   long _signalStrength = rfSignalStrength(_dataReadings); // Get average signal strength reading.
   char _bluetoothAddress[30]; // Hold Bluetooth address in a character array.
   Log.noticeln("<logSubsystemDetails> Core subsystem details.");
   // Core CPU
   Log.noticeln("<logSubsystemDetails> ... Core CPU details.");
   Log.noticeln("<logSubsystemDetails> ...... CPU Count = %d", ESP.getChipCores());
   Log.noticeln("<logSubsystemDetails> ...... CPU Model = %s", ESP.getChipModel());
   Log.noticeln("<logSubsystemDetails> ...... CPU Revision = %d", ESP.getChipRevision());
   Log.noticeln("<logSubsystemDetails> ...... CPU clock speed = %uMhz", ESP.getCpuFreqMHz());   
   // Core Memory
   Log.noticeln("<logSubsystemDetails> ... Core memory details.");
   Log.noticeln("<logSubsystemDetails> ...... ROM contains Espressif code and we do not touch that.");
   Log.noticeln("<logSubsystemDetails> ......... ROM size = %s bytes.", int32toa(XSHAL_ROM_SIZE, _buffer));
   Log.noticeln("<logSubsystemDetails> ...... SRAM is the binarys read/write area.");
   Log.noticeln("<logSubsystemDetails> ......... The Stack contains local variables, interrupt and function pointers.");
   Log.noticeln("<logSubsystemDetails> ............ Stack highwater mark = %s bytes", int32toa(uxTaskGetStackHighWaterMark(NULL), _buffer));
   Log.noticeln("<logSubsystemDetails> ......... Static memory (aka sketch memory) contains global and static variables.");
   Log.noticeln("<logSubsystemDetails> ............ Static data size = %s bytes.", int32toa(_STATIC_DATA_SIZE, _buffer));
   Log.noticeln("<logSubsystemDetails> ............ Sketch size = %s bytes.", int32toa(ESP.getSketchSize(), _buffer));
   Log.noticeln("<logSubsystemDetails> ............ Free sketch space = %s bytes.", int32toa(ESP.getFreeSketchSpace(), _buffer));
   Log.noticeln("<logSubsystemDetails> ......... The Heap contains dynamic data.");
   Log.noticeln("<logSubsystemDetails> ............ Heap size = %s bytes.", int32toa(ESP.getHeapSize(), _buffer));
   Log.noticeln("<logSubsystemDetails> ............ Free heap = %s bytes.", int32toa(ESP.getFreeHeap(), _buffer));   
   Log.noticeln("<logSubsystemDetails> ......... Total SRAM size (stack + heap + static data) = %s bytes.", int32toa(_SRAM_SIZE, _buffer));
   Log.noticeln("<logSubsystemDetails> Wireless subsystem details.");
   // Wireless 
//   btAddress(_bluetoothAddress); // Copy formatted Bluetooth address into the character array.
   Log.noticeln("<logSubsystemDetails> ... WiFi details."); 
   Log.noticeln("<logSubsystemDetails> ...... Access Point Name = %s.",WiFi.SSID().c_str()); 
   Log.noticeln("<logSubsystemDetails> ...... Access Point Encryption method = %X (%s).", encryption, translateEncryptionType(WiFi.encryptionType(encryption)));
   Log.noticeln("<logSubsystemDetails> ...... Wifi signal strength = %l (%s).", _signalStrength, evalSignal(_signalStrength));
   Log.noticeln("<logSubsystemDetails> ...... Local Wifi MAC address: %s.", WiFi.macAddress().c_str());
   Log.noticeln(F("<logSubsystemDetails> ...... Local WiFi IP address: %p."), WiFi.localIP()); 
//   Log.noticeln("<logSubsystemDetails> ... Bluetooth details."); 
   Log.noticeln("<logSubsystemDetails> ...... Local bluetooth MAC address: %s.", _bluetoothAddress); 
   Log.noticeln("<logSubsystemDetails> Crytographic subsystem details.");
   // Cryptographic hardware acceleration
   Log.noticeln("<logSubsystemDetails> ... SHA not implemented."); 
   Log.noticeln("<logSubsystemDetails> ... RSA not implemented."); 
   Log.noticeln("<logSubsystemDetails> ... AES not implemented."); 
   Log.noticeln("<logSubsystemDetails> ... RNG not implemented."); 
   // RTC
   Log.noticeln("<logSubsystemDetails> RTC subsystem details.");
   Log.noticeln("<logSubsystemDetails> ... Phasor measurement unit (PMU) not implemented."); 
   Log.noticeln("<logSubsystemDetails> ... Ultra Low Power (ULP) 32-bit co-processor not implemented."); 
   Log.noticeln("<logSubsystemDetails> ... Recovery memory not implemented.");    
   // Peripherals
   Log.noticeln("<logSubsystemDetails> Peripheral subsystem details.");
   Log.noticeln("<logSubsystemDetails> ... SPI accessible external memory details.");
   // Integrated Flash
   Log.noticeln("<logSubsystemDetails> ...... Flash memory details (Arduino binary resides here).");
   transFlashModeCode(*_details);
   Log.noticeln("<logSubsystemDetails> ......... Flash mode = %s", _details);
   Log.noticeln("<logSubsystemDetails> ......... Flash chip size = %s bytes.", int32toa(ESP.getFlashChipSize(), _buffer));
   Log.noticeln("<logSubsystemDetails> ......... Flash chip speed = %s bps.", int32toa(ESP.getFlashChipSpeed(), _buffer));   
   // PSRAM 
   Log.traceln("<logSubsystemDetails> ...... PSRAM is optional external RAM accessed via the SPI bus.");
   if(psramFound()) // Is SPI RAM (psudo ram) available?
   {
      Log.noticeln("<logSubsystemDetails> ......... PSRAM detected.");
      Log.noticeln("<logSubsystemDetails> ......... PSRAM size = %s", int32toa(ESP.getPsramSize(), _buffer));
      Log.noticeln("<logSubsystemDetails> ......... Free PSRAM = %s", int32toa(ESP.getFreePsram(), _buffer));
   } // if
   else
   {
      Log.noticeln("<logSubsystemDetails> ......... No PSRAM detected.");
   } // else   

   Log.noticeln("<logSubsystemDetails> ... General purpose I/O pins in use.");   
} // logSubsystemDetails()

/** 
 * @brief Show the environment details of this application on console.
 ******************************************************************************/
void showCfgDetails()
{
   Log.verboseln("<showCfgDetails> Robot Configuration Report");
   Log.verboseln("<showCfgDetails> ==========================");
   logSubsystemDetails(); // Display core0 information on the console.
//   appCpu.cfgToConsole(); // Display core0 information on the console.
   if(networkConnected == true)
   {
      Log.verboseln("<showCfgDetails> Network connection status = TRUE");
      network.cfgToConsole(); // Display network information on the console.
      if(mqttBrokerConnected == true)
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = TRUE");
         Log.verbose("<showCfgDetails> MQTT broker IP address = ");
         Log.verboseln(getMqttBrokerIP());
      } // if
      else
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = FALSE");
      } // else
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> Network connection status = FALSE");
   } // else
   if(lcdConnected == true)
   {
      Log.verboseln("<showCfgDetails> LCD connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> OLED connection status = FALSE.");
   } // else
} //showCfgDetails()

/** 
 * @brief Show the environment details of this application on OLED.
 * @details Uses menu system to show different information as needed.
 ******************************************************************************/
void displayCfgDetails(int8_t menuToShow)
{
   switch (menuToShow) 
   {
      case 2:
         Log.verboseln("<displayCfgDetails> Display second menu.");
         break;
      case 3:
         Log.verboseln("<displayCfgDetails> Display third menu.");
         break;
      default:
         Log.verboseln("<displayCfgDetails> Display first menu.");
         break;
   } // switch
} // displayCfgDetails()

/** 
 * @brief Check to see how the boot up process went.
 ******************************************************************************/
void checkBoot()
{
   Log.traceln("<checkBoot> Checking boot status flags."); 
   if(networkConnected == true && mqttBrokerConnected == true && lcdConnected == true && mobilityStatus == true)
   {
      Log.verboseln("<checkBoot> Bootup was normal. Set RGB LED to normal colour."); 
      setStdRgbColour(GREEN); // Indicates that bootup was normal.
   } // if
   else
   {
      Log.verboseln("<checkBoot> Bootup had an issue. Set RGB LED to warning colour."); 
      setStdRgbColour(YELLOW); // Indicates that there was a bootup issue.
   } // else
} // checkBoot

#endif // End of precompiler protected code block