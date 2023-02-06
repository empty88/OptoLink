#include "viessmann.h"
#include <VitoWiFi.h>
#include "mqtt.h"
#include "Arduino.h"
#include "filesystem.h"

VitoWiFi_setProtocol(P300);
SoftwareSerial swSer;

byte missingValuesCount = 0;
std::vector<LiveData> liveData;

//second parameter "group" is misused for the unit
DPTemp aussenTemp("AußenTemp", " °C", 0x0800);
DPHours betriebsstunden("Betriebsstunden", " Std.", 0x6568);
DPTemp abgasTemp("AbgasTemp", " °C", 0x0808);
DPTemp ladespeicherObenTemp("LadespeicherObenTemp", " °C", 0x0812);
DPTemp ladespeicherUntenTemp("LadespeicherUntenTemp", " °C", 0x0814);
DPTemp kesselIstTemp("KesselIstTemp", " °C", 0x0802);
DPTemp kesselSollTemp("KesselSollTemp", " °C", 0x555A);
DPTemp warmwasserSollTemp("WarmwasserSollTemp", " °C", 0x6300);
DPTemp warmwasserIstTemp("WarmwasserIstTemp", " °C", 0x0804);
DPTemp vorlaufSollTemp("VorlaufSollTemp", " °C", 0x3544);
DPTemp vorlaufIstTemp("VorlaufIstTemp", " °C", 0x3900);
DPTemp kollektorTemp("KollektorTemp", " °C", 0x6564);
DPTemp solarspeicherTemp("SolarspeicherTemp", " °C", 0x6566);
DPMode nachladeunterdrueckung("Nachladeunterdrückung", "", 0x6551);
DPMode solarpumpe("Solarpumpe", "", 0x6552);
DPCount waermemenge("Wärmemenge", " kWh", 0x6560);
DPCount solartagesertrag("Solartagesertrag", " Wh", 0xCF30);
DPMode solarinfo("Solarinfo", "", 0x7754);
DPMode speicherladepumpe("Speicherladepumpe", "", 0x6513);
DPMode betriebsart("Betriebsart", "", 0x3323);
DPMode sparbetrieb("Sparbetrieb", "", 0x3302);
DPHours brennerlaufzeit("Brennerlaufzeit", " Std.", 0x0886);
DPMode brennerstoerung("Brennerstörung", "", 0x0883);
DPCount brennerstarts("Brennerstarts", "", 0x088A);
DPMode flamme("Flamme", " %", 0x55D3);
DPMode umschaltventil("Umschaltventil", "", 0x0A10);
DPMode umwaelzpumpe("Umwälzpumpe", "", 0x7660);
DPMode umwaelzpumpeDrehzahl("UmwälzpumpeDrehzahl", " %", 0x0A3C);
DPMode heizkreispumpe("Heizkreispumpe", "", 0x3906);
//Notice: no working error datapoint found

DPRaw stoerungsmeldung1("Störungsmeldung1", "boiler", 0x7507);
DPRaw stoerungsmeldung2("Störungsmeldung2", "boiler", 0x7510);
DPRaw stoerungsmeldung3("Störungsmeldung3", "boiler", 0x7519);
DPRaw stoerungsmeldung4("Störungsmeldung4", "boiler", 0x7522);
DPRaw stoerungsmeldung5("Störungsmeldung5", "boiler", 0x752B);
DPRaw stoerungsmeldung6("Störungsmeldung6", "boiler", 0x7534);
DPRaw stoerungsmeldung7("Störungsmeldung7", "boiler", 0x753D);
DPRaw stoerungsmeldung8("Störungsmeldung8", "boiler", 0x7546);
DPRaw stoerungsmeldung9("Störungsmeldung9", "boiler", 0x754F);
DPRaw stoerungsmeldung10("Störungsmeldung10", "boiler", 0x7558);

String lastError = "";

const uint8_t resetBytes[] = {0x04};
const uint8_t initBytes[] = {0x16, 0x00, 0x00};
const uint8_t startBytes[] = {0x41, 0x05, 0x00, 0x01};

void tempCallbackHandler(const IDatapoint& dp, DPValue value);
void globalCallbackHandler(const IDatapoint& dp, DPValue value);
void stoerungsmeldungCallbackHandler(const IDatapoint& dp, DPValue value);
void addValueToLiveData(const IDatapoint& dp, String value);

