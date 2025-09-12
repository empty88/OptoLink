#pragma once
#include "Arduino.h"

#include <ESP8266WebServer.h>

extern ESP8266WebServer WebServer;

const char HTTP_HEADER[] PROGMEM =
"<!DOCTYPE html>\n"
"<html lang=\"de\">\n"
"<head>\n"
"<meta charset='utf-8'>\n"
"<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>\n"
"<title>OptoLink</title>\n";

const char HTTP_MAIN_SCRIPT[] PROGMEM =
"<script>\n"
"function init() {\n"
"	clock();\n"
"	setTimeout(message, 5000);\n"
"	setTimeout(getValues, 2000);\n"
"};\n"
"function clock() {\n"
"	const r = new XMLHttpRequest();\n"
"	r.onreadystatechange=function(){\n"
"		if(r.readyState==4&&r.status==200){\n"
"			document.getElementById(\"time\").innerHTML=r.responseText;\n"
"		};\n"
"	};\n"
"	r.open(\"GET\", \"getTime\");r.send();\n"
"	setTimeout(clock ,1000);\n"
"};\n"
"function message() {\n"
"	document.getElementById(\"onmessage\").innerHTML=\"\";\n"
"	document.getElementById(\"offmessage\").innerHTML=\"\";\n"
"};\n"
"function getValues() {\n"
"	const r = new XMLHttpRequest();\n"
"	r.onreadystatechange=function(){\n"
"		if(r.readyState==4&&r.status==200){\n"
"			var arr = JSON.parse(r.responseText);\n"
"			for (var item in arr) {\n"
"				if(arr[item].value != '') {\n"
"					if(arr[item].value_str != '') {\n"
"						document.getElementById(arr[item].name).innerHTML=arr[item].value_str;\n"
"					} else {\n"
"						document.getElementById(arr[item].name).innerHTML=arr[item].value;\n"
"					};\n"
"				};\n"
"				if(arr[item].name == 'Betriebsart') {\n"
"					document.getElementById('om_off').setAttribute(\"class\", \"\");\n"
"					document.getElementById('om_ww').setAttribute(\"class\", \"\");\n"
"					document.getElementById('om_h-ww').setAttribute(\"class\", \"\");\n"
"					if(arr[item].value == '0') { \n"
"						document.getElementById('om_off').classList.add('menu-item-active');\n"
"						document.getElementById('om_ww').classList.add('menu-item');\n"
"						document.getElementById('om_h-ww').classList.add('menu-item');\n"
"					} else if(arr[item].value == '1') {\n"
"						document.getElementById('om_ww').classList.add('menu-item-active');\n"
"						document.getElementById('om_off').classList.add('menu-item');\n"
"						document.getElementById('om_h-ww').classList.add('menu-item');\n"
"					} else if(arr[item].value == '2') {\n"
"						document.getElementById('om_h-ww').classList.add('menu-item-active');\n"
"						document.getElementById('om_off').classList.add('menu-item');\n"
"						document.getElementById('om_ww').classList.add('menu-item');\n"
"					} else {\n"
"						document.getElementById('om_off').classList.add('menu-item');\n"
"						document.getElementById('om_ww').classList.add('menu-item');\n"
"						document.getElementById('om_h-ww').classList.add('menu-item');\n"				
"					};\n"
"				};"
"			};\n"
"		};\n"
"	};\n"
"	r.open(\"GET\", \"getValues\");r.send();\n"
"	setTimeout(getValues, 10000);\n"
"};\n";

const char HTTP_LOGS_SCRIPT[] PROGMEM =
"<script>\n"
"function init() {\n"
"	logs();\n"
"};\n"
"function logs() {\n"
"	const r = new XMLHttpRequest();\n"
"	r.onreadystatechange=function(){\n"
"		if(r.readyState==4&&r.status==200){\n"
"			l = document.getElementById(\"logs\");\n"
"			l.value=r.responseText;\n"
"			l.scrollTop=99999;\n"
"			l.scrollTop;\n"
"		};\n"
"	};\n"
"	r.open(\"GET\", \"getLogs\");r.send();\n"
"	setTimeout(logs ,2000);\n"
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
".container {\n"
"	text-align: left;\n"
"	display: inline-block;\n"
"	min-width: 350px;\n"
"}\n"
".heading {\n"
"	text-align:center;\n"
"}\n"
".heading h2,h3 {\n"
"	margin-top: 5px;\n"
"	margin-bottom: 10px;\n"
"}\n"
"form {\n"
"	margin-bottom: 5px;\n"
"	text-align: center;\n"
"	line-height: 28px;\n"
"}\n"
".form-table {\n"
"	display: table;\n"
"	width: -webkit-fill-available;\n"
"}\n"
".form-row {\n"
"	display: table-row;\n"
"}\n"
".form-label {\n"
"	display: table-cell;\n"
"	width: auto;\n"
"	text-align: left;\n"
"	height: 28px;\n"
"}\n"
".form-input {\n"
"	display: table-cell;\n"
"	width: auto;\n"
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
"#onmessage {\n"
"	color: #99ff99;\n"
"}\n"
"#offmessage {\n"
"	color: #ff9999;\n"
"}\n"
"button {\n"
"	border: 0px;\n"
"	border-radius: 0.3rem;\n"
"	background-color: #df335c;\n"
"	color: #fff;\n"
"	-webkit-transition-duration: 0.4s;\n"
"	transition-duration: 0.4s;\n"
"	cursor: pointer;\n"
"}\n"
"div .menu-item-active button {\n"
"	background-color: #6967d3;\n"
"	border: #df335c 3px solid;\n"
"}\n"
"@media (max-width: 800px) {\n"
"button {\n"
"	line-height: 2.4rem;\n"
"	font-size: 1.2rem;\n"
"	width: 98%;\n"
"}\n"
"button:hover {\n"
"	background-color:#cf133c;\n"
"}\n"
"div, input {\n"
"	font-size: 1em;\n"
"}\n"
"div {\n"
"	line-height: 28px;\n"
"}\n"

