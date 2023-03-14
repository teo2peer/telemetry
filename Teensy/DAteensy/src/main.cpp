#include <Arduino.h>

#include "lib/CAN/OBD2.h"

#include "lib/persistence/persistance.h"
#include "lib/SD/SDstore.h"

#include "lib/SteeringWheel/SteeringWheel.h"
#include "lib/Radio/Radio.h"

#include "lib/sensors/gear.h"
#include "lib/sensors/buttons.h"

#include "settings.h"

OBD2::OBD2sensordata OBD2db = {0};
RADIO::Packet RadioPacket = {0};
// IntervalTimer EmulateDashTimer;

void setup()
{

// init serial
#ifdef DEBUG
    Serial.begin(115200);
#endif

    // EmulateDashTimer.priority(255);
    // EmulateDashTimer.begin(emulateDash, 100000);

    BUTTONS::initButtons();

    DISPLAYY::initScreen(ScreenUART);
    RADIO::initRadio(RadioUART);

    initOBD2(OBD2db);

    SDSTORE::initSD();

    GEAR::initGear();
    pinMode(OIL_PRESSURE_PIN, INPUT);

    DISPLAYY::rpmled(0);
    OBD2db.engine_rpmA = 0;
    OBD2db.engine_rpmB = 0;

    pinMode(DEBUG_LED, OUTPUT);
}

uint32_t time_engine_on = 0;

uint32_t elapsed_minute = 0;
uint32_t elapsed_second = 0;
uint32_t elapsed_100ms = 0;
uint32_t elapsed_50ms = 0;

boolean previous_contact = false;
boolean previous_fss = false;

void loop()
{
    // execute always

    OBD2::OBD2events();

    // shutdown screen if contact is off
    if (OBD2::isContact())
    {
        if (!previous_contact)
        {
            DISPLAYY::setMainScreen();
            previous_contact = true;
        }
    }
    else
    {
        if (previous_contact)
        {
            DISPLAYY::setSplashScreen();
            previous_contact = false;
        }
    }

    if (millis() - elapsed_50ms > 50)
    {

        // updateScreen
        DISPLAYY::sendOBDdata(OBD2db);
        DISPLAYY::sendGear(GEAR::getGear());

        DISPLAYY::sendOil(digitalRead(OIL_PRESSURE_PIN));

        DISPLAYY::sendTimeEngineOn(time_engine_on);

        // update rpm LEDS
        DISPLAYY::rpmledInverse(OBD2CONVERSIONS::OBD2RPM(OBD2db) / 1000);

        // check buttons
        // green_button.events();
        // red_button.events();
        BUTTONS::checkButtons();

        elapsed_50ms = millis();
    }
    // execute each 100ms
    if (millis() - elapsed_100ms > 100)
    {
        // emulateDash
        if (GEAR::getDesiredGear() != 128)
        {
            OBD2::emulateDash(GEAR::getDesiredGear());
        }
        else
        {
            OBD2::emulateDash(GEAR::getGear());
        }

        // print stuff to read rpm from yamaha CAN
        // Serial.print(getBufferRPM());
        // Serial.print("||");
        // Serial.println(OBD2RPM(OBD2db));

        // print data to sd
        String to_save = "";
        to_save += millis();
        to_save += ",";
        to_save += OBD2toCSV(OBD2db);
        // add gear and oil to log
        to_save += ",";
        to_save += String(GEAR::getGear());
        to_save += ",";
        to_save += String(digitalRead(OIL_PRESSURE_PIN));
        RADIO::sendData(to_save);
        SDSTORE::saveLine(to_save);
        elapsed_100ms = millis();
    }

    // execute each second
    if (millis() - elapsed_second > 1000)
    {
        // update radio packet
        // RadioPacket.rpm = OBD2RPM(OBD2db);

        // send data over radio
        // sendPacket(RadioPacket);

        // printOBD2ALL(OBD2db);
        if (OBD2db.fuel_system_status != 0)
        {
            time_engine_on += 1;
        }
        else
        {
            time_engine_on = 0;
        }

        elapsed_second = millis();
    }

    // execute each minute
    if (millis() - elapsed_minute > 60 * 1000)
    {

        // increase time alive counter
        PERSISTANCE::increaseTimeCounter(EEPROM_time_base_address);
        // increase engine on time

        if (OBD2db.fuel_system_status != 0)
        {
            if (previous_fss)
            {
                PERSISTANCE::increaseTimeCounter(EEPROM_fss_base_address);
            }
            previous_fss = true;
        }
        else
        {
            previous_fss = false;
        }

        elapsed_minute = millis();
    }
}