void setupVito() {
    VitoWiFi.setup(&swSer);
    VitoWiFi.setGlobalCallback(&globalCallbackHandler);
    stoerungsmeldung1.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung2.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung3.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung4.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung5.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung6.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung7.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung8.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung9.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung10.setCallback(&stoerungsmeldungCallbackHandler);
    stoerungsmeldung1.setLength(9);     //just read the first 9 bytes
    stoerungsmeldung2.setLength(9);
    stoerungsmeldung3.setLength(9);
    stoerungsmeldung4.setLength(9);
    stoerungsmeldung5.setLength(9);
    stoerungsmeldung6.setLength(9);
    stoerungsmeldung7.setLength(9);
    stoerungsmeldung8.setLength(9);
    stoerungsmeldung9.setLength(9);
    stoerungsmeldung10.setLength(9);

    for (auto it = IDatapoint::getCollection().begin(); it != IDatapoint::getCollection().end(); ++it) {
        LiveData data = {(*it), ""};
        liveData.push_back(data);       // fill the vector with all registered datapoints in the form of LiveData struct
    }

    lastError = readLastError();
}

void globalCallbackHandler(const IDatapoint& dp, DPValue value) {
    missingValuesCount = 0;             // reset error count on response
    publishMqtt("error", "0");          // reset error status over mqtt

    char value_str[15] = {0};
    value.getString(value_str, sizeof(value_str));
    publishMqtt(dp.getName(),value_str);
    strcat(value_str, dp.getGroup());   // add unit for web display
    addValueToLiveData(dp, value_str);
}

void stoerungsmeldungCallbackHandler(const IDatapoint& dp, DPValue value) {
    char dpBuffer[128];
    value.getString(dpBuffer,128);

    String dpString = String(dpBuffer);
    // 0-1: Error Code, 2-5 yyyy, 6-7 MM, 8-9 dd, 10-11 weekday (01-07), 12-13 HH, 14-15 mm, 16-17 ss
    String timeString = dpString.substring(8,10) + "." + dpString.substring(6,8) + "." + dpString.substring(4,6) + " " + dpString.substring(12,14) + ":" + dpString.substring(14,16) + ":" + dpString.substring(16,18) + " Uhr";
    uint8_t errorCode = strtol(dpString.substring(0,2).c_str(), 0, 16);
    String errorMessage = getErrorMessage(errorCode);       // resolve error code to error message

    publishMqtt(dp.getName(), (timeString + "#" + errorMessage + "#" + dpString.substring(0,2)).c_str() );

    if(dp.getName() == "Störungsmeldung1") {
        String errorIdentifier = timeString + "#" + errorCode;      // create unique string to compare errors

        if (lastError != "" && lastError != errorIdentifier && errorCode != 0) {
            publishMqtt("Störung","1");     // send error impulse once cause we are not able to determine the end of the error
            Log("Device in error state");
        }
        if(lastError == "" || lastError != errorIdentifier) {
            saveLastError(errorIdentifier);    // initially write error to filesystem and update on change
            lastError = errorIdentifier;
        }
    }
    publishMqtt("Störung","0"); 
}

void getVitoData() {
    VitoWiFi.readAll();
}

