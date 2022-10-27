#include "mqtt.h"

WiFiClient mqttWifiClient;
PubSubClient mqttClient(mqttWifiClient);


void setupMqtt() {
    mqttClient.setServer("192.168.178.2", 1883);
}

void publishMqtt(String topic, const char* payload) {
    //Log("publish " + topic_prefix + topic + "->" + String(payload));
    mqttClient.publish((topic_prefix + topic).c_str(), payload);
}

void checkMqtt() {
    if (!mqttClient.connected()) {
        Log("Attempting MQTT connection... ");

        String clientId = "optolink";

        // Attempt to connect
        if (mqttClient.connect(clientId.c_str())) {
            Log("connected");
        
        } else {
            Log(String(mqttClient.state()));
        }
    }
}