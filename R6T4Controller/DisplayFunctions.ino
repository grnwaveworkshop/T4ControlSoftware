/*

  DisplayFunctions.ino

  Functions for controlling the display

*/
#include <Adafruit_GFX.h>    // Core graphics library
#include <ST7789_t3.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "definitions.h"


//ST7789_t3 tft = ST7789_t3(TFT_CS,  TFT_DC, TFT_RST);


unsigned long DisplayPrevMillis = millis();


void DisplaySetup(void)
{

	Serial.println("Setup Display");

	// Setup and turn on Backlight
	pinMode(TFT_BLK, OUTPUT);
	digitalWrite(TFT_BLK, HIGH);

	tft.init(240, 240, SPI_MODE2);           // Init ST7789 240x240 no CS

	tft.fillScreen(ST7735_BLACK);
	tft.setRotation(3);
	tft.setTextSize(3);
	tft.setCursor(0, 0);
	tft.setTextColor(ST7735_WHITE);

	tft.setTextWrap(true);
	tft.useFrameBuffer(true);
	tft.println("Booting. . .");
	tft.updateScreen();
	//	tft.enableScroll();
	//	tft.setScrollTextArea(125, 40, 115, 200);

	tft.updateScreen();
	tft.setTextSize(2);

	//  delay(1000);
	//  tft.fillScreen(ST7735_BLACK);

	if (!SD.begin(SD_CS)) {
		tft.fillScreen(ST7735_BLACK);
		//	tft.setCursor(5, tft.height() / 2 - 6);
		tft.print("Unable to access");
		//	tft.setCursor(32, tft.height() / 2 + 6);
		tft.println("SD card");
		tft.updateScreen();
	}
	else {
		//	tft.setCursor(5, tft.height() / 2 - 6);
		tft.println("SD Access OK...");
		tft.updateScreen();
	}
	//tft.fillScreen(ST7735_BLACK);
	bmpDraw("R6O120WB.bmp", 0, 0);

	//bmpDraw("R6240WB.bmp",0,0);
	// 	   bmpDraw("R6_120.bmp",0,0);
	//delay(2000);
	tft.setCursor(70, 0);

	///*	tft.enableScroll();
	//	t*/ft.setScrollTextArea(0, 00, 115, 100);

	tft.setTextColor(ST7735_RED);
	tft.setTextSize(3);
	tft.println("R6-T28");
	tft.updateScreen();
	//tft.setScrollTextArea(125, 40, 115, 160);

	tft.setCursor(125, 40);
	tft.updateScreen();
	tft.setTextSize(2);
	tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
}

void pre(void)
{
	tft.fillScreen(ST7735_BLACK);
	bmpDraw("R6240WB.bmp", 0, 0);
	tft.setCursor(200, 0);
	tft.setTextColor(ST7735_RED);
	tft.setTextSize(3);
	tft.print("R6-T28");
	tft.setCursor(125, 40);

}



void DisplayLoop(void)
{


	//tft.setTextWrap(true);

	if (millis() - DisplayPrevMillis > 1200) {
		// delay(1500);
		//Serial.println("Display Loop");
		//    tft.setCursor(0, 75);

	//	pre();
		//if (isDataPanelOpen()) {
			tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
			//    tft.setTextSize(3);
			//   tft.println("Hello World!");
			tft.setTextSize(2);

			tft.setCursor(150, 200);
			tft.print("RT:");
			tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);
			tft.print(millis() / 1000);
			tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
			tft.println("s");
			tft.setCursor(150, 220);
			tft.setTextColor(ST7735_BLUE, ST7735_BLACK);
			tft.print(ReadInputVolts(), 2);
			// Serial.println(ReadInputVolts(), 2);
			tft.println(" V");
			tft.setCursor(125, 40);
			//tft.print(analogRead(8));
			//tft.println(" A8");
			//  tft.setScrollTextArea(50,00, 240, 160);
			//tft.setScrollTextArea(125, 40, 115, 159);
			tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
			DisplayAudioStatus();
			DisplayPrevMillis = millis();
			tft.updateScreen();
		//}
		//else {
		//	tft.fillScreen(ST7735_BLACK);
		//	tft.updateScreen();
		//}
	}

}

void DisplayPrint(char* string, uint8_t x, uint16_t y) {
	tft.setCursor(x, y);
	tft.print(string);
	tft.updateScreen();
}
void DisplayPrintln(char* string) {
	//tft.setCursor(x,y);
	tft.println(string);
	tft.updateScreen();
}

void DisplayAudioStatus() {

}

