/*
 * ShootingStrategy.cpp
 */

#include "ShootingStrategy.h"
#include "AlignWithGoal.h"
#include "SearchForGoal.h"
#include "CollideWithWall.h"
#include "TraverseObstacleSteps.h"

/**
 * Use world start and target position to calculate angle to move to search for goal
 * Switch to vision based targeting step
 * 	- PID controllers for X and Y to send data to crab
 */

void ShootingStrategy::Init(World* world) {
	bool useVision =  world->GetTargetGoal() < 4 || world->GetTargetGoal() == 6;	// FIXME: Target Goal ID Handling
	bool waitForArm = true;

	//----------------------------------------------------------------------//

	locationSteps[0] = {
			new LockWheels()
	};

	//----------------------------------------------------------------------//

	locationSteps[1] = {};
	constructLeftGoalSteps(locationSteps[1], useVision, waitForArm, false);


	//----------------------------------------------------------------------//

	locationSteps[2] = {
			new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm),
			new ControlShooterMotors(true)
	};
	if (useVision) {
		locationSteps[2].push_back(new SearchForGoal(5, 0.3));
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
		locationSteps[3].push_back(new SearchForGoal(5, 0.3));
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
		locationSteps[4].push_back(new SearchForGoal(5, 0.3));
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
		locationSteps[5].push_back(new SearchForGoal(5, 0.3));
		locationSteps[5].push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		locationSteps[5].push_back(new MoveToWallShootingPosition(10, 0.3));
		locationSteps[5].push_back(new SnugToWall(0.3, 0.25));
		locationSteps[5].push_back(new ShootBall(false));
	}

	//----------------------------------------------------------------------//

	locationSteps[6] = {};
	constructLeftGoalSteps(locationSteps[6], useVision, waitForArm, true);
}

void ShootingStrategy::constructLeftGoalSteps(
		std::vector<Step*> &vec, const bool useVision, const bool waitForArm, const bool retraverse) {
	cout << "ShootingStrategy::constructLeftGoalSteps\n";

	vec.push_back(new MoveAlongMoveToWallShootingLine(0.5, 0.3)); // kick out from left wall
	vec.push_back(new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm));
	vec.push_back(new ControlShooterMotors(true));

	if (useVision) {
		vec.push_back(new SearchForGoal(5, 0.3));
		vec.push_back(new AlignWithGoalAndShoot(5, 0.2));
	} else {
		vec.push_back(new MoveToWallShootingPosition(10, 0.3));
		vec.push_back(new SnugToWall(0.3, 0.25));
		vec.push_back(new ShootBall(false));
	}

	if (retraverse) {
		vec.push_back(new SetArmPosition(SetArmPosition::Position::Pickup, false));
		vec.push_back(new ControlBeaterBar(false));
		vec.push_back(new CollideWithWall(0.0, -0.3, 0.0, 0.8));
		vec.push_back(new TraverseObstacleWithGyroAndSonarLockingValues(-0.3, -2.0, 5, false, 0.0));
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
