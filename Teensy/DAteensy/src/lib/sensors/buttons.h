#pragma once

#include <Arduino.h>
#include "lib/CAN/OBD2.h"
#include "lib/SteeringWheel/SteeringWheel.h"
#include "lib/sensors/gear.h"
#include "settings.h"

namespace BUTTONS {

    typedef void (*TransFuncPtr)();

    class Button{
    private:
        bool _state;
        uint32_t time;
        bool time_flag;
        uint8_t _pin;

        TransFuncPtr onButtonPressed;
        TransFuncPtr onButtonLongPressed;
        TransFuncPtr onButtonReleased;

    public:
        void setPressed(TransFuncPtr func);
        void setLongPressed(TransFuncPtr func);
        void setReleased(TransFuncPtr func);


        Button(uint8_t pin);
        void begin();
        void events();

    };

    void initButtons();
    void checkButtons();

}