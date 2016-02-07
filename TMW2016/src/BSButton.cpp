/*
 * BSButton.cpp
 *
 *  Created on: Feb 7, 2016
 *      Author: Programmer
 */

#include "BSButton.h"

BSButton::BSButton(std::shared_ptr<Joystick> joystick, int button) {
	joy = joystick;
	but = button;
	pressed = false;
	unpressed = false;
}

BSButton::~BSButton() {
	// TODO Auto-generated destructor stub
}

bool BSButton::Pressed() {
	return joy->GetRawButton(but);
}

bool BSButton::RisingEdge() {
	bool risingEdge = false;
	if(joy->GetRawButton(but)) {
		if(!pressed) {
			pressed = true;
			risingEdge = true;
		}
	}
	else {
		pressed = false;
	}

	return risingEdge;
}

bool BSButton::FallingEdge() {
	bool fallingEdge = false;
	if(!joy->GetRawButton(but)) {
			if(!unpressed) {
				unpressed = true;
				fallingEdge = true;
			}
		}
		else {
			unpressed = false;
		}

		return fallingEdge;
}
