// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef OI_H
#define OI_H

#include "WPILib.h"
#include "BSButton.h"
class OI {
private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<Joystick> gamepad;
	std::shared_ptr<Joystick> driverRight;
	std::shared_ptr<Joystick> driverLeft;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	double pi;
	double scaledRadians(double radians);
public:
	OI();

	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES

	std::shared_ptr<Joystick> getDriverLeft();
	std::shared_ptr<Joystick> getDriverRight();
	std::shared_ptr<Joystick> getGamepad();

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES
		double getScaledJoystickRadians();
		double getJoystickMagnitude();
		double getJoystickTwist();
		double getJoystickX();
		double getJoystickY();
		double getLeftJoystickXRadians();
		double getScaledJoystick (double value, double scaling);
		BSButton* GPX;
		BSButton* GPY;
		BSButton* GPA;
		BSButton* GPB;
		BSButton* GPLT;
		BSButton* GPRT;
		BSButton* GPBack;
		BSButton* GPStart;
		BSButton* DL1;
		BSButton* DL2;
		BSButton* DL3;
		BSButton* DL4;
		BSButton* DL5;
		BSButton* DL6;
		BSButton* DL7;
		BSButton* DL8;
		BSButton* DL9;
		BSButton* DL10;
		BSButton* DL11;
		BSButton* DL12;
		BSButton* DR1;
		BSButton* DR2;
		BSButton* DR3;
		BSButton* DR4;
		BSButton* DR5;
		BSButton* DR6;
		BSButton* DR7;
		BSButton* DR8;
		BSButton* DR9;
		BSButton* DR10;
		BSButton* DR11;
		BSButton* DR12;
};

#endif
