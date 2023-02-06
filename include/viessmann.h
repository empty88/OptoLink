#pragma once
#include <SoftwareSerial.h>
#include <Logging.h>
#include <VitoWiFi.h>

extern VitoWiFi_setProtocol(P300);
extern SoftwareSerial swSer;

extern byte missingValuesCount;

struct LiveData {
    IDatapoint* dp;
    String value;
};

extern std::vector<LiveData> liveData;

void setupVito();

void getVitoData();
String getErrorMessage(uint8_t code);