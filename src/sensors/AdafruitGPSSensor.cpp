#include "AdafruitGPSSensor.h"

// Remove singleton pattern
// AdafruitGPSSensor& AdafruitGPSSensor::getInstance() {
//     static AdafruitGPSSensor instance;
//     return instance;
// }

#define GPSECHO false

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


// TODO: Need a recordData method to store the data or something so that it can be logged in sthe state manager
void AdafruitGPSSensor::outputData(Stream &output) {
   
    output.print("Fix: "); output.println(gps.fix);
    output.print("Fix quality: "); output.println(gps.fixquality);
    output.print("Satellites: "); output.println(gps.satellites);

    output.print("Time: ");
    output.print(gps.hour, DEC); output.print(':');
    output.print(gps.minute, DEC); output.print(':');
    output.print(gps.seconds, DEC); output.print('.');
    output.println(gps.milliseconds);

    output.print(gps.secondsSinceTime()); output.println(" seconds since last time");

    output.print("Location: ");
    output.print(gps.latitude, 4); output.print(gps.lat);
    output.print(", ");
    output.print(gps.longitude, 4); output.println(gps.lon);
}

void AdafruitGPSSensor::outputData(BluetoothManager &btManager) {
    String data = "Location: ";
    data += String(gps.latitude, 4) + String(gps.lat) + ", " + String(gps.longitude, 4) + String(gps.lon);
    btManager.sendStatusMessage(data.c_str());
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