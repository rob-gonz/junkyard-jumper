// Core System Libraries
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

// Custom Sensor Libraries
#include "BNO055Sensor.h"
#include "AdafruitGPSSensor.h"

// Custom Utility Libraries
#include "BluetoothManager.h"
#include "RocketStateManager.h"



/*---- PIN Definitions ----*/
// I2C Pins
#define SDA_PIN 1
#define SCL_PIN 2

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
/**************************************/

void setup() {
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
  bnoSensor.begin(SDA_PIN, SCL_PIN); // Pass SDA and SCL pins to the begin method
  /* **********************************************************************/

  /* -------------------- Bluetooth Manager Initialization ---------------------*/
  btManager.begin();
  /* **********************************************************************/

  /* -------------------- Adafruit GPS Sensor Initialization ---------------------*/
  gpsSensor = new AdafruitGPSSensor(GPS_Serial); // Create an instance of AdafruitGPSSensor
  gpsSensor->begin(); //Initialize the GPS sensor
  /* **********************************************************************/
  
  // Initialize rocket state manager variables
  rocketStateManager.initStateVariables();
}

void loop() {
  // Update rocket state manager with sensor data
  rocketStateManager.updateTimeSinceLastStateUpdate(millis());
  // command handling
  if (btManager.isCommandAvailable()) rocketStateManager.executeBluetoothCommand(btManager); // Execute Bluetooth command if available



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