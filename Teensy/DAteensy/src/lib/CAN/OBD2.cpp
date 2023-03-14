#include "OBD2.h"

namespace OBD2
{

    uint8_t idList[17] = {0x01, 0x03, 0x04, 0x05, 0x07, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x11, 0x14, 0x21, 0x33, 0x42, 0x45, 0x4d};
    uint8_t idIndex = 0;
    uint32_t time_received = 0;

    boolean contact = false;
    boolean readdtc_flag = false;

    FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> myCan;
    OBD2sensordata *_db;

    void initOBD2(OBD2sensordata &database)
    {
        // passed by reference, so other parts of the  code can access
        _db = &database;

        myCan.begin();
        myCan.setBaudRate(500000);
        myCan.setMaxMB(20);

        myCan.enableFIFO();
        myCan.enableFIFOInterrupt();

        myCan.setMB(MB8, RX, STD);
        myCan.enableMBInterrupt(MB8);

        myCan.onReceive(MB8, receivedRPMcallback);

        myCan.onReceive(FIFO, receivedOBD2callback);

        myCan.setMBFilter(REJECT_ALL);
        myCan.setMBFilter(MB8, 0x20A);

        // set up FIFO filter (OBD2)
        myCan.setFIFOFilter(REJECT_ALL);
        myCan.setFIFOFilter(0, 0x7E8, STD);

        // myCan.mailboxStatus();
    }

    void askPID(uint8_t id)
    {
        CAN_message_t askPID;
        askPID.id = 0x7DF;

        askPID.buf[0] = 0x02; // message length
        askPID.buf[1] = 0x01; // service 0x01 (current data)
        askPID.buf[2] = id;

        askPID.len = 8;
        myCan.write(askPID);
    }

    void clearDTC()
    {
        CAN_message_t askPID;
        askPID.id = 0x7DF;

        askPID.buf[0] = 0x01; // message length
        askPID.buf[1] = 0x04; // service 0x04 (clear dtc)

        askPID.len = 8;
        myCan.write(askPID);
    }

    uint8_t multiframe_response[32];
    uint8_t multiframe_length;
    uint8_t multiframe_readed_bytes;
    boolean multiframe_processed = true;

    void receivedOBD2callback(const CAN_message_t &msg)
    {
#ifdef DEBUG
// Serial.println("Callback reached: ");
#endif
        contact = true;

        // multi frame response
        if ((msg.buf[0] & 0b11110000) == 0x10)
        {
            multiframe_length = 0;
            multiframe_readed_bytes = 0;
            multiframe_processed = false;
            multiframe_length = msg.buf[1]; //(in bytes) the second half of the first byte too, but i think that will never need to receive so many bytes
            for (int i = 2; i < msg.len; i++)
            {
                multiframe_response[i - 2] = msg.buf[i];
                multiframe_readed_bytes++;
            }

            // ask for continuation
            CAN_message_t askCont;
            askCont.id = 0x7E0;

            askCont.buf[0] = 0x30; // ISO-TP message type 3, ask for continuation
            askCont.len = 8;
            myCan.write(askCont);
            return;
        }

        // multi frame response continuations
        if ((msg.buf[0] & 0b11110000) == 0x20)
        {
            for (int i = 1; i < 7; i++)
            {
                multiframe_response[multiframe_readed_bytes] = msg.buf[i] - 1;
                multiframe_readed_bytes++;
                if (multiframe_readed_bytes >= multiframe_length)
                {
                    break;
                }
            }
            if (multiframe_length != multiframe_readed_bytes)
            {
                return;
            }
        }

        // handle multiframe
        if (!multiframe_processed)
        {
            // for(int i = 0; i<multiframe_readed_bytes; i++)
            //     Serial.printf("%#x ", multiframe_response[i]);
            // Serial.println();
            dispatchMessage(multiframe_response, multiframe_length);
            multiframe_processed = true;
        }

        // if we need to ask for dtc
        if (readdtc_flag)
        {
            CAN_message_t askDTC;
            askDTC.id = 0x7DF;

            askDTC.buf[0] = 0x01; // message length
            askDTC.buf[1] = 0x03; // service 0x03 (get dtc)

            askDTC.len = 8;
            myCan.write(askDTC);

            readdtc_flag = false;
            return;
        }

        dispatchMessage((const uint8_t *)&msg.buf[1], msg.len - 1); // send without msg length, which is part of the ISO-TP protocol

        askPID(idList[idIndex]);
        idIndex = (idIndex + 1) % sizeof(idList);
        time_received = millis();
    }

