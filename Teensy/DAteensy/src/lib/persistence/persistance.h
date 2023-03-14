#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"

namespace PERSISTANCE {
    uint32_t readCounter(uint32_t base_address);
    void increaseTimeCounter(uint32_t base_address);
    String timeToString(uint32_t time);
}
