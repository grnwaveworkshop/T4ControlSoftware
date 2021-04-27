/*

  LED Banks:
  Data Panel: 56 LEDs
  Charge Panel: 23 LEDs

  http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    DataPanel: LEDBankA: pin 40 pixels 0-55
    Charge Port: LEDBankB: pin 39 pixels 56 - 79

    DataPanel LED arrangement:

*/

#include <OctoWS2811.h>
#include <Arduino.h>
#include "definitions.h"

// Any group of digital pins may be used
const uint8_t numPins = 2;
byte pinList[numPins] = {40, 39};  // DataPanel, Charge Port

const uint8_t ledsPerStrip = 56;  // DataPanel max 56 LEDs

const uint8_t numBankALEDs = 24;
const uint8_t numBankBLEDs = 8;
const uint8_t numBarLEDs = 8;

const uint8_t BankAStartLED = 32;
const uint8_t BankAEndLED = 55;
const uint8_t BankBStartLED = 24;
const uint8_t BankBEndLED = 31;
const uint8_t BarAStartLED = 0;
const uint8_t BarAEndLED = 7;
const uint8_t BarBStartLED = 8;
const uint8_t BarBEndLED = 15;
const uint8_t BarCStartLED = 16;
const uint8_t BarCEndLED = 23;

const uint32_t ColorArray[16] {RED, BLUE, WHITE, GREEN, YELLOW, ORANGE};

// These buffers need to be large enough for all the pixels.
// The total number of pixels is "ledsPerStrip * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is
// 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
int drawingMemory[ledsPerStrip * numPins * 3 / 4];

unsigned long swap_intBankA[numBankALEDs];
unsigned long prev_millisBankA[numBankALEDs];
bool BankALED_ON[numBankALEDs];

unsigned long swap_intBankB[numBankBLEDs];
unsigned long prev_millisBankB[numBankBLEDs];
bool BankBLED_ON[numBankBLEDs];

unsigned long LEDPrevMillis = millis();


int audiomax = 0;
int audiomin = 1500;
int audiomid = 500;

const uint8_t numCPLEDs = 20;

const uint8_t CPStartLED = 56;

unsigned long swap_CP[numCPLEDs];
unsigned long prev_millisCP[numCPLEDs];
bool CPLED_ON[numCPLEDs];


const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);

void LEDsetup() {
  Serial.println("Setup LEDs");
  leds.begin();
  leds.show();


  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, random(10, 60), random(10, 60), random(10, 60));
    leds.show();
    delay(100);
  }
    for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, 0);
 
 //   delay(100);
  }
     leds.show();

  for (int i = BarAStartLED; i < BarAEndLED+1; i++) {
    leds.setPixel(i, WHITE);
    leds.show();
    delay(10);
  }

  for (int i = BarBStartLED; i < BarBEndLED+1; i++) {
    leds.setPixel(i, BLUE);
    leds.show();
    delay(10);
  }
  for (int i = BarCStartLED; i < BarCEndLED+1; i++) {
    leds.setPixel(i, YELLOW);
    leds.show();
    delay(10);
  }

  for (int i = BankAStartLED; i < BankAEndLED+1; i++) {
    leds.setPixel(i, GREEN);
    leds.show();
    delay(10);
  }

    for (int i = BankBStartLED; i < BankBEndLED+1; i++) {
    leds.setPixel(i, ORANGE);
    leds.show();
    delay(10);
  }
  
  // Seed variable intervals
  for (int i = 0; i < numBankALEDs; i++) { // For each LED in strip...
    swap_intBankA[i] = random(300, 800);
    prev_millisBankA[i] = millis();
    BankALED_ON[i] = 0;
  }

  for (int i = 0; i < numBankBLEDs; i++) { // For each LED in strip...
    swap_intBankB[i] = random(300, 800);
    prev_millisBankB[i] = millis();
    BankBLED_ON[i] = 0;
  }

    for (int i = 0; i < numCPLEDs; i++) { // For each LED in strip...
    swap_CP[i] = random(300, 800);
    prev_millisCP[i] = millis();
    CPLED_ON[i] = 0;
  }
}

