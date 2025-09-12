#include "filesystem.h"
#include <LittleFS.h>
#include "Logging.h"

String removeTrailingCR(String input);

String GLOBAL::WlanSSID = "";
String GLOBAL::WlanPasswd = "";
String GLOBAL::NtpServerIP = "";
String GLOBAL::MqttTopicPrefix = "";
String GLOBAL::MqttBrokerIP = "";
String GLOBAL::MqttClientId = "";
String GLOBAL::MqttUsername = "";
String GLOBAL::MqttPassword = "";

void readConfig() {
	LittleFS.begin();
	File configFile = LittleFS.open("_configFile", "r");
	if (configFile) {
		//Log("lese Config-Datei...\n" + configFile.readString());
	} else {
		Log("Config file does not exist");
		resetConfig();
		configFile = LittleFS.open("_configFile", "r");
	}

	configFile.seek(0);
	GLOBAL::WlanSSID = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::WlanPasswd = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::NtpServerIP = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::MqttTopicPrefix = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::MqttBrokerIP = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::MqttClientId = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::MqttUsername = removeTrailingCR(configFile.readStringUntil('\n'));
	GLOBAL::MqttPassword = removeTrailingCR(configFile.readStringUntil('\n'));
	configFile.close();
}

void saveConfig() {
	Log("Write config file...");
	File configFile = LittleFS.open("_configFile", "w");
	configFile.println(GLOBAL::WlanSSID); 		// WLAN SSID
	configFile.println(GLOBAL::WlanPasswd); 	// WLAN password
	configFile.println(GLOBAL::NtpServerIP); 	// NTP server
	configFile.println(GLOBAL::MqttTopicPrefix); // MQTT topic prefix
	configFile.println(GLOBAL::MqttBrokerIP); 	// MQTT broker
	configFile.println(GLOBAL::MqttClientId); 	// MQTT client id
	configFile.println(GLOBAL::MqttUsername);	// MQTT Username
	configFile.println(GLOBAL::MqttPassword);	// MQTT Password
	configFile.close();
	configFile = LittleFS.open("_configFile", "r");
	//Log(configFile.readString());
	configFile.close();
}

void resetConfig() {
	Log("Create default settings");
	File configFile = LittleFS.open("_configFile", "w");
	configFile.println(""); 					// WLAN SSID
	configFile.println(""); 					// WLAN password
	configFile.println(""); 					// NTP server
	configFile.println("optolink/"); 			// MQTT topic prefix
	configFile.println(""); 					// MQTT server
	configFile.println("optolink"); 			// MQTT client Id
	configFile.println("");						// MQTT Username
	configFile.println("");						// MQTT Password
	configFile.close();
}

String readLastError() {
	Log("Read last error");
	File configFile = LittleFS.open("_lastError", "r");
	configFile.seek(0);
	String result = configFile.readString();
	configFile.close();
	return result;
}

void saveLastError(String lastError) {
	Log("Write last error");
	File configFile = LittleFS.open("_lastError", "w");
	configFile.print(lastError);
	configFile.close();
}

String removeTrailingCR(String input)
{
	if (!input)
		return String();
	if (input.charAt(input.length() - 1) == 0x0d)
	{
		input.remove(input.length() - 1);
	}
	return input;
}