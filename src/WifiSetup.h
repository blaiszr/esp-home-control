#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Preferences.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

Preferences preferences;
extern AsyncWebServer server;

String ssid = "";
String password = "";

String htmlConfigPage() {
  return R"rawliteral(
    <html>
      <body style='font-family:sans-serif; text-align:center;'>
        <h2>Wi-Fi Configuration</h2>
        <form action='/save' method='POST'>
          SSID: <input name='ssid' type='text'><br><br>
          Password: <input name='password' type='password'><br><br>
          <input type='submit' value='Save & Connect'>
        </form>
      </body>
    </html>
  )rawliteral";
}

void handleSave(AsyncWebServerRequest *request) {
    if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
    }

    if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
    }

    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    request->send(200, "text/html",
      "<h2>Network Details Saved! Rebooting Now...</h2>");

    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
    
    esp_timer_handle_t timer;

    const esp_timer_create_args_t reboot_args = {
        .callback = [](void*) { ESP.restart(); },
        .arg = nullptr,                            
        .name = "reboot_timer"                     
    };

    esp_timer_create(&reboot_args, &timer);
    esp_timer_start_once(timer, 3000000);
}

void startSetupPortal() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("HomeControl_Setup", "12345678");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Setup portal running at: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlConfigPage());
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
    handleSave(request);
  });

  server.begin();
}


bool connectToSaved() {
  preferences.begin("wifi", true);

  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
  preferences.end();

  if (ssid == "" || password == "") {
      startSetupPortal();
      return false;
  }

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long wifiStartTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - wifiStartTime < 15000) {
      delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
      server.begin();
      return true;
  } else {
      startSetupPortal();
      return false;
  }
}

IPAddress getSetupIP() {
  if (WiFi.getMode() == WIFI_MODE_AP) {
    return WiFi.softAPIP();
  } else {
    return IPAddress(0, 0, 0, 0);
  }
}

void resetSavedWifi() {
  preferences.begin("wifi", false);
  preferences.putString("ssid", "");
  preferences.putString("password", "");
  preferences.end();

  ESP.restart();
}

bool connectionStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  } else {
    return false;
  }
}

#endif