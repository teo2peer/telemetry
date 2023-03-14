#pragma once

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "settings.h"

#include "lib/Utils/Utils.h"
#include "lib/sensors/gear.h"

// ids available on MOTOR
#define MONITOR_STATUS_SINCE_DTCS_CLEARED 0x01
#define FUEL_SYSTEM_STATUS 0x03

#define CALCULATED_ENGINE_LOAD 0x04
#define ENGINE_COOLANT_TEMPERATURE 0x05
#define LONG_TERM_FUEL_TRIM_BANK_1 0x07
#define INTAKE_MANIFOLD_ABSOLUTE_PRESSURE 0x0b
#define ENGINE_RPM 0x0c
#define VEHICLE_SPEED 0x0d
#define TIMING_ADVANCE 0x0e
#define AIR_INTAKE_TEMPERATURE 0x0f
#define THROTTLE_POSITION 0x11
#define OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM 0x14

#define DISTANCE_TRAVELED_WITH_MIL_ON 0x21
#define ABSOLULTE_BAROMETRIC_PRESSURE 0x33

#define CONTROL_MODULE_VOLTAGE 0x42
#define RELATIVE_THROTTLE_POSITION 0x45
#define TIME_RUN_WITH_MIL_ON 0x4d

namespace OBD2
{

    typedef struct
    {
        uint8_t MIL_on = false;         // true if dtc should be on
        uint8_t DTC_CNT = 0;            // number of errors detected
        uint8_t fuel_system_status = 0; // look table on https://en.wikipedia.org/wiki/OBD-II_PIDs#Service_01_PID_03

        uint8_t Calculated_Engine_load = 0;     // formula : A/2.55
        uint8_t Engine_coolant_temperature = 0; // formula : A-40

        uint8_t long_term_fuel_trim = 0;               // formula : A/1.28 - 100
        uint8_t intake_manifold_absolute_pressure = 0; // forumula : A
        uint8_t engine_rpmA = 0;
        uint8_t engine_rpmB = 0;            // formula : (256A + B)/4
        uint8_t vehicle_speed = 0;          // formula : A
        uint8_t timing_advance = 0;         // formula: A/2 -64
        uint8_t intake_air_temperature = 0; // formula A - 40
        uint8_t throttle_position = 0;      // formula : A/2.55

        uint8_t oxygen_sensor_voltage = 0;             // formula : A/200
        uint8_t oxygen_sensor_long_term_fuel_trim = 0; // formula : B/1.28 - 100

        uint8_t distance_traveled_MIL_on_A = 0;
        uint8_t distance_traveled_MIL_on_B = 0; // formula : 256A + B

        uint8_t absolute_barometric_presure = 0; // formula : A

        uint8_t control_module_voltage_A = 0;
        uint8_t control_module_voltage_B = 0; // formula : (256A + B) / 1000

        uint8_t relavite_throttle_position = 0; // formula : A/2.55

        uint8_t time_run_with_mil_on_A = 0;
        uint8_t time_run_with_mil_on_B = 0; // formula : 256A + B minutes

    } OBD2sensordata;

    // functions
    void initOBD2(OBD2sensordata &database);
    void askPID(uint8_t id);
    // ask
    void clearDTC();

    void OBD2events();

    void receivedOBD2callback(const CAN_message_t &msg);
    void receivedRPMcallback(const CAN_message_t &msg);
    void dispatchMessage(const uint8_t msg[], uint8_t lenght);

    void printOBD2ALL(OBD2sensordata database);

    String OBD2toCSV(OBD2sensordata database);
    String getBufferRPM();

    boolean isContact();

    void readDTC();

    void emulateDash(uint8_t gear);

}

#include "lib/SteeringWheel/SteeringWheel.h"