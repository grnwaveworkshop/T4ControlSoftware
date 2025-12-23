#ifndef LEDFUNCTIONS_HPP
#define LEDFUNCTIONS_HPP

#include <Arduino.h>

void LEDsetup();
void LEDloop();
void nextLEDPatternDP();
void KITTBarsAudio();
void SpectrumBarsSBUS();
void SpectrumBars(uint32_t color, int cycles, int wait);
void twinkleBankA();
void twinkleBankB(uint32_t color);
void SBUSStatusLED();
void twinkleCP(uint32_t color);
void CPVoltLEDs();
void FadeLEDs(uint8_t StartLED, uint8_t StopLED, uint16_t FadeAmount);
void DPortOff();
void DPortKnightRiderRED();
void DPortKnightRider(uint32_t colorfg, uint32_t colorbg, uint16_t wait);
void DPortShortCircuit();
void StartDPortCardLED();
void DPortCard();
void DPortSiren();
void TurnCPLEDsOff();
void TurnCPLEDsOn();
void TurnDPLEDsOff();
void TurnDPLEDsOn();
void TurnDPortLEDsOff();
void TurnDPortLEDsOn();
void SetDPortLEDMode(byte mode);

#endif