#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFiIcons.h"
#include "SensorControl.h"

//Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    return false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  return true;
}

bool displayWiFi(bool wifiConnected) {
  //Wi-Fi icon (top right)
  display.drawBitmap(100, 0, wifi_icon_bmp, 8, 8, SSD1306_WHITE);

  //Connection indicator next to it
  if (wifiConnected) { 
    display.drawBitmap(110, 0, checkmark_bmp, 8, 8, SSD1306_WHITE);
    return true;
  } else {
    display.drawBitmap(110, 0, xmark_bmp, 8, 8, SSD1306_WHITE);

    IPAddress ip = getSetupIP();

    display.setCursor(2, 15);
    display.println("Setup WiFi. Visit");
    display.setCursor(2, 25);
    display.println(ip);
    display.setCursor(2, 35);
    display.println("After connecting to");
    display.setCursor(2, 45);
    display.println("ESP network");
    display.display();

    return false;
  }
}

void displayReadings() {
  display.setCursor(10, 20);
  display.println("Temperature:");
  display.setCursor(10, 30);
  display.println("Humidity:");
  display.setCursor(10, 40);
  display.println("Light Level:");

  display.setCursor(90, 20);
  display.println(getTemperature(), 0);
  display.setCursor(70, 30);
  display.println(getHumidity(), 0);
  display.setCursor(82, 30);
  display.println("%");
  display.setCursor(90, 40);
  display.println(getLightLevel());

  display.display();
}

void clearScreen() {
  display.clearDisplay();
}

#endif