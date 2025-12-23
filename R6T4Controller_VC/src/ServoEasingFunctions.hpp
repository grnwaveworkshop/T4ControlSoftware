#ifndef SERVOEASINGFUNCTIONS_HPP
#define SERVOEASINGFUNCTIONS_HPP

#include <Arduino.h>
// Only include the header declarations, not the implementation
#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>

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

extern BodyServo ServoList[];
extern uint8_t ChannelList[];

void ServoSetup();
void ServoLoop();
void ServoPassthrough();
void DomeLoop();
void UpdateServos();
void CheckLimits();
void HomeServos();
void DetachAll();
void AttachAllServos();
void AttachServo(uint8_t ServoNum);
void OpenUtilityArmTop();
void CloseUtilityArmTop();
void OpenUtilityArmBot();
void CloseUtilityArmBot();

#endif