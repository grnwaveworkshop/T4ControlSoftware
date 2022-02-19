// RS485 Serial Port Functions
// 485 used Serial Port 4
// TX4 - D17
// RX4 - D16
// RTS4 - D41
// RTS sets the Direction for half duplex operation.  Low - Receive Mode, High - Transmit

#include <EasyTransfer.h>

#define LOGICLEDADDRESS 1   // Logic Display Address
#define PSIFRONTADDRESS 4   // PSI Front Address
#define SMOKE1ADDRESS 11    // Front Smoke Address
#define PIESERVOADDRESS 20  // Pie Servo Controller Address
#define HOLOFRONTADDRESS 7   // PSI Front Address
#define HOLOREARADDRESS 7   // PSI Rear Address


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*PSIPatternList[])();
PSIPatternList gPSIPatterns = {
  PSIFrontOff,		// 0
  PSIAudioPattern,	// 1
  PSIShortCircuit,	// 2
  PSIRedLight,		// 3
  PSIGreenLight,	// 4
  PSIRedBlueSwirl,	// 5
  PSIRadar,			// 6
  PSIHeart,			// 7
};
byte PSIMode = 1;		// Index to the PSI Pattern List  


//create object
EasyTransfer DataOut;

struct SEND_DATA_STRUCTURE {
	//Send Data Structure - Must match Receive Structure
	uint16_t address = 99; // Device address
	uint16_t command = 'z'; // Command to execute
	uint16_t Data1 = 0;
	uint16_t Data2 = 0;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mysenddata;

#define Serial485 Serial4
#define RTS4 41

char rx_byte = 0;

unsigned long SerialMillis = millis();

void Setup485() {
	Serial485.begin(9600);
	Serial485.transmitterEnable(RTS4);

	Serial.println("Setup 485");
	Serial485.println("Setup 485");

	DataOut.begin(details(mysenddata), &Serial485);   // Use Hardware Serial

}

void Serialloop() {
	if (millis() - SerialMillis > 25) {
		if (Serial.available() > 0) {    // is a character available?
			rx_byte = Serial.read();       // get the character

			// check if a number was received
			if ((rx_byte >= '0') && (rx_byte <= '9')) {
				Serial.print("Number received: ");
				Serial.println(rx_byte);
				if (rx_byte == '0') {
					PSIFrontOff();

				}
				if (rx_byte == '9') {
					PSIFrontNextPattern();

				}
			}
			else {
				Serial.println("Not a number.");
			}
		} // end: if (Serial.available() > 0)
		SerialMillis = millis();
		ProcessPSI();

	}
}

// ***********************************
// ***** Global Dome Functions ******
// ***********************************
void AllDomeLEDsOff() {
	mysenddata.address = 0; // broadcast all
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();
	SetPSIMode(0);

}

void AllDomeLEDsOn() {
	mysenddata.address = 0; // broadcast all
	mysenddata.command = 'T';
	mysenddata.Data1 = 1;

	DataOut.sendData();
	SetPSIMode(1);
	HolosRandom();
}

// ***********************************
// ***** Logic Display Functions *****
// ***********************************

void LogicOff() {
	Serial.println("Logic Off");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();

}
void LogicShortCircuit() {
	Serial.println("Logic Short Circuit");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 10;

	DataOut.sendData();
}

void LogicMatrixDrop() {
	Serial.println("Logic Matrix Drop");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 8;

	DataOut.sendData();
}

void LogicTwinkleRed() {
	Serial.println("Logic Twinkle Red");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 2;

	DataOut.sendData();
}

void LogicNextPattern() {
	Serial.println("Logic Next Pattern");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 99;

	DataOut.sendData();
}

void LogicNextPatternDelay(uint16_t delaytime) {
	Serial.println("Logic Next Pattern Delay");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 99;

	DataOut.sendData();
}

void TetrisRotate() {
	Serial.println("Tetris Rotate");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'Z';
	mysenddata.Data1 = 'w';

	DataOut.sendData();
}
void TetrisLeft() {
	Serial.println("Tetris Left");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'Z';
	mysenddata.Data1 = 'a';

	DataOut.sendData();
}
void TetrisRight() {
	Serial.println("Tetris Right");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'Z';
	mysenddata.Data1 = 'd';

	DataOut.sendData();
}
void TetrisDrop() {
	Serial.println("Tetris Drop");
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'Z';
	mysenddata.Data1 = 's';

	DataOut.sendData();
}

void TetrisStart() {
	mysenddata.address = LOGICLEDADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 88;

	DataOut.sendData();
	
}
// ***********************************
// ******** PSI Functions ************
// ***********************************

void ProcessPSI() {

	gPSIPatterns[PSIMode]();

}

void SetPSIMode(byte mode) {
	PSIMode = mode;
}

byte GetPSIMode() {
	return PSIMode;
}

void PSIFrontOff() {
	//	Serial.println("PSIFrontOff");
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();
	PSIMode = 0;
}

void PSIFrontNextPattern() {
	Serial.println((sizeof(gPSIPatterns) / 4));
	Serial.println(PSIMode);
	if (PSIMode < (sizeof(gPSIPatterns) / 4) - 1 ){
		PSIMode++;
	}
	else	PSIMode = 1;
}

void PSIAudioPattern() {
	//	Serial.println("PSI Audio");
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 92;
	mysenddata.Data2 = AudioLevel;

	DataOut.sendData();
	PSIMode = 1;
}

void PSIShortCircuit() {
	//Serial.println("PSI Short Circuit");
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 4;

	DataOut.sendData();
	PSIMode = 2;
}

void PSIGreenLight() {

	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 18;

	DataOut.sendData();
	PSIMode = 4;
}

void PSIRedLight() {

	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 17;

	DataOut.sendData();
	PSIMode = 3;
}

void PSIRadar() {
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 8;

	DataOut.sendData();
	PSIMode = 6;
}

void PSIRedBlueSwirl() {
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 7;

	DataOut.sendData();
	PSIMode = 5;
}

void PSIHeart() {
	mysenddata.address = PSIFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 9;

	DataOut.sendData();
	PSIMode = 7;
}

