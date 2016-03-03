/*
 * ShootingStrategy.cpp
 */

#include "ShootingStrategy.h"
#include "AlignWithGoal.h"

/**
 * Use world start and target position to calculate angle to move to search for goal
 * Switch to vision based targeting step
 * 	- PID controllers for X and Y to send data to crab
 */

void ShootingStrategy::Init() {
	locationSteps[0] = {
			new LockWheels()
	};
	locationSteps[1] = {
			new TimedCrab(0.5, 0.0, 0.5, 0.5),	// kick out from left wall
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
//			new SearchForGoal(3, 0.3),
//			new AlignWithGoal(3, 0.3),
			new MoveToWallShootingPosition(10, 0.3),
//			new ShootBall()
	};
	locationSteps[2] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
//			new SearchForGoal(2, 0.3),
//			new AlignWithGoal(2, 0.3),
			new MoveToWallShootingPosition(10, 0.3),
//			new ShootBall()
	};
	locationSteps[3] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
//			new SearchForGoal(2, 0.3),
//			new AlignWithGoal(1.0, 0.3),
			new MoveToWallShootingPosition(10, 0.3),
			new ShootBall()
	};
	locationSteps[4] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
//			new SearchForGoal(2, 0.3),
//			new AlignWithGoal(0.5, 0.3),
			new MoveToWallShootingPosition(10, 0.3),
//			new ShootBall()
	};
	locationSteps[5] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, false),
			new ControlShooterMotors(true),
//			new SearchForGoal(2, 0.3),
//			new AlignWithGoal(2, 0.3),
			new MoveToWallShootingPosition(10, 0.3),
//			new ShootBall()
	};
}

bool ShootingStrategy::Run(World *world) {
	// Check for target goal = 0, which means stop processing and run special location 0
	std::vector<Step*> steps;
	if (world->GetTargetGoal() != 0) {
		steps = locationSteps[world->GetStartingPosition()];
	} else {
		steps = locationSteps[0];
	}

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
