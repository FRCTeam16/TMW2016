/*
 * ShootingStrategy.cpp
 */

#include "ShootingStrategy.h"

/**
 * Use world start and target position to calculate angle to move to search for goal
 * Switch to vision based targeting step
 * 	- PID controllers for X and Y to send data to crab
 */

void ShootingStrategy::Init() {
	locationSteps[0] = {};
	locationSteps[1] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
			new TimedCrab(3, 60.0, 0.30, 0.0),
			new ShootBall()
	};
	locationSteps[2] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
			new TimedCrab(2, 60.0, 0.30, 0.0),
			new ShootBall()
	};
	locationSteps[3] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
			new TimedCrab(1.0, 0.0, 0.30, 0.0),
			new ShootBall()
	};
	locationSteps[4] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
			new TimedCrab(0.5, 0.0, -0.30, 0.0),
			new ShootBall()
	};
	locationSteps[5] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
			new TimedCrab(2, -60.0, 0.30, 0.0),
			new ShootBall()
	};
}

bool ShootingStrategy::Run(World *world) {
	const std::vector<Step*> steps = locationSteps[world->GetStartingPosition()];
	if (currentStep >= steps.size()) {
		return true;
	}

	const bool stepComplete = steps[currentStep]->operator ()(world);
	RunPeriodicManagers(steps[currentStep]->GetCrabInfo());

	if (stepComplete) {
		currentStep++;
		cout << "Advancing to Step: " << currentStep << '\n';
	}
	return false;
}
