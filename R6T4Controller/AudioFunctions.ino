#include <DFPlayerMini_Fast.h>
#include "definitions.h"

#define DFSERIAL Serial8
DFPlayerMini_Fast DFPlayer;




int AudioState = 0;
int SwitchState = 0;
//char PrevAudioState = 0;

uint8_t FolderNum = 0;
uint8_t CurrentTrack = 0;
uint8_t NumFolders = 0;
uint8_t NumTracksFolder[NUMAUDIOFOLDERS];
uint16_t CurrentAudioVolume = 0;
unsigned long AudioPrevMillis = millis();
unsigned long AudioPlayTime = millis();

void AudioSetup() {
  DFSERIAL.begin(9600);
  DFPlayer.begin(DFSERIAL);
  Serial.println("Setup Audio");

  DFPlayer.wakeUp();
  DFPlayer.volume(CurrentAudioVolume);
  delay(20);
  //DisplayPrint("DFPlayer Version: ",10,100);
  //  tft.print("DFPlayer Version: ");
  //  tft.println(DFPlayer.currentVersion());
  //  tft.updateScreen();

  Serial.print("DFPlayer Version: ");
  Serial.println(DFPlayer.currentVersion());
  delay(20);
  Serial.print("Num SD Tracks ");
  Serial.println(DFPlayer.numSdTracks());

  delay(20);
  Serial.print("Current SD Track");
  Serial.println( DFPlayer.currentSdTrack());

  delay(20);
  Serial.print("Number Folders ");
  Serial.println( DFPlayer.numFolders());


  //  tft.print("Num SD Tracks ");
  //  tft.println(DFPlayer.numSdTracks());
  //  tft.print("Current SD Track");
  //  tft.println( DFPlayer.currentSdTrack());
  //  tft.print("Number Folders ");
  //  tft.println( DFPlayer.numFolders());
  //  tft.updateScreen();
  delay(1000);
  // CurrentTrack = 1;//DFPlayer.currentSdTrack();
  tft.println( "start folder query");

//Disable folder scan
#if 1
  for (uint8_t y = 1; y < DFPlayer.numFolders(); y++) {
    NumTracksFolder[y] = DFPlayer.numTracksInFolder(y);
    tft.println( NumTracksFolder[y]);
    Serial.println( NumTracksFolder[y]);
    tft.updateScreen();
    //  delay(250);
  }
#endif
  tft.println( "end folder query");
  tft.updateScreen();
  delay(2000);
}

void AudioLoop() {
  // Audio Processing Loop
  if ((millis() - AudioPrevMillis) > 50 ) {
  //  switch (AudioState) {
  //    case AudioIdle:

  //      if (!SwitchState && ChannelData(AUDIOSWITCH) > 500 && SBUSOK()  ) {
  //        CurrentTrack = random(1, NumTracksFolder[GetAudioFolder()]);
  //        DFPlayer.playFolder(GetAudioFolder(), CurrentTrack);
  //        AudioPlayTime = millis();
  //        Serial.println("play audio folder");
  //        // DFPlayer.playAdvertisement(CurrentTrack);
  //        AudioState = AudioPlaying;
  //        SwitchState = 1;
  //        DisplayAudioFolderTrack(GetAudioFolder(), CurrentTrack);

  //        Serial.println( CurrentTrack);
  //        Serial.print("in state 0 go to AudioState: ");
  //        Serial.println(AudioState);
  //        Serial.print("SwitchState: ");
  //        Serial.println(SwitchState);
  //        break;
  //      }
  //      if ((ChannelData(AUDIOSWITCH) < 500 ) && SBUSOK()) {
  //        SwitchState = 0;
  //        //          Serial.print("AudioState: ");
  //        //          Serial.println(AudioState);
  //        //          Serial.print("SwitchState: ");
  //        //          Serial.println(SwitchState);
  //      }
  //      break;
  //    case AudioPlaying:
  //      if (!SwitchState && ChannelData(AUDIOSWITCH) > 500 && SBUSOK() ) {
  //        DFPlayer.pause();
  //        Serial.println("Pause");
  //        AudioState = AudioIdle;
  //        SwitchState = 1;
  //        Serial.print("in state 1 go to AudioState: ");
  //        Serial.println(AudioState);
  //        Serial.print("SwitchState: ");
  //        Serial.println(SwitchState);
  //        break;
  //      }
  //      if ((ChannelData(AUDIOSWITCH) < 500) && SBUSOK()) {
  //        SwitchState = 0;
  //        //          Serial.print("AudioState: ");
  //        //          Serial.println(AudioState);
  //        //          Serial.print("SwitchState: ");
  //        //          Serial.println(SwitchState);
  //      }
  //      if ( (millis() - AudioPlayTime) > 250 ) {
  //        if (!DFPlayer.isPlaying()) {
  //          AudioState = AudioIdle;
  //          Serial.print("in state 1 not playing, go to AudioState: ");
  //          Serial.println(AudioState);
  //        }
  //        AudioPlayTime = millis();
  //      }
  //      break;
  //    default:
  //      // statements
  //      SwitchState = 0;
  //      break;
  //  }
  //  AudioPrevMillis = millis();
  }
}

void IncVolume() {
  DFPlayer.incVolume();
}

void DecVolume() {
  DFPlayer.decVolume();
}

void UpdateVolume() {
  if (CurrentAudioVolume != ChannelData(AudioVolume) / 60) {
    CurrentAudioVolume = ChannelData(AudioVolume) / 60;
    DFPlayer.volume(CurrentAudioVolume);
    Serial.print("AudioVolume: ");
    Serial.println(CurrentAudioVolume );
    
  }
}

uint8_t GetAudioFolder() {
  Serial.print("AudioFolder: ");
  Serial.println((ChannelData(AUDIOFOLDER) / 120) );
  return (( ChannelData(AUDIOFOLDER) / 120) );
}