void DisplayAudioFolderTrack(uint8_t Folder, uint8_t Track) {

	tft.setTextSize(2);
	//  tft.fillRect(10, 120, 160, 20, ST7735_BLACK);

	tft.setCursor(10, 120);
	tft.setTextColor(ST7735_BLUE);
	tft.print("Playing F ");
	tft.print(Folder);
	tft.print(" T ");
	tft.println(Track);
	tft.updateScreen();

	Serial.println("finish TFT print Audio");
}

void bmpDraw(const char* filename, uint8_t x, uint16_t y) {

	File     bmpFile;
	int      bmpWidth, bmpHeight;   // W+H in pixels
	uint8_t  bmpDepth;              // Bit depth (currently must be 24)
	uint32_t bmpImageoffset;        // Start of image data in file
	uint32_t rowSize;               // Not always = bmpWidth; may have padding
	uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
	uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
	boolean  goodBmp = false;       // Set to true on valid header parse
	boolean  flip = true;        // BMP is stored bottom-to-top
	int      w, h, row, col;
	uint8_t  r, g, b;
	uint32_t pos = 0;
	//uint32_t startTime = millis();

	uint16_t awColors[320];  // hold colors for one row at a time...

	if ((x >= tft.width()) || (y >= tft.height())) return;

	//  Serial.println();
	//  Serial.print(F("Loading image '"));
	//  Serial.print(filename);
	//  Serial.println('\'');

	// Open requested file on SD card
	bmpFile = SD.open(filename);
	if (!bmpFile) {
		Serial.print("File not found");
		tft.fillScreen(ST7735_BLACK);
		tft.setCursor(12, tft.height() / 2 - 12);
		tft.print("Unable to");
		tft.setCursor(12, tft.height() / 2 - 0);
		tft.print("read file: ");
		tft.setCursor(12, tft.height() / 2 + 12);
		tft.setTextColor(ST7735_YELLOW);
		tft.print(filename);
		return;
	}

	// Parse BMP header
	if (read16(bmpFile) == 0x4D42) { // BMP signature
	  //Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
		read32(bmpFile);  // files size
		(void)read32(bmpFile); // Read & ignore creator bytes
		bmpImageoffset = read32(bmpFile); // Start of image data
		//  Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
		// Read DIB header
		//   Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
		read32(bmpFile);  // header size
		bmpWidth = read32(bmpFile);
		bmpHeight = read32(bmpFile);
		if (read16(bmpFile) == 1) { // # planes -- must be '1'
			bmpDepth = read16(bmpFile); // bits per pixel
			//   Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
			if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

				goodBmp = true; // Supported BMP format -- proceed!
				//      Serial.print(F("Image size: "));
				//      Serial.print(bmpWidth);
				//      Serial.print('x');
				//      Serial.println(bmpHeight);

				// BMP rows are padded (if needed) to 4-byte boundary
				rowSize = (bmpWidth * 3 + 3) & ~3;

				// If bmpHeight is negative, image is in top-down order.
				// This is not canon but has been observed in the wild.
				if (bmpHeight < 0) {
					bmpHeight = -bmpHeight;
					flip = false;
				}

				// Crop area to be loaded
				w = bmpWidth;
				h = bmpHeight;
				if ((x + w - 1) >= tft.width())  w = tft.width() - x;
				if ((y + h - 1) >= tft.height()) h = tft.height() - y;

				for (row = 0; row < h; row++) { // For each scanline...

				  // Seek to start of scan line.  It might seem labor-
				  // intensive to be doing this on every line, but this
				  // method covers a lot of gritty details like cropping
				  // and scanline padding.  Also, the seek only takes
				  // place if the file position actually needs to change
				  // (avoids a lot of cluster math in SD library).
					if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
						pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
					else     // Bitmap is stored top-to-bottom
						pos = bmpImageoffset + row * rowSize;
					if (bmpFile.position() != pos) { // Need seek?
						bmpFile.seek(pos);
						buffidx = sizeof(sdbuffer); // Force buffer reload
					}

					for (col = 0; col < w; col++) { // For each pixel...
					  // Time to read more pixel data?
						if (buffidx >= sizeof(sdbuffer)) { // Indeed
							bmpFile.read(sdbuffer, sizeof(sdbuffer));
							buffidx = 0; // Set index to beginning
						}

						// Convert pixel from BMP to TFT format, push to display
						b = sdbuffer[buffidx++];
						g = sdbuffer[buffidx++];
						r = sdbuffer[buffidx++];
						awColors[col] = tft.Color565(r, g, b);
					} // end pixel
					tft.writeRect(0, row, w, 1, awColors);
				} // end scanline
				//        Serial.print(F("Loaded in "));
				//        Serial.print(millis() - startTime);
				//        Serial.println(" ms");
			} // end goodBmp
		}
	}

	bmpFile.close();
	if (!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
	uint16_t result;
	((uint8_t*)&result)[0] = f.read(); // LSB
	((uint8_t*)&result)[1] = f.read(); // MSB
	return result;
}

