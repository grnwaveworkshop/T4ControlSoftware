//#include <DFPlayerMini_Fast.h>
// Requires DFRobotDFPlayerMini Library https://github.com/DFRobot/DFRobotDFPlayerMini
#include "DFRobotDFPlayerMini.h"
#include "definitions.h"

#define DFSERIAL Serial8
//DFPlayerMini_Fast DFPlayer;

DFRobotDFPlayerMini DFPlayer;


uint8_t AudioState = AUDIOMANUAL;
unsigned long AudioAutoMillis = millis();
unsigned long AudioAutoWait = 3000;

//int SwitchState = 0;
//char PrevAudioState = 0;

uint8_t FolderNum = 0;
uint8_t CurrentTrack = 0;
uint8_t NumFolders = 0;
uint8_t NumTracksFolder[NUMAUDIOFOLDERS + 1];
uint16_t CurrentAudioVolume = 10;
unsigned long AudioPrevMillis = millis();
unsigned long AudioPlayTime = millis();

const uint8_t AutoAudioFolderList[5] = { 2, 5, 6, 8, 9 };

int audiomax = 0;
int audiomin = 1500;
int audiomid = 505;

uint16_t AudioLevel = 0;

void AudioSetup() {
	DFSERIAL.begin(9600);
	DFPlayer.begin(DFSERIAL);
	Serial.println("Setup Audio");

	DFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

	//DFPlayer.reset();
	DFPlayer.volume(CurrentAudioVolume);
	DFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

	Serial.print("Current Volume: ");
	Serial.println(CurrentAudioVolume);
	delay(2000);
	//DisplayPrint("DFPlayer Version: ",10,100);
	//  tft.print("DFPlayer Version: ");
	//  tft.println(DFPlayer.currentVersion());
	//  tft.updateScreen();

	Serial.print("DFPlayer State: ");
	Serial.println(DFPlayer.readState()); //read mp3 state

	//Serial.print("DFPlayer Version: ");
	//Serial.println(DFPlayer.currentVersion());
	//delay(200);
	Serial.print("Num SD Tracks ");
	Serial.println(DFPlayer.readFileCounts());


	// NumFolders = DFPlayer.numFolders();
	//   Serial.print("Number Folders: ");
	//   Serial.println( NumFolders);

	// delay(1000);
	 // CurrentTrack = 1;//DFPlayer.currentSdTrack();
	Serial.println("start folder query");

	//Disable folder scan
#if 1
	for (uint8_t y = 1; y <= NUMAUDIOFOLDERS; y++) {
		NumTracksFolder[y] = DFPlayer.readFileCountsInFolder(y);
		// tft.println( NumTracksFolder[y]);
		Serial.print("Folder: ");
		Serial.print(y);
		Serial.print("  Folder Tracks: ");
		Serial.println(NumTracksFolder[y]);
		// tft.updateScreen();
		delay(50);
	}
#endif

}

void AudioLoop() {
	// Audio Processing Loop
	if ((millis() - AudioPrevMillis) > 5) {

		//  AudioPrevMillis = millis();
		SampleAudio();

		// Auto Mode Audio
		if (ChannelData(AUDIOMODE) > 500) {
			if (millis() - AudioAutoMillis  > AudioAutoWait) {
				uint8_t RandomFolder = AutoAudioFolderList[random(0, 4)];
				CurrentTrack = random(1, NumTracksFolder[RandomFolder]);
				DFPlayer.playFolder(RandomFolder, CurrentTrack);
				AudioAutoMillis = millis();
				AudioAutoWait = random(3500, 15000);
			}

		}
	}


}

void IncVolume() {
	DFPlayer.volumeUp();
}

void DecVolume() {
	DFPlayer.volumeDown();
}

void UpdateVolume() {
	if (CurrentAudioVolume != ChannelData(AUDIOVOLUME) / 60) {
		CurrentAudioVolume = ChannelData(AUDIOVOLUME) / 60;
		DFPlayer.volume(CurrentAudioVolume);
		Serial.print("AudioVolume: ");
		Serial.println(CurrentAudioVolume);
		tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
		tft.setCursor(150, 75);
		tft.print("Vol: ");
		tft.print(CurrentAudioVolume);
		tft.print(" ");
		tft.updateScreen();
	}
}

uint8_t GetAudioFolder() {
	Serial.print("AudioFolder: ");
	Serial.println((ChannelData(AUDIOFOLDER) / 120));
	return ((ChannelData(AUDIOFOLDER) / 120));
}

float SampleAudio() {
	int audio = (analogRead(AUDIN));
	//  Serial.println(audio);
	if (audio > audiomax) {
		audiomax = audio;
	}
	// decay audio max back to midpoint
	if (audiomax > audiomid) audiomax--;

	if (audio < audiomin) {
		audiomin = audio;
	}
	// decay audio min back to mid point
	if (audiomin < audiomid) audiomin++;


	//int audiomid = 505;//(audiomax + audiomin) / 2;

	//int level = (audiomax - audiomid) * 8 * abs(audio - audiomid);
	/*Serial.printf("audiomax: %d",audiomax);
	Serial.printf("level: %d", level);
	Serial.println("");*/
	float level = (-0.35 * CurrentAudioVolume + 10.7) * abs(audio - audiomid);
	AudioLevel = map(level, 0, audiomid, 0, 4);

	return level;
}

void PlayTrackorAdvert(uint8_t Folder, uint8_t Track, uint8_t AdvertStart, uint8_t AdvertEnd) {
	int state = DFPlayer.readState();
	Serial.println(state);
	state = DFPlayer.readState();
	Serial.println(state);
	while (state < 0) {
		state = DFPlayer.readState();
		Serial.println(state);
		
	}

	if (state != 513) {
		
		if (!Track) {
			CurrentTrack = random(1, NumTracksFolder[Folder]);
			Serial.print("play track ");
			Serial.println(CurrentTrack);
		}
		else {
			CurrentTrack = Track;
			Serial.print("play track ");
			Serial.println(CurrentTrack);
		}
		DFPlayer.playFolder(Folder, CurrentTrack);
		DFPlayer.playFolder(Folder, CurrentTrack);
	}
	else {
	
		CurrentTrack = random(AdvertStart, AdvertEnd);
		Serial.print("play advertise ");
		Serial.println(CurrentTrack);
		DFPlayer.advertise(CurrentTrack);
		DFPlayer.advertise(CurrentTrack);
	}
}