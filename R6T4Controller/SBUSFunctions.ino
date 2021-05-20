
#include "SBUS.h"
#include "definitions.h"


// a SBUS object, which is on hardware
// serial port 1
SBUS x8r(Serial1);
int z = 0;

// channel, fail safe, and lost frames data

uint16_t channelsTemp[16];
uint16_t channels[16];
bool failSafe;
bool lostFrame;
bool SBUS_OK = 0;
uint8_t SBUSokCount = MAXSBUSOK;

void SBUSsetup() {
  // begin the SBUS communication
  Serial.println("Setup SBUS");
  x8r.begin();
}

void SBUSloop() {
  //Serial.println("SBus Loop ");
  // look for a good SBUS packet from the receiver
  if (x8r.read(&channelsTemp[0], &failSafe, &lostFrame)) {
    //    Serial.print(z);
    //    Serial.print(" ");
    // z++;

// Turn on SBUS serial outputs
#if 0
    for (int i = 0; i < 16; i++) {
      Serial.print("C");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(channelsTemp[i]);
      Serial.print(" ");

    }
    Serial.print(failSafe);
    Serial.print(" ");
    Serial.print(lostFrame);
    Serial.println(" ");
#endif
  }
  if (!failSafe && !lostFrame) {
    for (int i = 0; i < 16; i++) { // check if any channels are < 170, if so, toss the frame
      if (channelsTemp[i] <= 170) {
        lostFrame = 1;
        if(SBUSokCount > 0) SBUSokCount--;
      }
    }
    if (!lostFrame) {
      for (int i = 0; i < 16; i++) { // copy data across if lost frame is ok.
        channels[i] = channelsTemp[i];
        if(SBUSokCount >= MAXSBUSOK)SBUSokCount = MAXSBUSOK;
        else SBUSokCount++;

      }
    }

  }
  else if(SBUSokCount > 0) SBUSokCount--;
}

bool SBUSOK() {
if (SBUSokCount >= MAXSBUSOK) SBUS_OK = 1;
else SBUS_OK = 0;
    return SBUS_OK;
 
}

uint16_t ChannelData(uint16_t channelnum) {
  return channels[channelnum];

}
