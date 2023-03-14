#include "SteeringWheel.h"
HardwareSerial* ser;

namespace DISPLAYY {
    void rpmled(int nleds){
    switch (nleds){
        case 0:
        analogWrite(33,0);
        break;
        case 1:
        analogWrite(33, 10);
        break;
        case 2:
        analogWrite(33,25);
        break;
        case 3:
        analogWrite(33,43);
        break;
        case 4:
        analogWrite(33,55);
        break;
        case 5:
        analogWrite(33,74);
        break;
        case 6:
        analogWrite(33, 87);
        break;
        case 7:
        analogWrite(33,100);
        break;
        case 8:
        analogWrite(33,117);
        break;
        case 9:
        analogWrite(33,130);
        break;
        case 10:
        analogWrite(33,150);
        break;


    }
    }

    void rpmledInverse(int nleds){
    switch (nleds){
        case 0:
        analogWrite(33,255);
        break;
        case 1:
        analogWrite(33, 135);
        break;
        case 2:
        analogWrite(33,120);
        break;
        case 3:
        analogWrite(33,110);
        break;
        case 4:
        analogWrite(33,87);
        break;
        case 5:
        analogWrite(33,74);
        break;
        case 6:
        analogWrite(33, 55);
        break;
        case 7:
        analogWrite(33,40);
        break;
        case 8:
        analogWrite(33,30);
        break;
        case 9:
        analogWrite(33,10);
        break;
        case 10:
        analogWrite(33,0);
        break;


    }
    }


    void initScreen(HardwareSerial &_serial){
        ser = &_serial;
        ser->begin(115200);

        void* buffer = malloc(1024);

        ser->addMemoryForWrite(buffer, 1024);
        //ser->print("bauds=921600");

    }

    void endMessage(){
        byte a = 0xff;
        for ( int i = 0; i < 3; i ++ ){
        ser->write(a);
        }
    }


    void sendGear(int str) {

        if (str == 255){
            ser->printf("gear.txt=\"E\"");
            endMessage();
            return;
        }
        
        ser->printf("gear.txt=\"%d\"",str);
        endMessage();
    }

    void sendDesiredGear(int str){
        if (str == 255){
            ser->printf("gear2.txt=\"E\"");
            endMessage();
            return;
        }
        if (str == 128){
            ser->printf("gear2.txt=\"F\"");
            endMessage();
            return;
        }
        
        ser->printf("gear2.txt=\"%d\"",str);
        endMessage();

    }

    void sendOil(bool str){
        if (str){
            ser->printf("oil.txt=\"OFF\"");
        } else{
            ser->printf("oil.txt=\"ON\"");
        }
        endMessage();
    }

    void sendRPM(int str) {
        
        ser->printf("rpm.txt=\"%d\"",str);
        endMessage();

    }
    void sendVoltage(float str) {
        
        ser->printf("voltage.txt=\"%.2f\"",str);
        endMessage();

    }

    void sendCOLTMP(float str) {
        
        ser->printf("watertemp.txt=\"%.1f\"",str);
        endMessage();

    }

    void sendAIRTMP(float str) {
        
        ser->printf("airtemp.txt=\"%.1f\"",str);
        endMessage();

    }

    void sendTPS(float str){
        uint8_t str_int = round(str);
        //ser->printf("throttlebar.val=%d",str_int);
        //endMessage();
        ser->printf("throttle.txt=\"%.1f\"%",str);
        endMessage();

    }

    void sendTPSrel(float str){
        uint8_t str_int = round(str);
        //ser->printf("throttlebar.val=%d",str_int);
        //endMessage();
        ser->printf("throttlerel.txt=\"%.1f\"%",str);
        endMessage();

    }
    void sendEngineLoad(float str){
        uint8_t str_int = round(str);
        ser->printf("engineload.txt=\"%2.2f\"%",str);
        endMessage();

    }

    void sendTrim(float str){
        ser->printf("FT.txt=\"%.3f\"",str);
        endMessage();

    }

    void send02Trim(float str){
        ser->printf("O2trim.txt=\"%.3f\"",str);
        endMessage();
    }

    void send02Volt(float str){
        ser->printf("O2v.txt=\"%.2f\"",str);
        endMessage();
    }


    void sendDTCcount(int str){
        ser->printf("dtc.txt=\"%d\"",str);
        endMessage();
    }

    void sendAbsPressure(int str){
        ser->printf("presAbs.txt=\"%d\"",str);
        endMessage();

    }

    void sendIntakePressure(int str){
        ser->printf("presIntake.txt=\"%d\"",str);
        endMessage();

    }

