#include "BNO055Sensor.h"

// Constructor: Initializes the BNO055 sensor with the specified ID and I2C address
BNO055Sensor::BNO055Sensor() : bno(55, 0x28) {}

// Initializes the BNO055 sensor and sets up I2C communication
void BNO055Sensor::begin() {
    Wire.begin(1, 2); // Initialize I2C communication with SDA on pin 1 and SCL on pin 2
    if (!bno.begin()) {
        Serial.println("No BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1); // Halt execution if sensor initialization fails
    }
    Serial.println("BNO055 detected and initialized successfully");
    Serial.println("Calibrating BNO055 Do Not Touch ...");
    delay(1000); // Wait for sensor to stabilize
    bno.setExtCrystalUse(true); // Use external crystal for better accuracy
    delay(1000); // Wait for sensor to stabilize
}

// Retrieves the orientation data from the BNO055 sensor
void BNO055Sensor::getOrientation(sensors_event_t &event) {
    bno.getEvent(&event);
}

// Retrieves the acceleration data from the BNO055 sensor
void BNO055Sensor::getAcceleration(sensors_event_t &event) {
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    
    // // Debugging output to check if the sensor is providing valid data
    // Serial.print("Debug: Acceleration X: "); Serial.print(event.acceleration.x);
    // Serial.print(" Y: "); Serial.print(event.acceleration.y);
    // Serial.print(" Z: "); Serial.println(event.acceleration.z);
}