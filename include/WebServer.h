#pragma once
#include "Arduino.h"

#include <ESP8266WebServer.h>

extern ESP8266WebServer WebServer;

const char HTTP_HEADER[] PROGMEM =
"<!DOCTYPE html><html lang=\"de\" class=\"\">\n"
"<head>\n"
"<meta charset='utf-8'>\n"
"<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>\n"
"<title>OptoLink</title>\n";

const char HTTP_MAIN_SCRIPT[] PROGMEM =
"<script>\n"
"var r;\n"
"function init() {\n"
"clock();\n"
"setTimeout(message, 5000);\n"
"};\n"
"function clock() {\n"
"r = new XMLHttpRequest();\n"
"r.onreadystatechange=function(){\n"
"if(r.readyState==4&&r.status==200){\n"
"document.getElementById(\"time\").innerHTML=r.responseText;\n"
"};\n"
"};\n"
"r.open(\"GET\", \"getTime\");r.send();\n"
"setTimeout(clock ,1000);\n"
"}\n"
"function message() {\n"
"document.getElementById(\"onmessage\").innerHTML=\"&nbsp;\";\n"
"document.getElementById(\"offmessage\").innerHTML=\"&nbsp;\";\n"
"};\n";

const char HTTP_LOGS_SCRIPT[] PROGMEM =
"<script>\n"
"var r;\n"
"function init() {\n"
"	logs();\n"
"};\n"
"function logs() {\n"
"	r = new XMLHttpRequest();\n"
"	r.onreadystatechange=function(){\n"
"	if(r.readyState==4&&r.status==200){\n"
"		l = document.getElementById(\"logs\");\n"
"		l.value=r.responseText;\n"
"		l.scrollTop=99999;\n"
"		l.scrollTop;\n"
"	};\n"
"};\n"
"r.open(\"GET\", \"getLogs\");r.send();\n"
"setTimeout(logs ,2000);\n"
"};\n";



const char HTTP_HEADER_RELOAD[] PROGMEM =
"setTimeout(function(){location.href='.';},4000);\n";

const char HTTP_SCRIPT_END[] PROGMEM =
"</script>\n";

const char HTTP_STYLE[] PROGMEM =
"<style>\n"
"body {\n"
"	font-family: verdana;\n"
"	text-align: center;\n"
"}\n"
"button {\n"
"	border: 0px;\n"
"	border-radius: 0.3rem;\n"
"	background-color: #df335c;\n"
"	color: #fff;\n"
"	line-height: 2.4rem;\n"
"	font-size: 1.2rem;\n"
"	width: 98%;\n"
"	-webkit-transition-duration: 0.4s;\n"
"	transition-duration: 0.4s;\n"
"	cursor: pointer;\n"
"}\n"
"button:hover {\n"
"	background-color:#cf133c;\n"
"}\n"
"div, input {\n"
"	padding: 3px;\n"
"	font-size: 1em;\n"
"}\n"
"div {\n"
"	display: flex;\n"
"	flex-direction: column;\n"
"	line-height: 28px;\n"
"}\n"
".form_label {\n"
"	float: left;\n"
"	width: 170px;\n"
"	text-align: left;\n"
"	height: 28px;\n"
"}\n"
"form {\n"
"	margin-bottom: 5px;\n"
"	text-align: center;\n"
"}\n"
".messages {\n"
"	text-align: center;\n"
"	display: inline-block;\n"
"}\n"
"span {\n"
"	font-size: 0.7em;\n"
"	margin-bottom: 5px;\n"
"}\n"
"#onmessage, #offmessage {\n"
"	font-size: 1.4em;\n"
"	font-weight: bold;\n"
"}\n"
"textarea {\n"
"	resize: none;\n"
"	width: 98%;\n"
"	height: 300px;\n"
"}\n"
"</style>\n"
"</head>\n";

const char HTTP_CONTAINER[] PROGMEM =
"<body onload=\"init()\">\n"
"<div style=\"text-align: left; display: inline-block;min-width: 350px;\">\n"
"<div style=\"text-align:center;\"><h2>Optolink</h2><h3 id=\"time\"></h3></div>\n";

const char HTTP_MAIN[] PROGMEM =
"<div class=\"messages\"><span id=\"onmessage\" style=\"color: #99ff99;\">{onmessage}</span><span id=\"offmessage\" style=\"color: #ff9999;\">{offmessage}</span></div>\n"
"<br>\n";

