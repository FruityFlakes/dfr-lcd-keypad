#include "DFR_LCDKeypad.h"

/*
 * File: DFR_LCDKeypad.cpp
 * Hardware: DFRobot LCD Keypad Shield for Arduino
 * Author: Paul R. Nelson III
 * Version: 0.5
 * License: BSD 2-Clause (Simplified BSD)
 * Description:  Library for easy interaction with 'DFRobot LCD Keypad Shield V1.0'
 *   This library works conjunction with the LiquidCrystal library, and adds DFRobot 
 *   specific features, such as backlight control on pin 10.  Readability and functionality
 *   where higher concerns than absolute minimal byte count.  To library size further,
 *   eliminate the setCalibration() function.  Further cutes could be made by removing the 
 *   isKey() function and _calibration[] array, and putting the comparisons directly into
 *   the update function.
 *   Features - buttonUp, buttonDown, buttonHeld callbacks
 *            - setBacklight(byte brightness) function
 *            - setable calibration array for analog button values
 */

DFR_LCDKeypad::DFR_LCDKeypad()
		: _buttonDown(0), _buttonUp(0), _buttonHeld(0), _buttonState(eNone), 
		  _heldInterval(cDefaultHeldInterval), _prevMs(0)
{
	// array ordered eNone, eUp, eDown, eLeft, eRight, eSelect
	int analogReadValues[6] = { 1023, 144, 329, 505, 0, 742 };
	setCalibration ( analogReadValues );
	pinMode(cBacklightPin, OUTPUT);
	analogWrite(cBacklightPin, 255);
}

// your device may differ from mine, can set the calibration constants
// using memcpy instead of individual assignment actually cost 62 bytes
void 
DFR_LCDKeypad::setCalibration(int *array) 
{
	_calibration[eNone]   = array[eNone];
	_calibration[eUp]     = array[eUp];
	_calibration[eDown]   = array[eDown];
	_calibration[eLeft]   = array[eLeft];
	_calibration[eRight]  = array[eRight];
	_calibration[eSelect] = array[eSelect];
}

// Called from within loop() and it will dispatch calls to button handler callbacks
// if the calling program has set the values in the class when the state changes, or
// button down exceeds the held interval.
// May need some key debounce, but I did not see the need on mine, I think the A/D 
// conversion probably soaks up the transients.
void
DFR_LCDKeypad::update() 
{
	int value = analogRead(cKeyPin);
	eDFRKey keyRead = eNone;
	if (      isKey(eUp,     value) ) keyRead = eUp;
	else if ( isKey(eDown,   value) ) keyRead = eDown;
	else if ( isKey(eLeft,   value) ) keyRead = eLeft;
	else if ( isKey(eRight,  value) ) keyRead = eRight;
	else if ( isKey(eSelect, value) ) keyRead = eSelect;
	
    // if button state differs from previous state, call appropriate callback function
	if( keyRead != _buttonState) {
		eDFRKey prevKey = _buttonState;
		_buttonState = keyRead;
		if( prevKey == eNone ) {
			if(_buttonDown)
				(*_buttonDown)(keyRead);	// call button down callback
		} else {
			if(_buttonUp)
				(*_buttonUp)(prevKey);		// call button up callback
		}
	} else {
		if(_buttonState != eNone && _buttonHeld) {
			// button held down - check update interval
			unsigned long current = millis();
			if(current - _prevMs > _heldInterval) {
				_prevMs = current;
				(*_buttonHeld)(keyRead);	// call button held callback
			}
		}
	}
	
}






