#pragma once

#include <Arduino.h>
#include "OBD2.h"

namespace OBD2CONVERSIONS {
    // conversions
    uint16_t OBD2RPM(OBD2::OBD2sensordata db);
    float OBD2TMP(uint8_t tmp);
    float OBD2PC(uint8_t tmp);
    float OBD2Trim(uint8_t tmp);
    float OBD2Volt(OBD2::OBD2sensordata db);
    float OBD2VoltO2(uint8_t v);
    float OBD2Advance(uint8_t v);

    char* getDTCstring(uint16_t dtc);
}

