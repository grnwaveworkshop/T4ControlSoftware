/*

  LED Banks:
  Data Panel: 56 LEDs
  Charge Panel: 23 LEDs
  Data Port: 

  http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
	DataPanel: LEDBankA: pin 40 pixels 0-55
	Charge Port: LEDBankB: pin 39 pixels 56 - 79

	DataPanel LED arrangement:

							32  33  34  35

							36  37  38  39

							40  41  42  43
		  23   15   7
		  22   14   6       44  45  46  47
		  21   13   5
		  20   12   4       48  49  50  51
		  19   11   3
		  18   10   2       52  53  54  55
		  17   9    1
		  16   8    0
							  30    31
		  24    25    26

		  27    28    29
*/

#include <OctoWS2811.h>
#include <Arduino.h>
#include "definitions.h"

// Any group of digital pins may be used
const uint8_t numPins = 3;
byte pinList[numPins] = { 40, 39, 38 };  // DataPanel, Charge Port, Data Port

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

const uint32_t ColorArray[16]{ WHITE, RED, BLUE, GREEN, YELLOW, ORANGE };

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

// flags to enable LEDs for sections
bool CPLEDsON = 1;
bool DPLEDsON = 1;
bool DPortLEDsON = 1;

extern uint16_t CurrentAudioVolume;

// Charge Panel LED Variables
const uint8_t numCPLEDs = 20;
const uint8_t CPStartLED = 56;
const uint8_t CPVLED1StartLED = 78; // Voltage display start LED 1 (Bottom)
const uint8_t CPVLED2StartLED = 77;	// Voltage display start LED 2 (Middle)
const uint8_t CPVLED3StartLED = 76;	// Voltage display start LED 1 (Top)

unsigned long swap_CP[numCPLEDs];
unsigned long prev_millisCP[numCPLEDs];
bool CPLED_ON[numCPLEDs];

const uint8_t numDPortLEDs = 33;
const uint8_t DPortStartLED = 112;  // Data Port String Start LED
byte gDPortIndex = 0;
byte gDPortKRUp = 1;
unsigned long DPPortLEDMillis = millis();

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*LEDSpectrumPatternList[])();
LEDSpectrumPatternList gLEDSpectrumPatterns = { SpectrumBarsSBUS, KITTBarsAudio };

uint8_t gCurrentLEDSpectrumNumber = 1; // Index number of which pattern is current

// List of Data Port patterns to cycle through. 
typedef void (*LEDDataPortPatternList[])();
LEDSpectrumPatternList gLEDDataPortPatterns = { DPortOff, DPortKnightRiderRED , DPortShortCircuit, DPortCard, DPortSiren };

uint8_t gCurrentLEDDPortNumber = 1; // Index number of which pattern is current

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);

void LEDsetup() {
	Serial.println("Setup LEDs");
	leds.begin();
	leds.show();

#if 0 // Test patterns
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

	for (int i = BarAStartLED; i < BarAEndLED + 1; i++) {
		leds.setPixel(i, WHITE);
		leds.show();
		delay(10);
	}

	for (int i = BarBStartLED; i < BarBEndLED + 1; i++) {
		leds.setPixel(i, BLUE);
		leds.show();
		delay(10);
	}
	for (int i = BarCStartLED; i < BarCEndLED + 1; i++) {
		leds.setPixel(i, YELLOW);
		leds.show();
		delay(10);
	}

	for (int i = BankAStartLED; i < BankAEndLED + 1; i++) {
		leds.setPixel(i, GREEN);
		leds.show();
		delay(10);
	}

	for (int i = BankBStartLED; i < BankBEndLED + 1; i++) {
		leds.setPixel(i, ORANGE);
		leds.show();
		delay(10);
	}

	for (int i = DPortStartLED; i <= DPortStartLED + numDPortLEDs; i++) {
		leds.setPixel(i, ORANGE);
		leds.show();
		delay(250);
	}
#endif


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

	if (millis() - LEDPrevMillis > 15) {

		gLEDSpectrumPatterns[gCurrentLEDSpectrumNumber]();
		gLEDDataPortPatterns[gCurrentLEDDPortNumber]();
		// SpectrumBarsSBUS(BLUE);
		 //KITTBarsAudio(WHITE, RED);
		twinkleBankA();
		twinkleBankB(WHITE);
		SBUSStatusLED();
		twinkleCP(RED);
		CPVoltLEDs();
		
		LEDPrevMillis = millis();

	}

	if (!leds.busy()) leds.show();

} // End Loop

void nextLEDPatternDP()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentLEDSpectrumNumber = (gCurrentLEDSpectrumNumber + 1) % ARRAY_SIZE(gLEDSpectrumPatterns);
}

