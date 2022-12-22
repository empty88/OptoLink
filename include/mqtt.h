#pragma once
#include <PubSubClient.h>
#include "Logging.h"
#include <ESP8266WiFi.h>

extern PubSubClient mqttClient;

void setupMqtt(String server, String clientid, String prefix);
void publishMqtt(String topic, const char* payload);
void checkMqtt();