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
			cout << "**** HIT NEGATIVE ***\n";

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
	cout << "TraverseObstacleWithGyro()\n";
	const float pitch = Robot::driveBase->imu->GetRoll();
	const int distance = Robot::driveBase->ultrasonics->GetDistance(2);
	const double currentTime = world->GetClock();

	cout << "currentTime: " << currentTime << '\n';
	cout << "startTime  : " << startTime << '\n';
	cout << "negativeCounter: " << negativeCounter << '\n';
	cout << "startedObstacle: " << startedObstacle << '\n';
	cout << "hitNegative    : " << hitNegative << '\n';
	cout << "quietCount     : " << quietCount << '\n';
	cout << "pitch          : " << pitch << '\n';
	cout << "distance       : " << distance << '\n';
	cout << "last distance  : " << last_distance << '\n';
	cout << "in_obstacle    : " << distance_in_obstacle << '\n';

	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
		last_distance = distance;
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
			cout << "**** HIT NEGATIVE ***\n";
		}
		quietCount = 0;	// reset quiet count
	}
	if (!hitNegative && pitch > 0.0) {
		negativeCounter = 0;
	}

	if (startedObstacle && !distance_in_obstacle) {
		if (distance < 60 && (last_distance - distance > DELTA_ULTRASONIC)) {
			distance_in_obstacle = true;
			last_distance = distance;
		}
	}

	if (startedObstacle && distance_in_obstacle) {
		if (distance - last_distance > DELTA_ULTRASONIC) {
			cout << "*** ULTRASONICS DETECTED OBSTACLE CLEARANCE ***\n";
			return true;
		}
		last_distance = distance;
	}

	if (startedObstacle && hitNegative) {
		if (pitch > 0.0) {
			quietCount++;
		}
	}

	SmartDashboard::PutBoolean("Obstacle Started", startedObstacle);
	SmartDashboard::PutNumber("Quiet Count", quietCount);
	SmartDashboard::PutNumber("Last Dist", last_distance);

	if (quietCount > 5) {
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	}
}


bool TraverseObstacleWithGyroAndSonarLockingValues::operator ()(World* world) {
	cout << "TraverseObstacleWithGyroAndSonarLockingValues()\n";
	const float pitch = Robot::driveBase->imu->GetRoll();
	const int distance = Robot::driveBase->ultrasonics->GetDistance(2);
	const double currentTime = world->GetClock();

	cout << "currentTime: " << currentTime << '\n';
	cout << "startTime  : " << startTime << '\n';
	cout << "negativeCounter: " << negativeCounter << '\n';
	cout << "startedObstacle: " << startedObstacle << '\n';
	cout << "hitNegative    : " << hitNegative << '\n';
	cout << "quietCount     : " << quietCount << '\n';
	cout << "pitch          : " << pitch << '\n';
	cout << "distance       : " << distance << '\n';
	cout << "last distance  : " << last_distance << '\n';
	cout << "in_obstacle    : " << distance_in_obstacle << '\n';

	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
		last_distance = distance;
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
			cout << "**** HIT NEGATIVE ***\n";
		}
		quietCount = 0;	// reset quiet count
	}
	if (!hitNegative && pitch > 0.0) {
		negativeCounter = 0;
	}

	if (startedObstacle && !distance_in_obstacle) {
		if (distance < 60 && (last_distance - distance > 20)) {
			distance_in_obstacle = true;
			last_distance = distance;
		}
	} else if (startedObstacle && distance_in_obstacle) {
		if (distance - last_distance > 20) {
			cout << "*** ULTRASONICS DETECTED OBSTACLE CLEARANCE ***\n";
			return true;
		}
	}

	if (startedObstacle && hitNegative) {
		if (pitch > 0.0) {
			quietCount++;
		}
	}

	SmartDashboard::PutBoolean("Obstacle Started", startedObstacle);
	SmartDashboard::PutNumber("Quiet Count", quietCount);
	SmartDashboard::PutNumber("Last Dist", last_distance);

	if (quietCount > 5) {
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	}
}

