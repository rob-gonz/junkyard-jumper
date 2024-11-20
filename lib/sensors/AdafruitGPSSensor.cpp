#include "AdafruitGPSSensor.h"

// Remove singleton pattern
// AdafruitGPSSensor& AdafruitGPSSensor::getInstance() {
//     static AdafruitGPSSensor instance;
//     return instance;
// }

#define GPSECHO false

/* ------------------------- Initialization functions -------------------------*/

AdafruitGPSSensor::AdafruitGPSSensor(HardwareSerial &serialPort) : serialPort(serialPort) {
    // serialPort.begin(9600, SERIAL_8N1, 18, 17);  // TODO: fix this
    this->gps = Adafruit_GPS(&serialPort);
}

void AdafruitGPSSensor::begin() {
    startupSuccessful = gps.begin(9600);
    if (!startupSuccessful) Serial.println("GPS sensor failed to initialize.");

    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    // gps.sendCommand(PGCMD_ANTENNA);
}

/**************************************************************************** */


/* ------------------------- GPS data update functions -------------------------*/
void AdafruitGPSSensor::update() {
    /*
    Update should be called frequently to ensure that the GPS sensor data is up to date.
    You would only want to stop call this if you are trying to conserve power. Or GPS data is no longer needed.
    */
   clearGPSData();

    char c;
    while (!gps.newNMEAreceived()) {
        c = gps.read();
        if (GPSECHO)
            if (c) Serial.print(c);
    }
    gps.parse(gps.lastNMEA());
}

void AdafruitGPSSensor::clearGPSData() {
    /*
    Helper function to clear out any queue dat from the GPS module 
    so that the next time we attempt to read data, we get the most recent data.
    */
    char c;
    while(!gps.newNMEAreceived()) {
        c = gps.read();
    }
    gps.parse(gps.lastNMEA());

    while(!gps.newNMEAreceived()) {
        c = gps.read();
    }
    gps.parse(gps.lastNMEA());
    
}

/**************************************************************************** */



/* ------------------------- Debug output functions -------------------------*/
// TODO: Need a recordData method to store the data or something so that it can be logged in sthe state manager
void AdafruitGPSSensor::outputDebugData(Stream &output) {
   
    // Generic information
    output.println(" --- GPS Data ---");
    output.print("GPS Fix: "); 
    gps.fix ? output.println("Yes") : output.println("No");
    output.print("Tracking "); output.print(gps.satellites); output.print(" satellites");
    output.print(" with quality "); output.println(gps.fixquality);

    output.print("Time: ");
    output.print(gps.hour, DEC); output.print(':');
    output.print(gps.minute, DEC); output.print(':');
    output.print(gps.seconds, DEC); output.print('.');
    output.println(gps.milliseconds);

    // output.print(gps.secondsSinceTime()); output.println(" seconds since last time");

    // Standard output
    // output.print("Location: ");
    // output.print(gps.latitude, 4); output.print(gps.lat);
    // output.print(", ");
    // output.print(gps.longitude, 4); output.println(gps.lon);

    // Degress, as used by Google Maps
    output.print("Location: ");
    output.print(gps.latitudeDegrees, 6); //output.print(gps.lat);
    output.print(", ");
    output.print(gps.longitudeDegrees, 6); //output.println(gps.lon);

    output.print("Speed (knots): "); output.println(gps.speed);
    output.print("Speed (mph): "); output.println(gps.speed * 1.15078);
    output.print("Heading (degrees): "); output.println(gps.angle); // What is this?
    output.print("Altitude (meters): "); output.println(gps.altitude);
    
    output.println(" -- End GPS Data --");
}

void AdafruitGPSSensor::outputDebugData(BluetoothManager &btManager) {
     // Generic information

    btManager.sendStatusMessage(" --- GPS Data ---\n");
    btManager.sendStatusMessage("GPS Fix: " + String(gps.fix ? "Yes\n" : "No\n"));
    if (gps.fix) {
        btManager.sendStatusMessage("Tracking " + String(gps.satellites) + " satellites with quality " + String(gps.fixquality) + "\n");
        btManager.sendStatusMessage("Time: " + String(gps.hour) + ":" + String(gps.minute) + ":" + String(gps.seconds) + "." + String(gps.milliseconds) + "\n");
        btManager.sendStatusMessage("Location: " + String(gps.latitudeDegrees, 6) + ", " + String(gps.longitudeDegrees, 6) + "\n");
        btManager.sendStatusMessage("Speed (knots): " + String(gps.speed) + "\n");
        btManager.sendStatusMessage("Speed (mph): " + String(gps.speed * 1.15078) + "\n");
        btManager.sendStatusMessage("Heading (degrees): " + String(gps.angle) + "\n");
        btManager.sendStatusMessage("Altitude (meters): " + String(gps.altitude) + "\n");
    }
}


/**************************************************************************** */
