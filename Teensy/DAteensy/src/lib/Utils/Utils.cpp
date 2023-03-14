#include "Utils.h"

namespace UTILS {
    int counter = 0;
    int ms = 0;
    void printFreq(){
            if (millis()-ms > 1000){
            Serial.println(counter);
            counter = 0;
            ms = millis();
        }
        counter++;
    }

}


