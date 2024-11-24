// Core System Libraries
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

// Custom Sensor Libraries
#include "BNO055Sensor.h"
#include "AdafruitGPSSensor.h"
#include "DPS310Sensor.h"

// Custom Utility Libraries
#include "BluetoothManager.h"
#include "RocketStateManager.h"



/*---- PIN Definitions ----*/
// I2C Pins
#define SDA_PIN 1
#define SCL_PIN 2
#define DEDICATED_SDA_PIN 43
#define DEDICATED_SCL_PIN 44

// GPS Pins
#define GPS_RX_PIN 18
#define GPS_TX_PIN 17
/**************************/

/* Communication Serials*/
HardwareSerial GPS_Serial(1); // Define Serial1 as HardwareSerial instance



/*---- Global Object Declarations ----*/
TFT_eSPI tft = TFT_eSPI();                                                    // TFT display object
BNO055Sensor bnoSensor;                                                       // BNO055 sensor object
BluetoothManager& btManager = BluetoothManager::getInstance();                // Bluetooth LE Communication manager object
RocketStateManager& rocketStateManager = RocketStateManager::getInstance();   // Rocket state manager object
AdafruitGPSSensor* gpsSensor;                                                 // [POINTER] Adafruit GPS sensor object
DPS310Sensor& dps310Sensor = DPS310Sensor::getInstance();                     // DPS310 sensor object
/**************************************/

// Used for orientation sensor data
// TwoWire I2C_1 = TwoWire(0); // Create a TwoWire instance for the first I2C bus
// Default I2C bus plg-n-play
TwoWire I2C_2 = TwoWire(1); // Create a TwoWire instance for the second I2C bus

void setup() {
  
  // Initialize I2C buses
  // Eventually these will become a single bus after I rewire the board
  // I2C_1.begin(SDA_PIN, SCL_PIN); // Initialize the first I2C bus
  I2C_2.begin(DEDICATED_SDA_PIN, DEDICATED_SCL_PIN); // Initialize the second I2C bus
  
  // Initialize Serial Monitors
  Serial.begin(115200); // Initialize serial monitor early
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN); // Initialize Serial1 with RX and TX pins

  /* -------------------- TFT Display Initialization ---------------------*/
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Initializing", 10, 10);
  /* **********************************************************************/
  
  /* -------------------- BNO055 Sensor Initialization ---------------------*/
  //TODO: use I2C_1 bus for BNO055 sensor
  bnoSensor.begin(SDA_PIN, SCL_PIN); // Pass SDA and SCL pins to the begin method
  /* **********************************************************************/

  /* -------------------- Bluetooth Manager Initialization ---------------------*/
  btManager.begin();
  /* **********************************************************************/

  /* -------------------- Adafruit GPS Sensor Initialization ---------------------*/
  gpsSensor = new AdafruitGPSSensor(GPS_Serial); // Create an instance of AdafruitGPSSensor
  gpsSensor->begin(); //Initialize the GPS sensor
  /* **********************************************************************/
  
  /* -------------------- DPS310 Sensor Initialization ---------------------*/
  dps310Sensor.begin(I2C_2); // Initialize DPS310 sensor with the second I2C bus
  dps310Sensor.calibrateStartingPressure(); // Calibrate the starting pressure with the sea level pressure
  /* **********************************************************************/
  
  // Initialize rocket state manager variables
  rocketStateManager.initStateVariables();

  /**
   * TDOO: Run all calibrations here
   */
  // TODO: Calibrate the BNO055 sensor
  // TODO: Calibrate the DPS310 sensor
}

void loop() {
  // command handling
  if (btManager.isCommandAvailable()) rocketStateManager.executeBluetoothCommand(btManager); // Execute Bluetooth command if available

  /* --------------------- Update State Manager with Sensor Data ------------------------- */
  rocketStateManager.updateTimeSinceLastStateUpdate(millis());
  rocketStateManager.updateGPSState(gpsSensor->getLatitude(), gpsSensor->getLongitude(), gpsSensor->getAltitude(), gpsSensor->getSpeedKmh(), gpsSensor->hasFix());
  rocketStateManager.updateAglAltitude(dps310Sensor.getAltitudeMeters()); // Update altitude with DPS310 sensor data
  rocketStateManager.updateAslAltitude(dps310Sensor.getAltitudeFromSeaLevelPressure()); // Update altitude with DPS310 sensor data
  sensors_event_t orientationEvent, accelerationEvent;
  bnoSensor.getOrientation(orientationEvent);
  bnoSensor.getAcceleration(accelerationEvent);
  rocketStateManager.updateOrientation(orientationEvent.orientation.x, orientationEvent.orientation.y, orientationEvent.orientation.z);
  rocketStateManager.upateAccelerationState(accelerationEvent.acceleration.x, accelerationEvent.acceleration.y, accelerationEvent.acceleration.z);
  /************************************************************************************** */
  
  
  // Evaluate the current state of the rocket and update the flight stage as needed
  rocketStateManager.evaluateState();



  switch (rocketStateManager.getFlightStage()) {
    case PRE_LAUNCH:
      // Handle PRE_LAUNCH state
      Serial.println("-------------- PRE-LAUNCH MODE ACTIVE --------------");
      btManager.sendStatusMessage("---- PRE-LAUNCH ACTIVE ---- \n");
      // Detect launch and change state to IGNITION
      break;
    case IGNITION:
      // Handle IGNITION state
      break;
    case POWERED_ASCENT:
      // Handle POWERED_ASCENT state
      break;
    case COAST:
      // Handle COAST state
      break;
    case APOGEE:
      // Handle APOGEE state
      break;
    case DESCENT:
      // Handle DESCENT state
      break;
    case TOUCH_DOWN:
      // Handle TOUCH_DOWN state
      break;
    case DEBUG:
      // Handle DEBUG state
      Serial.println("-------------- DEBUG MODE ACTIVE --------------");
      btManager.sendStatusMessage("---- DEBUG MODE ACTIVE ---- \n");
      gpsSensor->outputDebugData(Serial);
      gpsSensor->outputDebugData(btManager);
      rocketStateManager.displayState(btManager, true, true);
      delay(5000); // Slow down for debug mode. 
      break;
  }

  bnoSensor.tftOutputUpdate(tft); // TFT display update from BNO055 sensor data
  // btManager.sendStatusMessage("Status update");

  // rocketStateManager.displayState(btManager, true, true); // Display rocket state on serial monitor and Bluetooth

  gpsSensor->update();
  // gpsSensor->outputData(Serial);
  // gpsSensor->outputData(btManager);

  delay(1000);
}




/*
TODO:
* Test how much memory is used by the current code.
* Test if disabling the BluetoothManager::sendStatusMessage() method call increases speed drastically.
* 










Notes:

read raw response form a serial communication port sending data to the board:
  // String response = "";
  // while(GPS_Serial.available()) {
  //   char c = GPS_Serial.read();
  //   response += c;
  // }
  // Serial.print("[loop]GPS RAW Response: ");
  // Serial.println(response);
*/