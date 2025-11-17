#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include "WiFiSetup.h"
#include "DisplayOLED.h"
#include "DisplayWebServer.h"
#include "IntegrateMQTT.h"

#define GREEN_LIGHT 15
#define RED_LIGHT 16
#define RESET_BUTTON 12
#define DEBOUNCE_TIME 80

AsyncWebServer server(80);

bool wifiConnected = false;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

unsigned long lastSend = 0;

void handleButton();

void setup() {
  Serial.begin(115200);
  Wire.begin(9, 8);

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);

  if (!initDisplay()) {
    Serial.println("Could not find a valid OLED screen, check wiring!");
    while (1); //Stop execution
  }

  wifiConnected = connectToSaved();
  displayWiFi(wifiConnected);

  while (!wifiConnected) {
    digitalWrite(RED_LIGHT, HIGH);
    wifiConnected = connectionStatus();
    delay(5000);
  }

  initMQTT();
  startWebServer();
  
  if (!initBME()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1); //Stop execution
  }
}

void loop() {
  wifiConnected = connectionStatus();

  if (wifiConnected) {
    digitalWrite(GREEN_LIGHT, HIGH);
    digitalWrite(RED_LIGHT, LOW);
  } else {
    digitalWrite(GREEN_LIGHT, LOW);
    digitalWrite(RED_LIGHT, HIGH);
    return;
  }

  handleMQTT();
  handleButton();

  if (millis() - lastSend > 3000) {
    lastSend = millis();
    
    publishSensorData();
  }

  clearScreen();
  displayWiFi(wifiConnected);
  displayReadings();
}

void handleButton() {
  int buttonReading = digitalRead(RESET_BUTTON);

  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      if (buttonState == LOW) {
        resetSavedWifi();
      }
    }
  }

  lastButtonState = buttonReading;
}