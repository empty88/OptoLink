#include <Timer.h>

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include "mqtt.h"
#include "logging.h"
#include "webServer.h"
#include "viessmann.h"
#include "filesystem.h"
#include "ntpclient.h"

Timer t;

const int LED_PIN = LED_BUILTIN;

const IPAddress _STA_ip = IPAddress(192,168,0,1);
const IPAddress _STA_gw = IPAddress(192,168,0,1);
const IPAddress _STA_sn = IPAddress(255,255,255,0);
const char _STA_ssid[] = "optolink_config";

bool configMode = false;

void wifiSerialLoop();
void setupArduinoOta();
void getValues();
void setupWiFi();

void setup() {
	Serial.begin(74880);
	pinMode(LED_PIN, OUTPUT);

	readConfig();

	if ((GLOBAL::WlanSSID == "") | (GLOBAL::WlanPasswd == "")){ // start access point and web server in config mode
		Log("Start WLAN AP");
		WiFi.mode(WIFI_AP);
		WiFi.hostname("optolink_config");
  		WiFi.softAPConfig(_STA_ip, _STA_gw, _STA_sn);
  		WiFi.softAP(_STA_ssid);
		StartWebServer(true);
		configMode = true;
	} else {
  		setupWiFi();

		setupArduinoOta();

		// get time per hour(), minute(), second(), now() functions
 		if (GLOBAL::NtpServerIP == "") {
			setTimeServer(WiFi.gatewayIP());
		} else {
			IPAddress ip;
			ip.fromString(GLOBAL::NtpServerIP);
			setTimeServer(ip);
		}

		updateTime();
		t.every(43200000, updateTime); 	// ntp update every 12 hours
	 
	 	if ((GLOBAL::MqttBrokerIP != "") && (GLOBAL::MqttClientId != "") && (GLOBAL::MqttTopicPrefix != "")) {
			setupMqtt(GLOBAL::MqttBrokerIP, GLOBAL::MqttClientId, GLOBAL::MqttTopicPrefix);
			t.every(10000,checkMqtt);		// check if mqtt is still connected, reconnect if needed
			Log("MQTT started");
		} else {
			Log("MQTT configuration broken, MQTT is disabled");
		}

		StartWebServer();
		swSer.begin(4800, SWSERIAL_8E2, D2, D1);

		setupVito();

		t.every(10000, getValues);		// request datapoints every 10 secs
		getVitoData();

		IPAddress HTTPS_ServerIP = WiFi.localIP();
		char text[50];
		sprintf(text,"Server IP is: %s", HTTPS_ServerIP.toString().c_str() );
		Log(text);
	}
}

void loop() {
	WebServer.handleClient();
	if (!configMode) {
		ArduinoOTA.handle();
		mqttClient.loop();
		if (missingValuesCount < 2) VitoWiFi.loop();
	}
  	t.update();
	delay(10);
}

void getValues() {
  	//Log("getvalues()");
	missingValuesCount++;
	if (missingValuesCount > 1) {
		digitalWrite(LED_PIN, LOW);		// LED on while skipping data requests
		if (missingValuesCount == 7) {	// retry after 6 loops (10 secs per loop = 60 secs)
 			missingValuesCount = 0;
		} else if(missingValuesCount == 2) {
			publishMqtt("error", "1");	// send error status over mqtt once
		} else {
			Log("Skipping...");
			return;
		}
	}
	digitalWrite(LED_PIN, LOW);			// short LED signal on data request
	delay(200);
	digitalWrite(LED_PIN, HIGH);
  	getVitoData();
}

void setupWiFi() {
	Log("Connecting to WLAN");
	WiFi.mode(WIFI_STA);
	wifi_set_sleep_type(NONE_SLEEP_T);
	WiFi.setAutoReconnect(true);
	WiFi.hostname("OptoLink");
	WiFi.begin(GLOBAL::WlanSSID, GLOBAL::WlanPasswd);

	int attempts = 0;
	while (WiFi.status() != WL_CONNECTED && attempts < 200) {
		digitalWrite(LED_PIN, LOW);
		delay(200);
		digitalWrite(LED_PIN, HIGH);
		delay(200);
		Serial.print(".");
		attempts++;
	}
	Serial.println();
	if (WiFi.status() != WL_CONNECTED) {
		GLOBAL::WlanPasswd = "";
		GLOBAL::WlanSSID = "";
		saveConfig();
		ESP.restart();
	}
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
