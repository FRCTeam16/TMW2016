/*
 * TraverseObstacleSteps.cpp
 */

#include "TraverseObstacleSteps.h"
#include "../Robot.h"
#include <iostream>

bool TraverseObstacleWithGyro::operator ()(World *world) {
	cout << "TraverseObstacleWithGyro()\n";
	const float pitch = Robot::driveBase->imu->GetRoll();
	const double currentTime = world->GetClock();

	cout << "currentTime: " << currentTime << '\n';
	cout << "startTime  : " << startTime << '\n';
	cout << "negativeCounter: " << negativeCounter << '\n';
	cout << "startedObstacle: " << startedObstacle << '\n';
	cout << "hitNegative    : " << hitNegative << '\n';
	cout << "quietCount     : " << quietCount << '\n';
	cout << "pitch          : " << pitch << '\n';

	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
	}

	if (++loopCounter > MAX_LOOPS) {
		cerr << "Aborting due to timeout of loops\n";
		// TODO: Just stop
		crab->Stop();
		return false;
	}


	//
	// Normal Operations
	//
	if (!startedObstacle && pitch > 5.0) {
		startedObstacle = true;
		startTime = currentTime;	// As soon as we start the obstacle, start our timer for attempting to cross it
	}
	if (pitch < 4.0) {
		if (negativeCounter++ >= NEGATIVE_COUNTER_TARGET) {
			hitNegative = true;
			cout << "**** HIT NEGATIVE ***";

		}
		quietCount = 0;	// reset quiet count
	}
	if (!hitNegative && pitch > 0.0) {
		negativeCounter = 0;
	}
	if (startedObstacle && hitNegative && pitch > 0.0) {
		quietCount++;
	}

	SmartDashboard::PutBoolean("Obstacle Started", startedObstacle);
	SmartDashboard::PutNumber("Quiet Count", quietCount);

	if (quietCount > 5) {
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	}
}

// --------------------------------------------------------------------------//


bool TraverseObstacleWithGyroAndSonar::operator ()(World* world) {
	cout << "TraverseObstacleWithGyroAndSonar()\n";
	const double currentTime = world->GetClock();
	const float pitch = Robot::driveBase->imu->GetRoll();
	const int distance_one = Robot::driveBase->ultrasonics->GetDistance(1);
	const int distance_two = Robot::driveBase->ultrasonics->GetDistance(2);

	if (startTime < 0) {
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
	}

	cout << "startTime  : " << startTime << '\n';
	cout << "currentTime: " << currentTime << '\n';
	cout << "pitch      : " << pitch << '\n';
	cout << "Dist 1     : " << distance_one << '\n';
	cout << "Dist 2     : " << distance_two << '\n';
	cout << "quiet cntr : " << quietCounter << '\n';
	cout << "started def: " << startedObstacle << '\n';

	if (!startedObstacle && pitch > 5.0) {
		startedObstacle = true;
		startTime = currentTime;	// As soon as we start the obstacle, start our timer for attempting to cross it
	}

	if (startedObstacle) {
		if ((distance_one == 0) && (distance_two == 0)) {
			if (quietCounter++ > 5) {
				cout << "TraverseObstacleWithGyroAndSonar detected zero distance and quieted\n";
				return true;
			} else {
				cout << "Quieting..\n";
			}
		} else {
			quietCounter = 0;	// reset counter
		}
	}
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
	return false;
}


// --------------------------------------------------------------------------//
bool TraverseObstacleWithGyroAndProximity::operator ()(World* world) {
	cout << "TraverseObstacleWithGyroAndProximity()\n";
	const double currentTime = world->GetClock();
	const float pitch = Robot::driveBase->imu->GetRoll();
	const bool prox_left = Robot::driveBase->proximityLeft->Get();
	const bool prox_right = Robot::driveBase->proximityRight->Get();

	if (startTime < 0) {
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
	} else if ((currentTime - startTime) > MAX_TRY_TIME) {
		cout << "*** TIMED OUT *** Aborting attempt\n";
		crab->lock = true;
		return false;
	}

	cout << "\tstartTime  : " << startTime << '\n';
	cout << "\tcurrentTime: " << currentTime << '\n';
	cout << "\tpitch      : " << pitch << '\n';
	cout << "\tprox left  : " << prox_left << '\n';
	cout << "\tprox right  : " << prox_right << '\n';
	cout << "\tstarted def: " << startedObstacle << '\n';

	if (!startedObstacle && pitch > 5.0) {
		startedObstacle = true;
	}

	if (startedObstacle) {
		if (!prox_left || !prox_right) {
			// Appears we've cleared the obstacle
			cout << "Proximity indicators flagged we cleared defense\n";
			return true;
		}
	}
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
	return false;
}
