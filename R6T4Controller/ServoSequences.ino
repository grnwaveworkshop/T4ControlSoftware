#include "ServoSequences.h"


// State Machine Declarations
uint8_t IntArmState = 0;
uint8_t GripperState = 0;
uint8_t UtilityArmState = 0;
uint8_t DataPanelDoorState = 0;

// state flags
bool bDataDoorOpen = 0;
bool bUtilityOpen = 0;


// structure of the servo move
struct ServoSeq_t {
	uint8_t ServoNum; // Which servo to act on.
	uint16_t pos;            // position of servo in us
	uint16_t move_dur;       // duration of move (easing amount) in ms
	uint16_t hold_time;      // ms to hold position
};

// structure to hold the status of the sequence
struct SeqStatus_t {
	// struct ServoSeq* Sequence[];      // Sequence to play
	uint8_t numMoves = 0;               // Number of moves
	uint8_t currentMove = 0;            // Current move number in the sequence
	bool isServoMoving = 0;             // indicates a sequence is moving active
	bool isSeqActive = 0;               // indicates the sequence is active
	unsigned long ServoSequenceMillis;  // start time for timing
};

// define sequences
// --------------------------------------
// -------------  Gripper ---------------
// --------------------------------------
// Open Gripper arm and door
ServoSeq_t GripOpen[] = {
  {GRIPARMDOOR, GripArmDoorOpen, 20, 250},
  {GRIPARM, GripArmOpen, 100, 500},
  {GRIPPER, GripperOpen, 100, 150},
  {GRIPPER, GripperClosed, 100, 150},
  {GRIPPER, GripperOpen, 100, 150},
  {GRIPPER, GripperClosed, 100, 0},

};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripStatOpen = { 5, 0, 0, 0, millis() };

// Close Gripper arm and door
ServoSeq_t GripClose[] = {
  {GRIPPER, GripperClosed, 280, 180},
  {GRIPARM, GripArmClosed, 220, 500},
  {GRIPARMDOOR, GripArmDoorClosed, 50, 10},
};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripStatClose = { 3, 0, 0, 0, millis() };

// Open Gripper
ServoSeq_t OpenGripper[] = {

  {GRIPPER, GripperOpen, 230, 125},

};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripperStatOpen = { 1, 0, 0, 0, millis() };

// Close Gripper
ServoSeq_t CloseGripper[] = {
  {GRIPPER, GripperClosed, 280, 180},
};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripperStatClose = { 1, 0, 0, 0, millis() };