void KITTBarsAudio()//uint32_t color, uint32_t color2)
{
	int i = 0;
	int level1;
	int level2;
	int level3;
	//	float level;
	uint32_t color = 0x555555; // white
	uint32_t color2 = 0x770000; // red

//	level = AudioLevel;

	level1 = AudioLevel - 1;	//map(level * .8, 0, audiomid, 0, 4);
	level2 = AudioLevel;	//map(level, 0, audiomid, 0, 4);
	level3 = AudioLevel - 1;	//map(level * .8, 0, audiomid, 0, 4);

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

void SpectrumBarsSBUS() //uint32_t color)
{
	int i = 0;
	int level1 = ChannelData(0);
	int level2 = ChannelData(1);
	int level3 = ChannelData(2);
	uint32_t color = BLUE;

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

// Random blinking of bank A LEDs
// Group bars together in 3 () indicates bank a offset
// BankA1 33(1)  34  35
// BankA2 37(5)  38  39
// BankA3 41(9)  42  43
// BankA4 45(13) 46  47
// BankA5 49(17) 50  51
// BankA6 53(21) 54  55
void twinkleBankA()
{
	// Check if need to turn all LEDs off
	if (DPLEDsON) {
		// Singles
		for (int i = 0; i < numBankALEDs; i = i + 4) { // For each LED in strip...
			if (millis() - prev_millisBankA[i] >= swap_intBankA[i]) {
				if (BankALED_ON[i]) { // LED is on, so fade it off
					//leds.setPixel(BankAStartLED + i, 0);
					BankALED_ON[i] = 0;
					prev_millisBankA[i] = millis();
					swap_intBankA[i] = random(500, 2000);
				}
				else {// fade the LED on{
					leds.setPixel(BankAStartLED + i, ColorArray[random(0, 3)]);
					BankALED_ON[i] = 1;
					prev_millisBankA[i] = millis();
					swap_intBankA[i] = random(100, 2000);
				}
			}

			// Check if need to fade LED
			if (!BankALED_ON[i]) {
				FadeLEDs(BankAStartLED + i, BankAStartLED + i, 10);
			}
		}
		// Check each 'bar' and light up all three LEDs at once.
		for (int y = 1; y < 20; y = y + 4) {
			if (millis() - prev_millisBankA[y] >= swap_intBankA[y]) {

				if (BankALED_ON[y]) {
					BankALED_ON[y] = 0;
					prev_millisBankA[y] = millis();
					swap_intBankA[y] = random(500, 2000);
				}
				else {
					leds.setPixel(BankAStartLED + y, ColorArray[random(1, 5)]);
					leds.setPixel(BankAStartLED + y + 1, leds.getPixel(BankAStartLED + y));
					leds.setPixel(BankAStartLED + y + 2, leds.getPixel(BankAStartLED + y));

					BankALED_ON[y] = 1;
					prev_millisBankA[y] = millis();
					swap_intBankA[y] = random(100, 2000);
				}
			}
			if (!BankALED_ON[y]) {
				FadeLEDs(BankAStartLED + y, BankAStartLED + y + 2, 10);
			}
		}
	}
	else {
		for (int z = 0; z < numBankALEDs; z++) {
			FadeLEDs(BankAStartLED + z, BankAStartLED + z, 10);
		}
	}
}

void twinkleBankB(uint32_t color)
{
	if (DPLEDsON) {
		for (int i = 0; i < numBankBLEDs - 1; i++) { // For each LED in strip...
			if (millis() - prev_millisBankB[i] >= swap_intBankB[i]) {
				if (BankBLED_ON[i]) { //LED is on, so fade it off
					//leds.setPixel(BankBStartLED + i, 0);
					BankBLED_ON[i] = 0;
					prev_millisBankB[i] = millis();
					swap_intBankB[i] = random(100, 2000);

				}
				else {    // turn the LED off
					leds.setPixel(BankBStartLED + i, ColorArray[random(0, 4)]);
					BankBLED_ON[i] = 1;
					prev_millisBankB[i] = millis();
					swap_intBankB[i] = random(500, 2000);

				}
			}
			if (!BankBLED_ON[i]) {
				FadeLEDs(BankBStartLED + i, BankBStartLED + i, 1);
			}
		}
	}
	else {
		for (int z = 0; z < numBankBLEDs; z++) {
			FadeLEDs(BankBStartLED + z, BankBStartLED + z, 10);
		}
	}
}

void SBUSStatusLED()
{
	int SBUSLED = 7;
	if (SBUSOK()) {
		if (millis() - prev_millisBankB[SBUSLED] >= swap_intBankB[SBUSLED]) {
			if (BankBLED_ON[SBUSLED]) { // LED is on, so fade it off

				//leds.setPixel(BankBStartLED + SBUSLED, 0);
				BankBLED_ON[SBUSLED] = 0;
				prev_millisBankB[SBUSLED] = millis();
				swap_intBankB[SBUSLED] = random(100, 800);

			}
			else { //Turn LED off
				leds.setPixel(BankBStartLED + SBUSLED, BLUE);
				BankBLED_ON[SBUSLED] = 1;
				prev_millisBankB[SBUSLED] = millis();
				swap_intBankB[SBUSLED] = random(100, 800);


			}
		}
	}
	else {
		if (millis() - prev_millisBankB[SBUSLED] >= swap_intBankB[SBUSLED]) {
			if (BankBLED_ON[SBUSLED]) { // LED is on, so fade it off
				//leds.setPixel(BankBStartLED + SBUSLED, 0);
				BankBLED_ON[SBUSLED] = 0;
				prev_millisBankB[SBUSLED] = millis();
				swap_intBankB[SBUSLED] = random(100, 800);


			}
			else {// turn the LED on
				leds.setPixel(BankBStartLED + SBUSLED, RED);
				BankBLED_ON[SBUSLED] = 1;
				prev_millisBankB[SBUSLED] = millis();
				swap_intBankB[SBUSLED] = random(100, 800);
			}
		}
	}
	if (!BankBLED_ON[SBUSLED]) {
		FadeLEDs(BankBStartLED + SBUSLED, BankBStartLED + SBUSLED, 1);
	}
}

void twinkleCP(uint32_t color)
{
	if (CPLEDsON) {
		//FadeLEDs(CPStartLED, CPStartLED + numCPLEDs, 1);
		for (uint8_t i = 0; i < numCPLEDs; i++) { // For each LED in panel...
			if (millis() - prev_millisCP[i] >= swap_CP[i]) { // timer expired, do something
				if (CPLED_ON[i]) { // LED is on, so fade it off
					//leds.setPixel(CPStartLED + i, 0);
					CPLED_ON[i] = 0;
					prev_millisCP[i] = millis();
					swap_CP[i] = random(100, 2000);
				}
				else {
					leds.setPixel(CPStartLED + i, color);
					CPLED_ON[i] = 1;
					prev_millisCP[i] = millis();
					swap_CP[i] = random(500, 2000);

				}
			}
			if (!CPLED_ON[i]) {
				FadeLEDs(CPStartLED + i, CPStartLED + i, 1);
			}
		}
	}
	else {
		for (uint8_t z = 0; z < numCPLEDs; z++)
			FadeLEDs(CPStartLED + z, CPStartLED + z, 1);
	}
}

void CPVoltLEDs() {
	float BattVolts = ReadInputVolts();

	if (BattVolts < 12.3) {
		leds.setPixel(CPVLED1StartLED, 0x180000);
		leds.setPixel(CPVLED2StartLED, 0);
		leds.setPixel(CPVLED3StartLED, 0);
	}
	if (BattVolts >= 12.3 && BattVolts < 12.8) {
		leds.setPixel(CPVLED1StartLED, 0x180000);
		leds.setPixel(CPVLED2StartLED, 0x161600);
		leds.setPixel(CPVLED3StartLED, 0);
	}
	if (BattVolts >= 12.8) {
		leds.setPixel(CPVLED1StartLED, 0x180000);
		leds.setPixel(CPVLED2StartLED, 0x161600);
		leds.setPixel(CPVLED3StartLED, 0x002000);
	}

}

// Function to Fade LEDs specified by amount
// StartLED <= StopLED
void FadeLEDs(uint8_t StartLED, uint8_t StopLED, uint16_t FadeAmount) {
	uint32_t color;
	uint32_t r;
	uint32_t g;
	uint32_t b;
	float Fade = FadeAmount;
	for (uint8_t i = 0; i <= StopLED - StartLED; i++) {
		color = leds.getPixel(StartLED + i);
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;

		r = r * (1 - (Fade / 100));
		g = g * (1 - (Fade / 100));
		b = b * (1 - (Fade / 100));

		leds.setPixel(StartLED + i, r, g, b);
	}
}

void DPortOff() {
	FadeLEDs(DPortStartLED, DPortStartLED + numDPortLEDs, 25);
	DPortLEDsON = 0;
}

void DPortKnightRiderRED() {
	DPortKnightRider(0xFF0000, 0, 10);
}

void DPortKnightRider(uint32_t colorfg, uint32_t colorbg, uint16_t wait)
{
	/*Serial.print(DPortKRUp);
	Serial.print(" i: ");
	Serial.println(DPortIndex);*/
		if (gDPortKRUp) {
			if (gDPortKRUp > 1) {
				gDPortKRUp--;
			}
			else {
				if ((millis() - DPPortLEDMillis) > wait) {
					if (gDPortIndex > 0) {
						gDPortIndex--;
						leds.setPixel(DPortStartLED + gDPortIndex, colorfg);
					//	leds.setPixel(DPortStartLED + DPortIndex + 1, colorbg);
					}
					else {
						gDPortKRUp = 0;
						
					}
					DPPortLEDMillis = millis();
					//Serial.println(DPPortLEDMillis);
				}
			}
		}
		else {
			if ((millis() - DPPortLEDMillis) > (wait)) {
				if (gDPortIndex < numDPortLEDs - 1) {
					gDPortIndex++;
					leds.setPixel(DPortStartLED + gDPortIndex, colorfg);
				//	leds.setPixel(DPortStartLED + DPortIndex - 1, colorbg);
				}
				else {
					gDPortKRUp = 60;
				//	leds.setPixel(DPortStartLED + DPortIndex, colorbg);

				}
				DPPortLEDMillis = millis();
			}
		}
	
	FadeLEDs(DPortStartLED, DPortStartLED + numDPortLEDs , 25);
}

void DPortShortCircuit() {
	byte pixnum;
	byte i;
	if (millis() - DPPortLEDMillis > gDPortIndex) {
		// Select a random number of pixels for color 1
		pixnum = random(1, 2);
		for (i = 0; i < pixnum; i++) {
			leds.setPixel(DPortStartLED + random(0, numDPortLEDs - 1), 0xCCCCCC);
		}

		// Select a random number of pixels for color 2
		pixnum = random(1, 2);
		for (i = 0; i < pixnum; i++) {
			leds.setPixel(DPortStartLED + random(0, numDPortLEDs - 1), 0x0000CC);
		}

		pixnum = random(1, 2);
		for (i = 0; i < pixnum; i++) {
			leds.setPixel(DPortStartLED + random(0, numDPortLEDs - 1), 0xCC0000);
		}
		gDPortIndex = gDPortIndex + 2;
		FadeLEDs(DPortStartLED, DPortStartLED + numDPortLEDs - 1, 80);
		DPPortLEDMillis = millis();
	}
}

void StartDPortCardLED() {
	gDPortIndex = 0;
	gCurrentLEDDPortNumber = 3;
	gDPortKRUp = 0;
	DPPortLEDMillis = millis();
}

void DPortCard() {
	if ((millis() - DPPortLEDMillis) > 25) {
		if (gDPortIndex < 10) {
			leds.setPixel(DPortStartLED + gDPortIndex, 0xFF0000);
			leds.setPixel(DPortStartLED + numDPortLEDs - gDPortIndex, 0xFF0000);
			//	DPortIndex++;
			//	DPortKRUp++;
		}

		if (gDPortIndex < 25) {

			gDPortIndex++;
		}
		else
		{
			for (byte x = 12; x < numDPortLEDs - 12; x++) {
				leds.setPixel(DPortStartLED + x, 0xFFFFFFF);
			}
			gDPortIndex = 0;

		}
		if (gDPortKRUp < 220) gDPortKRUp++;
		else gCurrentLEDDPortNumber = 0;
		FadeLEDs(DPortStartLED, DPortStartLED + numDPortLEDs, 15);
		DPPortLEDMillis = millis();
	}
}

void DPortSiren() {
	if ((millis() - DPPortLEDMillis) > 25) {

		if (gDPortKRUp == 12) {
			for (int i = 0; i < 6; i++) {
				leds.setPixel(DPortStartLED + i, 0xDDDDDD);
			}

			for (int i = numDPortLEDs - 5; i < numDPortLEDs; i++) {
				leds.setPixel(DPortStartLED + i, 0x0000FF);
			}

		}
		if (gDPortKRUp == 24) {
			for (int i = 0; i < 6; i++) {
				leds.setPixel(DPortStartLED + i, 0x0000FF);
			}

			for (int i = numDPortLEDs - 5; i < numDPortLEDs; i++) {
				leds.setPixel(DPortStartLED + i, 0xDDDDDD);
			}
			gDPortKRUp = 1;
		}
		gDPortKRUp++;


		FadeLEDs(DPortStartLED, DPortStartLED + numDPortLEDs, 15);
		DPPortLEDMillis = millis();
	}
}

void TurnCPLEDsOff() {
	CPLEDsON = false;
}

void TurnCPLEDsOn() {
	CPLEDsON = true;
}

void TurnDPLEDsOff() {
	DPLEDsON = false;
	digitalWrite(TFT_BLK, LOW);
}

void TurnDPLEDsOn() {
	DPLEDsON = true;
	digitalWrite(TFT_BLK, HIGH);
}

void TurnDPortLEDsOff() {
	DPortLEDsON = false;
	gCurrentLEDDPortNumber = 0;
}

void TurnDPortLEDsOn() {
	DPortLEDsON = true;
	gCurrentLEDDPortNumber = 1;
}

void SetDPortLEDMode(byte mode) {
	gCurrentLEDDPortNumber = mode;
}