// Helper Functions


unsigned long CardMillis;
bool DispensingCard = 0;


// Helper Setup Function
void HelperSetup() {
  Serial.println("Setup Helpers");
  pinMode(DIRPIN, OUTPUT);
  pinMode(CARDDISPENSER, OUTPUT);

  digitalWrite(DIRPIN, HIGH);        //Set direction pin to output for translator
  digitalWrite(CARDDISPENSER, LOW);  // Turn off Card Dispenser

  //DispenseCard();
}

void ProcessHelpers() {
  ProcessCardDispenser();
}


// --- AddDelay ---
// Function to add a delay to the array and call the specified function
//void AddDelay(*Func)

// Function for reading the input voltage
float ReadInputVolts() {
  // .235 divider  Vin = Vout/0.235
  float Vin;

  Vin = (analogRead(9) * (3.275 / 1024)) / .235;

  return Vin;
}

// Function to Dispense a Single Card
void ProcessCardDispenser() {
  if (DispensingCard) {

    if (millis() - CardMillis > CARDONTIME) {  // time expired, stop dispensing card
      digitalWrite(CARDDISPENSER, LOW);
      DispensingCard = 0;
    }
  }
}

void DispenseCard() {
  if (!DispensingCard) {  // If not already dispensing a card, start dispensing one
    DispensingCard = 1;
    //CardDispenseLEDSeq();
    //CardDispenseAudio();
    CardMillis = millis();              // Set timer
    digitalWrite(CARDDISPENSER, HIGH);  // Start dispensing card
    StartDPortCardLED();
    PlayElectrocute();
    scheduler.schedule(PlayHappyExtreme, 1250);
    CurrentTrack = random(1, NumTracksFolder[3]);
   // DFPlayer.playFolder(3, CurrentTrack);
  }
}