"textarea {\n"
"	resize: none;\n"
"	width: 98%;\n"
"	height: 300px;\n"
"}\n"
".livedata {\n"
"	display: table;\n"
"	font-size: 10pt;\n"
"	width: -webkit-fill-available;\n"
"	margin: 5px;\n"
"}\n"
".livedata .item {\n"
"	display: table-row;\n"
"	min-width: 98%;\n"
"	text-align: left;\n"
"}\n"
".livedata .name {\n"
"	display: table-cell;\n"
"	line-height: 15px;\n"
"	width: auto;\n"
"	border: #6967d3 solid;\n"
"	border-width: 1px 1px 0px 1px;\n"
"	padding: 1px;\n"
"}\n"
".livedata .value {\n"
"	display: table-cell;\n"
"	text-align: right;\n"
"	line-height: 15px;\n"
"	width: auto;\n"
"	border: #6967d3 solid;\n"
"	border-width: 1px 1px 0px 0px;\n"
"	padding: 1px;\n"
"}\n"
".livedata .item:last-child .name  {\n"
"	border: #6967d3 solid;\n"
"	border-width: 1px 1px 1px 1px;\n"
"}\n"

".livedata .item:last-child .value {\n"
"	border: #6967d3 solid;\n"
"	border-width: 1px 1px 1px 0px;\n"
"}\n"
"}\n"

"@media (min-width: 800px) {\n"
"button {\n"
"	line-height: 2.4rem;\n"
"	font-size: 1.2rem;\n"
"	min-width: 200px;\n"
"	margin: 5px;\n"
"}\n"
"div .menu {\n"
"	line-height: 50px;\n"
"	width: 700px;\n"
"	text-align: center;\n"
"	margin: auto;\n"
"}\n"
"div .menu-item, .menu-item-active {\n"
"	display: inline-block;\n"
"	text-align: center;\n"
"}\n"
"div {\n"
"	text-align: center;\n"
"}\n"
"textarea {\n"
"	resize: none;\n"
"	width: 750px;\n"
"	height: 600px;\n"
"}\n"
".livedata {\n"
"	font-size: 10pt;\n"
"	width: 700px;\n"
"	text-align: center;\n"
"}\n"
".livedata .item {\n"
"	display: inline-block;\n"
"	min-width: 200px;\n"
"	text-align: center;\n"
"	border: 0px;\n"
"	border-radius: 0.3rem;\n"
"	background-color: #6967d3;\n"
"	color: #fff;\n"
"	margin: 5px;\n"
"	line-height: 20px;\n"
"}\n"
"}\n"
"</style>\n"
"</head>\n";

const char HTTP_CONTAINER[] PROGMEM =
"<body onload=\"init()\">\n"
"<div class=\"container\">\n"
"<div class=\"heading\"><h2>Optolink</h2><h3 id=\"time\"></h3></div>\n";

const char HTTP_MAIN[] PROGMEM =
"<div class=\"messages\"><span id=\"onmessage\">{onmessage}</span><span id=\"offmessage\">{offmessage}</span></div>\n"
"<br>\n";

