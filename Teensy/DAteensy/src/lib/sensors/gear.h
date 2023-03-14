#ifndef GEARSENSOR
#define GEARSENSOR

#include <Arduino.h>

namespace GEAR {
    void initGear();
    int getGear();

    uint8_t getDesiredGear(); // desired gear = 128 to clone real gear
    void setDesiredGear(uint8_t gear);
}


#endif