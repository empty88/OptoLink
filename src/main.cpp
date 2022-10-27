#include <Timer.h>

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include "mqtt.h"
#include "Logging.h"
#include "WebServer.h"
#include "viessmann.h"

const char* WIFI_SSID = "***";
const char* WIFI_PW = "***";
const int LED_PIN = LED_BUILTIN;
const int PORT = 81;

WiFiServer* server = NULL;
WiFiClient serverClient;

Timer t;

void wifiSerialLoop();
void setupArduinoOta();
void getValues();
void setupWiFi();

void setup() {
	Serial.begin(4800);
	pinMode(LED_PIN, OUTPUT);

  setupWiFi();

	setupArduinoOta();
  
  setupMqtt();
  t.every(10000,checkMqtt);

	IPAddress HTTPS_ServerIP = WiFi.localIP();
	Log("Server IP is: " + HTTPS_ServerIP.toString());

	StartWebServer();
  swSer.begin(4800, SWSERIAL_8E2, D2, D1);
  server = new WiFiServer(PORT);
  server->begin();
  setupVito();

  t.every(10000,getValues);
  getVitoData();
}

void loop() {
	WebServer.handleClient();
	ArduinoOTA.handle();
  mqttClient.loop();
  VitoWiFi.loop();
  t.update();
}

void getValues() {
  Log("getvalues()");
  getVitoData();
}

void setupWiFi() {
  	// -------- WLAN Connection
	Log("Connecting to WLAN");
	WiFi.mode(WIFI_STA);
	WiFi.hostname("OptoLink");
	WiFi.begin(WIFI_SSID, WIFI_PW);

	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(LED_PIN, HIGH);
		delay(200);
		digitalWrite(LED_PIN, LOW);
		delay(200);
		Serial.print(".");
	}
	Serial.println();
}

void setupArduinoOta() {
  ArduinoOTA.onStart([]() {

		});
	ArduinoOTA.onEnd([]() {
		//Serial.println("\nEnd");
		});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		//Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		});
	ArduinoOTA.onError([](ota_error_t error) {
		//Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			//Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			//Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			//Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR) {
			//Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			//Serial.println("End Failed");
		}
		});
	ArduinoOTA.setHostname("optolink");
	ArduinoOTA.begin();
}
