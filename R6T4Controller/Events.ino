// Radio Event triggering
// Mapping radio switches etc. to functions.
//  Requires Scheduler Library.  Modified Library by TZ

#include "definitions.h"
#include <Scheduler.h>

uint8_t RunMode = 0;
uint16_t ButtonState = 0;
bool ButtonPressed = false;	// Button Pressed indicator for button bank
bool ButtonPressedTR = false;	// Button Pressed indicator for Trim Rud
bool ButtonLongPressed = false;
bool TetrisMode = false;
bool ScaredOpen = false;

int ButtonCount = 0;	// Counter for Button Bank
int ButtonCountTR = 0;	// Counter for Trim Rud

extern bool bDataDoorOpen;
extern bool bUtilityOpen;
extern uint8_t DataPanelDoorState;

unsigned long EventMillis = millis();

Scheduler scheduler = Scheduler();      //create a scheduler
//typedef void *func;
//void CheckLongPress(void (*)());

void ProcessEventsLoop() {
	if (millis() - EventMillis > 100) {
		ProcessButtons();
		UpdateVolume();
		UpdateMasterVolume();
		EventMillis = millis();
	}
	scheduler.update();                 //update the scheduler, maybe it is time to execute a function?
}


// Function to read the button channel, and assign a button # to it based on the nominal value + a buffer either side.
uint16_t GetButtonState() {
	//Serial.println(ChannelData(ButtonSwitch));
	return ChannelData(BUTTONSWITCH);
}

// Function to check the button, and active Bank A or Bank B function
void CheckButtonPress(void (*funcA)(), void (*funcB)()) {
	// Check if first time button pressed.
	if (!ButtonPressed  ) {
		// Check which bank to activate
		if (ChannelData(BUTTONBANK) < 1000) 	funcA();
		else funcB();
		if(!TetrisMode)	ButtonPressed = 1;
	}
}

void CheckLongPress(void (*func)(), bool continuous) {

	ButtonCount++;
	if (ButtonCount >= LongPressCount && !ButtonLongPressed) {
		func();
		if (!continuous) ButtonLongPressed = true;
	}
}

// Function to determine which button was pressed and run the function associated with the click
void ProcessButtons() {
	// Check button bank presses
	switch (GetButtonState()) {
		case (ButtonOff - ButtonDeadband) ... (ButtonOff + ButtonDeadband) :
			ButtonPressed = 0;
			ButtonLongPressed = 0;
			ButtonCount = 0;
			 //     Serial.println("Buttons Off");
			break;
			case (CButtonA1 - ButtonDeadband) ... (CButtonA1 + ButtonDeadband) :
				CheckButtonPress(ButtonA1Click, ButtonA1Click);
				CheckLongPress(ButtonA1LongClick, 0);
				break;
				case (CButtonA2 - ButtonDeadband) ... (CButtonA2 + ButtonDeadband) :
					CheckButtonPress(ButtonA2Click, ButtonA2ClickB);
					CheckLongPress(ButtonA2LongClick, 0);
					break;
				case (CButtonA3 - ButtonDeadband) ... (CButtonA3 + ButtonDeadband) :
					CheckButtonPress(ButtonA3Click, ButtonA3ClickB);
					break;
				case (CButtonA4 - ButtonDeadband) ... (CButtonA4 + ButtonDeadband) :
					CheckButtonPress(ButtonA4Click, ButtonA4ClickB);
					break;
				case (CButtonB1 - ButtonDeadband) ... (CButtonB1 + ButtonDeadband) :
					CheckButtonPress(ButtonB1Click, ButtonB1ClickB);
					break;
				case (CButtonB2 - ButtonDeadband) ... (CButtonB2 + ButtonDeadband) :
					CheckButtonPress(ButtonB2Click, ButtonB2ClickB);
					break;
				case (CButtonB3 - ButtonDeadband) ... (CButtonB3 + ButtonDeadband) :
					CheckButtonPress(ButtonB3Click, ButtonB3ClickB);
					break;
				case (CButtonB4 - ButtonDeadband) ... (CButtonB4 + ButtonDeadband) :
					CheckButtonPress(ButtonB4Click, ButtonB4ClickB);
					CheckLongPress(ButtonB4LongClick, 0);
					break;
				case (CButtonC1 - ButtonDeadband) ... (CButtonC1 + ButtonDeadband) :
					CheckButtonPress(ButtonC1Click, ButtonC1ClickB);
					CheckLongPress(ButtonC1LongClick, 1);
					break;
				case (CButtonC2 - ButtonDeadband) ... (CButtonC2 + ButtonDeadband) :
					CheckButtonPress(ButtonC2Click, ButtonC2ClickB);
					break;
				case (CButtonC3 - ButtonDeadband) ... (CButtonC3 + ButtonDeadband) :
					CheckButtonPress(ButtonC3Click, ButtonC3ClickB);
					break;
				case (CButtonC4 - ButtonDeadband) ... (CButtonC4 + ButtonDeadband) :
					CheckButtonPress(ButtonC4Click, ButtonC4ClickB);
					CheckLongPress(ButtonC4LongClick, 0);
					break;
				case (CButtonD1 - ButtonDeadband) ... (CButtonD1 + ButtonDeadband) :
					CheckButtonPress(ButtonD1Click, ButtonD1ClickB);
					CheckLongPress(ButtonD1LongClick, 0);
					break;
				case (CButtonD2 - ButtonDeadband) ... (CButtonD2 + ButtonDeadband) :
					CheckButtonPress(ButtonD2Click, ButtonD2ClickB);
					break;
				case (CButtonD3 - ButtonDeadband) ... (CButtonD3 + ButtonDeadband) :
					CheckButtonPress(ButtonD3Click, ButtonD3ClickB);
					CheckLongPress(ButtonD3LongClick, 0);
					break;
				case (CButtonD4 - ButtonDeadband) ... (CButtonD4 + ButtonDeadband) :
					CheckButtonPress(ButtonD4Click, ButtonD4ClickB);
					CheckLongPress(ButtonD4LongClick, 0);
					break;
				default:
					break;
	}

	// Check Trim Rud Button
	if (ChannelData(CDCHANNEL) > 1200) {
		ButtonCountTR++;
		if (ButtonCountTR > LongPressCount-4) DispenseCard();
	}
	else ButtonCountTR = 0;
}



