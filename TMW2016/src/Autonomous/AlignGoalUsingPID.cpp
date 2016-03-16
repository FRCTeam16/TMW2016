/*
 * AlignGoalUsingPID.cpp
 */

#include "AlignGoalUsingPID.h"
#include "../Robot.h"
#include <iostream>

AlignGoalUsingPID::AlignGoalUsingPID(const float speed_) :
	speed(speed_)
{
	pidX.reset(new PIDController(0.05, 0, 0, xAdapter.get(), xAdapter.get(), 0.2));
	pidX->SetSetpoint(0.0);
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
}

AlignGoalUsingPID::~AlignGoalUsingPID() {

}

bool AlignGoalUsingPID::operator ()(World* world) {
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

	if (fabs(goal.xposition) < X_THRESHOLD) {
		std::cout << "!!! Goal Aligned !!!\n";
		return true;
	} else {
		const float targetAngle = Robot::driveBase->imu->GetYaw();	// todo: calculate?
		const float radians = targetAngle * M_PI / 180.0;
		const float magnitude = xAdapter->GetOutputValue();
		const float x = magnitude * sin(radians) * speed;
		const float y = magnitude * cos(radians) * speed;
		std::cout << "Target Angle: " << targetAngle << '\n'
				<< "Magnitude: " << magnitude << '\n'
				<< "X: " << x << '\n'
				<< "Y: " << y << '\n';
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	}

	return false;
}
