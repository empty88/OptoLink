#include "mqtt.h"

WiFiClient mqttWifiClient;
PubSubClient mqttClient(mqttWifiClient);
String broker = "";
String topic_prefix = "";
String client_id = "";

void setupMqtt(String server, String clientId, String prefix) {
    broker = server;
    static char c[256];                 // important workaround!
    strcpy(c, server.c_str());          //
    mqttClient.setServer(c, 1883);      //
    topic_prefix = prefix;
    client_id = clientId;
}

void publishMqtt(String topic, const char* payload) {
    //Log("MQTT: publish " + topic_prefix + topic + "->" + String(payload));
    mqttClient.publish((topic_prefix + topic).c_str(), payload);
}

void checkMqtt() {
    if (!mqttClient.connected()) {
        switch (mqttClient.state()) {
            case MQTT_CONNECTION_TIMEOUT:
                Log("MQTT: connection timeout");
                break;
            case MQTT_CONNECTION_LOST:
                Log("MQTT: connection lost");
                break;
        }
        Log("MQTT (client id: " + client_id + "): connecting to " + broker + "... ");

        // Attempt to connect
        if (mqttClient.connect(client_id.c_str())) {
            Log("MQTT: Connected");
        
        } else {
            switch (mqttClient.state()) {
                case MQTT_CONNECT_FAILED:
                    Log("MQTT: connect failed");
                    break;
                case MQTT_CONNECT_BAD_PROTOCOL:
                    Log("MQTT: bad protocol");
                    break;
                case MQTT_CONNECT_BAD_CLIENT_ID:
                    Log("MQTT: bad client id");
                    break;
                case MQTT_CONNECT_UNAVAILABLE:
                    Log("MQTT: unavailable");
                    break;
                case MQTT_CONNECT_BAD_CREDENTIALS:
                    Log("MQTT: bad credentials");
                    break;
                case MQTT_CONNECT_UNAUTHORIZED:
                    Log("MQTT: unauthorized");
                    break;
                default:
                    Log(String(mqttClient.state()));
            }
        }
    }
}