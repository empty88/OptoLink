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
"setTimeout(message, 5000);\n"
"};\n"
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
"	width: 100%;\n"
"	-webkit-transition-duration: 0.4s;\n"
"	transition-duration: 0.4s;\n"
"	cursor: pointer;\n"
"}\n"
"button:hover {\n"
"	background-color:#cf133c;\n"
"}\n"
"div, select, input {\n"
"	padding: 5px;\n"
"	font-size: 1em;\n"
"}\n"
"#onmessage, #offmessage {\n"
"	font-size: 1.4em;\n"
"	font-weight: bold;\n"
"}\n"
"select {\n"
"	width: 100%;\n"
"}\n"
"textarea {\n"
"	resize: none;\n"
"	width: 98%;\n"
"	height: 300px;\n"
"}\n"
".slider {\n"
"	width: 100%;\n"
"}\n"
"</style>\n"
"</head>\n";

const char HTTP_CONTAINER[] PROGMEM =
"<body onload=\"init()\">\n"
"<div style=\"text-align: left; display: inline-block;min-width: 440px;\">\n"
"<div style=\"text-align:center;\"><h2>Optolink</h2><h3 id=\"outsideTemp\"></h3></div>\n";

const char HTTP_MAIN[] PROGMEM =
"<div style=\"text-align:center;\"><span id=\"onmessage\" style=\"color: #99ff99;\">{onmessage}</span><span id=\"offmessage\" style=\"color: #ff9999;\">{offmessage}</span></div>\n"
"<br>\n";

const char HTTP_MENU[] PROGMEM =
"<form action=\"Logs\" method=\"get\"><button>Logs</button></form>\n<br>\n"
"<form action=\"restart\" method=\"get\"><button>Neustart</button></form>\n<br>\n";

const char HTTP_MENU_BACK[] PROGMEM =
"<form action=\"/\" method=\"get\"><button>Zurück</button></form>\n<br>\n";

const char HTTP_END[] PROGMEM =
"<br/>\n"
"</div>\n"
"</body>\n"
"</html>\n";

extern String onmessage;
extern String offmessage;

void HandleRoot();

void HandleLogs();

void HandleGetLogs();

void HandleRestart();

void HandleOutsideTemp();

void StartWebServer();

void HandleTest();


class WebServer
{
public:
	WebServer();
	~WebServer();
};
