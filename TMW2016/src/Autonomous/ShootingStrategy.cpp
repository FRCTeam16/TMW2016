/*
 * ShootingStrategy.cpp
 */

#include "ShootingStrategy.h"

void ShootingStrategy::Init() {
	locationSteps[0] = {};
	locationSteps[1] = {
			new BlindSetArmPosition(710),
			new ControlShooterMotors(true),
			new TimedCrab(3, 60.0, 0.30, 0.0),
			new TimedCrab(0.5, 60.0, 0.0, 0.0),
			new ShootBall(),
			new ControlShooterMotors(false)
	};
	locationSteps[2] = {
				new BlindSetArmPosition(710),
				new ControlShooterMotors(true),
				new TimedCrab(3, 60.0, 0.30, 0.0),
				new TimedCrab(0.5, 60.0, 0.0, 0.0),
				new ShootBall(),
				new ControlShooterMotors(false)
	};
	locationSteps[3] = {
				new BlindSetArmPosition(710),
				new ControlShooterMotors(true),
				new TimedCrab(1.0, 0.0, 0.30, 0.0),
				new ShootBall(),
				new ControlShooterMotors(false)
	};
	locationSteps[4] = {
				new BlindSetArmPosition(710),
				new ControlShooterMotors(true),
				new TimedCrab(0.5, 0.0, 0.30, 0.0),
				new TimedCrab(0.1, 60.0, 0.0, 0.0),
				new ShootBall(),
				new ControlShooterMotors(false)
	};
	locationSteps[5] = {
				new BlindSetArmPosition(710),
				new ControlShooterMotors(true),
				new TimedCrab(3, -60.0, 0.30, 0.0),
				new TimedCrab(0.1, 60.0, 0.0, 0.0),
				new ShootBall(),
				new ControlShooterMotors(false)
	};
}

bool ShootingStrategy::Run(World *world) {
	const std::vector<Step*> steps = locationSteps[world->GetStartingPosition()];
	if (currentStep >= steps.size()) {
		return true;
	}
	const bool stepComplete = steps[currentStep]->operator ()(world);
	if (stepComplete) {
			currentStep++;
			cout << "Advancing to Step: " << currentStep << '\n';
	}
	return false;
}