const char HTTP_MAIN_CONFIG_MODE[] PROGMEM =
"<div class=\"messages\"><span id=\"onmessage\" style=\"color: #99ff99;\">{onmessage}</span><span id=\"offmessage\" style=\"color: #ff9999;\">{offmessage}</span></div>\n"
"<br>\n"
"<form action=\"setConfigMode\" method=\"get\">\n"
"<div class=\"form_label\">WLAN SSID: *</div><div><input type=\"text\" id=\"ssid\" name=\"ssid\" required size=\"10\" value=\"{wlanssid}\"></div>\n"
"<div class=\"form_label\">WLAN Passwort: *</div><div><input type=\"password\" id=\"passwd\" name=\"passwd\" required size=\"10\"></div>\n"
"<div class=\"form_label\">Zeitserver:</div><div><input type=\"text\" id=\"timesvr\" name=\"timesvr\" size=\"10\" value=\"{ntpserver}\"><span>wenn leer => Gateway</span></div>\n"
"<div class=\"form_label\">MQTT Broker: *</div><div><input type=\"text\" id=\"mqttsvr\" name=\"mqttsvr\" size=\"10\" value=\"{mqttserver}\"></div>\n"
"<div class=\"form_label\">MQTT Client Id: *</div><div><input type=\"text\" id=\"mqttclientid\" name=\"mqttclientid\" size=\"10\" value=\"{mqttclientid}\"></div>\n"
"<div class=\"form_label\">MQTT Topic Prefix: *</div><div><input type=\"text\" id=\"mqtttopicprefix\" name=\"mqtttopicprefix\" size=\"10\" value=\"{mqtttopicprefix}\"></div>\n"
"<div class=\"form_label\" >* Pflichtfelder</div>"
"<br />\n"
"<button>Übernehmen</button>\n"
"</form>\n";

const char HTTP_CONFIG[] PROGMEM =
"<form action=\"setConfig\" method=\"get\">\n"
"<div class=\"form_label\">WLAN SSID: *</div><div><input type=\"text\" id=\"ssid\" name=\"ssid\" required size=\"10\" value=\"{wlanssid}\"></div>\n"
"<div class=\"form_label\">WLAN Passwort: *</div><div><input type=\"password\" id=\"passwd\" name=\"passwd\" required size=\"10\" ></div>\n"
"<div class=\"form_label\">Zeitserver:</div><div><input type=\"text\" id=\"timesvr\" name=\"timesvr\" size=\"10\" value=\"{ntpserver}\"><span>wenn leer => Gateway</span></div>\n"
"<div class=\"form_label\">MQTT Broker: *</div><div><input type=\"text\" id=\"mqttsvr\" name=\"mqttsvr\" size=\"10\" value=\"{mqttserver}\"></div>\n"
"<div class=\"form_label\">MQTT Client Id: *</div><div><input type=\"text\" id=\"mqttclientid\" name=\"mqttclientid\" size=\"10\" value=\"{mqttclientid}\"></div>\n"
"<div class=\"form_label\">MQTT Topic Prefix: *</div><div><input type=\"text\" id=\"mqtttopicprefix\" name=\"mqtttopicprefix\" size=\"10\" value=\"{mqtttopicprefix}\"></div>\n"
"<div class=\"form_label\">* Pflichtfelder</div>"
"<br />\n"
"<button>Übernehmen</button>\n"
"</form>\n";

const char HTTP_MENU[] PROGMEM =
"<form action=\"Logs\" method=\"get\"><button>Logs</button></form>\n"
"<form action=\"config\" method=\"get\"><button>Konfiguration</button></form>\n"
"<form action=\"restart\" method=\"get\"><button>Neustart</button></form>\n";

const char HTTP_MENU_BACK[] PROGMEM =
"<form action=\"/\" method=\"get\"><button>Zurück</button></form>\n";

const char HTTP_END[] PROGMEM =
"<br/>\n"
"</div>\n"
"</body>\n"
"</html>\n";

extern String onmessage;
extern String offmessage;

void HandleRoot();
void HandleRootConfigMode();
void HandleConfigMode();
void HandleSetConfigMode();

void HandleLogs();
void HandleConfig();
void HandleSetConfig();

void HandleGetLogs();
void HandleGetTime();

void HandleRestart();

void HandleOutsideTemp();

void StartWebServer();
void StartWebServer(bool configMode);

void HandleTest();


class WebServer
{
public:
	WebServer();
	~WebServer();
};
