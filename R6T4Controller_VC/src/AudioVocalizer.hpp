#ifndef AUDIOVOCALIZER_HPP
#define AUDIOVOCALIZER_HPP

#include <Arduino.h>

void VocalizerSetup();
void VocalizerAudioLoop();
void UpdateMasterVolume();
void PlayHappy();
void PlayHappyMild();
void PlayHappyExtreme();
void PlaySadMild();
void PlaySadExtreme();
void PlayMadMild();
void PlayMadExtreme();
void PlayScaredMild();
void PlayScaredExtreme();
void PlayElectrocute();
void PlayMuse();
void MuseEnable();
void MuseDisable();
void PlayWavA();
void PlayWavA(int num);
void PlayNextWavA();
void PlayWavB(int num);
void VocalAllStop();
void UpdateVocalVolume(int Volume);
void UpdateWavAVolume(int Volume);
void UpdateWavBVolume(int Volume);
void VocalUpdateMasterVolume(int Volume);
void VocalSerialTransmit(String text);
void WavAisPlaying();
void CheckAudioSerial();
void ParseData(char ReceivedChar[32]);

extern uint16_t gCurrentWavTrack;
extern bool gAutoPlayWav;
extern const uint16_t MaxWavTracks;

#endif