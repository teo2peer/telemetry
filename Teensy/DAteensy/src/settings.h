#ifndef SETTINGS
#define SETTINGS

#define ScreenUART Serial7
#define RadioUART Serial5

#define EEPROM_time_base_address 0
#define EEPROM_fss_base_address 4

#define CAN CAN3
#define DEBUG

#define OIL_PRESSURE_PIN 9
// reading 1 from this pin means that the cable is connected to GND, reading 0 means that is disconnected

#define DEBUG_LED 12

#define GREEN_BUTTON 40 // clear dtc
#define RED_BUTTON 41   // switch screen view

#endif