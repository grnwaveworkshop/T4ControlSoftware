
//#include <Servo.h>
//#include "ServoEasing.h"

uint8_t IntArmState = 0;
uint8_t GripperState = 0;
uint8_t UtilityArmState = 0;
uint8_t DataPanelDoorState = 0;

// structure of the servo move
struct ServoSeq_t {
  uint8_t ServoNum; // Which servo to act on.
  int pos;            // position of servo in degrees
  int move_dur;       // duration of move (easing amount) in ms
  int hold_time;      // ms to hold position
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
// Open Gripper arm and door
ServoSeq_t GripOpen[] = {
  {GRIPARMDOOR, 2500, 20, 250},
  {GRIPARM, 2500, 220, 500},
  {GRIPPER, 2500, 230, 320},
  {GRIPPER, 100, 230, 320},
  {GRIPPER, 2500, 230, 320},

};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripStatOpen = {5, 0, 0, 0, millis()};

// Close Gripper arm and door
ServoSeq_t GripClose[] = {
  {GRIPPER, 100, 280, 180},
  {GRIPARM, 100, 220, 400},
  {GRIPARMDOOR, 100, 20, 10},
};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripStatClose = {3, 0, 0, 0, millis()};

// Open Gripper
ServoSeq_t GripperOpen[] = {

  {GRIPPER, 2500, 230, 125},

};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripperStatOpen = {1, 0, 0, 0, millis()};

// Close Gripper
ServoSeq_t GripperClose[] = {
  {GRIPPER, 100, 280, 180},
};
// num_moves, current_move, isActive, sequenceActivetime
SeqStatus_t GripperStatClose = {1, 0, 0, 0, millis()};

// Open Interface Arm and Door
ServoSeq_t IntArmOpen[] = {
  {INTARMDOOR, 2400, 20, 100},
  {INTARM, 2400, 220, 100},
  {INTERFACE, 2400, 50, 500},
  {INTERFACE, 100, 50, 500},
  {INTERFACE, 2400, 50, 500},
  {INTERFACE, 100, 50, 500},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t IntArmStatOpen = {6, 0, 0, 0, millis()};

// Close Interface Arm and Door
ServoSeq_t IntArmClose[] = {
  {INTERFACE, 100, 50, 100},
  {INTARM, 200, 220, 100},
  {INTARMDOOR, 200, 20, 10},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t IntArmStatClose = {3, 0, 0, 0, millis()};


// Open Interface
ServoSeq_t InterfaceOpen[] = {

  {INTERFACE, 2500, 230, 125},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t InterfaceStatOpen = {1, 0, 0, 0, millis()};

// Close Interface
ServoSeq_t InterfaceClose[] = {
  {INTERFACE, 100, 280, 180},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t InterfaceStatClose = {1, 0, 0, 0, millis()};

//-----------------------
// Data Panel Door Open
ServoSeq_t DataDoorOpen[] = {

  {DATAPPANELDOOR, 2500, 230, 125},

};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t DataDoorStatOpen = {1, 0, 0, 0, millis()};

// Close Interface
ServoSeq_t DataDoorClose[] = {
  {DATAPPANELDOOR, 100, 280, 180},
};
// num_moves, current_move, isActive, sequenceActive time
SeqStatus_t DataDoorStatClose = {1, 0, 0, 0, millis()};



void SequenceLoop() {

  switch (IntArmState) {
    case 0:
      // do nothing until switch changes
//Serial.println("in Int State 0");
      if (ChannelData(IntArmSwitch) > 500) IntArmState = 1;
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
      if (IntArmStatOpen.isSeqActive) PlaySequence(&IntArmStatOpen, IntArmOpen);
      else IntArmState = 3;
      break;
    case 3:
      // Wait until close sequence
  //    Serial.println("in Int State 3");
      if (ChannelData(IntArmSwitch) < 500) IntArmState = 4;
      else if (ChannelData(IntArmSwitch) > 1200) {
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
      if (IntArmStatClose.isSeqActive) PlaySequence(&IntArmStatClose, IntArmClose);
      else IntArmState = 0;
      break;

    case 7:
      // Wait here till complete
  //    Serial.println("in Int State 7");
      if (InterfaceStatOpen.isSeqActive) PlaySequence(&InterfaceStatOpen, InterfaceOpen);
      else  if (ChannelData(IntArmSwitch) < 1200) {
        InterfaceStatClose.isSeqActive = 1;
        IntArmState = 8;
      }
      break;

    case 8:
      // Wait here till complete
  //    Serial.println("in Int State 8");
      if (InterfaceStatClose.isSeqActive) PlaySequence(&InterfaceStatClose, InterfaceClose);
      else if (ChannelData(IntArmSwitch) < 500) {
        InterfaceStatClose.isSeqActive = 1;
        IntArmState = 4;
        break;
      }
      else if (ChannelData(IntArmSwitch) > 1200) {
        InterfaceStatOpen.isSeqActive = 1;
        IntArmState = 7;
        break;
      }
      break;
    default:
      break;
  }



  switch (GripperState) {
    case 0:
      //Serial.println("in Grip State 0");
      // do nothing until switch changes
      if (ChannelData(GripArmSwitch) > 500) GripperState = 1;
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
      if (ChannelData(GripArmSwitch) < 500) GripperState = 4;
      else if (ChannelData(GripArmSwitch) > 1200) {
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
      if (GripperStatOpen.isSeqActive) PlaySequence(&GripperStatOpen, GripperOpen);
      else  if (ChannelData(GripArmSwitch) < 1200) {
        GripperStatClose.isSeqActive = 1;
        GripperState = 8;
      }
      break;

    case 8:
      // Wait here till complete
      //      Serial.println("in Grip State 8");
      if (GripperStatClose.isSeqActive) PlaySequence(&GripperStatClose, GripperClose);
      else if (ChannelData(GripArmSwitch) < 500) {
        GripStatClose.isSeqActive = 1;
        GripperState = 4;
        break;
      }
      else if (ChannelData(GripArmSwitch) > 1200) {
        GripperStatOpen.isSeqActive = 1;
        GripperState = 7;
        break;
      }
      break;

    default:
      break;
  }

// Data Panel Door State Machine
  switch (DataPanelDoorState) {
    case 0:
      //Serial.println("in Data Door 0");
      // do nothing until switch changes
      if (ChannelData(DataPanelSwitch) > 500) DataPanelDoorState = 1;
      break;
    case 1:
      // Run Open Sequence
      //     Serial.println("in Data Door 1");
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
      if (ChannelData(DataPanelSwitch) < 500) DataPanelDoorState = 4;
     
 
      break;
    case 4:
      // Run Close Sequence
      //   Serial.println("in Data Door 4");
      DataDoorStatClose.isSeqActive = 1;
      DataPanelDoorState = 5;
      break;


    case 5:
      // Wait here till complete
      //    Serial.println("in Data Door 5");
      if (DataDoorStatClose.isSeqActive) PlaySequence(&DataDoorStatClose, DataDoorClose);
      else DataPanelDoorState = 0;
      break;
  }

  // Utility Arms
  
}


void PlaySequence(struct SeqStatus_t *theSeqStatus, struct ServoSeq_t *moveList) {

  if (theSeqStatus->isSeqActive) {
    if (!theSeqStatus->isServoMoving) { // Finished last move, so start the move
      theSeqStatus->ServoSequenceMillis = millis(); // reset time

      ServoList[moveList[theSeqStatus->currentMove].ServoNum].target_position = moveList[theSeqStatus->currentMove].pos;  // start moving servo

      theSeqStatus->isServoMoving = 1;
      Serial.print("state A current Move "); Serial.println(theSeqStatus->currentMove);
    }
    else {
      Serial.print("State B wait for time: "); Serial.print((millis() - theSeqStatus->ServoSequenceMillis));
      Serial.print(" hold time: "); Serial.println(moveList[theSeqStatus->currentMove].hold_time);
      if ((millis() - theSeqStatus->ServoSequenceMillis) >= moveList[theSeqStatus->currentMove].hold_time) {  // Finished the hold time, so finish the sequence
        theSeqStatus->isServoMoving = 0;
        Serial.print("state B.1 current Move "); Serial.println(theSeqStatus->currentMove);
        if (theSeqStatus->currentMove < theSeqStatus->numMoves - 1) theSeqStatus->currentMove++;
        else {
          theSeqStatus->currentMove = 0;
          theSeqStatus->isSeqActive = 0;
          Serial.print("State B active check"); Serial.println(theSeqStatus->isSeqActive);
        }
        Serial.print("state B.2 current Move "); Serial.println(theSeqStatus->currentMove);
      }
    }
  }
}
