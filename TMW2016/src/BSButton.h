/*
 * BSButton.h
 *
 *  Created on: Feb 7, 2016
 *      Author: Programmer
 */

#ifndef BSBUTTON_H_
#define BSBUTTON_H_

#include "WPILib.h"

class BSButton {
public:
	BSButton(std::shared_ptr<Joystick> joystick, int button);
	virtual ~BSButton();
	bool Pressed();
	bool RisingEdge();
	bool FallingEdge();
private:
	std::shared_ptr<Joystick> joy;
	int but;
	bool pressed;
	bool unpressed;
};

#endif /* BSBUTTON_H_ */