const char HTTP_MAIN_CONFIG_MODE[] PROGMEM =
"<div class=\"messages\"><span id=\"onmessage\">{onmessage}</span><span id=\"offmessage\">{offmessage}</span></div>\n"
"<br>\n"
"<form action=\"setConfigMode\" method=\"get\">\n"
"<div class=\"form-table\">\n"
"<div class=\"form-row\"><div class=\"form-label\">WLAN SSID: *</div><div class=\"form-input\"><input type=\"text\" id=\"ssid\" name=\"ssid\" required size=\"10\" value=\"{wlanssid}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">WLAN Passwort: *</div><div class=\"form-input\"><input type=\"password\" id=\"passwd\" name=\"passwd\" required size=\"10\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">Zeitserver:</div><div class=\"form-input\"><input type=\"text\" id=\"timesvr\" name=\"timesvr\" size=\"10\" value=\"{ntpserver}\"><span>wenn leer => Gateway</span></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Broker: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttsvr\" name=\"mqttsvr\" size=\"10\" value=\"{mqttserver}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Client Id: *</div><div class=\"form-input\"><input type=\"text\" id=\"mqttclientid\" name=\"mqttclientid\" size=\"10\" value=\"{mqttclientid}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Topic Prefix: *</div><div class=\"form-input\"><input type=\"text\" id=\"mqtttopicprefix\" name=\"mqtttopicprefix\" size=\"10\" value=\"{mqtttopicprefix}\"></div></div>\n"

"<div class=\"form-row\"><div class=\"form-label\">MQTT Username: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttusername\" name=\"mqttusername\" size=\"10\" value=\"{mqttusername}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Passwort: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttpassword\" name=\"mqttpassword\" size=\"10\" value=\"{mqttpassword}\"></div></div>\n"

"<div class=\"form-row\"><div class=\"form-label\" >* Pflichtfelder</div></div>\n"
"</div>\n"
"<br />\n"
"<button>Übernehmen</button>\n"
"</form>\n";

const char HTTP_CONFIG[] PROGMEM =
"<div class=\"messages\"><span id=\"onmessage\">{onmessage}</span><span id=\"offmessage\">{offmessage}</span></div>\n"
"<br>\n"
"<form action=\"setConfig\" method=\"get\">\n"
"<div class=\"form-table\">\n"
"<div class=\"form-row\"><div class=\"form-label\">WLAN SSID: *</div><div class=\"form-input\"><input type=\"text\" id=\"ssid\" name=\"ssid\" required size=\"10\" value=\"{wlanssid}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">WLAN Passwort: *</div><div class=\"form-input\"><input type=\"password\" id=\"passwd\" name=\"passwd\" required size=\"10\" ></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">Zeitserver:</div><div class=\"form-input\"><input type=\"text\" id=\"timesvr\" name=\"timesvr\" size=\"10\" value=\"{ntpserver}\"><br /><span>wenn leer => Gateway</span></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Broker: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttsvr\" name=\"mqttsvr\" size=\"10\" value=\"{mqttserver}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Client Id: *</div><div class=\"form-input\"><input type=\"text\" id=\"mqttclientid\" name=\"mqttclientid\" size=\"10\" value=\"{mqttclientid}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Topic Prefix: *</div><div class=\"form-input\"><input type=\"text\" id=\"mqtttopicprefix\" name=\"mqtttopicprefix\" size=\"10\" value=\"{mqtttopicprefix}\"></div></div>\n"

"<div class=\"form-row\"><div class=\"form-label\">MQTT Username: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttusername\" name=\"mqttusername\" size=\"10\" value=\"{mqttusername}\"></div></div>\n"
"<div class=\"form-row\"><div class=\"form-label\">MQTT Passwort: </div><div class=\"form-input\"><input type=\"text\" id=\"mqttpassword\" name=\"mqttpassword\" size=\"10\" value=\"{mqttpassword}\"></div></div>\n"

"<div class=\"form-row\"><div class=\"form-label\">* Pflichtfelder</div></div>"
"</div>\n"
"<br />\n"
"<button>Übernehmen</button>\n"
"</form>\n";

const char HTTP_MENU[] PROGMEM =
"<div class=\"menu\">\n"
"<div class=\"menu-item\"><form action=\"Logs\" method=\"get\"><button>Logs</button></form></div>\n"
"<div class=\"menu-item\"><form action=\"config\" method=\"get\"><button>Konfiguration</button></form></div>\n"
"<div class=\"menu-item\"><form action=\"restart\" method=\"get\"><button>Neustart</button></form></div>\n"
"</div>\n";

const char HTTP_OM_MENU[] PROGMEM =
"<div class=\"menu\">\n"
"<div id=\"om_ww\" class=\"menu-item{wwActive}\" ><form action=\"setWW\" method=\"get\"><button>Warmwasser</button></form></div>\n"
"<div id=\"om_h-ww\" class=\"menu-item{h-wwActive}\"><form action=\"setH-WW\" method=\"get\"><button>Heizen + WW</button></form></div>\n"
"<div id=\"om_off\" class=\"menu-item{offActive}\"><form action=\"setOff\" method=\"get\"><button>Abschaltbetrieb</button></form></div>\n"
"</div>\n";

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
void HandleGetValues();

void HandleRestart();

void HandleOutsideTemp();

void StartWebServer();
void StartWebServer(bool configMode);

void HandleTest();

void HandleSetWW();
void HandleSetH_WW();
void HandleSetOff();


class WebServer
{
public:
	WebServer();
	~WebServer();
};
