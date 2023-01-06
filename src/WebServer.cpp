#include "WebServer.h"
#include "Logging.h"
#include "viessmann.h"
#include "filesystem.h"
#include "ntpclient.h"
#include <ESP8266WebServer.h>

ESP8266WebServer WebServer(80);

String onmessage = "&nbsp;";
String offmessage = "&nbsp;";

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
		WebServer.on("/restart", HandleRestart);
	}
	WebServer.begin();
}

void ShowPage(String content, String script, String style) {
	String page = FPSTR(HTTP_HEADER);
	page += script;
	page += FPSTR(HTTP_SCRIPT_END);
	page += style;
	page += FPSTR(HTTP_CONTAINER);

	// replacements
	content.replace("{menu}", FPSTR(HTTP_MENU));
	content.replace("{onmessage}", onmessage);
	content.replace("{offmessage}", offmessage);
	content.replace("{color}", "#FF0000");
	onmessage = "&nbsp;";
	offmessage = "&nbsp;";
	page += content;
	page += FPSTR(HTTP_END);
	WebServer.send(200, "text/html", page);
}

void HandleRoot() {
	Serial1.println("HandleRoot");
	String content = FPSTR(HTTP_MAIN);
	content += "{menu}";

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
	if (!mqtttopicprefix.endsWith("/")) mqtttopicprefix += "/";
	if ((ssid == "") | (passwd == "") | (mqtttopicprefix == "") | (mqttsvr == "") | (mqttclientid == "")){
		offmessage = "Daten unvollständig";
		WebServer.sendHeader("Location", String("/"), true);
		WebServer.send(302, "text/plain", "");
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
	if (!mqtttopicprefix.endsWith("/")) mqtttopicprefix += "/";
	if ((mqtttopicprefix == "") | (mqttsvr == "") | (mqttclientid == "")){
		offmessage = "Daten unvollständig";
		WebServer.sendHeader("Location", String("/config"), true);
		WebServer.send(302, "text/plain", "");
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
	String content = "<textarea readonly id=\"logs\" cols=\"45\" wrap=\"off\"></textarea>";
	content += FPSTR(HTTP_MENU_BACK);

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
	content += FPSTR(HTTP_MENU_BACK);

	ShowPage(content, FPSTR(HTTP_MAIN_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleGetTime() {
	char timestr[19];
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

	WebServer.send(200, "text/html", timestr);
}

void HandleRestart() {
	Log("HandleRestart");

	onmessage = "Restart wird ausgeführt!";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	ESP.restart();
}

void HandleGetLogs() {
	WebServer.send(200, "text/html", GetLogs());
}

