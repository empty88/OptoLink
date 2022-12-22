#include <WiFiUdp.h>
#include "Logging.h"

time_t getNtpTime();
boolean summertime_EU(int year, int month, int day, int hour, int tzHours);

void updateTime();
void setTimeServer(IPAddress timesvr);

class NtpClient
{
public:
	NtpClient();
	~NtpClient();
};
