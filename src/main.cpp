#include <Arduino.h>
#include <TFT_eSPI.h>
#include "sensors/BNO055Sensor.h"
#include "util/BluetoothManager.h"
#include "util/RocketStateManager.h"

/* PIN Definitions*/

// I2C Pins
#define SDA_PIN 1
#define SCL_PIN 2

TFT_eSPI tft = TFT_eSPI();   // TFT display object
BNO055Sensor bnoSensor;     // BNO055 sensor object
BluetoothManager btManager; // Bluetooth manager object
RocketStateManager& rocketStateManager = RocketStateManager::getInstance();

void setup() {
  Serial.begin(115200); // Initialize serial monitor early
  
  /* -------------------- TFT Display Initialization ---------------------*/
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Initializing", 10, 10);
  /* **********************************************************************/
  
  /* -------------------- BNO055 Sensor Initialization ---------------------*/
  bnoSensor.begin(SDA_PIN, SCL_PIN); // Pass SDA and SCL pins to the begin method
  /* **********************************************************************/

  btManager.begin();
}

void loop() {
  // Update rocket state manager with sensor data
  rocketStateManager.updateTimeSinceLastStateUpdate(millis());
  // Add calls to update other state variables as needed

  rocketStateManager.evaluateState();

  bnoSensor.tftOutputUpdate(tft); // TFT display update from BNO055 sensor data
  btManager.sendStatusMessage("Status update");

  delay(500);
}

