#include <Arduino.h>
#include <TFT_eSPI.h>
#include "sensors/BNO055Sensor.h"

TFT_eSPI tft = TFT_eSPI();
BNO055Sensor bnoSensor;

void setup() {
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Initializing", 10, 10);

  Serial.begin(115200);
  bnoSensor.begin();
}

void loop() {
  sensors_event_t orientationEvent;
  bnoSensor.getOrientation(orientationEvent);

  sensors_event_t accelerationEvent;
  bnoSensor.getAcceleration(accelerationEvent);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.print("Orientation");
  tft.setCursor(10, 30);
  tft.print("X: "); tft.print(orientationEvent.orientation.x, 4);
  tft.setCursor(10, 50);
  tft.print("Y: "); tft.print(orientationEvent.orientation.y, 4);
  tft.setCursor(10, 70);
  tft.print("Z: "); tft.print(orientationEvent.orientation.z, 4);

  tft.setCursor(10, 90);
  tft.print("Acceleration");
  tft.setCursor(10, 110);
  tft.print("X: "); tft.print(accelerationEvent.acceleration.x, 4);
  tft.setCursor(10, 130);
  tft.print("Y: "); tft.print(accelerationEvent.acceleration.y, 4);
  tft.setCursor(10, 150);
  tft.print("Z: "); tft.print(accelerationEvent.acceleration.z, 4);

  Serial.print("X: "); Serial.print(orientationEvent.orientation.x, 4);
  Serial.print("\tY: "); Serial.print(orientationEvent.orientation.y, 4);
  Serial.print("\tZ: "); Serial.println(orientationEvent.orientation.z, 4);

  Serial.print("Accel X: "); Serial.print(accelerationEvent.acceleration.x, 4);
  Serial.print("\tY: "); Serial.print(accelerationEvent.acceleration.y, 4);
  Serial.print("\tZ: "); Serial.println(accelerationEvent.acceleration.z, 4);

  // Check for maximum measurable acceleration
  if (abs(accelerationEvent.acceleration.x) >= 39.24 || 
      abs(accelerationEvent.acceleration.y) >= 39.24 || 
      abs(accelerationEvent.acceleration.z) >= 39.24) {
    Serial.println("Max measurable acceleration reached!");
  }

  delay(500);
}

