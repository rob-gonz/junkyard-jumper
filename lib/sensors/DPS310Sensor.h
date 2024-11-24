#ifndef DPS310SENSOR_H
#define DPS310SENSOR_H

#include <Adafruit_DPS310.h>
#include <Wire.h>

class DPS310Sensor {
public:
    static DPS310Sensor& getInstance() {
        static DPS310Sensor instance;
        return instance;
    }

    void begin(TwoWire &wirePort = Wire);
    float calibrateStartingPressure(bool useSeaLevelPressure = false);

    float readTemperature();
    float readPressure();
    float readAltitude();

    float getAltitudeMeters();
    float getAltitudeFromSeaLevelPressure(bool fastCheck = true);

private:
    DPS310Sensor() = default;
    DPS310Sensor(const DPS310Sensor&) = delete;
    DPS310Sensor& operator=(const DPS310Sensor&) = delete;

    Adafruit_DPS310 dps310;
    Adafruit_Sensor *dps_temp;
    Adafruit_Sensor *dps_pressure;
    float seaLevelPressure = 1013.25; // estimated sea level pressure in hPa
    float calibratedStartPressure = 1013.25; // ground pressure in hPa set at initialization
    float lastPressureReading = 0.0; // last pressure reading in hPa
};

#endif // DPS310SENSOR_H