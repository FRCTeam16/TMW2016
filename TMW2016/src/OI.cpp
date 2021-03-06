// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "OI.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "SmartDashboard/SmartDashboard.h"
#include "Commands/AutonomousCommand.h"
#include "Commands/SetDartOffset.h"
#include "Commands/SetWheelOffsets.h"
#include "Commands/ZeroGyro.h"


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

OI::OI() {
    // Process operator interface input here.
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    gamepad.reset(new Joystick(2));
    
    driverRight.reset(new Joystick(1));
    
    driverLeft.reset(new Joystick(0));
    

    // SmartDashboard Buttons
    SmartDashboard::PutData("ZeroGyro", new ZeroGyro());
    SmartDashboard::PutData("SetDartOffset", new SetDartOffset());
    SmartDashboard::PutData("SetWheelOffsets", new SetWheelOffsets());
    SmartDashboard::PutData("Autonomous Command", new AutonomousCommand());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    pi = 3.14159;
	GPA = new BSButton(gamepad, 1);
	GPB = new BSButton(gamepad, 2);
	GPX = new BSButton(gamepad, 3);
	GPY = new BSButton(gamepad, 4);
	GPLT = new BSButton(gamepad, 5);
	GPRT = new BSButton(gamepad, 6);
	GPBack = new BSButton(gamepad, 7);
	GPStart = new BSButton (gamepad, 8);
	DL1 = new BSButton(driverLeft,1);
	DL2 = new BSButton(driverLeft,2);
	DL3 = new BSButton(driverLeft,3);
	DL4 = new BSButton(driverLeft,4);
	DL5 = new BSButton(driverLeft,5);
	DL6 = new BSButton(driverLeft,6);
	DL7 = new BSButton(driverLeft,7);
	DL8 = new BSButton(driverLeft,8);
	DL9 = new BSButton(driverLeft,9);
	DL10 = new BSButton(driverLeft,10);
	DL11 = new BSButton(driverLeft,11);
	DL12 = new BSButton(driverLeft,12);
	DR1 = new BSButton(driverRight,1);
	DR2 = new BSButton(driverRight,2);
	DR3 = new BSButton(driverRight,3);
	DR4 = new BSButton(driverRight,4);
	DR5 = new BSButton(driverRight,5);
	DR6 = new BSButton(driverRight,6);
	DR7 = new BSButton(driverRight,7);
	DR8 = new BSButton(driverRight,8);
	DR9 = new BSButton(driverRight,9);
	DR10 = new BSButton(driverRight,10);
	DR11 = new BSButton(driverRight,11);
	DR12 = new BSButton(driverRight,12);

}

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS

std::shared_ptr<Joystick> OI::getDriverLeft() {
   return driverLeft;
}

std::shared_ptr<Joystick> OI::getDriverRight() {
   return driverRight;
}

std::shared_ptr<Joystick> OI::getGamepad() {
   return gamepad;
}


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS

double OI::getScaledJoystickRadians() {
	double steerAngle = pi/2;
	steerAngle = driverRight->GetDirectionRadians();
	if(steerAngle > pi)
		steerAngle = pi;
	if(steerAngle < -pi)
		steerAngle = -pi;

	if(steerAngle < -pi/2)
		steerAngle = -pi/2 - steerAngle;
	else if(steerAngle < pi/2)
		steerAngle = pi/2 + steerAngle;
	else
		steerAngle = 3*pi/2 - steerAngle;
//	scalingFactor = driverJoystick->GetTwist()/2+1.5;

	return scaledRadians(steerAngle);
}
double OI::getJoystickMagnitude() {
	if(driverRight->GetMagnitude() < .1)
		return 0;
	else
		if (driverRight->GetY()<0)
			return -driverRight->GetMagnitude();
		else
			return driverRight->GetMagnitude();
}
double OI::getJoystickTwist() {
	if(fabs(driverLeft->GetX()) < 0.1)
			return 0;
	else
		return driverLeft->GetX()/2;
}
double OI::getJoystickX() {
	if (fabs(driverRight->GetX()) < 0.1)
		return 0;
	else
		return driverRight->GetX();
}
double OI::getJoystickY() {
	if (fabs(driverRight->GetY()) < 0.1)
		return 0;
	else
		return driverRight->GetY();
}
double OI::getLeftJoystickXRadians() {
	if(fabs(driverLeft->GetX())<.00)
		return pi/2;
	else
		return scaledRadians(pi/2 + driverLeft->GetX()*pi/2);
}
double OI::scaledRadians(double radians) {
	double scaledradians = pi/2;
	double scalingFactor = 1.8;
	if(radians <= pi/2)
		scaledradians = (-(pi/2)/pow(pow(-pi/2,2),scalingFactor/2))*pow(pow(radians-pi/2,2),scalingFactor/2) + pi/2;
	else //if(steerAngle <= pi)
		scaledradians = ((pi/2)/pow((pi/2),scalingFactor))*pow(radians-pi/2,scalingFactor) + pi/2;

	return scaledradians;
}

double OI::getScaledJoystick (double value, double scaling) {
	return pow(value,scaling);
}

