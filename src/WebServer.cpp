#include "WebServer.h"
#include "Logging.h"
#include "viessmann.h"
#include "filesystem.h"
#include "ntpclient.h"
#include <ESP8266WebServer.h>

ESP8266WebServer WebServer(80);

String onmessage = "";
String offmessage = "";

void StartWebServer() {
	StartWebServer(false);
}

void StartWebServer(bool configMode) {
	Serial.println("Start WebServer");

	if (configMode) {
		WebServer.on("/", HandleRootConfigMode);
		WebServer.on("/setConfigMode", HandleSetConfigMode);
	} else {
		WebServer.on("/", HandleRoot);
		WebServer.on("/config", HandleConfig);
		WebServer.on("/setConfig", HandleSetConfig);
		WebServer.on("/Logs", HandleLogs);
		WebServer.on("/getLogs", HandleGetLogs);
		WebServer.on("/getTime", HandleGetTime);
		WebServer.on("/getValues", HandleGetValues);
		WebServer.on("/restart", HandleRestart);

		WebServer.on("/setWW", HandleSetWW);
		WebServer.on("/setH-WW", HandleSetH_WW);
		WebServer.on("/setOff", HandleSetOff);
	}
	WebServer.begin();
}

void ShowPage(String content, String script, String style) {
	WebServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    WebServer.send(200, F("text/html"), "");
	WebServer.sendContent_P(HTTP_HEADER);
	WebServer.sendContent(script);
	WebServer.sendContent_P(HTTP_SCRIPT_END);
	WebServer.sendContent(style);
	WebServer.sendContent_P(HTTP_CONTAINER);

	String om_menu = FPSTR(HTTP_OM_MENU);
	if(operationMode == 0) {
		om_menu.replace("{offActive}", "-active");
	} else if (operationMode == 1) {
		om_menu.replace("{wwActive}", "-active");
	} else if(operationMode == 2) {
		om_menu.replace("{h-wwActive}", "-active");
	}
	om_menu.replace("{offActive}", "");
	om_menu.replace("{wwActive}", "");
	om_menu.replace("{h-wwActive}", "");

	// replacements
	content.replace("{om_menu}", om_menu);
	content.replace("{menu}", FPSTR(HTTP_MENU));
	content.replace("{onmessage}", onmessage);
	content.replace("{offmessage}", offmessage);
	content.replace("{color}", "#FF0000");
	onmessage = "";
	offmessage = "";
	
	WebServer.sendContent(content);
	WebServer.sendContent_P(HTTP_END);
}