void ButtonA1Click() {
	//DFPlayer.pause();
	scheduler.stopall();
	StopAllServoSequences();
	VocalAllStop();
	gAutoPlayWav = 0;
	StopBubbles();
}

void ButtonA1LongClick() {
	Serial.println("Button A1 Long Click");
	// Home all functions
	ServosAllClosed();
	AllDomeLEDsOff();
	TurnDPLEDsOff();
	TurnCPLEDsOff();
	TurnDPortLEDsOff();
	VocalAllStop();
	gAutoPlayWav = 0;
}

void ButtonA2Click() {
	Serial.println("Button A2 Click");
	gAutoPlayWav = 0;
	FolderNum = 1;
	//CurrentTrack = random(1, NumTracksFolder[FolderNum]);
	//DFPlayer.playFolder(FolderNum, CurrentTrack);
	gCurrentWavTrack = random(1, MaxWavTracks + 1);
	PlayWavA();
}

void ButtonA2ClickB() {
	//if (CurrentTrack < NumTracksFolder[1]) CurrentTrack++;
	//else CurrentTrack = 1;
	//DFPlayer.playFolder(1, CurrentTrack);
	PlayNextWavA();
	//DFPlayer.next();
}

void ButtonA2LongClick() {

	//CurrentTrack = random(1, NumTracksFolder[1]);
	//DFPlayer.loopFolder(1);
	gAutoPlayWav = 1;
}

void ButtonA3Click() {
	//CurrentTrack = random(1, NumTracksFolder[2]);
	//DFPlayer.playFolder(2, CurrentTrack);

	PlayHappy();
}

void ButtonA3ClickB() {
	// Play Imperial March
	CurrentTrack = 2;
	//DFPlayer.playFolder(1, CurrentTrack);
	PlayWavA(2);
}

void ButtonA4Click() {
	//CurrentTrack = random(1, NumTracksFolder[3]);
	//DFPlayer.playFolder(3, CurrentTrack);
}

void ButtonA4ClickB() {

	//Serial.println(DFPlayer.readState());
	//delay(200);
	//PlayTrackorAdvert(11, 4, 6, 6);
	PlayWavB(2004);
	//DFPlayer.advertise(6);	// No Thank you
}

void ButtonB1Click() {
	//CurrentTrack = random(1, NumTracksFolder[4]);
	//DFPlayer.playFolder(4, CurrentTrack);
	PlayMadMild();
}

void ButtonB1ClickB() {
	SetPSIMode(8);  //Siren
	SetDPortLEDMode(4);
	PlayWavB(3002);
	HolosBright();
	LogicShortCircuit();
}

void ButtonB2Click() {
	//CurrentTrack = random(1, NumTracksFolder[5]);
	//DFPlayer.playFolder(5, CurrentTrack);
	PlaySadMild();
}

void ButtonB2ClickB() {
	//PieOpenSequencial();
	PieOpenAll();
	StartBubbles(3000);
	scheduler.schedule(PieCloseSequencial, 3000);
}