void LEDloop() {

  if (millis() - LEDPrevMillis > 10) {
    //SpectrumBarsSBUS(BLUE);
    KITTBarsAudio(WHITE, RED);
    twinkleBankA(BLUE);
    twinkleBankB(WHITE);
    SBUSStatusLED();
     twinkleCP(RED);
    LEDPrevMillis = millis();
  }

  if (!leds.busy()) leds.show();

}


void KITTBarsAudio(uint32_t color, uint32_t color2)
{
  int i = 0;
  int level1;
  int level2;
  int level3;


  int audio = (analogRead(AUDIN));
//  Serial.println(audio);
  if (audio >= audiomax) {
    audiomax = audio;
  }
  if (audio <= audiomin) {
    audiomin = audio;
  }
  int audiomid = 505;//(audiomax + audiomin) / 2;

  int level = 4 * abs(audio - audiomid);

  level1 = map(level * .8, 0, audiomid, 0, 4);
  level2 = map(level, 0, audiomid, 0, 4);
  level3 = map(level * .8 , 0, audiomid, 0, 4);

  //    Serial.print(audio);
  //    Serial.print(" ");
  //    Serial.print(audiomin);
  //    Serial.print(" ");
  //
  //    Serial.print(audiomid);
  //    Serial.print(" ");
  //    Serial.print(audiomax);
  //     Serial.print(" ");
  //    Serial.print(level1);
  //       Serial.print(" ");
  //    Serial.print(level2);
  //       Serial.print(" ");
  //    Serial.print(level3);
  //    Serial.print(" ");
  //    Serial.println(level);



  //Clear bars
  for (i = 0; i <= BarCEndLED; i++) {
    leds.setPixel(i, 0);
  }

  for (i = 0; i < level1; i++) {
    if (i < 1) {
      leds.setPixel(BarAStartLED + 4 + i, color);
      leds.setPixel(BarAStartLED + 3 - i, color);
    }
    else {
      leds.setPixel(BarAStartLED + 4 + i, color2);
      leds.setPixel(BarAStartLED + 3 - i, color2);
    }
  }

  for (i = 0; i < level2; i++) {
    if (i < 1) {
      leds.setPixel(BarBStartLED + 4 + i, color);
      leds.setPixel(BarBStartLED + 3 - i, color);
    }
    else {
      leds.setPixel(BarBStartLED + 4 + i, color2);
      leds.setPixel(BarBStartLED + 3 - i, color2);
    }
  }

  for (i = 0; i < level3; i++) {
    if (i < 1) {
      leds.setPixel(BarCStartLED + 4 + i, color);
      leds.setPixel(BarCStartLED + 3 - i, color);
    }
    else {
      leds.setPixel(BarCStartLED + 4 + i, color2);
      leds.setPixel(BarCStartLED + 3 - i, color2);
    }
  }

}

void SpectrumBarsSBUS(uint32_t color)
{
  int i = 0;
  int level1 = ChannelData(1);
  int level2 = ChannelData(2);
  int level3 = ChannelData(3);

  level1 = map(level1, 172, 1811, 0, 8);
  level2 = map(level2, 172, 1811, 0, 8);
  level3 = map(level3, 172, 1811, 0, 8);

  if (level1 > 9) level1 = 9;
  if (level2 > 9) level2 = 9;
  if (level2 > 9) level2 = 9;

  //Clear bars
  for (i = 0; i <= BarCEndLED; i++) {
    leds.setPixel(i, 0);
  }

  for (i = 0; i < level1; i++) {
    leds.setPixel(BarAStartLED + i, color);
  }

  for (i = 0; i < level2; i++) {
    leds.setPixel(BarBStartLED + i, color);
  }
  for (i = 0; i < level3; i++) {
    leds.setPixel(BarCStartLED + i, color);
  }

}

void SpectrumBars(uint32_t color, int cycles, int wait)
{
  int i = 0;
  for (int c = 0; c <= cycles; c++) { // Number of times to cycle through
    // Select 3 Random Levels
    int level1 = random(0, 9);
    int level2 = random(0, 9);
    int level3 = random(0, 9);

    //Clear bars
    for (i = 0; i <= BarCEndLED; i++) {
      leds.setPixel(i, 0);
    }
    // Turn on the 3 Bars with random levels
    //        strip.setBrightness(b);
    for (i = 0; i < level1; i++) {

      leds.setPixel(BarAStartLED + i, color);
    }
    for (i = 0; i < level2; i++) {

      leds.setPixel(BarBStartLED + i, color);
    }
    for (i = 0; i < level3; i++) {

      leds.setPixel(BarCStartLED + i, color);
    }


  }
}

