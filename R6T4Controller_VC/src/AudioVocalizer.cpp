#include "AudioVocalizer.hpp"

// Functions to talk to Human Cyborg Relations Vocalizer

#include "Definitions.h"
#include <Arduino.h>
#include "SBUSFunctions.hpp"
#include "AudioFunctions.hpp"

#define VOCALSERIAL Serial3


uint8_t VocalAudioState = AUDIOMANUAL;
unsigned long VocalAudioAutoMillis = millis();
unsigned long VocalAudioAutoWait = 3000;

uint16_t gCurrentWavTrack = 1;
const uint16_t MaxWavTracks = 24;
uint16_t gCurrentVocalAudioVolume = 8;
uint16_t gCurrentWavAAudioVolume = 6;
uint16_t gCurrentWavBAudioVolume = 6;
uint16_t gCurrentMasterVolume = 5;	// 0 to 9 range;
unsigned long AudioVocalPrevMillis = millis();
unsigned long AudioVocalPlayTime = millis();
bool MuseEnabled = 0;
bool gAutoPlayWav = 0;
int gWavAisPlaying = 0;
int gWavAFinished = 0;


//Serial
char gAudio_rx_byte = 0;
const uint8_t maxDataLength = 30;  // maxDataLength is the maximum length allowed for received data.
char receivedChars[31];
String RxString = "";
bool newData = false;		// newData is used to determine if there is a new command
bool recvInProgress = false;
uint8_t ndx = 0;
const char startMarker = '<';
const char endMarker = '>';


void VocalizerSetup() {
	VOCALSERIAL.begin(115200);

	Serial.println("Setup Vocalizer");

	Serial.print("Current Vocal Volume: ");
	Serial.println(gCurrentVocalAudioVolume);


	VOCALSERIAL.println("Vocalizer start");

	PlayHappy();

	VocalUpdateMasterVolume(gCurrentMasterVolume);
	UpdateVocalVolume(gCurrentVocalAudioVolume);
	UpdateWavAVolume(gCurrentWavAAudioVolume);
	UpdateWavBVolume(gCurrentWavBAudioVolume);

}

void VocalizerAudioLoop() {
	// Audio Processing Loop
	CheckAudioSerial();
if ((millis() - AudioVocalPrevMillis) > 400) {

AudioVocalPrevMillis = millis();


		// Auto Mode Audio
		if (ChannelData(AUDIOMODE) > 1500) {
			if (millis() - VocalAudioAutoMillis > VocalAudioAutoWait) {
				MuseEnabled = 1;
				MuseEnable();
				PlayMuse();
				VocalAudioAutoMillis = millis();
				VocalAudioAutoWait = random(3500, 15000);
			}
		}
		else if (MuseEnabled) {
			MuseEnable();
			MuseEnabled = 0;
		}

		if (gAutoPlayWav) {
			WavAisPlaying();
			// if no wave is playing - play a file.
			if (!gWavAisPlaying) {
				if (!gWavAFinished) { // Previous one finished
					PlayNextWavA();
					gWavAFinished = 1;
				}
			}
			else gWavAFinished = 0;
		}
	}
}

//void IncVolume() {
//	DFPlayer.volumeUp();
//}
//
//void DecVolume() {
//	DFPlayer.volumeDown();
//}

void UpdateMasterVolume() {
	uint16_t volume;
	if (gCurrentMasterVolume != ChannelData(AUDIOVOLUME)) {
		gCurrentMasterVolume = ChannelData(AUDIOVOLUME);
		if (gCurrentMasterVolume >= 175)	volume = map(gCurrentMasterVolume, 175, 1812, 1, 99);
		else volume = 0;
		//VocalUpdateMasterVolume(volume);
		Serial.print("MasterVolume: ");
		Serial.println(volume);
	}
	if (ChannelData(VOCALVOLUME) >= 175) volume = map(ChannelData(VOCALVOLUME), 175, 1812, 1, 99);
	else volume = 0;
	if (gCurrentVocalAudioVolume != volume) {
		gCurrentVocalAudioVolume = volume;
		//volume = map(CurrentVocalAudioVolume, 150, 1850, 0, 9);
		Serial.print("CH: ");
		Serial.print(ChannelData(VOCALVOLUME));
		Serial.print("VocalVolume: ");
		Serial.println(volume);
		UpdateVocalVolume(volume);
		UpdateWavBVolume(volume);
	}
	if (ChannelData(WAVVOLUME) >= 175) volume = map(ChannelData(WAVVOLUME), 175, 1812, 1, 99);
	else volume = 0;
	if (gCurrentWavAAudioVolume != volume) {
		gCurrentWavAAudioVolume = volume;
		Serial.print("CH: ");
		Serial.print(ChannelData(WAVVOLUME));
		Serial.print(" WavAVolume: ");
		Serial.println(volume);
		UpdateWavAVolume(volume);

	}
}


