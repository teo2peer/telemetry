#include <Arduino.h>
#include <CRC32.h>
/*
typedef struct Packet {
uint32_t rpm;
uint32_t crc;
} Packet;

void setup() {
    Serial.begin(115200);
    Serial.println("Initialized");
}

uint32_t i = 0;
uint32_t upper = 10000;
uint32_t lower = 1000;
bool increase = true;
void loop() {

    // create packet
    Packet p = {0};
    p.rpm = i;
    p.crc = CRC32::calculate(&p, sizeof(Packet) - sizeof(uint32_t));

    Serial.write((uint8_t*)&p, sizeof(p));
    
    // send end of frame
    uint32_t eof = 0x0f0f0f0f;
    Serial.write((uint8_t*)&eof, sizeof(uint32_t));


    // increase test variable
    if ( increase == true && i <= upper ){
        i += 1000;
    } else if (increase == true && i > upper){
        increase = false;
    } else if (increase == false && i >= lower){
        i -= 1000;
    } else if (increase == false && i < lower){
        increase = true;
    } 
    delay(100);

}
*/