#pragma once
#include <TimeLib.h>
#include "Arduino.h"

void Log(String message);
void Log(String message, bool dummyTime);

String GetLogs();

class Logging
{
public:
	Logging();
	~Logging();
};
