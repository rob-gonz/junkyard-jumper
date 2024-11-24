#include "DPS310Sensor.h"
#include <Wire.h>

#define DPS310_ADDRESS 0x77 // Define the I2C address for the DPS310 sensor

/**
 * @brief Initialize the DPS310 sensor with the given I2C bus
 * 
 * @param wirePort 
 */
void DPS310Sensor::begin(TwoWire &wirePort) {
    dps310.begin_I2C(DPS310_ADDRESS, &wirePort);
    dps_temp = dps310.getTemperatureSensor();
    dps_pressure = dps310.getPressureSensor();

    // Setup highest precision
    dps310.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    dps310.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

}
/**
 * @brief Calibrate the starting pressure of the DPS310 sensor
 * 
 * @param useSeaLevelPressure 
 * @return float 
 */
float DPS310Sensor::calibrateStartingPressure(bool useSeaLevelPressure) {
    if (useSeaLevelPressure) {
        calibratedStartPressure = seaLevelPressure;
    } else {
        calibratedStartPressure = readPressure();
    }
    if (calibratedStartPressure == 0.0) {
        return -1.0;
    }
    return calibratedStartPressure;
}

/**
 * @brief Read the temperature from the DPS310 sensor in degrees Celsius
 * 
 * @return float 
 * 
 * @note Returns -273.15 if temperature is not available
 */
float DPS310Sensor::readTemperature() {
    sensors_event_t temp_event;
    if (dps310.temperatureAvailable()) {
        dps_temp->getEvent(&temp_event);
        // Serial.print(F("Temperature = "));
        // Serial.print(temp_event.temperature);
        // Serial.println(" *C");
        // Serial.println();
        return temp_event.temperature;
    }
    return -273.15; // Return absolute zero if temperature is not available
}

/**
 * @brief Read the pressure from the DPS310 sensor in hPa
 * 
 * @return float 
 * 
 * @note Returns 0.0 if pressure is not available
 */
float DPS310Sensor::readPressure() {
    sensors_event_t pressure_event;
    if (dps310.pressureAvailable()) {
        dps_pressure->getEvent(&pressure_event);
        // Serial.print(F("Pressure = "));
        // Serial.print(pressure_event.pressure);
        // Serial.println(" hPa"); 
        // Serial.println();
        lastPressureReading = pressure_event.pressure;
        return pressure_event.pressure;
    }
    
    return 0.0; // Return 0.0 if pressure is not available
}

/**
 * @brief Get the altitude in meters from the DPS310 sensor
 * 
 * @return float 
 * 
 * @note Returns the altitude in meters
 * FIXME: is double better than float for this???
 */
float DPS310Sensor::getAltitudeMeters() {
    float pressure = readPressure(); // in hPa
    const float exponent = 0.1903;  // Exponent in simplified formula
    const float scaleFactor = 44330; // Scale factor in meters
    return scaleFactor * (1 - pow(pressure / calibratedStartPressure, exponent));
}

/**
 * @brief Get the altitude in meters from the DPS310 sensor using the sea level pressure
 * 
 * @param fastCheck 
 * @return float 
 * 
 * @note Returns the altitude in meters
 */
float DPS310Sensor::getAltitudeFromSeaLevelPressure(bool fastCheck) {
    float pressure;

    if (fastCheck) {
        pressure = lastPressureReading ? lastPressureReading : readPressure();
    }else {
        pressure = readPressure();
    }
    const float exponent = 0.1903;  // Exponent in simplified formula
    const float scaleFactor = 44330; // Scale factor in meters
    return scaleFactor * (1 - pow(pressure / seaLevelPressure, exponent));
}