void ButtonB3Click() {
	//CurrentTrack = random(1, NumTracksFolder[6]);
	//DFPlayer.playFolder(6, CurrentTrack);
	PlayScaredMild();
}

void ButtonB3ClickB() {
	PieCloseSequencial();
	CurrentTrack = 16;
	//DFPlayer.playFolder(1, CurrentTrack);
	PlayWavA(16);
}

void ButtonB4Click() {
	// Scream function
	//CurrentTrack = random(1, NumTracksFolder[7]);
	//DFPlayer.playFolder(7, CurrentTrack);
	/*int state = DFPlayer.readState();
	Serial.println(state);
	state = DFPlayer.readState();
	Serial.println(state);
	while (state < 0) {
		state = DFPlayer.readState();
		Serial.println(state);
	}

	if (state != 513) {
		Serial.println("play track");
		CurrentTrack = random(1, NumTracksFolder[7]);
		DFPlayer.playFolder(7, CurrentTrack);
	}
	else {
		Serial.println("play advertise");
		CurrentTrack = random(1, 5);
		DFPlayer.advertise(CurrentTrack);
	}*/
	if (!ScaredOpen) {
		ScaredOpen = 1;
		
		//PlayTrackorAdvert(7, 0, 1, 5);
		PieOpenAll();
		//scheduler.schedule(PieCloseAll, 1000);

		HolosBright();
		OpenAllBodyServos();  // Open all servo doors
		//scheduler.schedule(ServosAllClosed, 1000);
		PlayScaredExtreme();
	}
}

void ButtonB4ClickB() {
	PlayElectrocute();
	//CurrentTrack = random(1, NumTracksFolder[12]);
	//DFPlayer.playFolder(12, CurrentTrack);
	SetPSIMode(2); // PSI Short Circuit
	SetDPortLEDMode(2);
	scheduler.schedule(HolosBright, 40);
	scheduler.schedule(LogicShortCircuit, 40);
	scheduler.schedule(PieOpen0, random(10, 1500));
	scheduler.schedule(PieOpen1, random(10, 1500));
	scheduler.schedule(PieOpen2, random(10, 1500));
	scheduler.schedule(PieOpen3, random(10, 1500));
	scheduler.schedule(PieOpen4, random(10, 1500));
	scheduler.schedule(PieOpen5, random(10, 1500));
	scheduler.schedule(HolosBright, 1800);
	scheduler.schedule(PieClose0, random(1500, 2700));
	scheduler.schedule(PieClose1, random(1500, 2700));
	scheduler.schedule(PieClose2, random(1500, 2700));
	scheduler.schedule(PieClose3, random(1500, 2700));
	scheduler.schedule(PieClose4, random(1500, 2700));
	scheduler.schedule(PieClose5, random(1500, 2700));
	scheduler.schedule(PieOpen2, 3300);
	scheduler.schedule(PieOpen3, 3000);
	scheduler.schedule(PieClose2, 4500);
	scheduler.schedule(PieClose3, 5000);
	SmokeOn(12000);
	ScreamCrazyStart();

	scheduler.schedule(TurnCPLEDsOff, 5000);
	scheduler.schedule(TurnDPLEDsOff, 6000);
	scheduler.schedule(TurnDPortLEDsOff, 3500);
	scheduler.schedule(AllDomeLEDsOff, 6000);
	scheduler.schedule(ServosAllClosed, 6020);;
	scheduler.schedule(AllDomeLEDsOff, 6040);
	scheduler.schedule(AllDomeLEDsOff, 6080);

	scheduler.schedule(AllDomeLEDsOn, 12000);

}


void ButtonB4LongClick() {
	// Scream function
	Serial.println("Button B4 Long Click");
	if (ScaredOpen) {
		ServosAllClosed();
		AllDomeLEDsOn();
		ScaredOpen = 0;
	}
}


void ButtonC1Click() {
	// PSIFrontOff();
	SmokeOff();
	PlayMadExtreme();

}

void ButtonC1ClickB() {
	
	LogicMatrixDrop();
}

void ButtonC1LongClick() {
	Serial.println("Button C1 Long Click");
	SmokeOn(10000);
}

void ButtonC2Click() {
	if (TetrisMode) TetrisRotate();
	else {
		
		//CurrentTrack = random(1, NumTracksFolder[8]);
		//DFPlayer.playFolder(8, CurrentTrack);
		PlaySadExtreme();
	}
}

void ButtonC2ClickB() {
	//LogicTwinkleRed(); 
	LogicNextPattern();
	LogicNextPatternDelay(15000);
}

