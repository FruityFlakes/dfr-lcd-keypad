ReadMe.txt

Support for DFRobotics LCD Keypad Shield V1.0

None of the sample code performed exercised the display the way I 
wanted.  I also wanted event driven callbacks on button presses.

This library adds the backlight code within the DFR_LCDKeypad class.
More importantly the class adds hooks for user program callback
functions to be triggered when a button is pressed, released or held
for longer than a settable interval.  The update handlers can be
changed at run time, which is how the demo program changes modes on
the select button.

Demo progrogram: 
	 displays initial banner in setup
	 downloads user defined characters to LCD
	 Two Display modes chosen by the selecte button
	 Display Character mode:  (default mode)

	    Left and Right buttons scroll through character set displaying 
	    character and value on the LCD as shown below.
            Display Char: !
            Value: 33
	
		Up and Down buttons raise and lower the backlight brightness

		Holding down a button will cause it to advance automaticially,
		holding longer will cause the advance to occur faster.
 
	Mouse Mode:
		Up/Down/Left/Right moves a blinking block cursor around
		display


Class Usage:

DFR_LCDKeypad keypad;

...

void myButtonUp(eDFRKey key) {
 // do something on key
}

void myButtonHeld(eDFRKey key) {
// do something on key held down
}

...

void setup() {
...
// note: without any handlers it will be a really dull program...
keypad.setButtonUpHandler(myButtonUp);		  // optional 
keypad.setButtonHeldHandler(myButtonHeld);	  // optional
keypad.setButtonHeldHandler(myButtonDown);	  // optional

keypad.setBacklight( some_brightness_value ); // optional


...
}

void loop() {
	 // update current keypress value and call button callbacks if necessary
	 keypad.update();
}
