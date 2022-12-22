#pragma once
#include "Arduino.h"

void readConfig();
void resetConfig();
void saveConfig();

struct GLOBAL {
	static String WlanSSID;
	static String WlanPasswd;
	static String NtpServerIP;
	static String MqttTopicPrefix;
	static String MqttBrokerIP;
	static String MqttClientId;
};