uint32_t read32(File f) {
	uint32_t result;
	((uint8_t*)&result)[0] = f.read(); // LSB
	((uint8_t*)&result)[1] = f.read();
	((uint8_t*)&result)[2] = f.read();
	((uint8_t*)&result)[3] = f.read(); // MSB
	return result;
}



void testfastlines(uint16_t color1, uint16_t color2) {
	tft.fillScreen(ST7735_BLACK);
	for (int16_t y = 0; y < tft.height(); y += 5) {
		tft.drawFastHLine(0, y, tft.width(), color1);
	}
	for (int16_t x = 0; x < tft.width(); x += 5) {
		tft.drawFastVLine(x, 0, tft.height(), color2);
	}
}

void testdrawrects(uint16_t color) {
	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color);
	}
}

void testfillrects(uint16_t color1, uint16_t color2) {
	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
		tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color1);
		tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color2);
	}
}

void testfillcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = radius; x < tft.width(); x += radius * 2) {
		for (int16_t y = radius; y < tft.height(); y += radius * 2) {
			tft.fillCircle(x, y, radius, color);
		}
	}
}

void testdrawcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
		for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
			tft.drawCircle(x, y, radius, color);
		}
	}
}

void testtriangles() {
	tft.fillScreen(ST7735_BLACK);
	int color = 0xF800;
	int t;
	int w = tft.width() / 2;
	int x = tft.height() - 1;
	int y = 0;
	int z = tft.width();
	for (t = 0; t <= 15; t += 1) {
		tft.drawTriangle(w, y, y, x, z, x, color);
		x -= 4;
		y += 4;
		z -= 4;
		color += 100;
	}
}

void testroundrects() {
	tft.fillScreen(ST7735_BLACK);
	int color = 100;
	int i;
	int t;
	for (t = 0; t <= 4; t += 1) {
		int x = 0;
		int y = 0;
		int w = tft.width() - 2;
		int h = tft.height() - 2;
		for (i = 0; i <= 16; i += 1) {
			tft.drawRoundRect(x, y, w, h, 5, color);
			x += 2;
			y += 3;
			w -= 4;
			h -= 6;
			color += 1100;
		}
		color += 100;
	}
}

void tftPrintTest() {
	tft.setTextWrap(false);
	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(0, 30);
	tft.setTextColor(ST7735_RED);
	tft.setTextSize(1);
	tft.println("Hello World!");
	tft.setTextColor(ST7735_YELLOW);
	tft.setTextSize(2);
	tft.println("Hello World!");
	tft.setTextColor(ST7735_GREEN);
	tft.setTextSize(3);
	tft.println("Hello World!");
	tft.setTextColor(ST7735_BLUE);
	tft.setTextSize(4);
	tft.print(1234.567);
	delay(1500);
	tft.setCursor(0, 0);
	tft.fillScreen(ST7735_BLACK);
	tft.setTextColor(ST7735_WHITE);
	tft.setTextSize(0);
	tft.println("Hello World!");
	tft.setTextSize(1);
	tft.setTextColor(ST7735_GREEN);
	tft.print(3.141593245, 6);
	tft.println(" Want pi?");
	tft.println(" ");
	tft.print(8675309, HEX); // print 8,675,309 out in HEX!
	tft.println(" Print HEX!");
	tft.println(" ");
	tft.setTextColor(ST7735_WHITE);
	tft.println("Sketch has been");
	tft.println("running for: ");
	tft.setTextColor(ST7735_MAGENTA);
	tft.print(millis() / 1000);
	tft.setTextColor(ST7735_WHITE);
	tft.print(" seconds.");
}

void mediabuttons() {
	// play
	tft.fillScreen(ST7735_BLACK);
	tft.fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
	tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
	delay(500);
	// pause
	tft.fillRoundRect(25, 90, 78, 60, 8, ST7735_WHITE);
	tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_GREEN);
	tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_GREEN);
	delay(500);
	// play color
	tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
	delay(50);
	// pause color
	tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_RED);
	tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_RED);
	// play color
	tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
}


void testlines(uint16_t color) {
	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(0, 0, x, tft.height() - 1, color);
		delay(200);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(0, 0, tft.width() - 1, y, color);
	}

	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(tft.width() - 1, 0, 0, y, color);
	}

	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(0, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
	}

	tft.fillScreen(ST7735_BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
	}
}
