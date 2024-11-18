#include "RocketStateManager.h"
#include "BluetoothManager.h"
#include <string>

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

bool RocketStateManager::updateFlightStage(FlightStage stage) {
  /*
  Ever update of the flightStage variable should be done through this method. 
  It should never be updated directly. This method will handle any special logic
  */
  if (flightStage == stage) return true; // No need to update if the stage is the same


  if (flightStage == DEBUG && (stage != PRE_LAUNCH && stage != DEBUG)) {
    Serial.println("Invalid stage transition from DEBUG mode");
    return false;
  }

  if ((flightStage != DEBUG && flightStage != PRE_LAUNCH) && stage < flightStage) {
    Serial.println("Invalid stage transition. Cannot move to a earlier stage.");
    //TODO: This is an error we want to log later.
    return false;
  }


  flightStage = stage;
  return true;
}


void RocketStateManager::executeBluetoothCommand(BluetoothManager &btManager) {
  /**
   * This method will be called in the main loop to check if a new command has been received from the Bluetooth module.
   * 
   * ONLY update flightStage variable through the updateFlightStage method.
   */

  String command = btManager.getLastCommand();
  command.toUpperCase();

  btManager.sendStatusMessage("Executing command: '" + command + "'");
  Serial.println("Executing command: '" + command +"'");

  if (command == "DEBUG_MODE") {
    if (updateFlightStage(DEBUG)){
      Serial.println("DEBUG_MODE activated");
      btManager.sendStatusMessage("DEBUG_MODE activated\n");
    }
  }
  else if (command == "READY") {
    if (updateFlightStage(PRE_LAUNCH)){
      Serial.println("flightStage set to PRE_LAUNCH");
      btManager.sendStatusMessage("flightStage set to PRE_LAUNCH\n");
    }
  }
  else {
    Serial.println("Unknown command received: " + command);
  }

}

void RocketStateManager::evaluateState() {
  // Implement state evaluation logic here
}

void RocketStateManager::displayState(BluetoothManager &btManager, bool toSerial, bool toBluetooth) {
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
    btManager.sendStatusMessage(stateInfo.c_str());
  }
}