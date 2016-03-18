/*
 * AlignGoalUsingPID.cpp
 */

#include "AlignGoalUsingPID.h"
#include "../Robot.h"
#include <iostream>

AlignGoalUsingPID::AlignGoalUsingPID(const float speed_) :
	speed(speed_), xAdapter(new VisionPIDAdapter()), yAdapter(new VisionPIDAdapter())
{
	pidX.reset(new PIDController(0.006, 0, 0.005, xAdapter.get(), xAdapter.get(), 0.2));
	pidX->SetSetpoint(GOAL_CENTER_OFFSET);
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
}

AlignGoalUsingPID::~AlignGoalUsingPID() {

}

bool AlignGoalUsingPID::operator ()(World* world) {
	cout << "AlignGoalUsingPID()\n";
	crab->lock = false;
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

	const int maxX = goal.xposition + X_THRESHOLD;
	const int minX = goal.xposition - X_THRESHOLD;
	if (GOAL_CENTER_OFFSET > minX && GOAL_CENTER_OFFSET < maxX) {
		std::cout << "!!! ====================== Goal Aligned  ====================== !!! " << fired << "\n";
		if (!fired) {
			cout << "***********************-=====> FIRING\n";
			fired = Robot::arm->Fire();
		}
		return false;
	} else {
		const int targetGoal = world->GetTargetGoal();
		float targetAngle = 0.0;
		float setpointAngle = 0.0;
		if (targetGoal == 1) {
			setpointAngle = -60.0;
		} else if (targetGoal == 2) {
			setpointAngle = 0.0;
		} else if (targetGoal == 3) {
			setpointAngle = 60.0;
		}
		targetAngle = setpointAngle - 90;
		Robot::driveBase->DriveControlTwist->SetSetpoint(setpointAngle);
		const float radians = targetAngle * M_PI / 180.0;
		const float magnitude = xAdapter->GetOutputValue();
		const float x = magnitude * sin(radians);
		const float y = magnitude * cos(radians);
		std::cout << "\tGoal X: " << goal.xposition
		 	 	<< "  Target Angle: " << targetAngle
				<< "  Setpoint Angle: " << setpointAngle
				<< "  Magnitude: " << magnitude
				<< "  X: " << x
				<< "  Y: " << y << '\n';
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);		// no twist, don't use Robot::driveBase->CrabSpeedTwist->Get()
	}
	return false;
}
