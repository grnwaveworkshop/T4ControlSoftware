
//#include <Servo.h>
#include "ServoEasing.h"

unsigned long ServoCurrentMillis = millis();

uint8_t ServoState = 0;

// Define struct to contain the servo and parameters
struct BodyServo {
	ServoEasing mServo;
	int min_usec;
	int max_usec;
	int center_usec;
	float current_position;
	float target_position;
	bool isAttached;
};

// Define the Servo
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



// create servo objects,  Set endpoints, center point here.
//                                Servo ,  min_usec, max_usec, center_usec, current_position, target_position, isAttached
BodyServo ServoDome = { Dome ,          1000 , 2000, 1500, 1500, 1500, 0 };
BodyServo ServoUtilityArmTop = { UtilityArmTop , 750  , TopUtilityClosed, 2100, 2100, 2100, 0 };   // 2150 = closed
BodyServo ServoUtilityArmBot = { UtilityArmBot , 650  , 1950, 1950, 1950, 1950, 0 };   // 1950 = closed
BodyServo ServoIntArmDoor = { IntArmDoor ,    1250 , 1950, 1900, 1900, 1900, 0 };   // 1950 = closed
BodyServo ServoIntArm = { IntArm ,        830  , 2200, 830 , 830 , 830 , 0 };   // 830  = down
BodyServo ServoInterface = { Interface ,     800  , 2200, 800 , 800 , 800 , 0 };   // 800  = in
BodyServo ServoGripArmDoor = { GripArmDoor ,   1150 , 1850, 1150, 1200, 1200, 0 };   // 1150 = closed
BodyServo ServoGripArm = { GripArm ,       800  , 2075, 2075, 2075, 2075, 0 };   // 2075 = down
BodyServo ServoGrip = { Grip ,          1150 , 1500, 1450, 1450, 1450, 0 };   // 1500 = closed
BodyServo ServoDataPanel = { DataPanel ,     1125 , 1825, 1125, 1125, 1125, 0 };   // 1125 = closed

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

