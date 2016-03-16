/*
 * SearchForGoal.cpp
 */

#include "SearchForGoal.h"
#include "../Robot.h"
#include "utils.h"

bool SearchForGoal::operator()(World *world) {
	cout << "SearchForGoal()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
			startTime = currentTime;
	}
	const bool goalDetected = world->GetVisionData().HasData();
	const float timeDelta = currentTime - startTime;
	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();

	// Lookup table of minimum travel times based on starting position and target goal
	// The minimum travel time allows us to travel to a location for a period of time
	// before starting to look for a goal
	float minTimes[5][3] = {
			{0.0, 1.5, 5.0},
			{1.5, 0.5, 5.0},
			{2.0, 0.0, 2.0},
			{2.0, 0.0, 2.0},
			{5.0, 3.0, 2.0}
	};
	const float minTime = minTimes[startingPosition-1][targetGoal-1];
	cout << "MinTime: " << minTime << '\n';

	// Startup & Timeout Checks

	if (timeDelta > timeout) {
		std::cerr << "SearchForGoal: timed out, halting\n";
		crab->Stop();
		return false;
	} else if (goalDetected && timeDelta > minTime) {
		cout << "SearchForGoal: found the goal\n";
		return true;
	}

	// Calculate movement
	const float driveAngleRadians = utils::CalculateDriveAngle(startingPosition, targetGoal, world->GetStartingDefense());
	cout << "SearchForGoal: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
	float x = speed * sin(driveAngleRadians);
	float y = speed * cos(driveAngleRadians);

	int dir = (x >= 0) ? 1 : -1;
	x = max(x, dir * 0.10f);
	dir = (y >= 0) ? 1 : -1;
	y = max(y, dir * 0.10f);

	float yawSetpoint;
	switch (targetGoal) {
	case 1:
		yawSetpoint = 60.0;
		break;
	case 2:
		yawSetpoint = 0.0;
		break;
	case 3:
		yawSetpoint = -60.0;
		break;
	}
	Robot::driveBase->DriveControlTwist->SetSetpoint(yawSetpoint);
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	return false;
}
