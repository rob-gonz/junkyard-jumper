#ifndef ADAFRUIT_GPS_SENSOR_H
#define ADAFRUIT_GPS_SENSOR_H

#define NO_SW_SERIAL 

#include <Adafruit_GPS.h>
#include "BluetoothManager.h"

class AdafruitGPSSensor {
public:
    bool startupSuccessful = false;

    // Initialization functions
    AdafruitGPSSensor(HardwareSerial &serialPort);
    void begin();

    // Gps data update functions
    void update();
    void clearGPSData();

    // Accessor functions
    float getLatitude() { return gps.latitudeDegrees; }
    float getLongitude() { return gps.longitudeDegrees; }
    float getAltitude() { return gps.altitude; }
    float getSpeedKnots() { return gps.speed; }
    float getSpeedMph() { return gps.speed * 1.15078; }
    float getSpeedKmh() { return gps.speed * 1.852; }
    float getHeading() { return gps.angle; }
    bool hasFix() { return gps.fix; }



    // Debug output functions
    void outputDebugData(Stream &output);
    void outputDebugData(BluetoothManager &btManager);
    



private:
    Adafruit_GPS gps;
    HardwareSerial& serialPort;
};

#endif // ADAFRUIT_GPS_SENSOR_H