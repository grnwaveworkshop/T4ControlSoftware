// Run Modes
#define RunIdle 1
#define RunMotion 2


// Audio Defines
// Audio States
#define AudioIdle 0
#define AudioPlaying 1

// SBUS defines
#define SBUSCENTER 988
#define SBUSDEADBAND 10
#define SBUSMIN 172
#define SBUSMAX 1811
#define MAXSBUSOK 200

// Servo States
#define ServoPassThrough 0
#define ServoSequences 1

//define channels
#define AUDIOSWITCH 15
#define AUDIOFOLDER 14

#define NUMAUDIOFOLDERS 12

// -------- Define Colors ----------
//#define RED    0xFF0000
//#define GREEN  0x00FF00
//#define BLUE   0x0000FF
//#define YELLOW 0xFFFF00
//#define PINK   0xFF1088
//#define ORANGE 0xE05800
//#define WHITE  0xFFFFFF

// Less intense...

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x222200 //101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x222222


// Display Pin Definitions
#define TFT_MISO  12
#define TFT_MOSI  11
#define TFT_SCK   13
#define TFT_DC   7
#define TFT_CS   10
#define TFT_RST  8
#define TFT_BLK 6

#define SD_CS     BUILTIN_SDCARD  // CS for SD card, can use any pin

#define BUFFPIXEL 2

// Analog Input Definitions
#define AUDIN 7

// Servo Time out.  Time to wait to detach servos. 
#define SERVOTIMEOUT 35


// Servo Endpoint Definitions
#define TopUtilityClosed 2300
#define TopUtilityOpen 750
#define BotUtilityClosed 1950
#define BotUtilityOpen 550

#define IntArmdoorClosed 1950
#define IntArmdoorOpen 1250 
#define IntArmClosed 830
#define IntArmOpen 2200
#define InterfaceClosed 800
#define InterfaceOpen 2200

#define GripArmDoorClosed 1150
#define GripArmDoorOpen 1850
#define GripArmClosed 2075
#define GripArmOpen 800
#define GripperClosed 1500
#define GripperOpen 1150

#define DataPanelDoorClosed 1125
#define DataPanelDoorOpen 1600


// Servo reference number
// Map of name to reference in array
#define DOME 0
#define UTILITYARMTOP 1
#define UTILITYARMBOT 2
#define INTARMDOOR 3
#define INTARM 4
#define INTERFACE 5
#define GRIPARMDOOR 6
#define GRIPARM 7
#define GRIPPER 8
#define DATAPPANELDOOR 9

// Passthrough Servo Channel mapping
// Not used...
#define DomeChannel 0
#define UtilityArmTopChannel 10
#define UtilityArmBotChannel 6
#define IntArmDoorChannel 12
#define IntArmChannel 4
#define InterfaceChannel 9
#define GripArmDoorChannel 7
#define GripArmChannel 8
#define GripChannel 5
#define DataPanelChannel 13

// Radio Switch Channel Definitions = Radio channel - 1
// Set to the Radio channel desired for each switch function
#define BUTTONSWITCH 3	// 16 button channel
#define SERVOENSWITCH 4	//	Switch used to enable the body servos
#define INTARMSWITCH 5		// Switch used to control the Interface Arm
#define GRIPARMSWITCH 6		// Switch used to control the gripper arm
//#define DataPanelSwitch 9
//#define UtilityArmTopSwitch 10
//#define UtilityArmBotSwitch 11
#define DRIVEENABLE 7	// Switch that activates the Dome and drive motors
#define AUDIOVOLUME 10	// Audio volume control
#define BUTTONBANK 9	// Switch to change the bank of buttom functions

// Servo output pin mapping
#define DomePin 24            // Servo 1
#define UtilityArmTopPin 25   // Servo 2
#define UtilityArmBotPin 26   // Servo 3
#define IntArmDoorPin 27      // Servo 4
#define IntArmPin 28          // Servo 5
#define InterfacePin 29       // Servo 6
#define GripArmDoorPin 30     // Servo 7
#define GripArmPin 31         // Servo 8
#define GripPin 32            // Servo 9
#define DataPanelPin 33       // Servo 10
//#define DataPanelPin 36          // Servo 11

// Switch Matrix button center points SBUS Values
// 4x4 Switch Matrix
#define ButtonOff 1811
#define CButtonA1 172
#define CButtonA2 272
#define CButtonA3 381
#define CButtonA4 487
#define CButtonB1 590
#define CButtonB2 693
#define CButtonB3 794
#define CButtonB4 896
#define CButtonC1 992
#define CButtonC2 1089
#define CButtonC3 1187
#define CButtonC4 1283
#define CButtonD1 1380
#define CButtonD2 1478
#define CButtonD3 1575
#define CButtonD4 1676

#define ButtonDeadband 5
#define LongPressCount 8
