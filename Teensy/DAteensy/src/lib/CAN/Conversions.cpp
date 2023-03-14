#include "OBD2.h"

namespace OBD2CONVERSIONS{
    uint16_t OBD2RPM(OBD2::OBD2sensordata db){
        return (db.engine_rpmA*256+db.engine_rpmB)/4;
    }


    float OBD2TMP(uint8_t tmp){
            // coolant Temp
        return float(tmp) - 40.0;
    }

    float OBD2PC(uint8_t tmp){
        return float(tmp) / 2.55;
    }

    float OBD2Trim(uint8_t tmp){
        return float(tmp)/1.28 - 100;
    }

    float OBD2Volt(OBD2::OBD2sensordata db){
        return(db.control_module_voltage_A*256+db.control_module_voltage_B)/1000.0;
    }

    float OBD2VoltO2(uint8_t v){
        return v / 200;
    }

    float OBD2Advance(uint8_t v){
        return (v / 2)-64;
    }

}