uint8_t ChannelList[] = {
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
uint16_t ServoMovingTimer = 0;
bool isServoMoving = false;




void ServoSetup() {

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
	// Process look if time expired
	if (millis() - prevmillis >= 25) {
		//ServoPassthrough();
		//  DomeLoop();
		if (SBUSOK()) {
			if (ChannelData(SERVOENSWITCH) < 1000) {
				// All Servos Off
				UpdateServos();

				DetachAll();
				//    Serial.println(" Servos Off ");
			}
			else {
				//          Serial.println(" Servos On ");
				AttachAllServos();
				DomeLoop();
				SequenceLoop();
				UpdateServos();
			}
			prevmillis = millis();
		}
		else {
			// Serial.println("***  SBUS BAD - Servos off ***");
			DetachAll();
		}
	}

}


void ServoPassthrough() {
	if (SBUSOK()) {
		AttachAllServos();
		for (uint8_t x = 1; x < 9; x++) {
			ServoList[x].target_position = map(channels[ChannelList[x]], 172, 1811, 500, 2500);
#if 0
			Serial.print("Clist: "); Serial.print(ChannelList[x]);
			Serial.print(" x "); Serial.println(channels[ChannelList[x]]);
#endif
		}
	}
	else {
		DetachAll();

	}
}

void DomeLoop() {
	float easing = 0.1;
	float Target;
	float dx;

	ServoCurrentMillis = millis();
	if (millis() - prevmillis >= 2) {
		if (SBUSOK()) {
			Target = channels[DomeChannel];
			// If crossing center, break at center first
			if ((ServoDome.current_position < SBUSCENTER - SBUSDEADBAND) && (Target >= SBUSCENTER)) {
				Target = SBUSCENTER;
			}
			if ((ServoDome.current_position > SBUSCENTER + SBUSDEADBAND) && (Target <= SBUSCENTER)) {
				Target = SBUSCENTER;
			}

			// Serial.print("Dome Target: ");
			// Serial.println(Target);

			dx = Target - ServoDome.current_position;
			ServoDome.current_position += dx * easing;

			if (ServoDome.current_position < SBUSCENTER) {
				val = map(ServoDome.current_position, SBUSMIN, SBUSCENTER, ServoDome.min_usec, ServoDome.center_usec);     // map SBUS to usec
			}
			else {
				val = map(ServoDome.current_position, SBUSCENTER, SBUSMAX, ServoDome.center_usec, ServoDome.max_usec);     // map SBUS to usec
			}

			ServoDome.mServo.writeMicroseconds(val);            // sets the servo position according to the scaled value
			//Serial.print("Dome val: ");
		   // Serial.println(val);
		}
		else {
			Target = 988;
			easing = 0.1;
			dx = Target - ServoDome.current_position;
			ServoDome.current_position += dx * easing;
			val = map(ServoDome.current_position, 172, 1811, ServoDome.min_usec, ServoDome.max_usec);     // map SBUS to usec
			ServoDome.mServo.writeMicroseconds(val);            // if SBUS not ok, stop dome

		}
		prevmillis = millis();
	}
}

void UpdateServos() {

	CheckLimits();
	for (uint8_t x = 1; x <= 9; x++) {
#if 0		
		Serial.print(" t ");
		Serial.print(x);
		Serial.print(": ");
		Serial.print(ServoList[x].target_position);
		Serial.print(" : ");
		Serial.print(ServoList[x].current_position);
		Serial.println();
#endif
		if (ServoList[x].current_position != ServoList[x].target_position) {
			AttachServo(x);
			//AttachAllServos();
			ServoList[x].mServo.writeMicroseconds(ServoList[x].target_position);

		//	ServoList[x].mServo.startEaseToD(ServoList[x].mServo.MicrosecondsOrUnitsToDegree(ServoList[x].target_position),1500,true);
			ServoList[x].current_position = ServoList[x].target_position;
			isServoMoving = true;
			ServoMovingTimer = SERVOTIMEOUT;
		}
		else {
			//	isServoMoving = false;
		}
	}
	if (isServoMoving) {
		if (ServoMovingTimer > 0) ServoMovingTimer--;
	}
	if (ServoMovingTimer == 0) {
		isServoMoving = false;
		DetachAll();
	}
	//Serial.print("ServoMovingTimer : ");
	//Serial.println(ServoMovingTimer);
}

void CheckLimits() {
	for (uint8_t x = 1; x <= 9; x++) {
		if (ServoList[x].target_position > ServoList[x].max_usec) ServoList[x].target_position = ServoList[x].max_usec;
		if (ServoList[x].target_position < ServoList[x].min_usec) ServoList[x].target_position = ServoList[x].min_usec;
	}
}

// Home Servos - set the servos to center
void HomeServos() {
	for (uint8_t x = 1; x <= 9; x++) {
		ServoList[x].mServo.writeMicroseconds(ServoList[x].center_usec);
	}
}

void DetachAll() {
	for (uint8_t x = 1; x <= 9; x++) {
		if (ServoList[x].isAttached == 1) {
			ServoList[x].mServo.detach();
			ServoList[x].isAttached = 0;
		}
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

	for (uint8_t x = 1; x <= 9; x++) {

		ServoList[x].isAttached = 1;
		ServoList[x].mServo.writeMicroseconds(ServoList[x].target_position);
	}

}


void AttachServo(uint8_t ServoNum) {

	switch (ServoNum)
	{
	case 1:
		if (!ServoList[UTILITYARMTOP].isAttached) ServoList[UTILITYARMTOP].mServo.attach(UtilityArmTopPin);

		break;
	case 2:
		if (!ServoList[UTILITYARMBOT].isAttached) ServoList[UTILITYARMBOT].mServo.attach(UtilityArmBotPin);
		break;
	case 3:
		if (!ServoList[INTARMDOOR].isAttached) ServoList[INTARMDOOR].mServo.attach(IntArmDoorPin);
		break;
	case 4:
		if (!ServoList[INTARM].isAttached) ServoList[INTARM].mServo.attach(IntArmPin);
		break;
	case 5:
		if (!ServoList[INTERFACE].isAttached) ServoList[INTERFACE].mServo.attach(InterfacePin);
		break;
	case 6:
		if (!ServoList[GRIPARMDOOR].isAttached) ServoList[GRIPARMDOOR].mServo.attach(GripArmDoorPin);
		break;
	case 7:
		if (!ServoList[GRIPARM].isAttached) ServoList[GRIPARM].mServo.attach(GripArmPin);
		break;
	case 8:
		if (!ServoList[GRIPPER].isAttached) ServoList[GRIPPER].mServo.attach(GripPin);
		break;
	case 9:
		if (!ServoList[DATAPPANELDOOR].isAttached) ServoList[DATAPPANELDOOR].mServo.attach(DataPanelPin);
		break;
	default:
		break;
	}

	ServoList[ServoNum].isAttached = 1;
	ServoList[ServoNum].mServo.writeMicroseconds(ServoList[ServoNum].target_position);

}




// Open top Utility Arm
void OpenUtilityArmTop() {
	ServoList[UTILITYARMTOP].target_position = TopUtilityOpen;
}

// Close top Utility Arm
void CloseUtilityArmTop() {
	ServoList[UTILITYARMTOP].target_position = TopUtilityClosed;
}

// Open Bottom Utility Arm
void OpenUtilityArmBot() {

	ServoList[UTILITYARMBOT].target_position = BotUtilityOpen;

}

// Close Bottom Utility Arm
void CloseUtilityArmBot() {

	ServoList[UTILITYARMBOT].target_position = BotUtilityClosed;

}