    void dispatchMessage(const uint8_t msg[], uint8_t length)
    {

        uint8_t service = msg[0]; // 41: sensor data
        uint8_t pid = msg[1];
        uint8_t A = msg[2];
        uint8_t B = msg[3];

        if (service == 0x41)
        { // we've got a response
            switch (pid)
            {
            case MONITOR_STATUS_SINCE_DTCS_CLEARED:

                _db->MIL_on = A & 0b10000000;
                _db->DTC_CNT = A & 0b01111111;
                break;

            case FUEL_SYSTEM_STATUS:
                _db->fuel_system_status = A;
                break;

            case CALCULATED_ENGINE_LOAD:
                _db->Calculated_Engine_load = A;
                break;

            case ENGINE_COOLANT_TEMPERATURE:
                _db->Engine_coolant_temperature = A;
                break;

            case LONG_TERM_FUEL_TRIM_BANK_1:
                _db->long_term_fuel_trim = A;
                break;

            case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
                _db->intake_manifold_absolute_pressure = A;
                break;

            case ENGINE_RPM:
                _db->engine_rpmA = A;
                _db->engine_rpmB = B;
                break;

            case VEHICLE_SPEED:
                _db->vehicle_speed = A;
                break;

            case TIMING_ADVANCE:
                _db->timing_advance = A;
                break;

            case AIR_INTAKE_TEMPERATURE:
                _db->intake_air_temperature = A;
                break;

            case THROTTLE_POSITION:
                _db->throttle_position = A;
                break;

            case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:
                _db->oxygen_sensor_voltage = A;
                _db->oxygen_sensor_long_term_fuel_trim = B;
                break;

            case DISTANCE_TRAVELED_WITH_MIL_ON:
                _db->distance_traveled_MIL_on_A = A;
                _db->distance_traveled_MIL_on_B = B;
                break;

            case ABSOLULTE_BAROMETRIC_PRESSURE:
                _db->absolute_barometric_presure = A;
                break;

            case CONTROL_MODULE_VOLTAGE:
                _db->control_module_voltage_A = A;
                _db->control_module_voltage_B = B;
                break;

            case RELATIVE_THROTTLE_POSITION:
                _db->relavite_throttle_position = A;
                break;

            case TIME_RUN_WITH_MIL_ON:
                _db->time_run_with_mil_on_A = A;
                _db->time_run_with_mil_on_B = B;
                break;
            }
        }
        if (service == 0x43)
        {
            // Serial.println("handling dtc reading");
            //  print to screen
            uint16_t dtc_list[(length - 2) / 2];
            for (int i = 2, ii = 0; i < length; i += 2, ii++)
            {
                // Serial.print("i=");
                // Serial.print(i);
                // Serial.print(" ii=");
                // Serial.print(ii);
                dtc_list[ii] = (msg[i] << 8) + msg[i + 1];
                // Serial.print(" dtc_list[ii]=");
                // Serial.println(dtc_list[ii]);
            }

            DISPLAYY::sendDTCDebugScreen(dtc_list, (length - 2) / 2);
        }
    }

    uint8_t bufferRPM[8];
    void receivedRPMcallback(const CAN_message_t &msg)
    {
        for (int i = 0; i < msg.len; i++)
        {
            bufferRPM[i] = msg.buf[i];
        }
    }

    String getBufferRPM()
    {
        String ret = "";
        for (int i = 0; i < 8; i++)
        {
            ret += bufferRPM[i];
            ret += " ";
        }

        return ret;
    }

    void readDTC()
    {
        readdtc_flag = true;
    }

    void OBD2events()
    {
        // process can callbacks
        myCan.events();

        // restart asking if not response
        if (millis() - time_received > 250)
        {
            contact = false;
#ifdef DEBUG
            Serial.println("Failed to receive response, restarting");
#endif

            askPID(MONITOR_STATUS_SINCE_DTCS_CLEARED);
            time_received = millis();
        }
    }