String getErrorMessage(uint8_t errorCode) {
    switch (errorCode) {
        case 0:
            return "Regelbetrieb (kein Fehler)";
            break;
        case 15:
            return "Wartung (für Reset Codieradresse 24 auf 0 stellen)";
            break;
        case 16:
            return "Kurzschluss Außentemperatursensor";
            break;
        case 24:
            return "Unterbrechung Außentemperatursensor";
            break;
        case 32:
            return "Kurzschluss Vorlauftemperatursensor";
            break;
        case 33:
            return "Kurzschluss Rücklauftemperatursensor";
            break;
        case 40:
            return "Unterbrechung Außentemperatursensor";
            break;
        case 41:
            return "Unterbrechung Rücklauftemperatursensor";
            break;
        case 48:
            return "Kurzschluss Kesseltemperatursensor";
            break;
        case 56:
            return "Unterbrechung Kesseltemperatursensor";
            break;
        case 64:
            return "Kurzschluss Vorlauftemperatursensor M2";
            break;
        case 66:
            return "Unterbrechung Vorlauftemperatursensor M2";
            break;
        case 80:
            return "Kurzschluss Speichertemperatursensor";
            break;
        case 88:
            return "Unterbrechung Speichertemperatursensor";
            break;
        case 146:
            return "Solar: Kurzschluss Kollektortemperatursensor";
            break;
        case 147:
            return "Solar: Kurzschluss Sensor S3";
            break;
        case 148:
            return "Solar: Kurzschluss Speichertemperatursensor";
            break;
        case 154:
            return "Solar: Unterbrechung Kollektortemperatursensor";
            break;
        case 155:
            return "Solar: Unterbrechung Sensor S3";
            break;
        case 156:
            return "Solar: Unterbrechung Speichertemperatursensor";
            break;
        case 158:
            return "Solar: Zu geringer oder kein Volumenstrom im Solarkreis oder Temperaturwächter hat ausgelöst";
            break;
        case 159:
            return "Solar: Fehlermeldung Solarteil (siehe Solarregler)";
            break;
        case 167:
            return "Bedienteil defekt";
            break;
        case 176:
            return "Kurzschluss Abgastemperatursensor";
            break;
        case 177:
            return "Kommunikationsfehler Bedieneinheit";
            break;
        case 180:
            return "Interner Fehler (Elektronik)";
            break;
        case 181:
            return "Interner Fehler (Elektronik)";
            break;
        case 182:
            return "Ungültige Hardwarekennung (Elektronik)";
            break;
        case 183:
            return "Interner Fehler (Kesselkodierstecker)";
            break;
        case 184:
            return "Unterbrechung Abgastemperatursensor";
            break;
        case 185:
            return "Interner Fehler (Dateneingabe wiederholen)";
            break;
        case 186:
            return "Kommunikationsfehler Erweiterungssatz für Mischerkreis M2";
            break;
        case 188:
            return "Kommunikationsfehler Fernbedienung Vitorol, Heizkreis M1";
            break;
        case 189:
            return "Kommunikationsfehler Fernbedienung Vitorol, Heizkreis M2";
            break;
        case 190:
            return "Falsche Codierung Fernbedienung Vitorol";
            break;
        case 193:
            return "Externe Sicherheitseinrichtung (Kessel kühlt aus)";
            break;
        case 194:
            return "Kommunikationsfehler Solarregelung";
            break;
        case 197:
            return "Kommunikationsfehler drehzahlgeregelte Heizkreispumpe, Heizkreis M1";
            break;
        case 198:
            return "Kommunikationsfehler drehzahlgeregelte Heizkreispumpe, Heizkreis M2";
            break;
        case 199:
            return "Falsche Codierung der Heizkreispumpe";
            break;
        case 201:
            return "Störmeldeeingang am Schaltmodul-V aktiv";
            break;
        case 205:
            return "Kommunikationsfehler Vitocom 100 (KM-BUS)";
            break;
        case 206:
            return "Kommunikationsfehler Schaltmodul-V";
            break;
        case 207:
            return "Kommunikationsfehler LON Modul";
            break;
        case 209:
            return "Brennerstörung";
            break;
        case 212:
            return "Sicherheitstemperaturbegrenzer hat ausgelöst oder Störmeldemodul nicht richtig gesteckt";
            break;
        case 218:
            return "Kurzschluss Raumtemperatursensor, Heizkreis M1";
            break;
        case 219:
            return "Kurzschluss Raumtemperatursensor, Heizkreis M2";
            break;
        case 221:
            return "Unterbrechung Raumtemperatursensor, Heizkreis M1";
            break;
        case 222:
            return "Unterbrechung Raumtemperatursensor, Heizkreis M2";
            break;
        case 228:
            return "Fehler Versorgungsspannung";
            break;
        case 229:
            return "Interner Fehler (Ionisationselektrode)";
            break;
        case 230:
            return "Abgas- / Zuluftsystem verstopft";
            break;
        case 240:
            return "Interner Fehler (Regelung tauschen)";
            break;
        case 241:
            return "Abgastemperaturbegrenzer ausgelöst";
            break;
        case 242:
            return "Temperaturbegrenzer ausgelöst";
            break;
        case 243:
            return "Flammensigal beim Brennerstart bereits vorhanden";
            break;
        case 244:
            return "Flammensigal nicht vorhanden";
            break;
        case 247:
            return "Differenzdrucksensor defekt";
            break;
        case 248:
            return "Brennstoffventil schließt zu spät";
            break;
        case 249:
            return "Gebläsedrehzahl beim Brennerstart zu niedrig";
            break;
        case 250:
            return "Gebläsestillstand nicht erreicht";
            break;
        case 253:
            return "Fehler Gasfeurungsautomat";
            break;
        case 254:
            return "Starkes Störfeld (EMV) in der Nähe oder Elektronik defekt";
            break;
        case 255:
            return "Starkes Störfeld (EMV) in der Nähe oder interner Fehler";
            break;
        default:
            return "Unbekannter Fehler";
    }
}    

void addValueToLiveData(const IDatapoint& dp, String value) {
    for (auto it = liveData.begin(); it != liveData.end(); ++it) {
        if (dp.getName() == (*it).dp->getName()) {
            (*it).value = value;
        }
    }

}