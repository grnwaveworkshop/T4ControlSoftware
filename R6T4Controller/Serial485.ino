// RS485 Serial Port Functions
// 485 used Serial Port 4
// TX4 - D17
// RX4 - D16
// RTS4 - D41
// RTS sets the Direction for half duplex operation.  Low - Receive Mode, High - Transmit

#include <EasyTransfer.h>

#define PSIFRONTADDRESS 4     // PSI Front Address


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
  }
}

void PSIFrontOff() {
    Serial.println("PSIFrontOff");
    mysenddata.address = PSIFRONTADDRESS;
    mysenddata.command = 'T';
    mysenddata.Data1 = 0;

   // mysenddata.blinks++;
    //mysenddata.pause++;

    DataOut.sendData();

}

void PSIFrontNextPattern() {
    Serial.println("PSI Front Next Pattern");
    mysenddata.address = PSIFRONTADDRESS;
    mysenddata.command = 'T';
    mysenddata.Data1 = 99;

    DataOut.sendData();

}