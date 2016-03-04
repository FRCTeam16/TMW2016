/*
 * Step.cpp
 */

#include "WPILib.h"
#include "Step.h"
#include "../Robot.h"
#include <algorithm>


// --------------------------------------------------------------------------//

bool TimedCrab::operator ()(World *world) {
	cout << "TimedCrab target time " << targetTime << '\n';
	if (startTime < 0) {
		startTime = world->GetClock();
		Robot::driveBase->DriveControlTwist->SetSetpoint(targetAngle);
	}

	if (world->GetClock() - startTime < targetTime) {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), ySpeed, xSpeed, true);
		return false;
	} else {
		crab->Stop();
		return true;
	}
}

// --------------------------------------------------------------------------//

bool LockWheels::operator ()(World *world) {
	cout << "LockWheels()\n";
	crab->lock = true;
	return false;
}

// --------------------------------------------------------------------------//

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

bool Turn::operator()(World *world) {
	cout << "Turn()\n";
	if (firstRun) {
		firstRun = false;
		Robot::driveBase->DriveControlTwist->SetSetpoint(angle);
	}
	SmartDashboard::PutNumber("DriveControlTwistError",
			Robot::driveBase->DriveControlTwist->GetError());
	SmartDashboard::PutNumber("DriveControlTwist Output",
				Robot::driveBase->CrabSpeedTwist->Get());

	if (fabs(Robot::driveBase->DriveControlTwist->GetError()) < 3) {
		crab->Stop();
		cout << "Exiting Turn\n";
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), 0.0, 0.0, true);
		return false;
	}
}

// --------------------------------------------------------------------------//

bool SetArmPosition::operator()(World *world) {
	cout << "SetArmPosition\n";
	if (++loopCounter > MAX_LOOPS) {
		cerr << "Aborting SetArmPosition\n";
		return true;
	}

	if (!running) {
		running = true;
		switch(position) {
		case Position::Custom:
			Robot::arm->DartPosition(customTarget);
			break;
		case Position::Pickup:
			Robot::arm->PickupPosition();
			break;
		case Position::Travel:
			Robot::arm->TravelPosition();
			break;
		case Position::ShooterLow:
			Robot::arm->ShooterLow();
			break;
		case Position::ShooterHigh:
			Robot::arm->ShooterHigh();
			break;
		default:
			std::cerr << "Unrecognized position requested: " << static_cast<int>(position) << ", aborting!\n";
			return true;
		}
	}

	if (wait) {
		cout << "Waiting for DART...";
		return Robot::arm->DartInPosition();
	} else {
		return true;
	}
}


// --------------------------------------------------------------------------//


ControlShooterMotors::ControlShooterMotors(bool enable_) : enable(enable_) {
	Preferences *prefs = Preferences::GetInstance();
	shooterSpeed = prefs->GetFloat("ShooterSpeed");
	feederSpeed = prefs->GetFloat("FeederSpeed");
}

bool ControlShooterMotors::operator()(World *world) {
	cout << "ControlShooterMotors()\n";
	if (enable) {
		Robot::arm->SetShooterSpeed(shooterSpeed, feederSpeed);
	} else {
		Robot::arm->RunShooter(false);
	}
	return true;
}


bool ShootBall::operator()(World *world) {
	cout << "ShootBall()\n";
	const double currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}

	if (waitForShooterWheels) {
		if (!Robot::arm->ShooterWheelsAtSpeed()) {
			cout << "Waiting for wheels\n";
			return false;
		}
	}

	const bool dartInPosition = Robot::arm->DartInPosition();
	if (!hasFired && dartInPosition) {
		cout << " !!!!! FIRING !!!!!\n";
		Robot::arm->Fire();
		hasFired = true;
		return false;
	} else if (hasFired && (currentTime - startTime) > fireWait) {
		return true;
	} else {
		cout << "Dart in position? " << dartInPosition << '\n';
		return false;
	}
}
