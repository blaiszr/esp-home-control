#ifndef SENSOR_CONTROL_H
#define SENSOR_CONTROL_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define LDR 1

Adafruit_BME280 bme;

bool initBME() {
  if (!bme.begin(0x76)) {
    return false;
  }

  return true;
}

float getTemperature() {
  return (bme.readTemperature() * (9.0 / 5)) + 32;
}

float getHumidity() {
  return bme.readHumidity();
}

String getLightLevel() {
  float lightLevel = analogRead(LDR);

  
  if (lightLevel >= 2000) {
    return "Bright";
  } else if (lightLevel >= 1000) {
    return "Dim";
  } else {
    return "Dark";
  }
}

#endif