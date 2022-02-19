//  Generic Helper functions


// Function for non-blocking delays
struct sDelaystruct
{
	typedef void *DelayFunction();
	unsigned long PrevDelayMillis;
	int DelayAmount;
};

sDelaystruct DelayArray[25];



// Helper Setup Function
void  HelperSetup() {

}

void ProcessHelpers() {
	
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