#include "Radio.h"
namespace RADIO
{
    HardwareSerial *ser1;

    void initRadio(HardwareSerial &_serial)
    {
        ser1 = &_serial;
        ser1->begin(115200);
    }

    void sendPacket(Packet packet)
    {

        packet.crc = CRC32::calculate(&packet, sizeof(Packet) - sizeof(uint32_t));
        packet.end = 0x0f0f0f0f;

        ser1->write((uint8_t *)&packet, sizeof(packet));
    }

    void sendData(String data)
    {
        ser1->println(data);
    }

}
