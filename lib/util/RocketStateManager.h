#ifndef ROCKET_STATE_MANAGER_H
#define ROCKET_STATE_MANAGER_H

#include <Arduino.h>
#include "BluetoothManager.h"


/* 
--- Define measured stages of the flight. ---
Measured stages of the flight are used to determine the current state of the rocket
based on the sensor data collected. It's a derived state.

PRE_LAUNCH: [DEFAULT] The rocket is on the launch pad and has not yet launched.
IGNITION: The rocket has launched and the engines are firing. (Basically indicates start - very short stage)
POWERED_ASCENT: The rocket is ascending under power and is experiencing positive acceleration.
COAST: The rocket is still ascending but the engines are no longer firing and experiencing negative acceleration.
APOGEE: The rocket has reached its highest point and is now descending. Acceleration is not trustworthy as wind can affect it.
DESCENT: The rocket is descending and is experiencing. Acceleration should be near the value of gravity
DEBUG: This is a debug state that is used to test the state manager. - code in here will change from time to time. Only accessable via blutooth Serial Commands


*/
enum FlightStage {
  PRE_LAUNCH,
  IGNITION,
  POWERED_ASCENT,
  COAST,
  APOGEE,
  DESCENT,
  TOUCH_DOWN,
  DEBUG
};

class RocketStateManager {
public:



  // Initialization functions
  static RocketStateManager& getInstance();
  void initStateVariables();

  // Update functions
  void updateGPSState(float latitude, float longitude, float altitude, float speedKmh, bool hasFix);
  void updateOrientationState(float x, float y, float z);
  void upateAccelerationState(float x, float y, float z);
  void updateAglAltitude(float altitude);
  void updateAslAltitude(float altitude);



  void updateTimeSinceLastStateUpdate(unsigned long time); // unneeded
  // void updateAirPressure(float pressure);// unneeded
  void updateOrientation(float x, float y, float z);// unneeded
  bool updateFlightStage(FlightStage stage);

  // Accessor functions
  FlightStage getFlightStage() { return flightStage; }

  /* Flight Monitoring Functions*/
  // Pre-launch monitoring for state change


  /*****************************/




  void evaluateState();
  void executeBluetoothCommand(BluetoothManager &btManager); // Updated method declaration


  // communication functions
  void displayState(BluetoothManager &btManager, bool toSerial, bool toBluetooth); // Updated method declaration

private:
  RocketStateManager() = default;
  unsigned long timeSinceLastStateUpdate;
  bool gpsHasFix;
  float gpsLatituteDegrees;
  float gpsLongitudeDegrees;
  float gpsAltitude; // meters (above sea level)
  float gpsSpeedKmh;
  // float airPressure; // hPa
  float aglAltitudeFromPressure; // meters (above ground level)
  float aslAltitudeFromPressure; // meters (above sea level)
  float orientationX;
  float orientationY;
  float orientationZ;
  float accelerationX;
  float accelerationY;
  float accelerationZ;
  //TODO: I think I can detect Rotation and acceleration from the BNO055 sensor

  // This variable should ONLY be updated through the updateFlightStage method. To do otherwise raises safety concerns.
  FlightStage flightStage;
};

#endif // ROCKET_STATE_MANAGER_H