#include <SD.h>
#include <SD_t3.h>

/*

 

*/

#include <OctoWS2811.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <ST7789_t3.h>
#include "definitions.h"

//#include <Wire.h>
//#include <TeensyThreads.h>


ST7789_t3 tft = ST7789_t3(TFT_CS,  TFT_DC, TFT_RST);





void setup()
{


  Serial.begin(115200);
  Serial.println("Booting... ");
  DisplaySetup();
  LEDsetup();

  SBUSsetup();
  AudioSetup();
  ServoSetup();

  tft.fillScreen(ST7735_BLACK);

  randomSeed(analogRead(0));

}


void loop() {
//Serial.println("Hello World");
  SBUSloop();
  LEDloop();
  DisplayLoop();
  AudioLoop();
  ServoLoop();
  Serialloop();
}

float ReadInputVolts() {
  // .235 divider  Vin = Vout/0.235
  float Vin;
  Vin = (analogRead(9) * (3.6 / 1023)) / .235;

  return Vin;

}
