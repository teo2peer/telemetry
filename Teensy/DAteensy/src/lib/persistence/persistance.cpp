#include "persistance.h"

namespace PERSISTANCE {
    uint32_t readCounter(uint32_t base_address){
        uint8_t b0 = EEPROM.read(base_address);
        uint8_t b1 = EEPROM.read(base_address+1);
        uint8_t b2 = EEPROM.read(base_address+2);
        uint8_t b3 = EEPROM.read(base_address+3);
        uint32_t stored = (b0 << 24) + (b1 << 16) + (b2 << 8) + b3;
        return stored;
    }

    void increaseTimeCounter(uint32_t base_address){
        // base_address should be changed each 60 days of use (max 100k writes)
        uint32_t stored = readCounter(base_address);
        if ( stored == 0xFFFFFFFF ){ // initialize if memory is not used
            EEPROM.write(base_address    , 0);
            EEPROM.write(base_address + 1, 0);
            EEPROM.write(base_address + 2, 0);
            EEPROM.write(base_address + 3, 0);

        } else{
            stored = (stored + 1) % (60 * 24 * 60); // reset each 60 days (counter in minutes), if resetted location of data should be modified to prevent eeprom wear out

            uint8_t b0_new = (stored >> 24) & 0xFF;
            uint8_t b1_new = (stored >> 16) & 0xFF;
            uint8_t b2_new = (stored >> 8)  & 0xFF;
            uint8_t b3_new = stored         & 0xFF;

            EEPROM.write(base_address    , b0_new);
            EEPROM.write(base_address + 1, b1_new);
            EEPROM.write(base_address + 2, b2_new);
            EEPROM.write(base_address + 3, b3_new);
        }
    }

    String timeToString(uint32_t time){
        uint32_t days = time / 1440;
        uint32_t hours = (time - days*1440) / 60;
        uint32_t minutes = (time - days*1440 - hours * 60 );
        
        String ret = "";
        ret += days;
        ret += " Days, ";
        ret += hours;
        ret += " Hours, ";
        ret += minutes;
        ret += " Minutes";

        return ret;
    }
}