void PlayHappy() {
	bool extreme = random(0, 2);
	if (extreme) {
		VocalSerialTransmit("SH1");
		Serial.println("Play Happy Extreme");
	}
	else {
		VocalSerialTransmit("SH0");
		Serial.println("Play Happy Mild");
	}
}

void PlayHappyMild() {
	VocalSerialTransmit("SH0");
}
void PlayHappyExtreme() {
	VocalSerialTransmit("SH1");
}

void PlaySadMild() {
	VocalSerialTransmit("SS0");
	Serial.println("Play Sad Mild");
}
void PlaySadExtreme() {
	VocalSerialTransmit("SS1");
	Serial.println("Play Sad Extreme");
}

void PlayMadMild() {
	VocalSerialTransmit("SM0");
	Serial.println("Play Mad Mild");
}
void PlayMadExtreme() {
	VocalSerialTransmit("SM1");
	Serial.println("Play Mad Extreme");
}

void PlayScaredMild() {
	VocalSerialTransmit("SC0");
	Serial.println("Play Scared Mild");
}

void PlayScaredExtreme() {
	VocalSerialTransmit("SC1");
	Serial.println("Play Scared Extreme");
}

void PlayElectrocute() {
	VocalSerialTransmit("SE");
	Serial.println("Play Electrocute");
}

void PlayMuse() {
	VocalSerialTransmit("MT");
	Serial.println("Play Muse");
}

void MuseEnable() {
	VocalSerialTransmit("M1");
	Serial.println("Enable Muse");
}

void MuseDisable() {
	VocalSerialTransmit("M0");
	Serial.println("Disable Muse");
}

// Play a Random wav file on Bank A
void PlayWavA() {
	//CurrentWavTrack = random(1, 18);
	VOCALSERIAL.print("<CA");
	VOCALSERIAL.printf("%04d", gCurrentWavTrack);
	Serial.printf("%04d", gCurrentWavTrack);
	VOCALSERIAL.print(">");
}

void PlayWavA(int num) {
	gCurrentWavTrack = num;
	VOCALSERIAL.print("<CA");
	VOCALSERIAL.printf("%04d", gCurrentWavTrack);
	Serial.printf("%04d", gCurrentWavTrack);
	VOCALSERIAL.print(">");
}

void PlayNextWavA() {
	if (gCurrentWavTrack < MaxWavTracks) gCurrentWavTrack++;
	else gCurrentWavTrack = 1;
	VOCALSERIAL.print("<CA");
	VOCALSERIAL.printf("%04d", gCurrentWavTrack);
	Serial.printf("%04d", gCurrentWavTrack);
	VOCALSERIAL.print(">");
}

void PlayWavB(int num) {
	gCurrentWavTrack = num;
	VOCALSERIAL.print("<CB");
	VOCALSERIAL.printf("%04d", gCurrentWavTrack);
	Serial.printf("%04d", gCurrentWavTrack);
	VOCALSERIAL.print(">");
}

// Send all Stop command
void VocalAllStop() {
	VocalSerialTransmit("PSX");
}

void UpdateVocalVolume(int Volume) {

	VOCALSERIAL.print("<PVV");
	VOCALSERIAL.printf("%02d", Volume);
	VOCALSERIAL.print(">");
}
void UpdateWavAVolume(int Volume) {

	VOCALSERIAL.print("<PVA");
	VOCALSERIAL.printf("%02d", Volume);
	VOCALSERIAL.print(">");
}
void UpdateWavBVolume(int Volume) {

	VOCALSERIAL.print("<PVB");
	VOCALSERIAL.printf("%02d", Volume);
	VOCALSERIAL.print(">");
}

// Update Master Volume
void VocalUpdateMasterVolume(int Volume) {

	VOCALSERIAL.print("<PVM");
	VOCALSERIAL.print(Volume);
	VOCALSERIAL.print(">");
}

