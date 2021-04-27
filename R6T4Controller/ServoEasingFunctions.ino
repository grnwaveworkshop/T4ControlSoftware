
#include <Servo.h>
#include "ServoEasing.h"

unsigned long ServoCurrentMillis = millis();

uint8_t ServoState = 0;


struct BodyServo {
  ServoEasing mServo;
  int min_usec;
  int max_usec;
  int center_usec;
  float current_position;
  float target_position;
  bool isAttached;
};

ServoEasing Dome;
ServoEasing UtilityArmTop;
ServoEasing UtilityArmBot;
ServoEasing IntArmDoor;
ServoEasing IntArm;
ServoEasing Interface;
ServoEasing GripArmDoor;
ServoEasing GripArm;
ServoEasing Grip;
ServoEasing DataPanel;



// create servo objects
// Servo , min_usec, max_usec, center_usec, current_position, target_position
BodyServo ServoDome = { Dome , 1000 , 2000, 1500, 1500 , 1500, 0};
BodyServo ServoUtilityArmTop = { UtilityArmTop , 1000 , 2000, 1500, 1500, 1500, 0 };
BodyServo ServoUtilityArmBot = { UtilityArmBot , 1000 , 2000, 1500, 1500, 1500 , 0};
BodyServo ServoIntArmDoor = { IntArmDoor , 1000 , 2000, 1500, 1500 , 1500, 0};
BodyServo ServoIntArm = { IntArm , 1000 , 2000, 1500, 1500, 1500, 0};
BodyServo ServoInterface = { Interface , 1000 , 2000, 1500, 1500, 1500, 0 };
BodyServo ServoGripArmDoor = { GripArmDoor , 1000 , 2000, 1500, 1500, 1500, 0 };
BodyServo ServoGripArm = { GripArm , 1000 , 1800, 1600, 1500, 1200 , 0};
BodyServo ServoGrip = { Grip , 1200 , 1800, 1700, 1700, 1500, 0 };
BodyServo ServoDataPanel = { DataPanel , 1000 , 2000, 1500, 1500, 1500, 0 };

BodyServo ServoList[] = {
  ServoDome,
  ServoUtilityArmTop,
  ServoUtilityArmBot,
  ServoIntArmDoor,
  ServoIntArm,
  ServoInterface,
  ServoGripArmDoor,
  ServoGripArm,
  ServoGrip,
  ServoDataPanel
};

uint8_t ChannelList [] = {
  // DomeChannel ,
  UtilityArmTopChannel ,
  UtilityArmBotChannel ,
  IntArmDoorChannel ,
  IntArmChannel ,
  InterfaceChannel ,
  GripArmDoorChannel ,
  GripArmChannel ,
  GripChannel ,
  DataPanelChannel
};

int val = 0;
int prevmillis = 0;



void ServoSetup() {
  // attach servos to pins
  //  ServoDome.mServo.attach(DomePin);
  //  ServoUtilityArmTop.mServo.attach(UtilityArmTopPin);
  //  ServoUtilityArmBot.mServo.attach(UtilityArmBotPin);
  //  ServoIntArmDoor.mServo.attach(IntArmDoorPin);
  //  ServoIntArm.mServo.attach(IntArmPin);
  //  ServoInterface.mServo.attach(InterfacePin);
  //  ServoGripArmDoor.mServo.attach(GripArmDoorPin);
  //  ServoGripArm.mServo.attach(GripArmPin);
  //  ServoGrip.mServo.attach(GripPin);
  //  ServoDataPanel.mServo.attach(DataPanelPin);

  AttachAllServos();
  ServoList[DOME].mServo.attach(DomePin);


  ServoList[DOME].mServo.writeMicroseconds(ServoDome.center_usec);

  ServoList[UTILITYARMTOP].mServo.setEasingType(EASE_CUBIC_IN_OUT);
  ServoList[UTILITYARMBOT].mServo.setEasingType(EASE_CUBIC_IN_OUT);
  ServoList[INTARM].mServo.setEasingType(EASE_CUBIC_IN_OUT);

  setSpeedForAllServos(20);

  HomeServos();
}

