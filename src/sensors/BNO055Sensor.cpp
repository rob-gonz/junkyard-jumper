#include "BNO055Sensor.h"

// Constructor: Initializes the BNO055 sensor with the specified ID and I2C address
BNO055Sensor::BNO055Sensor() : bno(55, 0x28) {}

// Initializes the BNO055 sensor and sets up I2C communication
void BNO055Sensor::begin(int sdaPin, int sclPin) {
    Wire.begin(sdaPin, sclPin); // Initialize I2C communication with specified SDA and SCL pins
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

void BNO055Sensor::tftOutputUpdate(TFT_eSPI &tft) {
    sensors_event_t orientationEvent;
    getOrientation(orientationEvent);

    sensors_event_t accelerationEvent;
    getAcceleration(accelerationEvent);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.print("Orientation");
    tft.setCursor(10, 30);
    tft.print("X: "); tft.print(orientationEvent.orientation.x, 4);
    tft.setCursor(10, 50);
    tft.print("Y: "); tft.print(orientationEvent.orientation.y, 4);
    tft.setCursor(10, 70);
    tft.print("Z: "); tft.print(orientationEvent.orientation.z, 4);

    tft.setCursor(10, 90);
    tft.print("Acceleration");
    tft.setCursor(10, 110);
    tft.print("X: "); tft.print(accelerationEvent.acceleration.x, 4);
    tft.setCursor(10, 130);
    tft.print("Y: "); tft.print(accelerationEvent.acceleration.y, 4);
    tft.setCursor(10, 150);
    tft.print("Z: "); tft.print(accelerationEvent.acceleration.z, 4);
}