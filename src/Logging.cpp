#include "Logging.h"
#include "Arduino.h"
#include <WiFiClient.h>
#include "ntpclient.h"
const int LOG_DATA_SIZE = 40;		//> 40 makes webinterface unstable after some time'
String LOG_DATA[LOG_DATA_SIZE];
int LOG_DATA_INDEX = 0;

void Log(String message) {
	Log(message, false);
}

void Log(String message, bool dummyTime) {
	Serial.println(message.c_str());
	
	int int_hour = hour();
	if (summertime_EU(year(), month(), day(), hour(), 1)) {
		if (int_hour == 24) {
			int_hour = 0;
		}
		else {
			int_hour = int_hour + 1;
		}
	}

	if (LOG_DATA_INDEX > LOG_DATA_SIZE - 1) {
		for (int i = 1; i < LOG_DATA_SIZE; i++) {
			LOG_DATA[i - 1] = LOG_DATA[i];
		}
		LOG_DATA_INDEX = LOG_DATA_SIZE - 1;
	}
	char logString[120];
	if (dummyTime) {
		sprintf(logString, "00.00. 00:00:00 - %s", message.c_str());
		LOG_DATA[LOG_DATA_INDEX] = logString;
	}
	else {
		sprintf(logString, "%02d.%02d. %02d:%02d:%02d - %s", day(), month(), int_hour, minute(), second(), message.c_str());
		LOG_DATA[LOG_DATA_INDEX] = logString;
	}
	LOG_DATA_INDEX++;
}

String GetLogs() {
	String response;
	for (int i = 0; i < LOG_DATA_INDEX; i++) {
		response += LOG_DATA[i] + "\n";
	}
	return response;
}