void HandleRoot() {
	Serial.println("HandleRoot");
	String content = FPSTR(HTTP_MAIN);
	content.concat("{om_menu}");
	content.concat("{menu}");
	
	//automatically generate livedata from livedata vector
	content.concat(F("<div class=\"livedata\">"));
	for (auto it = liveData.begin(); it != liveData.end(); ++it) {
		if (String((*it).value) == "") continue;
		String dpName = String((*it).dp->getName());
		content.concat(F("<div class=\"item\"><div class=\"name\">"));
		content.concat(dpName);
		content.concat(F(": </div><div id=\""));
		content.concat(dpName);
		content.concat(F("\" class=\"value\">"));
		if (String((*it).value_str) == "") {
			content.concat(String((*it).value));
		} else {
			content.concat(String((*it).value_str));
		}
		content.concat(F("</div></div>"));
    }
	content.concat(F("</div>"));

	ShowPage(content, FPSTR(HTTP_MAIN_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleRootConfigMode() {
	Serial.println("HandleRootConfigMode");
	String content = FPSTR(HTTP_MAIN_CONFIG_MODE);
	content.replace("{wlanssid}", GLOBAL::WlanSSID);
	content.replace("{wlanpassword}", GLOBAL::WlanPasswd);
	content.replace("{ntpserver}", GLOBAL::NtpServerIP);
	content.replace("{mqttserver}", GLOBAL::MqttBrokerIP);
	content.replace("{mqtttopicprefix}", GLOBAL::MqttTopicPrefix);
	content.replace("{mqttclientid}", GLOBAL::MqttClientId);

	ShowPage(content, "", FPSTR(HTTP_STYLE));
}

void HandleSetConfigMode() {
	String ssid = WebServer.arg("ssid");
	String passwd = WebServer.arg("passwd");
	String timesvr = WebServer.arg("timesvr");
	String mqtttopicprefix = WebServer.arg("mqtttopicprefix");
	String mqttsvr = WebServer.arg("mqttsvr");
	String mqttclientid = WebServer.arg("mqttclientid");
	if (!mqtttopicprefix.endsWith("/")) mqtttopicprefix.concat("/");
	if ((ssid == "") | (passwd == "") | (mqtttopicprefix == "") | (mqttclientid == "")){
		offmessage = "Data incomplete";
		WebServer.sendHeader(F("Location"), "/", true);
		WebServer.send(302, F("text/plain"), "");
		return;
	}
	GLOBAL::WlanSSID = ssid;
	GLOBAL::WlanPasswd = passwd;
	GLOBAL::NtpServerIP = timesvr;
	GLOBAL::MqttTopicPrefix = mqtttopicprefix;
	GLOBAL::MqttBrokerIP = mqttsvr;
	GLOBAL::MqttClientId = mqttclientid;
	saveConfig();
	ESP.restart();
}

void HandleSetConfig() {
	String timesvr = WebServer.arg("timesvr");
	String mqtttopicprefix = WebServer.arg("mqtttopicprefix");
	String mqttsvr = WebServer.arg("mqttsvr");
	String mqttclientid = WebServer.arg("mqttclientid");
	if (!mqtttopicprefix.endsWith("/")) mqtttopicprefix.concat("/");
	if ((mqtttopicprefix == "") | (mqttclientid == "")){
		offmessage = "Data incomplete";
		WebServer.sendHeader(F("Location"), String("/config"), true);
		WebServer.send(302, F("text/plain"), "");
		return;
	}
	GLOBAL::NtpServerIP = timesvr;
	GLOBAL::MqttTopicPrefix = mqtttopicprefix;
	GLOBAL::MqttBrokerIP = mqttsvr;
	GLOBAL::MqttClientId = mqttclientid;
	saveConfig();
	ESP.restart();
}

void HandleLogs() {
	Serial.println("HandleLogs");
	String content = F("<textarea readonly id=\"logs\" cols=\"45\" wrap=\"off\"></textarea>");
	content.concat(FPSTR(HTTP_MENU_BACK));

	ShowPage(content, FPSTR(HTTP_LOGS_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleConfig() {
	Serial.println("HandleConfig");
	String content = FPSTR(HTTP_CONFIG);
	content.replace("{wlanssid}", GLOBAL::WlanSSID);
	content.replace("{wlanpassword}", GLOBAL::WlanPasswd);
	content.replace("{ntpserver}", GLOBAL::NtpServerIP);
	content.replace("{mqttserver}", GLOBAL::MqttBrokerIP);
	content.replace("{mqtttopicprefix}", GLOBAL::MqttTopicPrefix);
	content.replace("{mqttclientid}", GLOBAL::MqttClientId);
	content.concat(FPSTR(HTTP_MENU_BACK));

	ShowPage(content, FPSTR(HTTP_MAIN_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleGetTime() {
	char timestr[21];
	int int_hour = hour();
	if (summertime_EU(year(), month(), day(), hour(), 1)) {
		if (int_hour == 24) {
			int_hour = 0;
		}
		else {
			int_hour = int_hour + 1;
		}
	}
	sprintf(timestr, "%02d.%02d.%02d - %02d:%02d:%02d", day(), month(), year(), int_hour, minute(), second());

	WebServer.send(200, F("text/html"), timestr);
}

void HandleRestart() {
	Log("HandleRestart");

	onmessage = F("Optolink will be restarted!");
	WebServer.sendHeader(F("Location"), "/", true);
	WebServer.send(303, F("text/plain"), "");
	ESP.restart();
}

void HandleGetLogs() {
	WebServer.send(200, F("text/html"), GetLogs());
}

void HandleGetValues() {
	String json = "[";

	for (auto it = liveData.begin(); it != liveData.end(); ++it) {
		if (it != liveData.begin()) json.concat(",");
		json.concat(F("{\"name\":\""));
		json.concat(String((*it).dp->getName()));
		json.concat(F("\",\"value\":\""));
		json.concat(String((*it).value));
		json.concat(F("\",\"value_str\":\""));
		json.concat(String((*it).value_str));
		json.concat(F("\"}"));
		
	}
	json.concat("]");
	
	WebServer.send(200, F("application/json"), json);
}

void HandleSetOff() {
	setOperationMode(0);
	readOperationMode();
	WebServer.sendHeader(F("Location"), "/", true);
	WebServer.send(303, F("text/html"), "");
}

void HandleSetWW() {
	setOperationMode(1);
	readOperationMode();
	WebServer.sendHeader(F("Location"), "/", true);
	WebServer.send(303, F("text/html"), "");
}

void HandleSetH_WW() {
	setOperationMode(2);
	readOperationMode();
	WebServer.sendHeader(F("Location"), "/", true);
	WebServer.send(303, F("text/html"), "");
}


