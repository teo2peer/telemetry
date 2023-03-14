#include "OBD2.h"
/*
const char* getDTCstring(uint16_t dtc){
    Serial.printf("%x \n",dtc);
int codes[30];
const char *a[30];
a[0] = "P0030,HO2S Heater Control Circuit \\r(Bank 1 Sensor 1) ";
codes[0] = 0x30;


a[1] = "P0107,Manifold Absolute Pressure \\r Circuit Low Input";
codes[1] = 0x107;

a[2] = "P0108,Manifold Absolute Pressure \\r Circuit High Input";
codes[2] = 0x108;

a[3] = "P0112,Intake Air Temperature \\rCircuit Low Input";
codes[3] = 0x112;

a[4] = "P0113,Intake Air Temperature \\rCircuit High Input";
codes[4] = 0x113;

a[5] = "P0117,Engine Coolant Temperature \\rCircuit Low Input";
codes[5] = 0x117;

a[6] = "P0118,Engine Coolant Temperature \\rCircuit High Input";
codes[6] = 0x118;

a[7] = "P0122,Throttle Position Sensor 1 Low Input";
codes[7] = 0x122;

a[8] = "P0123,Throttle Position Sensor 2 High Input";
codes[8] = 0x123;

a[9] = "P0132,O2 Sensor Circuit Low Voltage \\r (Bank 1 Sensor 1)";
codes[9] = 0x132;

a[10] = "P0170, Fuel Trim Malfunction";
codes[10] = 0x170;

a[11] = "P0201,Primary Injector Circuit Malfunction\\r - Cylinder 1";
codes[11] = 0x201;

a[12] = "P0202,Primary Injector Circuit Malfunction\\r - Cylinder 2";
codes[12] = 0x202;

a[13] = "P0335,Crankshaft Position Sensor Malfunction";
codes[13] = 0x335;

a[14] = "P0351,Ignition Coil Malfunction Cylind 1";
codes[14] = 0x351;

a[15] = "P0352,Ignition Coil Malfunction Cylind 2";
codes[15] = 0x352;

a[16] = "P0500,Rear Speed Sensor Malfunction";
codes[16] = 0x500;

a[17] = "P0507,Idle Control System RPM Lower \\r Than Expected";
codes[17] = 0x507;

a[18] = "P0511,Idle Air Control Circuit ";
codes[18] = 0x511;

a[19] = "P0560,¿Power Steering Pressure Sensor \\r Circuit Intermittent?";
codes[19] = 0x560;

a[20] = "P0601,Internal ECU Malfunction \\r(bad checksum)";
codes[20] = 0x601;

a[21] = "P0657,Fuel System Voltage Out of Range ";
codes[21] = 0x657;

a[22] = "P1601,Throttle Control Unit Throttle Position\\r Malfunction";
codes[22] = 0x1601;

a[23] = "P062F, Generic Internal Control Module\\r Warning";
codes[23] = 0x62f;

a[24] = "P1602,Internal ECU Power Supply Malfunction";
codes[24] = 0x1602;

a[25] = "P1604,TCU Unable To Control Desired Throttle\\r Angle";
codes[25] = 0x1604;

a[26] = "P1605,Loss of Power; Open Circuit";
codes[26] = 0x1605;

a[27] = "P2195,O2 Sensor Signal Stuck Lean Bank1\\r Sensor 1 ";
codes[27] = 0x2195;

a[28] = "P2A0C,ISO/SAE Reserved";
codes[28] = 0x2a0c;

a[29] = "P2A0D,ISO/SAE Reserved";
codes[29] = 0x2a0d;

for (int i = 0; i <30; i++){
        if (codes[i] == dtc){
            return a[i];
        }
    }
    return NULL;
}*/