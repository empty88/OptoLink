#include "mqtt.h"
#include <viessmann.h>

WiFiClient mqttWifiClient;
PubSubClient mqttClient(mqttWifiClient);
String broker = "";
String topic_prefix = "";
String client_id = "";
String username = "";
String password = "";
char subTopicOperationMode[100] = "";

void setupMqtt(String server, String clientId, String prefix) {
    setupMqtt(server, clientId, prefix, "", "");
}

void setupMqtt(String server, String clientId, String prefix, String user, String pass) {
    broker = server;
    static char c[256];                 // important workaround!
    strcpy(c, server.c_str());          //
    mqttClient.setServer(c, 1883);      //
    topic_prefix = prefix;
    client_id = clientId;
    if (user != "" && pass != "") {
        username = user;
        password = pass;
    }
    mqttClient.setCallback(callback);
    strcpy(subTopicOperationMode, topic_prefix.c_str());
    strcat(subTopicOperationMode, "set_betriebsart");
}

void publishMqtt(String topic, const char* payload) {
    //Log("MQTT: publish " + topic_prefix + topic + "->" + String(payload));
    char fulltopic[100];
    strcpy(fulltopic, topic_prefix.c_str());
    strcat(fulltopic, topic.c_str());
    mqttClient.publish(fulltopic, payload);
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
        char text[100];
        sprintf(text,"MQTT (client id: %s): connecting to %s... ", client_id.c_str(), broker.c_str());
        Log(text);

        // Attempt to connect
        if (mqttClient.connect(client_id.c_str(), username != "" ? username.c_str(): NULL, password != "" ? password.c_str() : NULL)) {
            if (username != "" && password != "") {
                Log("MQTT credentials provided");
            }
            Log("MQTT: Connected");
            if (mqttClient.subscribe(subTopicOperationMode)) {
                char text[50];
                sprintf(text,"Send operationMode to %s", subTopicOperationMode);
                Log(text);
            } else {
                Log("subscription failed");
            }
            
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

void callback(char* topic, byte* payload, unsigned int length) {
    if( (strcmp(topic,subTopicOperationMode) == 0)) {
        if ((char)payload[0] == '0') {
            setOperationMode(0);
        } else if ((char)payload[0] == '1') {
            setOperationMode(1);
        } else if ((char) payload[0] == '2') {
            setOperationMode(2);
        }
    }
}