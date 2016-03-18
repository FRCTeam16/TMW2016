/*
 * AlignGoalUsingPID.cpp
 */

#include "AlignGoalUsingPID.h"
#include "../Robot.h"
#include <iostream>

AlignGoalUsingPID::AlignGoalUsingPID(const float speed_) :
	speed(speed_), xAdapter(new VisionPIDAdapter()), yAdapter(new VisionPIDAdapter())
{
	pidX.reset(new PIDController(0.005, 0, 0.005, xAdapter.get(), xAdapter.get(), 0.2));
	pidX->SetSetpoint(0.0);
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
}

AlignGoalUsingPID::~AlignGoalUsingPID() {

}

bool AlignGoalUsingPID::operator ()(World* world) {
	cout << "AlignGoalUsingPID()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}

	if ((currentTime - startTime) > timeout) {
		std::cout << "*** TIMEOUT ***\n";
		crab->lock = true;
		return false;
	}

	const VisionData vd = world->GetVisionData();
	goal = vd.GetGoal(world->GetTargetGoal());
	if (goal.HasData()) {
		noGoalCounter = 0;
		xAdapter->Update(goal.xposition);
	} else {
		if (noGoalCounter++ > MAX_NO_GOAL_SCANS) {
			std::cout << "*** NO GOAL ***\n";
			crab->lock = true;
			return false;
		}
	}

	// TODO:
	// need to use drive angle, e.g. if we are pointed at 0 then we want -90 90
	// if we are at angle, +/= 60

	if (fabs(goal.xposition) < X_THRESHOLD) {
		std::cout << "!!! ====================== Goal Aligned  ====================== !!!\n";
		return false;
	} else {
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		float targetAngle = -90.0;
//		if (goal.xposition < 0 ) {
//			targetAngle = 90.0;
//		}
		const float radians = targetAngle * M_PI / 180.0;
		const float magnitude = xAdapter->GetOutputValue();
		const float x = magnitude * sin(radians);
		const float y = magnitude * cos(radians);
		std::cout << "Target Angle: " << targetAngle
				<< "  Magnitude: " << magnitude
				<< "  X: " << x
				<< "  Y: " << y << '\n';
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);		// no twist, don't use Robot::driveBase->CrabSpeedTwist->Get()
	}
	return false;
}