void twinkleBankA(uint32_t color)
{
  for (int i = 0; i < numBankALEDs; i++) { // For each LED in strip...
    if (millis() - prev_millisBankA[i] >= swap_intBankA[i]) {
      if (BankALED_ON[i]) { // LED is on, so fade it off
        leds.setPixel(BankAStartLED + i, ColorArray[random(0, 3)]); //random(10, 60), random(10, 60), random(10, 60));
        BankALED_ON[i] = 0;
        prev_millisBankA[i] = millis();
        swap_intBankA[i] = random(500, 2000);
      }
      else {// fade the LED on{

        leds.setPixel(BankAStartLED + i, 0);
        BankALED_ON[i] = 1;
        prev_millisBankA[i] = millis();
        swap_intBankA[i] = random(100, 2000);

      }
    }
  }
}

void twinkleBankB(uint32_t color)
{


  for (int i = 0; i < numBankBLEDs - 1; i++) { // For each LED in strip...
    if (millis() - prev_millisBankB[i] >= swap_intBankB[i]) {
      if (BankBLED_ON[i]) { //LED is on, so fade it off
        leds.setPixel(BankBStartLED + i, ColorArray[random(0, 3)]);
        BankBLED_ON[i] = 0;
        prev_millisBankB[i] = millis();
        swap_intBankB[i] = random(500, 2000);

      }
      else {    // turn the LED off

        leds.setPixel(BankBStartLED + i, 0);
        BankBLED_ON[i] = 1;
        prev_millisBankB[i] = millis();
        swap_intBankB[i] = random(100, 2000);

      }
    }
  }
}

void SBUSStatusLED()
{
  int SBUSLED = 7;
  if (SBUSOK()) {
    if (millis() - prev_millisBankB[SBUSLED] >= swap_intBankB[SBUSLED]) {
      if (BankBLED_ON[SBUSLED]) { // LED is on, so fade it off
        leds.setPixel(BankBStartLED + SBUSLED, BLUE);
        BankBLED_ON[SBUSLED] = 0;
        prev_millisBankB[SBUSLED] = millis();
        swap_intBankB[SBUSLED] = random(100, 800);

      }
      else { //Turn LED off

        leds.setPixel(BankBStartLED + SBUSLED, 0);
        BankBLED_ON[SBUSLED] = 1;
        prev_millisBankB[SBUSLED] = millis();
        swap_intBankB[SBUSLED] = random(100, 800);

      }
    }
  }
  else {
    if (millis() - prev_millisBankB[SBUSLED] >= swap_intBankB[SBUSLED]) {
      if (BankBLED_ON[SBUSLED]) { // LED is on, so fade it off
        leds.setPixel(BankBStartLED + SBUSLED, RED);
        BankBLED_ON[SBUSLED] = 0;
        prev_millisBankB[SBUSLED] = millis();
        swap_intBankB[SBUSLED] = random(100, 800);

      }
      else {// fade the LED on{

        leds.setPixel(BankBStartLED + SBUSLED, 0);
        BankBLED_ON[SBUSLED] = 1;
        prev_millisBankB[SBUSLED] = millis();
        swap_intBankB[SBUSLED] = random(100, 800);

      }
    }
  }
}

void twinkleCP(uint32_t color)
{
  for (int i = 0; i < numCPLEDs; i++) { // For each LED in strip...
    if (millis() - prev_millisCP[i] >= swap_CP[i]) {
      if (CPLED_ON[i]) { // LED is on, so fade it off
        leds.setPixel(CPStartLED + i, color); //random(10, 60), random(10, 60), random(10, 60));
        CPLED_ON[i] = 0;
        prev_millisCP[i] = millis();
        swap_CP[i] = random(500, 2000);
      }
      else {

        leds.setPixel(CPStartLED + i, 0);
        CPLED_ON[i] = 1;
        prev_millisCP[i] = millis();
        swap_CP[i] = random(100, 2000);

      }
    }
  }
}
