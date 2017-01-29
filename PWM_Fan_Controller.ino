#include "Arduino.h"
#include <PWM.h>

// Pick a pin that your chosen timer supports: 3, 5, 6, 9, 10, or 11
// Best not to use Timer 0 as this affects millis() and delay() accuracy
//
// Timer 0 - pins 5 & 6
// Timer 1 - pins 9 & 10
// Timer 2 - pins 11 & 3
#define pwmCoolFanPin 9
#define pwmMouseHousePin 3 // Pin 11 (it's pair) did not work!

// The switch will ground one of these pins to determine the PWM channel
#define fanOn 12
#define mouseOn 10
#define switchGnd 11

// Use as power and gnd
#define potPower 8
#define potGnd 7

// Turn off all power after 1 hour (or whatever you choose)
long startTime = millis();
long offTime = 60000 * 60; // millis in one minute * minutes = 1 hour

// Debug messages?
bool isDebug = true;

// Demo use of enum. Variables in this start at 0 and increment by 1.
enum outType {
	none, fan, mouse
};

// ----------------------------------------------------------------------------
// One size fits all Serial Monitor debugging messages
// ----------------------------------------------------------------------------
template<typename T>
void debugPrint(T printMe, bool newLine = false) {
	if (isDebug) {
		if (newLine) {
			Serial.println(printMe);
		}
		else {
			Serial.print(printMe);
		}
		Serial.flush();
	}
}

void setup() {
	// Debugging window
	Serial.begin(9600);

	// We will only play with timers that do NOT affect Timer 0 (millis and delay)
	InitTimersSafe();

	// Set the frequency of the PWM pins
	SetPinFrequencySafe(pwmCoolFanPin, 20000);
	SetPinFrequencySafe(pwmMouseHousePin, 1000);

	// Set the pins that determines which output we're controlling
	pinMode(fanOn, INPUT_PULLUP);
	pinMode(mouseOn, INPUT_PULLUP);

	// Use available digital pins as +5v and GND equivalents for the potentiometer
	pinMode(potPower, OUTPUT);
	pinMode(potGnd, OUTPUT);
	digitalWrite(potGnd, LOW);
	digitalWrite(potPower, HIGH);

	// And another digital pin as ground for switch
	pinMode(switchGnd, OUTPUT);
	digitalWrite(switchGnd, LOW);

	debugPrint("Setup finished successfully.\n");
}

// ---------------------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// ---------------------------------------------------------------------------------
void loop() {

	// Store old values so we can detect new ones
	static int oldFanMarkSpace = 0;
	static int oldMouseMarkSpace = 0;

	// TODO sort out this rather clumsy code (OK for demo)
	// TODO add in LED indicator that the unit is ON (has not auto switched off)

	// Are we controlling the fan or mouse mat?
	int currControl = 0;
	debugPrint("Controlling ");
	if (digitalRead(fanOn) == LOW) {
		currControl = outType::fan;
		debugPrint(" fan ");
	}
	else {
		if (digitalRead(mouseOn) == LOW) {
			currControl = outType::mouse;
			debugPrint(" mouse ");
		}
		else {
			// if neither then exit the loop()
			return;
		}
	}

	// Get the new mark space
	int newMarkSpace = getMarkSpace(currControl);

	// If different set the correct pin
	if (currControl == outType::fan) {
		if (newMarkSpace != oldFanMarkSpace) {

			// Output the frequency with the mark/space ratio
			pwmWrite(pwmCoolFanPin, newMarkSpace);

			oldFanMarkSpace = newMarkSpace;
		}
	}
	else {
		if (newMarkSpace != oldMouseMarkSpace) {

			// Output the frequency with the mark/space ratio
			pwmWrite(pwmMouseHousePin, newMarkSpace);

			oldMouseMarkSpace = newMarkSpace;
		}
	}

	// Switch off after predetermined period
	if (millis() > startTime + offTime){
		pwmWrite(pwmCoolFanPin, 0);
		pwmWrite(pwmMouseHousePin, 0);
	}

	// The PWM output will continue regardless of any delay (eg caused by other code) here
	delay(50);
}

// ---------------------------------------------------------------------------------
// Mark space refers to the amount of time the pulse lasts for during the ON time
// ---------------------------------------------------------------------------------
int getMarkSpace(int controlType) {

	// Read analog input A0 (yes, pin 14) for the mark/space ratio
	int amarkSpace = analogRead(14);
	debugPrint(amarkSpace);

	// Map to a more sensible range so fan does not stall, for example.
	int markSpace;
	if (outType::fan == controlType) {
		markSpace = map(amarkSpace, 0, 1023, 195, 255);
		debugPrint(" fan ");
	}
	else {
		markSpace = map(amarkSpace, 0, 1023, 50, 255);
		debugPrint(" mouse mat ");
	}
	debugPrint("\t");
	debugPrint(markSpace, true);

	// Return this mapped value
	return markSpace > 255 ? 255 : markSpace;
}