    // prints obd2db object to serial
    void printOBD2ALL(OBD2sensordata database)
    {
        Serial.print("MIL_ON: ");
        Serial.println(database.MIL_on);
        Serial.print("DTC COUNT: ");
        Serial.println(database.DTC_CNT);
        Serial.println();

        Serial.print("FUEL SYSTEM STATUS: ");
        switch (database.fuel_system_status)
        {
        case 0:
            Serial.println("motor OFF");
            break;
        case 1:
            Serial.println("Open loop due to insufficient engine temperature");
            break;
        case 2:
            Serial.println("Closed loop, using oxygen sensor feedback to determine fuel mix");
            break;
        case 4:
            Serial.println("Open loop due to engine load OR fuel cut due to deceleration");
            break;
        case 8:
            Serial.println("Open loop due to system failure");
            break;
        case 16:
            Serial.println("Closed loop, using at least one oxygen sensor but there is a fault in the feedback system");
        default:
            Serial.println("Invalid response");
        }

        Serial.println();

        Serial.print("Calculated Engine Load: ");
        Serial.print(database.Calculated_Engine_load / 2.55, 2);
        Serial.println("%");

        Serial.println();

        Serial.print("coolant Temp: ");
        Serial.print(float(database.Engine_coolant_temperature) - 40.0, 2);
        Serial.println("ºC");
        Serial.print("Air Temp: ");
        Serial.print(float(database.intake_air_temperature) - 40.0, 2);
        Serial.println("ºC");

        Serial.println();

        Serial.print("Long term fuel trim: ");
        Serial.print(database.long_term_fuel_trim / 1.28 - 100, 2);
        Serial.println("%");
        Serial.print("Intake manifold absolute pressure: ");
        Serial.print(database.intake_manifold_absolute_pressure);
        Serial.println("kPa");
        Serial.print("Absolute barometric presure: ");
        Serial.print(database.absolute_barometric_presure);
        Serial.println("kPa");

        Serial.println();
        Serial.print("Engine RPM: ");
        Serial.print((database.engine_rpmA * 256 + database.engine_rpmB) / 4.0, 2);
        Serial.println("rpm");
        Serial.print("Vehicle Speed: ");
        Serial.print(database.vehicle_speed);
        Serial.println("km/h");

        Serial.println();

        Serial.print("Timing advance: ");
        Serial.print(database.timing_advance / 2.0 - 64, 2);
        Serial.println("º before TDC");
        Serial.print("Throttle position: ");
        Serial.print(database.throttle_position / 2.55, 2);
        Serial.println("%");
        Serial.print("Relative throttle position: ");
        Serial.print(database.relavite_throttle_position / 2.55, 2);
        Serial.println("%");

        Serial.println();

        Serial.print("Oxygen sensor voltage: ");
        Serial.print(database.oxygen_sensor_voltage / 200.0, 2);
        Serial.println("V");
        Serial.print("Oxygen sensor long term fuel trim: ");
        Serial.print(database.oxygen_sensor_long_term_fuel_trim / 1.28 - 100, 2);
        Serial.println("%");

        Serial.println();

        Serial.print("Control module voltage: ");
        Serial.print((256 * database.control_module_voltage_A + database.control_module_voltage_B) / 1000.0, 2);
        Serial.println("V");
        Serial.print("Distance Traveled MIL: ");
        Serial.print(256 * database.distance_traveled_MIL_on_A + database.control_module_voltage_B);
        Serial.println("km");
        Serial.print("Time Traveled MIL: ");
        Serial.print(256 * database.time_run_with_mil_on_A + database.time_run_with_mil_on_B);
        Serial.println("minutes");
    }

    // transform OBD2db object to csv format
    String OBD2toCSV(OBD2sensordata database)
    {
        String ret = "";
        // MIL: ON or OFF
        if (database.MIL_on)
        {
            ret += "ON";
        }
        else
        {
            ret += "OFF";
        }

        ret += ",";

        // NUMBER OF DTC on
        ret += database.DTC_CNT;
        ret += ",";

        // fuel_system_status
        ret += database.fuel_system_status;

        ret += ",";

        // calculated engine load
        ret += String(database.Calculated_Engine_load / 2.55, 2);
        ret += ",";

        // coolant Temp
        ret += String(float(database.Engine_coolant_temperature) - 40.0, 2);
        ret += ",";

        // Air Temp
        ret += String(float(database.intake_air_temperature) - 40.0, 2);
        ret += ",";

        // Long term fuel trim:
        ret += String(database.long_term_fuel_trim / 1.28 - 100, 2);
        ret += ",";

        // Intake manifold absolute pressure
        ret += String(database.intake_manifold_absolute_pressure);
        ret += ",";

        // Absolute barometric presure
        ret += String(database.absolute_barometric_presure);
        ret += ",";

        // rpm
        ret += String((database.engine_rpmA * 256 + database.engine_rpmB) / 4.0, 2);
        ret += ",";

        // vehicle speed (OBD2)
        ret += String(database.vehicle_speed);
        ret += ",";

        // Timing advance
        ret += String(database.timing_advance / 2.0 - 64, 2);
        ret += ",";

        // Throttle position
        ret += String(database.throttle_position / 2.55, 2);
        ret += ",";

        // Relative throttle position
        ret += String(database.relavite_throttle_position / 2.55, 2);
        ret += ",";

        // Oxygen sensor voltage
        ret += String(database.oxygen_sensor_voltage / 200.0, 2);
        ret += ",";

        // Oxygen sensor long term fuel trim:
        ret += String(database.oxygen_sensor_long_term_fuel_trim / 1.28 - 100, 2);
        ret += ",";

        // Control module voltage:
        ret += String((256 * database.control_module_voltage_A + database.control_module_voltage_B) / 1000.0, 2);
        ret += ",";

        // Distance Traveled MIL:
        ret += String(256 * database.distance_traveled_MIL_on_A + database.control_module_voltage_B);
        ret += ",";
        //
        ret += String(256 * database.time_run_with_mil_on_A + database.time_run_with_mil_on_B);

        return ret;
    }

    boolean isContact()
    {
        return contact;
    }

    void emulateDash(uint8_t gear)
    {
        uint8_t to_send = gear << 5;
        CAN_message_t dashGear;
        dashGear.id = 0x236;
        dashGear.buf[0] = to_send; // put gear over here
        dashGear.len = 1;

        // Serial.printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(dashGear.buf[0]));
        // Serial.println();

        CAN_message_t dashButtons;
        dashButtons.id = 0x22e;
        dashButtons.buf[0] = 0x0; // intermitent buttons and so on
        dashButtons.buf[1] = 0x0;
        dashButtons.len = 2;

        myCan.write(dashGear);
        myCan.write(dashButtons);
    }

}
