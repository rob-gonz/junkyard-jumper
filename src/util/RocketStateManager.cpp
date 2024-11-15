
#include "RocketStateManager.h"

RocketStateManager& RocketStateManager::getInstance() {
  static RocketStateManager instance;
  return instance;
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