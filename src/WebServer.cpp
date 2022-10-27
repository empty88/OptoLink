#include "WebServer.h"
#include "Logging.h"
#include "viessmann.h"
#include <ESP8266WebServer.h>

ESP8266WebServer WebServer(80);

String onmessage = "&nbsp;";
String offmessage = "&nbsp;";


void StartWebServer() {
	Serial.println("Start WebServer");
	WebServer.on("/", HandleRoot);
	WebServer.on("/Logs", HandleLogs);
	WebServer.on("/getLogs", HandleGetLogs);
	WebServer.on("/restart", HandleRestart);

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

void HandleLogs() {
	Serial1.println("HandleGetLogs");
	String content = "<textarea readonly id=\"logs\" cols=\"45\" wrap=\"off\"></textarea>";
	content += FPSTR(HTTP_MENU_BACK);

	ShowPage(content, FPSTR(HTTP_LOGS_SCRIPT), FPSTR(HTTP_STYLE));
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

