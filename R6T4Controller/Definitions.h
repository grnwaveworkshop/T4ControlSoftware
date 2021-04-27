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

#define NUMAUDIOFOLDERS 20

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
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x555555


//Display Definitions
#define TFT_MISO  12
#define TFT_MOSI  11
#define TFT_SCK   13
#define TFT_DC   7
#define TFT_CS   10
#define TFT_RST  8

#define SD_CS     BUILTIN_SDCARD  // CS for SD card, can use any pin

#define BUFFPIXEL 2

// Analog Input Definitions
#define AUDIN 7

// Servo reference number
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

// Servo Channels
#define DomeChannel 3
#define UtilityArmTopChannel 10
#define UtilityArmBotChannel 6
#define IntArmDoorChannel 12
#define IntArmChannel 4
#define InterfaceChannel 9
#define GripArmDoorChannel 7
#define GripArmChannel 8
#define GripChannel 5
#define DataPanelChannel 13

#define IntArmSwitch 7
#define GripArmSwitch 5
#define DataPanelSwitch 9
#define UtilityArmTopSwitch 10
#define UtilityArmBotSwitch 11

#define DomePin 24
#define UtilityArmTopPin 25
#define UtilityArmBotPin 26
#define IntArmDoorPin 27
#define IntArmPin 28
#define InterfacePin 29
#define GripArmDoorPin 30
#define GripArmPin 31
#define GripPin 32
#define DataPanelPin 33
