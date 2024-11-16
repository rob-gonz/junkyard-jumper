#include "RocketStateManager.h"
#include "BluetoothManager.h"

RocketStateManager& RocketStateManager::getInstance() {
  static RocketStateManager instance;
  return instance;
}

void RocketStateManager::initStateVariables() {
  timeSinceLastStateUpdate = 0;
  gpsLatitude = 0;
  gpsLongitude = 0;
  gpsAltitude = 0;
  airPressure = 0;
  altitudeFromPressure = 0;
  orientationX = 0;
  orientationY = 0;
  orientationZ = 0;
  flightStage = PRE_LAUNCH;
}

void RocketStateManager::updateTimeSinceLastStateUpdate(unsigned long time) {
  timeSinceLastStateUpdate = time;
}

void RocketStateManager::updateGPSCoordinates(float latitude, float longitude) {
  gpsLatitude = latitude;
  gpsLongitude = longitude;
}

void RocketStateManager::updateGPSAltitude(float altitude) {
  gpsAltitude = altitude;
}

void RocketStateManager::updateAirPressure(float pressure) {
  airPressure = pressure;
}

void RocketStateManager::updateAltitudeFromPressure(float altitude) {
  altitudeFromPressure = altitude;
}

void RocketStateManager::updateOrientation(float x, float y, float z) {
  orientationX = x;
  orientationY = y;
  orientationZ = z;
}

void RocketStateManager::updateFlightStage(FlightStage stage) {
  flightStage = stage;
}

void RocketStateManager::evaluateState() {
  // Implement state evaluation logic here
}

void RocketStateManager::displayState(bool toSerial, bool toBluetooth) {
  String stateInfo = "State: " + String(flightStage) +
                     ", Time: " + String(timeSinceLastStateUpdate) +
                     ", GPS: (" + String(gpsLatitude, 6) + ", " + String(gpsLongitude, 6) + ")" +
                     ", Altitude: " + String(gpsAltitude) +
                     ", Pressure: " + String(airPressure) +
                     ", Orientation: (" + String(orientationX) + ", " + String(orientationY) + ", " + String(orientationZ) + ")\n";
  if (toSerial) {
    Serial.println(stateInfo);
  }
  if (toBluetooth) {
    extern BluetoothManager btManager; // Declare the external btManager instance
    btManager.sendStatusMessage(stateInfo.c_str());
  }
}