	void PSIConfetti() {
		mysenddata.address = PSIFRONTADDRESS;
		mysenddata.command = 'T';
		mysenddata.Data1 = 11;

		DataOut.sendData();
		PSIMode = 7;
	}

// ***********************************
// ******** Holo Functions ************
// ***********************************

void HolosOff() {
	Serial.println("Holos Off");
	mysenddata.address = HOLOFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();
}

void HolosRandom() {
	Serial.println("Holos Random");
	mysenddata.address = HOLOFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 1;

	DataOut.sendData();
}

void HolosBright() {
	Serial.println("Holos Bright");
	mysenddata.address = HOLOFRONTADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 2;

	DataOut.sendData();
}

// *******************************************************
// ***********  Smoke Generator Functions  ***************
// *******************************************************

void SmokeOff() {
	Serial.println("SmokeOff");
	mysenddata.address = SMOKE1ADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();

}

void SmokeOn(uint16_t SmokeOnTime) {
	Serial.println("SmokeOn");
	mysenddata.address = SMOKE1ADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 1;
	mysenddata.Data2 = SmokeOnTime;

	DataOut.sendData();

}


// ***********************************************
// ***********  Pie Panel Control  ***************
// ***********************************************

// Function to set the speed of the servos (degrees / sec)
void PieSetServoSpeed(uint16_t ServoSpeed) {
	Serial.print("Set Pie Servo Speed: ");
	Serial.println(ServoSpeed);

	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'P';
	mysenddata.Data1 = ServoSpeed;

	DataOut.sendData();
}

// Open all the Pie Servos
void PieOpenAll() {
	Serial.println("Pie Servo Open All");
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 1;

	DataOut.sendData();
}

void PieCloseAll() {
	Serial.println("Pie Servo Close All");
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 0;

	DataOut.sendData();
}

// Open all the Pie Servos sequencially
void PieOpenSequencial() {
	Serial.println("Pie Servo Open Sequencial");
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 2;

	DataOut.sendData();
}

// Close all the Pie Servos sequencially
void PieCloseSequencial() {
	Serial.println("Pie Servo Close Sequencial");
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 3;

	DataOut.sendData();
}

// Close all the Pie Servos sequencially
void PieOpenClose3(uint16_t NumCycles) {
	Serial.println("Pie Servo Open Close 3");
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 6;
	mysenddata.Data2 = NumCycles;

	DataOut.sendData();
}

// Open a single pie panel
void PieOpenSingle(uint16_t pie_num) {
	Serial.print("Pie Servo Open Single: ");
	Serial.println(pie_num);
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 4;
	mysenddata.Data2 = pie_num;
	DataOut.sendData();
}

// Close a single pie panel
void PieCloseSingle(uint16_t pie_num) {
	Serial.print("Pie Servo Close Single: ");
	Serial.println(pie_num);
	mysenddata.address = PIESERVOADDRESS;
	mysenddata.command = 'T';
	mysenddata.Data1 = 5;
	mysenddata.Data2 = pie_num;
	DataOut.sendData();
}

void PieOpen0() {
	PieOpenSingle(0);
}

void PieOpen1() {
	PieOpenSingle(1);
}

void PieOpen2() {
	PieOpenSingle(2);
}

void PieOpen3() {
	PieOpenSingle(3);
}

void PieOpen4() {
	PieOpenSingle(4);
}

void PieOpen5() {
	PieOpenSingle(5);
}

void PieClose0() {
	PieCloseSingle(0);
}

void PieClose1() {
	PieCloseSingle(1);
}

void PieClose2() {
	PieCloseSingle(2);
}

void PieClose3() {
	PieCloseSingle(3);
}

void PieClose4() {
	PieCloseSingle(4);
}

void PieClose5() {
	PieCloseSingle(5);
}
