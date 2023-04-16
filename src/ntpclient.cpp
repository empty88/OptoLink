#include "ntpclient.h"

#include <WiFiUdp.h>
#include "logging.h"


WiFiUDP Udp;

IPAddress timeServer; //Fritzbox Zeitserver
int timeZone = 1;     // Central European Time
unsigned int localPort = 8888;  // local port to listen for UDP packets

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

void sendNTPpacket(IPAddress &address);
time_t getNtpTime();

void setTimeServer(IPAddress timesvr) {
	char text[50];
	sprintf(text,"Configured NTP Server: %s",timesvr.toString().c_str());
	Log(text);
	timeServer = timesvr;
}

void updateTime() {
	time_t recTime = getNtpTime();
	if (recTime != 0) setTime(recTime);
	//setSyncProvider(getNtpTime);
	Log(F("NTP update finished"));
}

time_t getNtpTime()
{
	Udp.begin(localPort);
	while (Udp.parsePacket() > 0); // discard any previously received packets
	char text[50];
	sprintf(text, "Transmit NTP request to %s", timeServer.toString().c_str());
	Log(text, true);
	sendNTPpacket(timeServer);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
 		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Log("Receive NTP response", true);
			Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];

			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}
	Log("No NTP response :-(");
	return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:                 
	if (Udp.beginPacket(address, 123) == 1) { //NTP requests are to port 123
		Udp.write(packetBuffer, NTP_PACKET_SIZE);
		Udp.endPacket();
	};
}

boolean summertime_EU(int year, int month, int day, int hour, int tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
	if (month < 3 || month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
	if (month > 3 && month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
	if ((month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7))) || (month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7))))
		return true;
	else
		return false;
}


NtpClient::NtpClient()
{
}


NtpClient::~NtpClient()
{
}
