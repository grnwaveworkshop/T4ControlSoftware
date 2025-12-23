#ifndef SERVOSEQUENCES_HPP
#define SERVOSEQUENCES_HPP

#include <Arduino.h>

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
	uint8_t numMoves;               // Number of moves
	uint8_t currentMove;            // Current move number in the sequence
	bool isServoMoving;             // indicates a sequence is moving active
	bool isSeqActive;               // indicates the sequence is active
	unsigned long ServoSequenceMillis;  // start time for timing
};

extern uint8_t IntArmState;
extern uint8_t GripperState;
extern uint8_t UtilityArmState;
extern uint8_t DataPanelDoorState;
extern bool bDataDoorOpen;
extern bool bUtilityOpen;

void SequenceLoop();
void StopServoSequence(struct SeqStatus_t* SequenceStat);
void StopAllServoSequences();
void ServosAllClosed();
void OpenAllBodyServos();
void ProcessSequences();
void ProcessIntArm();
void ProcessGripper();
void ProcessDataPanel();
void PlaySequence(struct SeqStatus_t* theSeqStatus, struct ServoSeq_t* moveList);
void ScreamCrazyStart();
void ArmWaveStart();

#endif