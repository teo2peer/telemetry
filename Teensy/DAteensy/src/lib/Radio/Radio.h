#pragma once

#include <Arduino.h>
#include <CRC32.h>

namespace RADIO
{
    typedef struct Packet
    {
        uint32_t rpm;
        uint32_t crc;
        uint32_t end;
    } Packet;

    void initRadio(HardwareSerial &_serial);
    void sendPacket(Packet packet);
    void sendData(String data);

}