void ServoLoop() {
  if (millis() - prevmillis >= 20) {
    //ServoPassthrough();
    //  DomeLoop();

    switch (ServoState) {
      // All Servos Off
      case ServoPassThrough:
        //       Serial.println("in servo passthrough state");
        //ServoPassthrough();
        DetachAll();
        if (ChannelData(12) > 1000) {
          ServoState = ServoSequences;
          AttachAllServos();
          HomeServos();
        }
        break;

      case ServoSequences:
        //        Serial.println("in sequence state");
        SequenceLoop();
        if (ChannelData(12) < 1000) ServoState = ServoPassThrough;
        break;
      default:
        // statements
        ServoState = ServoPassThrough;
        break;
    }
    UpdateServos();
    prevmillis = millis();
  }
}
void ServoPassthrough() {
  if (SBUSOK()) {
    AttachAllServos();
    for (uint8_t x = 1; x < 9; x++) {
      ServoList[x].target_position = map(channels[ChannelList[x]], 172, 1811, 500, 2500) ;
#if 0
      Serial.print("Clist: "); Serial.print(ChannelList[x]);
      Serial.print(" x "); Serial.println(channels[ChannelList[x]]);
#endif
    }
  }
  else {
    DetachAll();
    //    Serial.println("sbus bad, setting to analog write 0");
    //    analogWrite( UtilityArmTopPin, 0);
    //    analogWrite( UtilityArmBotPin , 0);
    //    analogWrite( IntArmDoorPin , 0);
    //    analogWrite( IntArmPin , 0);
    //    analogWrite( InterfacePin , 0);
    //    analogWrite( GripArmDoorPin , 0);
    //    analogWrite( GripArmPin , 0);
    //    analogWrite( GripPin , 0);
    //    analogWrite( DataPanelPin , 0);
    //}
  }
}

void DomeLoop() {
  float easing = 0.02;
  float Target;
  float dx;

  ServoCurrentMillis = millis();
  if (millis() - prevmillis >= 2) {
    if (SBUSOK()) {
      Target = channels[3];
      // If crossing center, break at center first
      if ((ServoDome.current_position < SBUSCENTER - SBUSDEADBAND) && (Target >= SBUSCENTER)) {
        Target = SBUSCENTER;
      }
      if ((ServoDome.current_position > SBUSCENTER + SBUSDEADBAND) && (Target <= SBUSCENTER)) {
        Target = SBUSCENTER;
      }

      dx = Target - ServoDome.current_position;
      ServoDome.current_position += dx * easing;
      //
      //      Serial.print(channels[3]);
      //      Serial.print(" ");
      //      Serial.print(Target);
      //      Serial.print(" ");
      //      Serial.print(dx);
      //      Serial.print(" ");
      //      Serial.print(ServoDome.current_position);
      //      Serial.print(" ");
      //val = channels[3];                      // read SBUS channel 1
      if (ServoDome.current_position < SBUSCENTER) {
        val = map(ServoDome.current_position, SBUSMIN, SBUSCENTER, ServoDome.min_usec, ServoDome.center_usec);     // map SBUS to usec
      }
      else {
        val = map(ServoDome.current_position, SBUSCENTER, SBUSMAX, ServoDome.center_usec, ServoDome.max_usec);     // map SBUS to usec
      }
      //      Serial.print(val);
      //      Serial.println(" ");
      ServoDome.mServo.writeMicroseconds(val);            // sets the servo position according to the scaled value

    }
    else {
      Target = 988;
      easing = 0.1;
      dx = Target - ServoDome.current_position;
      ServoDome.current_position += dx * easing;
      val = map(ServoDome.current_position, 172, 1811, ServoDome.min_usec, ServoDome.max_usec);     // map SBUS to usec
      ServoDome.mServo.writeMicroseconds(val);            // if SBUS not ok, stop dome
      //    Serial.println("SBUS FAIL: ");
      //    Serial.print(ServoDome.center_usec);
      //    Serial.println(" ");
    }
    prevmillis = millis();
  }
}

