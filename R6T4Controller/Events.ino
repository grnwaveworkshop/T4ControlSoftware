// Radio Event triggering
// Mapping radio switches etc. to functions.


#include "definitions.h"
uint8_t RunMode = 0;
uint16_t ButtonState = 0;
bool ButtonPressed = 0;

int ButtonCount = 0;

extern bool bDataDoorOpen;
extern uint8_t DataPanelDoorState;
unsigned long EventMillis = millis();

void ProcessEventsLoop() {
  if (millis() - EventMillis > 100) {
    ProcessButtons();
    UpdateVolume();
    EventMillis = millis();
  }
}


// Function to read the button channel, and assign a button # to it based on the nominal value + a buffer either side.
uint16_t GetButtonState() {
  //Serial.println(ChannelData(ButtonSwitch));
  return ChannelData(ButtonSwitch);
}

// Function to determine which button was pressed and run the function associated with the click
void ProcessButtons() {
  switch (GetButtonState()) {
    case (ButtonOff - ButtonDeadband) ... (ButtonOff + ButtonDeadband):
      ButtonPressed = 0;
      ButtonCount = 0;
      //      Serial.println("Buttons Off");
      break;
    case (CButtonA1 - ButtonDeadband) ... (CButtonA1 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button A1 pressed");
        ButtonA1Click();
        ButtonPressed = 1;
      }
 //     Serial.println(ButtonCount);
      ButtonCount++;
      if (ButtonCount == LongPressCount) {
        ButtonA1LongClick();
      }
      break;
    case (CButtonA2 - ButtonDeadband) ... (CButtonA2 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button A2 pressed");
        ButtonA2Click();
        ButtonPressed = 1;
      }
      break;
    case (CButtonA3 - ButtonDeadband) ... (CButtonA3 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button A3 pressed");
        ButtonA3Click();
        ButtonPressed = 1;
      }
      break;
    case (CButtonA4 - ButtonDeadband) ... (CButtonA4 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button A4 pressed");
        ButtonA4Click();
        ButtonPressed = 1;
      }
      break;
    case (CButtonB1 - ButtonDeadband) ... (CButtonB1 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button B1 pressed");
        ButtonPressed = 1;
        ButtonB1Click();
      }
      break;
    case (CButtonB2 - ButtonDeadband) ... (CButtonB2 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button B2 pressed");
        ButtonPressed = 1;
        ButtonB2Click();
      }
      break;
    case (CButtonB3 - ButtonDeadband) ... (CButtonB3 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button B3 pressed");
        ButtonPressed = 1;
        ButtonB3Click();
      }
      break;
    case (CButtonB4 - ButtonDeadband) ... (CButtonB4 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button B4 pressed");
        ButtonPressed = 1;
        ButtonB4Click();
      }
      break;
    case (CButtonC1 - ButtonDeadband) ... (CButtonC1 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button C1 pressed");
        ButtonPressed = 1;
        ButtonC1Click();
      }
      break;
    case (CButtonC2 - ButtonDeadband) ... (CButtonC2 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button C2 pressed");
        ButtonPressed = 1;
        ButtonC2Click();
      }
      break;
    case (CButtonC3 - ButtonDeadband) ... (CButtonC3 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button C3 pressed");
        ButtonPressed = 1;
        ButtonC3Click();
      }
      break;
    case (CButtonC4 - ButtonDeadband) ... (CButtonC4 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button C4 pressed");
        ButtonPressed = 1;
        ButtonC4Click();
      }
      break;
    case (CButtonD1 - ButtonDeadband) ... (CButtonD1 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button D1 pressed");
        ButtonPressed = 1;
        ButtonD1Click();
      }
      break;
    case (CButtonD2 - ButtonDeadband) ... (CButtonD2 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button D2 pressed");
        ButtonPressed = 1;
        ButtonD2Click();
      }
      break;
    case (CButtonD3 - ButtonDeadband) ... (CButtonD3 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button D3 pressed");
        ButtonPressed = 1;
        ButtonD3Click();
      }
      break;
    case (CButtonD4 - ButtonDeadband) ... (CButtonD4 + ButtonDeadband):
      if (!ButtonPressed) {
        Serial.println("button D4 pressed");
        ButtonPressed = 1;
        ButtonD4Click();
      }
      break;
    default:
      //Serial.println("default button state");
      break;
  }
}

void ButtonA1Click() {
  DFPlayer.pause();
}

void ButtonA2Click() {

  CurrentTrack = random(1, NumTracksFolder[1]);
  DFPlayer.playFolder(1, CurrentTrack);
}
void ButtonA3Click() {
  CurrentTrack = random(1, NumTracksFolder[2]);
  DFPlayer.playFolder(2, CurrentTrack);
}
void ButtonA4Click() {
  CurrentTrack = random(1, NumTracksFolder[3]);
  DFPlayer.playFolder(3, CurrentTrack);
}

void ButtonB1Click() {

}
void ButtonB2Click() {

}
void ButtonB3Click() {

}
void ButtonB4Click() {

}

void ButtonC1Click() {
    PSIFrontOff();
}
void ButtonC2Click() {

}
void ButtonC3Click() {

}
void ButtonC4Click() {

}

void ButtonD1Click() {
  if (bDataDoorOpen) { // door open, close it
    DataPanelDoorState = 4;
    bDataDoorOpen = 0;
  } else { // Door Closed, open it
    DataPanelDoorState = 1;
    bDataDoorOpen = 1;
  }
}
void ButtonD2Click() {
  OpenUtilityArmBot() ;
  OpenUtilityArmTop();
}
void ButtonD3Click() {
  CloseUtilityArmBot();
  CloseUtilityArmTop();
}
void ButtonD4Click() {
  nextLEDPattern();
}


void ButtonA1LongClick() {
  Serial.println("Button A1 Long Click");

}
