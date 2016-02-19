/*
 * Strategy.cpp
 */

#include "Strategy.h"


// --------------------------------------------------------------------------//


bool StepStrategy::Run(World *world) {
	cout << "StepStrategy::Invoke\n";
	// All work is finished in this strategy
	if (currentStep >= steps.size()) {
		return true;
	}

	bool stepComplete = steps[currentStep]->operator ()(world);
	if (stepComplete) {
		currentStep++;
		cout << "Advancing to Step: " << currentStep << '\n';
	}
	return false;
}

// --------------------------------------------------------------------------//


bool NoOpStrategy::Run(World *world) {
	cout << "NoOpStrategy exiting\n";
	return true;
}

// --------------------------------------------------------------------------//

OuterworkAndShootStrategy::OuterworkAndShootStrategy(Strategy *outerworkStrategy_) {
	outerworkStrategy.reset(outerworkStrategy_);
	shootingStrategy.reset(new ShootingStrategy());
}

bool OuterworkAndShootStrategy::Run(World *world) {
	cout << "OuterworkAndShootStrategy base\n";
	if (!outerworkComplete) {
		if (firstOuterwork) {
			firstOuterwork = false;
			outerworkStrategy->Init();
		}
		outerworkComplete = outerworkStrategy->Run(world);
		return false;	// always return false to trigger shooting strategy run
	} else {
		if (firstShooting) {
			firstShooting = false;
			shootingStrategy->Init();
		}
		return shootingStrategy->Run(world);
	}
}


// --------------------------------------------------------------------------//


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

