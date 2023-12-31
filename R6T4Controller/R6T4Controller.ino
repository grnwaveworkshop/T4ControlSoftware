

/*

  Teensy 4.1 controller
  Servo Control
  Audio Control
  SBUS input
  LED control
  Display output

*/

#include <OctoWS2811.h>
#include <Arduino.h>
//#include <Adafruit_GFX.h>
#include <ST7789_t3.h>
#include "definitions.h"
#include <SD.h>
//#include <SD_t3.h>

//#include <Wire.h>
//#include <TeensyThreads.h>


//ST7789_t3 tft = ST7789_t3(TFT_CS,  TFT_DC, TFT_RST);
ST7789_t3 tft = ST7789_t3(-1,  TFT_DC, TFT_RST);




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


