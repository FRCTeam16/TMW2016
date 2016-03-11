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

void ShootingStrategy::Init(World* world) {
	bool useVision =  world->GetTargetGoal() < 4;
	bool waitForArm = true;

	//----------------------------------------------------------------------//

	locationSteps[0] = {
			new LockWheels()
	};

	//----------------------------------------------------------------------//

	locationSteps[1] = {
			new MoveAlongMoveToWallShootingLine(0.5, 0.3), // kick out from left wall
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[1].push_back(new SearchForGoal(3, 0.3));
		locationSteps[1].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[1].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[1].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[1].push_back(new ShootBall(false));
	}

	//----------------------------------------------------------------------//

	locationSteps[2] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[2].push_back(new SearchForGoal(2, 0.3));
		locationSteps[2].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[2].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[2].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[2].push_back(new ShootBall(false));
	}

	//----------------------------------------------------------------------//

	locationSteps[3] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[3].push_back(new SearchForGoal(2, 0.3));
		locationSteps[3].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[3].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[3].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[3].push_back(new ShootBall(false));
	}

	//----------------------------------------------------------------------//

	locationSteps[4] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[4].push_back(new SearchForGoal(2, 0.3));
		locationSteps[4].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[4].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[4].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[4].push_back(new ShootBall(false));
	}

	//----------------------------------------------------------------------//

	locationSteps[5] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[5].push_back(new SearchForGoal(2, 0.3));
		locationSteps[5].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[5].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[5].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[5].push_back(new ShootBall(false));
	}
}

bool ShootingStrategy::Run(World *world) {
	// Check for target goal = 0, which means stop processing and run special location 0
	std::vector<Step*> steps;
	if (world->GetTargetGoal() != 0) {
		steps = locationSteps[world->GetStartingPosition()];
	} else {
		// Target goal 0 is noop
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
