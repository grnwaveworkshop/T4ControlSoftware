#ifndef SERIAL485_HPP
#define SERIAL485_HPP

#include <Arduino.h>

void Setup485();
void Serialloop();
void ProcessPSI();
void SetPSIMode(byte mode);
byte GetPSIMode();
void PSIFrontOff();
void PSIFrontNextPattern();
void PSIAudioPattern();
void PSIShortCircuit();
void PSIGreenLight();
void PSIRedLight();
void PSIRadar();
void PSIRedBlueSwirl();
void PSIHeart();
void PSISiren();
void PSIConfetti();
void HolosOff();
void HolosRandom();
void HolosBright();
void SmokeOff();
void SmokeOn(uint16_t SmokeOnTime);
void PieSetServoSpeed(uint16_t ServoSpeed);
void PieOpenAll();
void PieCloseAll();
void PieOpenSequencial();
void PieCloseSequencial();
void PieOpenClose3(uint16_t NumCycles);
void PieOpenSingle(uint16_t pie_num);
void PieCloseSingle(uint16_t pie_num);
void PieOpen0();
void PieOpen1();
void PieOpen2();
void PieOpen3();
void PieOpen4();
void PieOpen5();
void PieClose0();
void PieClose1();
void PieClose2();
void PieClose3();
void PieClose4();
void PieClose5();
void StartBubbles(uint16_t Time);
void StopBubbles();
void LogicNextPattern();
void LogicNextPatternDelay(uint16_t delaytime);
void TetrisStart();
void AllDomeLEDsOn();
void TetrisLeft();
void TetrisDrop();
void TetrisRight();
void TetrisRotate();
void LogicShortCircuit();
void LogicMatrixDrop();
void AllDomeLEDsOff();

#endif