void VocalSerialTransmit(String text)
{
	//sendCache = text;
	VOCALSERIAL.println(("<" + text + ">").c_str());
}


void WavAisPlaying()
{
	VOCALSERIAL.print("<QPA>");
	
}
void CheckAudioSerial()
{
	// Process all available bytes in buffer for better throughput
	while (VOCALSERIAL.available() > 0) {
		gAudio_rx_byte = VOCALSERIAL.read();
		
		if (recvInProgress)
		{
			if (gAudio_rx_byte != endMarker)
			{
				if (ndx < maxDataLength) {
					receivedChars[ndx++] = gAudio_rx_byte;
					RxString += gAudio_rx_byte;
				} else {
					// Buffer full - reset to prevent overflow
					recvInProgress = false;
					ndx = 0;
					RxString = "";
				}
			}
			else
			{
				receivedChars[ndx] = '\0'; // terminate the string
				recvInProgress = false;
				ndx = 0;
				newData = true;
				break; // Process this message before reading more
			}
		}
		else if (gAudio_rx_byte == startMarker)
		{
			recvInProgress = true;
			ndx = 0; // Reset index for new message
			RxString = ""; // Clear old data
		}
	}
	
	if (newData)
	{
		ParseData(receivedChars);
		RxString = "";
		newData = false;
	}

	//	SerialMillis = millis();
	//}
}



void ParseData(char ReceivedChar[32]) {
	char Command;
	char Data1[6] = "0";
	char Data2[6] = "0";
	char Data3[6] = "0";

int D1 = 0;
int D2 = 0;
	int D3 = 0;

	uint8_t index = 0;
	uint8_t index2 = 0;
	uint8_t index3 = 0;

	//Serial485.print("Received Char ");
//	Serial.println(ReceivedChar);

	Command = ReceivedChar[0];
	//	Serial.print("Command ");
	//	Serial.println(Command);

		// Find first ','
	while ((ReceivedChar[index] != '\0') && (ReceivedChar[index] != ',') && index < 32) {
		index++;
		index2 = index + 1;
	}
	// Find second ','
	while ((ReceivedChar[index2] != '\0') && ReceivedChar[index2] != ',' && index2 < 32) {
		index2++;
		index3 = index2 + 1;
	}

	// Find third ','
	while ((ReceivedChar[index3] != '\0') && ReceivedChar[index3] != ',' && index3 < 32) {
		index3++;
	}
	// Copy data1 with bounds checking
	for (uint8_t x = 1; x < index && (x - 1) < 5; x++) {
		Data1[x - 1] = ReceivedChar[x];
	}
	Data1[5] = '\0'; // Ensure null termination

	// Copy data2 with bounds checking
	for (uint8_t x = index + 1; x < index2 && (x - index - 1) < 5; x++) {
		Data2[x - index - 1] = ReceivedChar[x];
	}
	Data2[5] = '\0'; // Ensure null termination

	// Copy data3 with bounds checking
	for (uint8_t x = index2 + 1; x < index3 && (x - index2 - 1) < 5; x++) {
		Data3[x - index2 - 1] = ReceivedChar[x];
	}
	Data3[5] = '\0'; // Ensure null termination




	D1 = atoi(Data1);

	D2 = atoi(Data2);

	D3 = atoi(Data3);
#if 0	
	Serial.print("Data1: ");
	Serial.print(Data1);


	Serial.print(" Data2: ");
	Serial.print(Data2);

	Serial.print(" Data3: ");
	Serial.println(Data3);

	Serial.print(" D1: ");
	Serial.print(D1);
	Serial.print(" D2: ");
	Serial.print(D2);
	Serial.print(" D3: ");
	Serial.println(D3);
#endif
	switch (Command)
	{
	case 'Q': //Mode
		if (ReceivedChar[1] == 'V') {
			if (ReceivedChar[2] == 'P') {
				Serial.print("QVP - ");
				Serial.println(D2);
				AudioLevel = D2;
			}
			
		}
		if (ReceivedChar[1] == 'P') {
			if (ReceivedChar[2] == 'A') {
				Serial.print("QPA - ");
				Serial.println(D2);
				if (D2 > 0) gWavAisPlaying = 1;
				else gWavAisPlaying = 0;
			}
		}

		break;


	default:
		break;
	}


}