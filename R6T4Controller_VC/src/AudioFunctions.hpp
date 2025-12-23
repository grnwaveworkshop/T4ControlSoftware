#ifndef AUDIOFUNCTIONS_HPP
#define AUDIOFUNCTIONS_HPP

#include <Arduino.h>

void AudioSetup();
void AudioLoop();
void IncVolume();
void DecVolume();
void UpdateVolume();
uint8_t GetAudioFolder();
float SampleAudio();
void PlayTrackorAdvert(uint8_t Folder, uint8_t Track, uint8_t AdvertStart, uint8_t AdvertEnd);

extern uint16_t AudioLevel;
extern uint8_t FolderNum;
extern uint8_t CurrentTrack;
extern uint8_t NumTracksFolder[];

#endif