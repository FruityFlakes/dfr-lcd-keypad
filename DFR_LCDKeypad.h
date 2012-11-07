#ifndef DFR_LCDKEYPAD_H
#define DFR_LCDKEYPAD_H
#include "Arduino.h"
/*
 * File: DFR_LCDKeypad.h
 * Hardware: DFRobot LCD Keypad Shield for Arduino
 * Author: Paul R. Nelson III
 * Version: 0.5
 * License: BSD 2-Clause (Simplified BSD)
 * Description:  Library for easy interaction with 'DFRobot LCD Keypad Shield V1.0'
 *   This library works conjunction with the LiquidCrystal library, and adds DFRobot 
 *   specific features, such as backlight control on pin 10.
 *   Features - buttonUp, buttonDown, buttonHeld callbacks
 *            - setBacklight(byte brightness) function
 *            - setable calibration array for analog button values
 */
typedef enum {
	eNone = 0,
	eUp,
	eDown,
	eLeft,
	eRight,
	eSelect
} eDFRKey;

const int cBacklightPin(10);
const int cKeyPin(0);
const int cThreshold(5);
const unsigned int cDefaultHeldInterval(250);

class DFR_LCDKeypad
{
	void (*_buttonDown)(eDFRKey button);
	void (*_buttonUp)(eDFRKey button);
	void (*_buttonHeld)(eDFRKey button);
	eDFRKey _buttonState;
	unsigned int _heldInterval;
	unsigned long _prevMs;
	int _calibration[6];

	inline bool isKey(eDFRKey key, int inValue) {
		return ( inValue > _calibration[key] - cThreshold && 
				 inValue < _calibration[key] + cThreshold);
	}
		
public:
	DFR_LCDKeypad();

	void update();
	void setCalibration( int *array );
	inline eDFRKey getState() {return _buttonState;}
	inline void setHeldInterval( unsigned int interval) { _heldInterval = interval; }
	inline void setButtonUpHandler( void (*func)(eDFRKey) )  { _buttonUp = func; }
	inline void setButtonDownHandler( void (*func)(eDFRKey) ) { _buttonDown = func; }
	inline void setButtonHeldHandler( void (*func)(eDFRKey) )  { _buttonHeld = func; }
	void setBacklight(byte brightness) { analogWrite(cBacklightPin, brightness); }
};

#endif
