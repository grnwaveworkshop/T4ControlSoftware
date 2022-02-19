// Radio Event triggering
// Mapping radio switches etc. to functions.


#include "definitions.h"
#include <Scheduler.h>

uint8_t RunMode = 0;
uint16_t ButtonState = 0;
bool ButtonPressed = false;
bool ButtonLongPressed = false;
bool TetrisMode = false;


int ButtonCount = 0;

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
	switch (GetButtonState()) {
		case (ButtonOff - ButtonDeadband) ... (ButtonOff + ButtonDeadband) :
			ButtonPressed = 0;
			ButtonLongPressed = 0;
			ButtonCount = 0;
			//      Serial.println("Buttons Off");
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
}



void ButtonA1Click() {
	DFPlayer.pause();
	scheduler.stopall();
	StopAllServoSequences();
}

void ButtonA1LongClick() {
	Serial.println("Button A1 Long Click");
	// Home all functions
	ServosAllClosed();
	AllDomeLEDsOff();
	TurnDPLEDsOff();
	TurnCPLEDsOff();

}

void ButtonA2Click() {
	Serial.println("Button A1 Click");
	FolderNum = 1;
	CurrentTrack = random(1, NumTracksFolder[FolderNum]);
	DFPlayer.playFolder(FolderNum, CurrentTrack);
}

void ButtonA2ClickB() {
	if (CurrentTrack < NumTracksFolder[1]) CurrentTrack++;
	else CurrentTrack = 1;
	DFPlayer.playFolder(1, CurrentTrack);
	//DFPlayer.next();
}

void ButtonA2LongClick() {

	//CurrentTrack = random(1, NumTracksFolder[1]);
	DFPlayer.loopFolder(1);
}

void ButtonA3Click() {
	CurrentTrack = random(1, NumTracksFolder[2]);
	DFPlayer.playFolder(2, CurrentTrack);
}

void ButtonA3ClickB() {

}

void ButtonA4Click() {
	CurrentTrack = random(1, NumTracksFolder[3]);
	DFPlayer.playFolder(3, CurrentTrack);
}

void ButtonA4ClickB() {

	Serial.println(DFPlayer.readState());
	delay(200);
	PlayTrackorAdvert(11, 4, 6, 6);
	//DFPlayer.advertise(6);	// No Thank you
}

void ButtonB1Click() {
	CurrentTrack = random(1, NumTracksFolder[4]);
	DFPlayer.playFolder(4, CurrentTrack);
}

void ButtonB1ClickB() {
	PieOpenClose3(3);
}

void ButtonB2Click() {
	CurrentTrack = random(1, NumTracksFolder[5]);
	DFPlayer.playFolder(5, CurrentTrack);
}

void ButtonB2ClickB() {
	PieOpenSequencial();
}

void ButtonB3Click() {
	CurrentTrack = random(1, NumTracksFolder[6]);
	DFPlayer.playFolder(6, CurrentTrack);
}

void ButtonB3ClickB() {
	PieCloseSequencial();
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
	PlayTrackorAdvert(7, 0, 1, 5);
	PieOpenAll();
	//scheduler.schedule(PieCloseAll, 1000);

	HolosBright();
	OpenAllBodyServos();  // Open all servo doors
	//scheduler.schedule(ServosAllClosed, 1000);
}

void ButtonB4ClickB() {
	CurrentTrack = random(1, NumTracksFolder[12]);
	DFPlayer.playFolder(12, CurrentTrack);
	SetPSIMode(2); // PSI Short Circuit
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
	scheduler.schedule(AllDomeLEDsOff, 6000);
	scheduler.schedule(ServosAllClosed, 6020);;
	scheduler.schedule(AllDomeLEDsOff, 6040);
	scheduler.schedule(AllDomeLEDsOff, 6080);

	scheduler.schedule(AllDomeLEDsOn, 8000);

}


void ButtonB4LongClick() {
	// Scream function
	Serial.println("Button B4 Long Click");
	ServosAllClosed();
	AllDomeLEDsOn();

}


void ButtonC1Click() {
	// PSIFrontOff();
	SmokeOff();

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
		
		CurrentTrack = random(1, NumTracksFolder[8]);
		DFPlayer.playFolder(8, CurrentTrack);
	}
}

void ButtonC2ClickB() {
	LogicTwinkleRed();
}

void ButtonC3Click() {
	
	CurrentTrack = random(1, NumTracksFolder[9]);
	DFPlayer.playFolder(9, CurrentTrack);
}

void ButtonC3ClickB() {

}

void ButtonC4Click() {
	CurrentTrack = random(1, NumTracksFolder[10]);
	DFPlayer.playFolder(10, CurrentTrack);
}

void ButtonC4LongClick() {
	ArmWaveStart();
}

void ButtonC4ClickB() {
	//Logic Next Pattern
	LogicNextPattern();
	LogicNextPatternDelay(15000);
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
		}
	}
}
void ButtonD1ClickB() {
	SetPSIMode(6);  //Radar
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
	if (GetPSIMode() == 3) SetPSIMode(4);
	else if (GetPSIMode() == 4) SetPSIMode(3);
	if (GetPSIMode() > 4 || GetPSIMode() < 3) {
		SetPSIMode(3);
		CurrentTrack = random(1, NumTracksFolder[3]);
		DFPlayer.playFolder(3, CurrentTrack);
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
		DFPlayer.playFolder(11, 3);
	}
	else {
		TetrisMode = true;
		DFPlayer.playFolder(11, 1);
		TetrisStart();
		Serial.println("Tetris Mode ON");
		SetPSIMode(0);
	}
}

bool isDataPanelOpen() {
	return bDataDoorOpen;
}




