#pragma once

#include <Arduino.h>
#include "lib/CAN/OBD2.h"
#include "lib/CAN/Conversions.h"
#include "lib/persistence/persistance.h"

namespace DISPLAYY {
    void rpmled(int nleds);
    void rpmledInverse(int nleds);

    void initScreen(HardwareSerial &_serial);

    void sendGear(int str);
    void sendDesiredGear(int str);
    void sendOil(bool str);
    void sendRPM(int str);
    void sendCOLTMP(float str);
    void sendAIRTMP(float str);

    void sendTimeEngineOn(uint32_t str);

    void sendTPS(float str);
    void sendTrim(float str);
    void send02Trim(float str);
    void sendDTCcount(int str);
    void sendVoltage(float str);

    void sendAbsPressure(int str);
    void sendIntakePressure(int str);

    void sendDTCDebugScreen(const uint16_t msg[], uint8_t length);

    void setMainScreen();
    void setSplashScreen();
    void setDebugScreen();
    void setSensorScreen();


    void sendOBDdata(OBD2::OBD2sensordata OBD2db);
}