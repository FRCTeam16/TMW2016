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
	cout << world->GetClock() << ':' << startTime << ':' << targetTime << (world->GetClock() - startTime) << '\n';

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


	/*
	 * After we start obstacle
	 * try for 2 seconds
	 * if we haven't started going down, then retry
	 *
	 *
	 */

	// Retry check / safety exit
//	if (startedObstacle && ++loopCounter > MAX_LOOPS ) {
//		if (++retryLoops > MAX_LOOPS) {
//			assert(false && " Aborting");
//		} else {
//			// attempt to move backwards off of obstacle
//			Robot::driveBase->Crab(0.0, -speed, 0.0, true);
//
//			// We think we moved off, reset counters
//			loopCounter = 0;
//			retryLoops = 0;
//		}
//	} else {
//
//	}


	if (!running) {
		running = true;
		Robot::driveBase->DriveControlTwist->SetSetpoint(0.0);
	}

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
	if (Robot::arm->DartInPosition(targetPosition)) {
		return true;
	} else if (!running) {
		running = true;
		Robot::arm->DartPosition(targetPosition);
	}
	return false;
}

// --------------------------------------------------------------------------//


ControlShooterMotors::ControlShooterMotors(bool enable_) : enable(enable_) {
	Preferences *prefs = Preferences::GetInstance();
	shooterSpeed = prefs->GetFloat("ShooterSpeed");
	feederSpeed = prefs->GetFloat("FeederSpeed");
}

bool ControlShooterMotors::operator()(World *world) {
	if (enable) {
		Robot::arm->RunShooter(true, shooterSpeed, feederSpeed);
	} else {
		Robot::arm->RunShooter(false, 0.0, 0.0);
	}
	return true;
}


bool ShootBall::operator()(World *world) {
	const double currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}

	if (!hasFired) {
		Robot::arm->Fire(true);
		hasFired = true;
	} else {
		if (currentTime - startTime > fireWait) {
			Robot::arm->Fire(false);
			return true;
		}
	}
	return false;
}
