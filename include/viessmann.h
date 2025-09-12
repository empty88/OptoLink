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
    String value_str;
};

extern std::vector<LiveData> liveData;
extern uint8_t operationMode;


void setupVito();

void getVitoData();
String getErrorMessage(uint8_t code);

extern void setOperationMode(uint8_t val);
void readOperationMode();