#ifndef ADAFRUIT_GPS_SENSOR_H
#define ADAFRUIT_GPS_SENSOR_H

#define NO_SW_SERIAL 

#include <Adafruit_GPS.h>
#include "BluetoothManager.h"

class AdafruitGPSSensor {
public:
    bool startupSuccessful = false;

    AdafruitGPSSensor(HardwareSerial &serialPort);
    void begin();
    void update();
    void outputDebugData(Stream &output);
    void outputDebugData(BluetoothManager &btManager);
    bool hasFix();
    void clearGPSData();

private:
    Adafruit_GPS gps;
    HardwareSerial& serialPort;
};

#endif // ADAFRUIT_GPS_SENSOR_H