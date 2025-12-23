#include <OctoWS2811.h>
#include <Arduino.h>
//#include <Adafruit_GFX.h>
#include <ST7789_t3.h>
#include "Definitions.h"
#include <SD.h>
//#include <SD_t3.h>

//#include <Wire.h>
//#include <TeensyThreads.h>


//ST7735_t3 tft = ST7735_t3(TFT_CS,  TFT_DC, TFT_RST);
ST7735_t3 tft = ST7735_t3(-1,  TFT_DC, TFT_RST);

#include "AudioFunctions.hpp"
#include "AudioVocalizer.hpp"
#include "ChargePanelLED.hpp"
#include "DisplayFunctions.hpp"
#include "Events.hpp"
#include "HelperFunctions.hpp"
#include "LEDFunctions.hpp"
#include "SBUSFunctions.hpp"
#include "Serial485.hpp"
#include "ServoEasingFunctions.hpp"
#include "ServoSequences.hpp"


void setup()
{
  Serial.begin(115200);
  Serial.println("Booting... ");
  DisplaySetup();
  LEDsetup();
  SBUSsetup();
  //AudioSetup();
  VocalizerSetup();
  ServoSetup();
  Setup485();
  HelperSetup();
  Serial.println("Setup Complete... ");
  randomSeed(analogRead(0));

}


void loop() {
  SBUSloop();
  LEDloop();
  DisplayLoop();
 // AudioLoop();
  VocalizerAudioLoop();
  ServoLoop();
  ProcessEventsLoop();
  Serialloop();
 ProcessHelpers();
}