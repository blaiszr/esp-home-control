#ifndef DISPLAY_WEB_SERVER_H
#define DISPLAY_WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include "SensorControl.h"

extern AsyncWebServer server;

String htmlHomePage() {
  return R"rawliteral(
    <html>
    <head>
      <meta charset="UTF-8">
      <title>ESP32 Home Control Dashboard</title>
      <style>
        body {font-family: Arial; text-align: center; background: #111; color: #eee;}
        .card {display: inline-block; margin: 20px; padding: 20px; background: #222; border-radius: 10px;}
        h1 {color: #00bcd4;}
      </style>
    </head>
    <body>
      <h1>Environment Readings</h1>
      <div class="card">
        <h2>🌡 Temperature</h2>
        <p id="temp">-- °F</p>
      </div>
      <div class="card">
        <h2>💧 Humidity</h2>
        <p id="hum">-- %</p>
      </div>
      <div class="card">
        <h2>💡 Light Level</h2>
        <p id="light">--</p>
      </div>

      <script>
        async function updateData() {
          try {
            const response = await fetch('/data', { cache: 'no-store' });
            if (!response.ok) throw new Error('HTTP ' + response.status);

            const data = await response.json();

            document.getElementById('temp').innerText  = data.temperature  + ' °F';
            document.getElementById('hum').innerText   = data.humidity     + ' %';
            document.getElementById('light').innerText = data.light;

            document.getElementById('status').innerText = 'OK'; // optional status element
          } catch (err) {
            console.error('Update failed:', err);
            document.getElementById('status').innerText = 'Offline';
          }
        }

        setInterval(updateData, 2000);
        updateData();
      </script>
    </body>
    </html>
  )rawliteral";
}

void startWebServer() {
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request -> send(200, "text/html", htmlHomePage());});

  server.on("/data", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String json = "{\"temperature\":" + String(getTemperature(), 0) +
                  ",\"humidity\":" + String(getHumidity(), 0) +
                  ",\"light\":\"" + getLightLevel() + "\"}";
    request -> send(200, "application/json", json);
  });
}

#endif