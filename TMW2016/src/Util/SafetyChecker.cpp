/*
 * SafetyChecker.cpp
 */

#include "SafetyChecker.h"
#include "../Robot.h"
#include <iostream>

bool SafetyChecker::CheckLimits() {
	bool success = true;
	if (Robot::driveBase->tankLeftHotCount > Robot::driveBase->tankDriveLimit) {
		cout << "<<< TANK LEFT EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::driveBase->tankRightHotCount > Robot::driveBase->tankDriveLimit) {
		cout << "<<< TANK RIGHT EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::driveBase->FLHotCount > Robot::driveBase->driveLimit) {
		cout << "<<< FL EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::driveBase->FRHotCount > Robot::driveBase->driveLimit) {
		cout << "<<< FR EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::driveBase->RLHotCount > Robot::driveBase->driveLimit) {
		cout << "<<< RL EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::driveBase->RRHotCount > Robot::driveBase->driveLimit) {
		cout << "<<< RR EXCEEDED LIMIT >>>\n";
		success = false;
	}

	if (Robot::arm->dartRightHotCount > Robot::arm->dartOutputLimit) {
		cout << "<<< DART RIGHT EXCEEDED LIMIT >>>\n";
		success = false;
	}
	if (Robot::arm->dartLeftHotCount > Robot::arm->dartOutputLimit) {
		cout << "<<< DART RIGHT EXCEEDED LIMIT >>>\n";
		success = false;
	}

	cout << "Simulate Error: " << simulateError << " COUNTER? " << simulatedErrorCounter << '\n';
	if (simulateError && simulatedErrorCounter++ > 10) {
		cout << "<<< SIMULATED ERROR EXCEEDED LIMIT >>>\n";
		success = false;
	}


	if (!success) {
		failed = true;
		Robot::driveBase->RunTanks(0.0);
		Robot::driveBase->EnableSteerPIDControllers(false);
	}

	return success;
}


void SafetyChecker::HaltPeriodic() {
	Robot::driveBase->Crab(0.0, 0.0, 0.0, true);
	Robot::driveBase->RunTanks(0.0);
	Robot::arm->DartManager();
	Robot::arm->FireManager();
	Robot::arm->ShooterManager();
}

bool SafetyChecker::IsFailed() const {
	return failed;
}
