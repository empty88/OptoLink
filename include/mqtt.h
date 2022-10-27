#pragma once
#include <PubSubClient.h>
#include "Logging.h"
#include <ESP8266WiFi.h>

extern PubSubClient mqttClient;

const String topic_prefix = "optolink/";

void setupMqtt();
void publishMqtt(String topic, const char* payload);
void checkMqtt();