    void sendFuelSystemStatus(int sta){
        char tosend[32];;
        if (sta == 0){
            char yeah[] = "ENGINE OFF";
            strcpy(tosend,yeah);
        } else if (sta == 1) {
            char yeah[] = "OPEN LOOP \\rNO ENOUGH\\rTEMP";
            strcpy(tosend,yeah);

        } else if (sta == 2){
            char yeah[] = "CLOSED \\r LOOP";
            strcpy(tosend,yeah);
        } else if (sta == 4){
            char yeah[] = "OPEN LOOP\\rDUE TO\\rfuel cut decel";
            strcpy(tosend,yeah);
        } else if (sta == 8){
            char yeah[] = "OPEN LOOP\\rSYSTEM \\rFAILURE";
            strcpy(tosend,yeah);

        } else if (sta == 16){
            char yeah[] = "CLOSED \\rLOOP\\rWITH FAULTS";
            strcpy(tosend,yeah);

        }

        //Serial.printf("fss.txt=\"%s\"\n",tosend);
        ser->printf("fss.txt=\"%s\"",tosend);
        endMessage();
    }

    void sendTimingAdvance(float sta){
        ser->printf("Tadv.txt=\"%.1f\"",sta);
        endMessage();
    }

    void sendSpeed(int sta){
        ser->printf("speed.txt=\"%d\"",sta);
        endMessage();
    }


    void setMainScreen(){
        ser->printf("page 1");
        endMessage();
    }

    void setSensorScreen(){
    ser->printf("page 2");
    endMessage();
    }

    void setSplashScreen(){
        ser->printf("page 0");
        endMessage();

    }

    void setDebugScreen(){
        ser->printf("page 3");
        endMessage();
    }

    void sendDebugmsg(int msg, const char* debugstring){
    //    Serial.printf("msg%d.txt=\"%s\"\n",msg,debugstring);
        ser->printf("msg%d.txt=\"%s\"",msg,debugstring);
        endMessage();
        delay(100);
    }

    void sendTime(){
        String to_send = "";
        to_send += "Time Alive: ";
        to_send += PERSISTANCE::timeToString(PERSISTANCE::readCounter(EEPROM_time_base_address));
        to_send += "\\rTime Engine On: ";
        to_send += PERSISTANCE::timeToString(PERSISTANCE::readCounter(EEPROM_fss_base_address));
        sendDebugmsg(7, to_send.c_str());
    }

    void sendTimeEngineOn(uint32_t str){
        uint32_t hours = str / 3600;
        uint32_t minutes = (str - hours * 3600) /60;
        uint32_t seconds = (str - hours*3600 - minutes * 60);
        ser->printf("time.txt=\"%d:%d:%d\"",hours, minutes, seconds);
        endMessage();
    }
    #define __disable_irq() __asm__ volatile("CPSID i":::"memory");
    #define __enable_irq()  __asm__ volatile("CPSIE i":::"memory");


    void sendDTCDebugScreen(const uint16_t msg[], uint8_t length){
        //__disable_irq();
        //for(int i = 0; i<length; i++)
        //Serial.printf("%#x ", msg[i]);
        //Serial.println();

        for (int i = 0; i < length; i++){
            char* yeah = OBD2CONVERSIONS::getDTCstring(msg[i]);
            if (yeah != NULL){
                sendDebugmsg(i, yeah);
            } else{
            char yeah2[16];
            sprintf(yeah2, "%x",msg[i]);
            sendDebugmsg(i, yeah);
            }

            if (i >= 6){
                break;
            }
        }

        sendTime();
        //__enable_irq();
    }


    void sendOBDdata(OBD2::OBD2sensordata OBD2db){
        // update screen
        sendRPM(OBD2CONVERSIONS::OBD2RPM(OBD2db));
        sendCOLTMP(OBD2CONVERSIONS::OBD2TMP(OBD2db.Engine_coolant_temperature));
        sendAIRTMP(OBD2CONVERSIONS::OBD2TMP(OBD2db.intake_air_temperature));
        sendTPS(OBD2CONVERSIONS::OBD2PC(OBD2db.throttle_position));
        sendTPSrel(OBD2CONVERSIONS::OBD2PC(OBD2db.relavite_throttle_position));

        sendAbsPressure(OBD2db.absolute_barometric_presure);
        sendIntakePressure(OBD2db.intake_manifold_absolute_pressure);
        sendDTCcount(OBD2db.DTC_CNT);

        sendVoltage(OBD2CONVERSIONS::OBD2Volt(OBD2db));
        sendEngineLoad(OBD2CONVERSIONS::OBD2PC(OBD2db.Calculated_Engine_load));
        sendFuelSystemStatus(OBD2db.fuel_system_status);

        sendTrim(OBD2CONVERSIONS::OBD2Trim(OBD2db.long_term_fuel_trim));
        send02Trim(OBD2CONVERSIONS::OBD2Trim(OBD2db.oxygen_sensor_long_term_fuel_trim));
        send02Volt(OBD2CONVERSIONS::OBD2VoltO2(OBD2db.oxygen_sensor_voltage));

        sendTimingAdvance(OBD2CONVERSIONS::OBD2Advance(OBD2db.timing_advance));
        sendSpeed(OBD2db.vehicle_speed);

        sendDesiredGear(GEAR::getDesiredGear());
    }
}

