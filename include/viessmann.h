#pragma once
#include <SoftwareSerial.h>
#include <Logging.h>
#include <VitoWiFi.h>

extern VitoWiFi_setProtocol(P300);
extern SoftwareSerial swSer;
void setupVito();

void getVitoData();
String getErrorMessage(uint8_t code);

class conv1_2_F : public DPType {
 public:
  void encode(uint8_t* out, DPValue in) {

  }
  DPValue decode(const uint8_t* in) {
    int32_t tmp = in[3] << 24 | in[2] << 16 | in[1] << 8 | in[0];
    DPValue out(tmp / 2.0f);
    return out;
  }
  const size_t getLength() const {
    return 1;
  }
};

typedef Datapoint<conv1_2_F> DPPercent;