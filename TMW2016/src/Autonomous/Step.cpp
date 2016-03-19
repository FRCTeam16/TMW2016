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
		crab->lock = true;
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
	cout << "ControlShooterMotors(" << enable << ")\n";
	if (enable) {
		cout << "Shooter Speed: " << shooterSpeed << " Feeder Speed: " << feederSpeed << '\n';
		Robot::arm->SetShooterSpeed(shooterSpeed, feederSpeed);
//		Robot::arm->RunShooter(true);
	} else {
		Robot::arm->RunShooter(false);
	}
	return true;
}

// --------------------------------------------------------------------------//


bool ControlBeaterBar::operator()(World *world) {
	cout << "ControlBeaterBar(" << enable << ")\n";
	crab->beater_bar = enable ? -1.0 : 0.0;
	return true;
}

// --------------------------------------------------------------------------//

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
