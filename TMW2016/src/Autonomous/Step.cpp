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

bool ForwardWithArm::operator ()(World *world) {
	if (startTime < 0) {
		startTime = world->GetClock();
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		Robot::arm->DartPosition(armTarget);
	}
	const float speed = 0.75 * (forward ? 1 : -1);

	if (world->GetClock() - startTime < targetTime) {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	} else {
		crab->Stop();
		return true;
	}
}

// --------------------------------------------------------------------------//

/**
 * Test detecting stop point with roll axis detection
 */
bool ForwardWithArmAndRoll::operator ()(World *world) {
	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		Robot::arm->DartPosition(armTarget);
	}
	const float speed = 0.75;
	const float roll = Robot::driveBase->imu->GetRoll();

	if (roll < 0) {
		hitNegative = true;
	}

	if (!startedObstacle && roll > 5.0) {
		startedObstacle = true;
	}

	if (startedObstacle && hitNegative && roll > 0.0) {
		quietCount++;
	}

	SmartDashboard::PutBoolean("Obstacle Started", startedObstacle);
	SmartDashboard::PutNumber("Quiet Count", quietCount);

	if (quietCount > 5) {
		// We have passed obstacle
		crab->Stop();
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	}
}

bool ForwardCheckRoll::operator ()(World *world) {
	cout << "ForwardCheckRoll()\n";
	const float roll = Robot::driveBase->imu->GetRoll();
	const double currentTime = world->GetClock();

	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
		startTime = currentTime;
	}

	if (++loopCounter > MAX_LOOPS) {
		cerr << "Aborting due to timeout of loops\n";
		// TODO: Just stop and exit
		return true;
	}


	//
	// Retry handling
	//
	if (running && !hitNegative && (currentTime - startTime) > MAX_TRY_TIME) {
		// we have not hit the negative slope, but we've been trying for two seconds
		// we can check for a negative retryStartTime to see if we've retried before
		inRetry = true;
		retryStartTime = currentTime;
	}
	if (inRetry) {
		if (currentTime - retryStartTime < MAX_TRY_TIME) {
			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), -speed, 0.0, true);
			return false;
		} else {
			inRetry = false;
			return false;
		}
	}

	//
	// Normal Operations
	//
	if (!startedObstacle && roll > 5.0) {
		startedObstacle = true;
	}
	if (roll < 0) {
		hitNegative = true;
	}
	if (startedObstacle && hitNegative && roll > 0.0) {
		quietCount++;
	}

	SmartDashboard::PutBoolean("Obstacle Started", startedObstacle);
	SmartDashboard::PutNumber("Quiet Count", quietCount);

	if (quietCount > 5) {
		crab->Stop();
		return true;
	} else {
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), speed, 0.0, true);
		return false;
	}
}


// --------------------------------------------------------------------------//

bool Turn::operator()(World *world) {
	cout << "Running Turn\n";
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

bool BlindSetArmPosition::operator()(World *world) {
	Robot::arm->DartPosition(targetPosition);
	return true;
}

bool SetArmPosition::operator()(World *world) {
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
	if (enable) {
		Robot::arm->SetShooterSpeed(shooterSpeed, feederSpeed);
	} else {
		Robot::arm->RunShooter(false);
	}
	return true;
}


bool ShootBall::operator()(World *world) {
	const double currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}

	if (!hasFired) {
		Robot::arm->Fire();
		hasFired = true;
		return false;
	} else if (currentTime - startTime > fireWait) {
		return true;
	} else {
		return false;
	}
}
