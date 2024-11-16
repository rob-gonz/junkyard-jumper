#ifndef ROCKET_STATE_MANAGER_H
#define ROCKET_STATE_MANAGER_H

#include <Arduino.h>

enum FlightStage {
  PRE_LAUNCH,
  IGNITION,
  POWERED_ASCENT,
  COAST,
  APOGEE,
  DESCENT,
  TOUCH_DOWN
};

class RocketStateManager {
public:
  static RocketStateManager& getInstance();

  void initStateVariables();

  void updateTimeSinceLastStateUpdate(unsigned long time);
  void updateGPSCoordinates(float latitude, float longitude);
  void updateGPSAltitude(float altitude);
  void updateAirPressure(float pressure);
  void updateAltitudeFromPressure(float altitude);
  void updateOrientation(float x, float y, float z);
  void updateFlightStage(FlightStage stage);

  void evaluateState();
  void displayState(bool toSerial, bool toBluetooth); // Updated method declaration

private:
  RocketStateManager() = default;
  unsigned long timeSinceLastStateUpdate;
  float gpsLatitude;
  float gpsLongitude;
  float gpsAltitude;
  float airPressure;
  float altitudeFromPressure;
  float orientationX;
  float orientationY;
  float orientationZ;
  FlightStage flightStage;
};

#endif // ROCKET_STATE_MANAGER_H