// ----------------------------------------------
// -------------  Interface Arm  ---------------
// ----------------------------------------------
// 
// Open Interface Arm and Door
ServoSeq_t OpenIntArm[] = {
  {INTARMDOOR, IntArmdoorOpen, 20, 200},
  {INTARM,    IntArmOpen, 220, 500},
  {INTERFACE, InterfaceOpen, 50, 500},
  // {INTERFACE, InterfaceClosed, 50, 500},
  // {INTERFACE, InterfaceOpen, 50, 500},
  // {INTERFACE, InterfaceClosed, 50, 500},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t IntArmStatOpen = { 3, 0, 0, 0, millis() };

// Close Interface Arm and Door
ServoSeq_t CloseIntArm[] = {
  {INTERFACE, InterfaceClosed, 50, 180},
  {INTARM, IntArmClosed, 220, 500},
  {INTARMDOOR, IntArmdoorClosed, 20, 10},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t IntArmStatClose = { 3, 0, 0, 0, millis() };


// Open Interface
ServoSeq_t OpenInterface[] = {

  {INTERFACE, InterfaceOpen, 230, 125},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t InterfaceStatOpen = { 1, 0, 0, 0, millis() };

// Close Interface
ServoSeq_t CloseInterface[] = {
  {INTERFACE, InterfaceClosed, 280, 180},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t InterfaceStatClose = { 1, 0, 0, 0, millis() };

// ----------------------------------------------
// -------------  Data Panel Door ---------------
// ----------------------------------------------
// Data Panel Door Open
ServoSeq_t DataDoorOpen[] = {

  {DATAPPANELDOOR, DataPanelDoorOpen, 230, 125},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t DataDoorStatOpen = { 1, 0, 0, 0, millis() };

// Close Interface
ServoSeq_t DataDoorClose[] = {
  {DATAPPANELDOOR, DataPanelDoorClosed, 280, 180},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t DataDoorStatClose = { 1, 0, 0, 0, millis() };

// ----------------------------------------------
// -----------  Scream Crazy Seq ----------------
// Door and utility arms open close a bunch and end closed
// ----------------------------------------------

ServoSeq_t ScreamCrazy[] = {

	// open all doors
	{DATAPPANELDOOR, DataPanelDoorOpen, 230, 0},
	{INTARMDOOR, IntArmdoorOpen, 20, 0},
	{GRIPARMDOOR, GripArmDoorOpen, 20, 500},
	
	//close the doors
	{DATAPPANELDOOR, DataPanelDoorClosed, 230, 0},
	{INTARMDOOR, IntArmdoorClosed, 20, 0},
	{GRIPARMDOOR, GripArmDoorClosed, 50, 0},

	// open the utility arms
	{UTILITYARMTOP, TopUtilityOpen, 50, 125 },
	{UTILITYARMBOT, BotUtilityOpen, 50, 100},

	// open doors again
	{DATAPPANELDOOR, DataPanelDoorOpen, 230, 0},
	{INTARMDOOR, IntArmdoorOpen, 20, 200},
	{GRIPARMDOOR, GripArmDoorOpen, 20, 500},

	// Close Data panel and Top Utility
	{DATAPPANELDOOR, DataPanelDoorClosed, 230, 100},
	{UTILITYARMTOP, TopUtilityClosed, 50, 50 },

	// Raise arms
	{INTARM, IntArmOpen, 220, 200},
	{GRIPARM, GripArmOpen, 100, 200},

	//  Open top Utility, Close Bottom
	{UTILITYARMTOP, TopUtilityOpen, 50, 50 },
	{UTILITYARMBOT, BotUtilityClosed, 50, 50 },
	{DATAPPANELDOOR, DataPanelDoorOpen, 230, 50},

	// Lower grip arm
	{GRIPARM, GripArmClosed, 100, 100},
	{UTILITYARMTOP, TopUtilityClosed, 50, 150 },
	{UTILITYARMBOT, BotUtilityOpen, 50, 150 },
	{INTARM, IntArmClosed, 220, 200},

	{UTILITYARMBOT, BotUtilityClosed, 50, 150 },

	// Raise arms
	{INTARM, IntArmOpen, 220, 200},
	{UTILITYARMTOP, TopUtilityOpen, 50, 250 },
	{GRIPARM, GripArmOpen, 100, 200},
	{UTILITYARMBOT, BotUtilityOpen, 50, 250 },

	// Lower Arms
	{GRIPARM, GripArmClosed, 100, 300},
	{INTARM, IntArmClosed, 220, 400},

	{DATAPPANELDOOR, DataPanelDoorClosed, 230, 150},
	{INTARMDOOR, IntArmdoorClosed, 20, 0},
	{GRIPARMDOOR, GripArmDoorClosed, 50, 0},
	{UTILITYARMBOT, BotUtilityClosed, 50, 250 },
	{UTILITYARMTOP, TopUtilityClosed, 50, 00 },


};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t ScreamCrazyStat = { sizeof(ScreamCrazy) / sizeof(SeqStatus_t), 0, 0, 0, millis() };

// ------------------------------------------------
// ------------------ Arm Wave --------------------
// Open doors, raise gripper and arm doors a bunch
// ------------------------------------------------

ServoSeq_t ArmWave[] = {

	// open doors
	{INTARMDOOR, IntArmdoorOpen, 100, 0},
	{GRIPARMDOOR, GripArmDoorOpen, 100, 500},

	// Raise arms
	{INTARM, IntArmOpen, 100, 0},
	{GRIPARM, GripArmOpen, 100, 500},

	// Lower arms
	{GRIPARM, GripArmClosed-150, 100, 0},
	{INTARM, IntArmClosed+150, 100, 400},

	// Raise arms
	{INTARM, IntArmOpen-200, 100, 50},
	{GRIPARM, GripArmOpen+200, 100, 400},

	// Lower Arms
	{GRIPARM, GripArmClosed-100, 100, 50},
	{INTARM, IntArmClosed+100, 100, 400},

	// Raise Arms
	{GRIPARM, GripArmOpen, 100, 50},
	{INTARM, IntArmOpen, 100, 400},

	// Lower Arms
	{GRIPARM, GripArmClosed, 100, 50},
	{INTARM, IntArmClosed, 100, 300},

	// Raise Arms
	{GRIPARM, GripArmOpen, 100, 50},
	{INTARM, IntArmOpen, 100, 400},

	// Lower Arms
	{GRIPARM, GripArmClosed, 100, 50},
	{INTARM, IntArmClosed, 100, 300},

	//Close Doors
	{INTARMDOOR, IntArmdoorClosed, 100, 0},
	{GRIPARMDOOR, GripArmDoorClosed, 100, 0},


};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t ArmWaveStat = { sizeof(ArmWave) / sizeof(SeqStatus_t), 0, 0, 0, millis() };


// ----------------------------------------------
// ------------- Sequence Loop ------------------
// ----------------------------------------------
void SequenceLoop() {

	ProcessIntArm();
	ProcessGripper();
	ProcessDataPanel();
	ProcessSequences();
	

}// ----------- End Loop -----------------------

// ---------------------------------------------
// -------- Stop All Servo Sequences -----------
// ---------------------------------------------

void StopServoSequence(struct SeqStatus_t* SequenceStat) {
	SequenceStat->isSeqActive = 0;
	SequenceStat->currentMove = 0;
}
//
void StopAllServoSequences() {
	StopServoSequence(&ScreamCrazyStat);

}

// Function to sequence everything to closed state
void ServosAllClosed() {
	HolosOff();
	SmokeOff();
	// Close Utility Arms
	CloseUtilityArmBot();
	CloseUtilityArmTop();
	bUtilityOpen = false;

	// Close Interface Arm
	IntArmState = 4;
	//   PlaySequence(&IntArmStatClose, CloseIntArm);

	   // Close Gripper
	//   PlaySequence(&GripStatClose, GripClose);
	GripperState = 4;
	   // Close Data Panel Door
	DataPanelDoorState = 4;
	bDataDoorOpen = 0;
	TurnDPLEDsOff();

	// Close Pie Panels
	PieCloseAll();
	SmokeOff();
	HolosOff();
	
}
void OpenAllBodyServos() {
	// Open Utility Arms
	OpenUtilityArmBot();
	OpenUtilityArmTop();

	// Open Data Panel Door
	DataPanelDoorState = 1;
	bDataDoorOpen = 1;
	TurnDPLEDsOn();

	// Open IntArmDoor
	ServoList[INTARMDOOR].target_position = IntArmdoorOpen;
	// Open GripperDoor
	ServoList[GRIPARMDOOR].target_position = GripArmDoorOpen;

}

// ---------------------------------------------
// ---- Process Interface Arm State Machine ----
// ---------------------------------------------
void ProcessSequences()
{
	
	//	IntArmStatOpen.isSeqActive = 1;
	
		if (ScreamCrazyStat.isSeqActive) PlaySequence(&ScreamCrazyStat, ScreamCrazy);
		if (ArmWaveStat.isSeqActive) PlaySequence(&ArmWaveStat, ArmWave);
		
}


// ---------------------------------------------
// ---- Process Interface Arm State Machine ----
// ---------------------------------------------
void ProcessIntArm()
{
	switch (IntArmState) {
	case 0:
		// do nothing until switch changes
		//Serial.println("in Int State 0");
		if (ChannelData(INTARMSWITCH) > 500) IntArmState = 1;
		break;
	case 1:
		// Run Open Sequence
		//      Serial.println("in Int State 1");
		IntArmStatOpen.isSeqActive = 1;
		IntArmState = 2;
		break;
	case 2:
		// Wait here till complete
		//      Serial.println("in Int State 2");
		if (IntArmStatOpen.isSeqActive) PlaySequence(&IntArmStatOpen, OpenIntArm);
		else IntArmState = 3;
		break;
	case 3:
		// Wait until close sequence
		//    Serial.println("in Int State 3");
		if (ChannelData(INTARMSWITCH) < 500) IntArmState = 4;
		else if (ChannelData(INTARMSWITCH) > 1200) {
			InterfaceStatOpen.isSeqActive = 1;
			IntArmState = 7;
		}

		break;
	case 4:
		// Run Close Sequence
		//Serial.println("in Int State 4");
		IntArmStatClose.isSeqActive = 1;
		IntArmState = 5;
		break;
	case 5:
		// Wait here till complete
		//Serial.println("in Int State 5");
		if (IntArmStatClose.isSeqActive) PlaySequence(&IntArmStatClose, CloseIntArm);
		else IntArmState = 0;
		break;

	case 7:
		// Wait here till complete
		//    Serial.println("in Int State 7");
		if (InterfaceStatOpen.isSeqActive) PlaySequence(&InterfaceStatOpen, OpenInterface);
		else  if (ChannelData(INTARMSWITCH) < 1200) {
			InterfaceStatClose.isSeqActive = 1;
			IntArmState = 8;
		}
		break;

	case 8:
		// Wait here till complete
		//    Serial.println("in Int State 8");
		if (InterfaceStatClose.isSeqActive) PlaySequence(&InterfaceStatClose, CloseInterface);
		else if (ChannelData(INTARMSWITCH) < 500) {
			InterfaceStatClose.isSeqActive = 1;
			IntArmState = 4;
			break;
		}
		else if (ChannelData(INTARMSWITCH) > 1200) {
			InterfaceStatOpen.isSeqActive = 1;
			IntArmState = 7;
			break;
		}
		break;
	default:
		break;
	}
}

// -----------------------------
// -- Gripper State Machine ----
// -----------------------------
void ProcessGripper()
{
	switch (GripperState) {
	case 0:
		//Serial.println("in Grip State 0");
		// do nothing until switch changes
		if (ChannelData(GRIPARMSWITCH) > 500) GripperState = 1;
		break;
	case 1:
		// Run Open Sequence
		//     Serial.println("in Grip State 1");
		GripStatOpen.isSeqActive = 1;
		GripperState = 2;
		break;
	case 2:
		// Wait here till complete
		//Serial.println("in Grip State 2");
		if (GripStatOpen.isSeqActive) {
			PlaySequence(&GripStatOpen, GripOpen);
			//     Serial.print("GripStatOpen.isSeqActive "); Serial.println(GripStatOpen.isSeqActive);
		}
		else GripperState = 3;
		break;
	case 3:
		// Wait until close sequence
		//     Serial.println("in Grip State 3");

		// do nothing until switch changes
		// Close Arm or Open Gripper
		if (ChannelData(GRIPARMSWITCH) < 500) GripperState = 4;
		else if (ChannelData(GRIPARMSWITCH) > 1200) {
			GripperStatOpen.isSeqActive = 1;
			GripperState = 7;
		}
		break;
	case 4:
		// Run Close Sequence
		//   Serial.println("in Grip State 4");
		GripStatClose.isSeqActive = 1;
		GripperState = 5;
		break;


	case 5:
		// Wait here till complete
		//    Serial.println("in Grip State 5");
		if (GripStatClose.isSeqActive) PlaySequence(&GripStatClose, GripClose);
		else GripperState = 0;
		break;


	case 7:
		// Wait here till complete
		//    Serial.println("in Grip State 7");
		if (GripperStatOpen.isSeqActive) PlaySequence(&GripperStatOpen, OpenGripper);
		else  if (ChannelData(GRIPARMSWITCH) < 1200) {
			GripperStatClose.isSeqActive = 1;
			GripperState = 8;
		}
		break;

	case 8:
		// Wait here till complete
		//      Serial.println("in Grip State 8");
		if (GripperStatClose.isSeqActive) PlaySequence(&GripperStatClose, CloseGripper);
		else if (ChannelData(GRIPARMSWITCH) < 500) {
			GripStatClose.isSeqActive = 1;
			GripperState = 4;
			break;
		}
		else if (ChannelData(GRIPARMSWITCH) > 1200) {
			GripperStatOpen.isSeqActive = 1;
			GripperState = 7;
			break;
		}
		break;

	default:
		break;
	}
}


// -----------------------------
// Data Panel Door State Machine
// -----------------------------

void ProcessDataPanel() {

#if 1 // Not Used! - Remapped to button
	switch (DataPanelDoorState) {
	case 0:
		//Serial.println("in Data Door 0");
		// do nothing until switch changes
	   // if (ChannelData(DataPanelSwitch) > 500) DataPanelDoorState = 1;
		break;
	case 1:
		// Run Open Sequence
		Serial.println("in Data Door 1");
		DataDoorStatOpen.isSeqActive = 1;
		DataPanelDoorState = 2;
		break;
	case 2:
		// Wait here till complete
		//Serial.println("in Data Door 2");
		if (DataDoorStatOpen.isSeqActive) {
			PlaySequence(&DataDoorStatOpen, DataDoorOpen);
			//     Serial.print("GripStatOpen.isSeqActive "); Serial.println(GripStatOpen.isSeqActive);
		}
		else DataPanelDoorState = 3;
		break;
	case 3:
		// Wait until close sequence
		//     Serial.println("in Data Door 3");

		// do nothing until switch changes
		// Close Arm or Open Gripper
	  //  if (ChannelData(DataPanelSwitch) < 500) DataPanelDoorState = 4;


		break;
	case 4:
		// Run Close Sequence
		Serial.println("in Data Door 4");
		DataDoorStatClose.isSeqActive = 1;
		DataPanelDoorState = 5;
		break;


	case 5:
		// Wait here till complete
		//    Serial.println("in Data Door 5");
		if (DataDoorStatClose.isSeqActive) PlaySequence(&DataDoorStatClose, DataDoorClose);
		else DataPanelDoorState = 0;
		break;
	default:
		Serial.println("in Data Door default...");
		break;
	}
#endif  // Data Panel Door State Machine
}


// -----------------------------
//      Play the Sequence
// requires to be continually called periodically to move through the sequence list.
// -----------------------------

void PlaySequence(struct SeqStatus_t* theSeqStatus, struct ServoSeq_t* moveList) {

	if (theSeqStatus->isSeqActive) {
		if (!theSeqStatus->isServoMoving) { // Finished last move, so start the move
			theSeqStatus->ServoSequenceMillis = millis(); // reset time

			ServoList[moveList[theSeqStatus->currentMove].ServoNum].target_position = moveList[theSeqStatus->currentMove].pos;  // start moving servo

			theSeqStatus->isServoMoving = 1;
			//	Serial.print("state A current Move "); Serial.println(theSeqStatus->currentMove);
		}
		else {
			//	Serial.print("State B wait for time: "); Serial.print((millis() - theSeqStatus->ServoSequenceMillis));
			//	Serial.print(" hold time: "); Serial.println(moveList[theSeqStatus->currentMove].hold_time);
			if ((millis() - theSeqStatus->ServoSequenceMillis) >= moveList[theSeqStatus->currentMove].hold_time) {  // Finished the hold time, so finish the sequence
				theSeqStatus->isServoMoving = 0;
				Serial.print("state B.1 current Move "); Serial.println(theSeqStatus->currentMove);
				if (theSeqStatus->currentMove < theSeqStatus->numMoves - 1) theSeqStatus->currentMove++;
				else {
					theSeqStatus->currentMove = 0;
					theSeqStatus->isSeqActive = 0;
					//	Serial.print("State B active check"); Serial.println(theSeqStatus->isSeqActive);
				}
				//	Serial.print("state B.2 current Move "); Serial.println(theSeqStatus->currentMove);
			}
		}
	}
}

void ScreamCrazyStart() {
	ScreamCrazyStat.isSeqActive = 1;
}

void ArmWaveStart() {
	ArmWaveStat.isSeqActive = 1;
}
