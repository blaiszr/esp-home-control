#ifndef INTEGRATE_MQTT_H
#define INTEGRATE_MQTT_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "SensorControl.h"

const char* HIVEMQ_HOST = "YOUR_CLUSTER_HERE.s1.eu.hivemq.cloud"; 
const int   MQTT_PORT = 8883;

const char* HIVEMQ_USER = "YOUR_USER_HERE";
const char* HIVEMQ_PASS = "YOUR_PASS_HERE";

const char* pub_temp   = "home/temperature";
const char* pub_hum    = "home/humidity";         
const char* pub_light  = "home/light";

WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

void reconnectMQTT() {

  //Loop until we successfully connect
  while (!mqttClient.connected()) {
    //Attempt connection with username/password authentication
    if (!mqttClient.connect("ESP32Client", HIVEMQ_USER, HIVEMQ_PASS)) {
      delay(5000);
    }
  }
}

void initMQTT() {
  //Select HiveMQ Cloud server + port
  mqttClient.setServer(HIVEMQ_HOST, MQTT_PORT);

  //sslClient.setCACert(HIVE_MQ_CA);
  //No CA with HiveMQ Free version (NOT RECOMMENDED, but for this project is ok)
  secureClient.setInsecure();

  secureClient.setTimeout(20000);

  mqttClient.connect("ESP32Client", HIVEMQ_USER, HIVEMQ_PASS);
}

void publishSensorData() {
  mqttClient.publish(pub_temp, String(getTemperature(), 0).c_str());
  mqttClient.publish(pub_hum, String(getHumidity(), 0).c_str());
  mqttClient.publish(pub_light, getLightLevel().c_str());
}

void handleMQTT() {
  //If connection dropped, reconnect
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  //Processes incoming messages and keeps alive with broker
  mqttClient.loop();
}

#endif