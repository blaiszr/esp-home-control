# esp-home-control
HomeControl ESP32 – Smart Wi-Fi Environmental Monitor (MQTT + Web Dashboard)

Full-stack IoT system built with an ESP32-S3, BME280 sensor, LDR light module, and SSD1306 OLED. The system is capable of monitoring temperature, humidity, and light while providing:

A local HTML dashboard

A cloud MQTT dashboard (HiveMQ)

A mobile Wi-Fi setup portal

A real-time OLED display

This project demonstrates embedded systems, web development, cloud IoT, and distributed communicatione.

Features
1. Smart Wi-Fi Setup Portal
  
    Device boots into Access Point mode if no saved credentials exist
  
    Users connect to HomeControl_Setup
  
    Web form stores SSID/password in Preferences
  
    LEDs to display Wi-Fi connection status
  
    Credentials persist through reboots
  
    Hardware reset button wipes saved Wi-Fi

2. Local Dashboard (AsyncWebServer)
  
    Accessible from within the home network
  
    Displays live temperature, humidity, and light values
  
    Auto-refreshing via JavaScript Fetch API
  
    UI Dark/Light modes
  
    Great for desktop or phone use

3. Cloud Dashboard via MQTT (HiveMQ)

    Secure MQTT over WebSockets
  
    ESP32 publishes JSON:
  
    Temperature
  
    Humidity
  
    Light Levels
  
    Browser dashboard subscribes remotely
  
    Includes reconnection and error handling

4. OLED Status Display
  
    Wi-Fi connection status icon
  
    Real-time sensor readings

5. Clean Modular Code Architecture

    WiFiSetup.h
  
    WiFiIcons.h
  
    SensorControl.h
  
    IntegrateMQTT.h
  
    DisplayWebServer.h
  
    DisplayOLED.h
  
    homeControl.ino
  
    Well-structured for scalability and readability.

📷 Screenshots

(TODO: ADD SCREENSHOTS)

Wi-Fi Setup Page

/screenshots/setup-page.png

Local Dashboard

/screenshots/local-dashboard.png

Remote MQTT Dashboard

/screenshots/mqtt-dashboard.png

OLED Status UI

/screenshots/oled-status.png

Hardware Used

ESP32-S3 Dev Board

BME280

I2C temp/humidity/pressure sensor

SSD1306 OLED

128×64 I2C display

LDR Light Sensor Module

Voltage divider with 10k resistor

RC low-pass filter for stability

LEDs (Green and Red)

Button

USB-C cable

Wiring Overview
I²C Bus (shared by BME280 + OLED)
ESP32-S3 → SDA → GPIO 9
ESP32-S3 → SCL → GPIO 8


Both devices share the same bus with independent addresses:

OLED → 0x3C

BME280 → 0x76

LDR Module
LDR + 10k divider → Analog Pin GPIO 4
0.1–1 µF capacitor → Analog pin → Ground

Wi-Fi Status LEDs
Green LED → 220Ω Resistor → GPIO 15
Green LED → GND
Red LED → 220Ω Resistor → GPIO 16
Red LED → GND

Reset Button
Button → GPIO 12 (Input Pullup)
Button → GND

MQTT Topic Structure

The ESP32 publishes data to:

home/temperature
home/humidity
home/light


Payload example:

{
  "temperature": 72,
  "humidity": 41,
  "light": "Bright"
}


The browser dashboard subscribes using:

client.subscribe("home/sensors");

Installation & Setup
1. Install Required Libraries

In Arduino IDE or PlatformIO:

Adafruit BME280

Adafruit Unified Sensor

Adafruit SSD1306

AsyncTCP

ESPAsyncWebServer

PubSubClient

2. Clone Repository
git clone https://github.com/blaiszr/esp-home-control.git
cd HomeControl-ESP32

3. Configure Wi-Fi on First Boot

Power the ESP

Connect to HomeControl_Setup
Default Password: 12345678

Open:

192.168.4.1

Enter home network SSID/password

Device reboots and connects automatically

Remote Cloud Dashboard Setup
Using HiveMQ Cloud (Free Tier)

Create an account at HiveMQ Cloud

Create a cluster

Get:

Endpoint URL  (Host)

Port: 8883 (TLS)

Username

Password

In IntegrateMQTT.h, replace username/password and host with yours

Use the HTML dashboard (dashboard.html) with:

const url = 'wss://YOUR-CLUSTER.s1.eu.hivemq.cloud:8884/mqtt'


The ESP32 will begin pushing real-time data.

📁 Project Structure
HomeControl-ESP32/
│
├── src/
│   ├── homeControl.ino
│   ├── WiFiSetup.h
│   ├── DisplayWebServer.h
│   ├── SensorControl.h
│   ├── IntegrateMQTT.h
│   ├── DisplayOLED.h
│   ├── WiFiIcons.h
│
├── dashboard/
│   ├── index.html
│   ├── mqtt.js
│
├── screenshots/
│
└── README.md

Troubleshooting
TODO: Add more troubleshooting

❗ No BME280 detected

Confirm I²C wiring

Ensure SDO → GND (address = 0x76)

License

MIT License — open-source and free to use.

Contributing

Feel free to submit issues, improve the dashboard UI, or extend automation capability!
