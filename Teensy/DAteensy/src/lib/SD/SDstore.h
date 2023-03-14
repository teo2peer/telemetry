#pragma once

#include <Arduino.h>
#include <SD.h>
#include "lib/persistence/persistance.h"

namespace SDSTORE {

    void initSD();
    void saveLine(String data);

}