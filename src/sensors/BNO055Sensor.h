#ifndef BNO055SENSOR_H
#define BNO055SENSOR_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// Class to interface with the BNO055 sensor
class BNO055Sensor {
public:
    // Constructor
    BNO055Sensor();

    // Initializes the BNO055 sensor
    void begin();

    // Retrieves the orientation data from the BNO055 sensor
    void getOrientation(sensors_event_t &event);

    // Retrieves the acceleration data from the BNO055 sensor
    void getAcceleration(sensors_event_t &event);

private:
    // Instance of the Adafruit_BNO055 class
    Adafruit_BNO055 bno;
};

#endif // BNO055SENSOR_H