void ButtonC3Click() {
	
	//CurrentTrack = random(1, NumTracksFolder[9]);
	//DFPlayer.playFolder(9, CurrentTrack);
	PieOpenClose3(3);

}

void ButtonC3ClickB() {
	CurrentTrack = 5;
	//DFPlayer.playFolder(1, CurrentTrack);
	PlayWavA(CurrentTrack);
}

void ButtonC4Click() {
//	CurrentTrack = random(1, NumTracksFolder[10]);
//	DFPlayer.playFolder(10, CurrentTrack);
	if (ScaredOpen) {
		ServosAllClosed();
		AllDomeLEDsOn();
		ScaredOpen = 0;
	}
}

void ButtonC4LongClick() {
	ArmWaveStart();
}

void ButtonC4ClickB() {
	//Scream and I feel good
	VocalAllStop();
	if (!ScaredOpen) {
		ScaredOpen = 1;

		//PlayTrackorAdvert(7, 0, 1, 5);
		PieOpenAll();
		//scheduler.schedule(PieCloseAll, 1000);

		HolosBright();
		OpenAllBodyServos();  // Open all servo doors
		//scheduler.schedule(ServosAllClosed, 1000);
		PlayScaredExtreme();
	}
	// play feel good song
	gCurrentWavTrack = 7;
	scheduler.schedule(PlayWavA, 1650);
	
}

void ButtonD1Click() {
	if (TetrisMode) TetrisLeft();
	else {
		if (bDataDoorOpen) { // door open, close it
			DataPanelDoorState = 4;
			bDataDoorOpen = 0;
			TurnDPLEDsOff();
		}
		else { // Door Closed, open it
			DataPanelDoorState = 1;
			bDataDoorOpen = 1;
			TurnDPLEDsOn();
			TurnCPLEDsOn();
			AllDomeLEDsOn();
			TurnDPLEDsOn();
			TurnDPortLEDsOn();
		}
	}
}
void ButtonD1ClickB() {
	SetPSIMode(8);  //Siren
	SetDPortLEDMode(4);
	PlayWavB(3001);
	HolosBright();
	LogicShortCircuit();
}

void ButtonD1LongClick() {
	TurnDPortLEDsOff();
}

void ButtonD2Click() {
	if (TetrisMode) TetrisDrop();
	else {
		OpenUtilityArmTop();
		scheduler.schedule(CloseUtilityArmTop, 750);
		scheduler.schedule(OpenUtilityArmTop, 950);
		scheduler.schedule(CloseUtilityArmTop, 1200);
		scheduler.schedule(OpenUtilityArmTop, 1450);
		scheduler.schedule(CloseUtilityArmTop, 1800);
		bUtilityOpen = false;
		
	}
}

void ButtonD2ClickB() {
	SetPSIMode(7);  //Heart Beat
	if (!bUtilityOpen) { // arms closed, open them
		OpenUtilityArmBot();
		OpenUtilityArmTop();
		bUtilityOpen = true;
	}
	else {
		CloseUtilityArmBot();
		CloseUtilityArmTop();
		bUtilityOpen = false;
	}
}

void ButtonD3Click() {
	if (TetrisMode) TetrisRight();
	else HolosBright();
}

void ButtonD3ClickB() {
	//Toggle Red Light Green Light on PSI
	if (GetPSIMode() == 3) { SetPSIMode(4); PlayHappyMild(); }
	else if (GetPSIMode() == 4) { SetPSIMode(3); PlayMadMild(); }
	if (GetPSIMode() > 4 || GetPSIMode() < 3) {
		SetPSIMode(3);
		//CurrentTrack = random(1, NumTracksFolder[3]);
		//DFPlayer.playFolder(3, CurrentTrack);
		PlayMadExtreme();
	}
}

void ButtonD3LongClick() {
	HolosOff();
}


void ButtonD4Click() {
	nextLEDPatternDP();
//	PSIShortCircuit();
//	SetPSIMode(1);
	
}

void ButtonD4ClickB() {
	PSIFrontNextPattern();
}

void ButtonD4LongClick() {
	Serial.println("D4 Long Click");
	if (TetrisMode) {// Exit Tetris Mode
		TetrisMode = false;
		LogicNextPattern();
		LogicNextPatternDelay(25000);
		SetPSIMode(1);
		Serial.println("Tetris Mode Off");
		//DFPlayer.playFolder(11, 3);
		PlayWavA(2003);
	}
	else {
		TetrisMode = true;
		//DFPlayer.playFolder(11, 1);
		PlayWavA(2001);
		TetrisStart();
		Serial.println("Tetris Mode ON");
		SetPSIMode(0);
	}
}

bool isDataPanelOpen() {
	return bDataDoorOpen;
}




