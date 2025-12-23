#ifndef DISPLAYFUNCTIONS_HPP
#define DISPLAYFUNCTIONS_HPP

#include <Arduino.h>
#include <SD.h>

void DisplaySetup(void);
void pre(void);
void DisplayLoop(void);
void DisplayPrint(char* string, uint8_t x, uint16_t y);
void DisplayPrintln(char* string);
void DisplayAudioStatus();
void DisplayAudioFolderTrack(uint8_t Folder, uint8_t Track);
void bmpDraw(const char* filename, uint8_t x, uint16_t y);
uint16_t read16(File f);
uint32_t read32(File f);
void testfastlines(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testtriangles();
void testroundrects();
void tftPrintTest();
void mediabuttons();
void testlines(uint16_t color);

#endif