void UpdateServos() {

  CheckLimits();
  for (uint8_t x = 1; x < 9; x++) {
    //    Serial.print(" t ");
    //    Serial.print(x);
    //    Serial.print(": ");
    //    Serial.print(ServoList[x].target_position);

    //    ServoList[x].target_position = random(1000,2000);
    ServoList[x].mServo.writeMicroseconds(ServoList[x].target_position);
  }
  //    Serial.println();
}

void CheckLimits() {
  for (uint8_t x = 1; x < 9; x++) {
    if (ServoList[x].target_position > ServoList[x].max_usec) ServoList[x].target_position = ServoList[x].max_usec ;
    if (ServoList[x].target_position < ServoList[x].min_usec) ServoList[x].target_position = ServoList[x].min_usec ;
  }
}

void HomeServos() {
  for (uint8_t x = 1; x < 9; x++) {
    ServoList[x].mServo.writeMicroseconds(ServoList[x].center_usec);
  }
}


void DetachAll() {
  for (uint8_t x = 1; x < 9; x++) {
    ServoList[x].mServo.detach();
    ServoList[x].isAttached = 0;
  }
}

void AttachAllServos() {
  if (!ServoList[UTILITYARMTOP].isAttached) ServoList[UTILITYARMTOP].mServo.attach(UtilityArmTopPin);
  if (!ServoList[UTILITYARMBOT].isAttached) ServoList[UTILITYARMBOT].mServo.attach(UtilityArmBotPin);
  if (!ServoList[INTARMDOOR].isAttached) ServoList[INTARMDOOR].mServo.attach(IntArmDoorPin);
  if (!ServoList[INTARM].isAttached) ServoList[INTARM].mServo.attach(IntArmPin);
  if (!ServoList[INTERFACE].isAttached) ServoList[INTERFACE].mServo.attach(InterfacePin);
  if (!ServoList[GRIPARMDOOR].isAttached) ServoList[GRIPARMDOOR].mServo.attach(GripArmDoorPin);
  if (!ServoList[GRIPARM].isAttached) ServoList[GRIPARM].mServo.attach(GripArmPin);
  if (!ServoList[GRIPPER].isAttached) ServoList[GRIPPER].mServo.attach(GripPin);
  if (!ServoList[DATAPPANELDOOR].isAttached) ServoList[DATAPPANELDOOR].mServo.attach(DataPanelPin);

  for (uint8_t x = 1; x < 9; x++) {

    ServoList[x].isAttached = 1;
  }

}
// Utility Arm State Machine
void UtilityArmLoop() {

}

bool OpenServo(int tServo) {

}

// Open top Utility Arm
// return true when open
bool OpenUtilityArmTop() {
  if (ServoUtilityArmTop.mServo.readMicroseconds() < ServoUtilityArmTop.max_usec) {
    ServoUtilityArmTop.mServo.writeMicroseconds(ServoUtilityArmTop.max_usec);
    return false;
  } else return true;
}

// Close top Utility Arm
// return true when closed
bool CloseUtilityArmTop() {
  ServoUtilityArmTop.mServo.writeMicroseconds(ServoUtilityArmTop.min_usec);
  return false;
}

// Open top Utility Arm
// return true when open
bool OpenUtilityArmBot() {
  if (ServoUtilityArmTop.mServo.readMicroseconds() < ServoUtilityArmTop.max_usec) {
    ServoUtilityArmTop.mServo.writeMicroseconds(ServoUtilityArmTop.max_usec);
    return false;
  } else return true;
}

// Close top Utility Arm
// return true when closed
bool CloseUtilityArmBot() {
  ServoUtilityArmTop.mServo.writeMicroseconds(ServoUtilityArmTop.min_usec);
  return false;
}
