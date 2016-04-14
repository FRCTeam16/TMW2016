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
	const int targetGoal = world->GetTargetGoal();
	bool useVision =  targetGoal < 4 || targetGoal == 6 || targetGoal == 7;
	bool waitForArm = true;

	//----------------------------------------------------------------------//

	locationSteps[0] = {
			new LockWheels()
	};

	//----------------------------------------------------------------------//

	locationSteps[1] = {};
	constructLeftGoalSteps(locationSteps[1], useVision, waitForArm, false);


	//----------------------------------------------------------------------//

	locationSteps[2] = {};
	constructCenterGoalSteps(locationSteps[2], useVision, waitForArm, false);

	//----------------------------------------------------------------------//

	locationSteps[3] = {};
	constructCenterGoalSteps(locationSteps[3], useVision, waitForArm, false);

	//----------------------------------------------------------------------//

	locationSteps[4] = {};
	constructCenterGoalSteps(locationSteps[4], useVision, waitForArm, false);

	//----------------------------------------------------------------------//

	locationSteps[5] = {};
	constructCenterGoalSteps(locationSteps[5], useVision, waitForArm, false);

	//----------------------------------------------------------------------//
	// Left Goal with Return
	locationSteps[6] = {};
	constructLeftGoalSteps(locationSteps[6], useVision, waitForArm, true);

	//----------------------------------------------------------------------//
	// Center Goal with Return
	locationSteps[7] = {};
	constructLeftGoalSteps(locationSteps[7], useVision, waitForArm, true);
}

void ShootingStrategy::constructLeftGoalSteps(
		std::vector<Step*> &vec, const bool useVision, const bool waitForArm, const bool retraverse) {
	cout << "ShootingStrategy::constructLeftGoalSteps(re-traverse? " << retraverse << "\n";

	vec.push_back(new SetArmPosition(SetArmPosition::Position::ShooterHigh, false));
	vec.push_back(new MoveAlongMoveToWallShootingLine(0.5, 0.3)); // kick out from left wall
	vec.push_back(new ControlShooterMotors(true));

	if (useVision) {
		vec.push_back(new SearchForGoal(5, 0.3));
		vec.push_back(new AlignWithGoalAndShoot(8, 0.2));
	} else {
		vec.push_back(new MoveToWallShootingPosition(10, 0.3));
		vec.push_back(new SnugToWall(0.3, 0.25));
		vec.push_back(new ShootBall(false));
	}

	if (retraverse) {
		vec.push_back(new SetArmPosition(SetArmPosition::Position::Pickup, false));
		vec.push_back(new ControlBeaterBar(false));
		vec.push_back(new CollideWithWall(0.0, -0.3, -0.5, 0.8));
		vec.push_back(new TraverseObstacleWithGyroAndSonarLockingValues(-0.5, -2.0, 5, false, 0.0));
	}
}

void ShootingStrategy::constructCenterGoalSteps(
		std::vector<Step*> &vec, const bool useVision, const bool waitForArm, const bool retraverse) {
	cout << "ShootingStrategy::constructCenterGoalSteps(re-traverse? " << retraverse << "\n";
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
		vec.push_back(new TimedCrab(1.0, 0.0, 0.3, -0.6));
		vec.push_back(new CollideWithWall(0.0, -0.5, -0.0, 7));
		vec.push_back(new TraverseObstacleWithGyroAndSonarLockingValues(-0.5, -2.0, 5, false, 0.0));
	}
}

bool ShootingStrategy::Run(World *world) {
	// Check for target goal = 0, which means stop processing and run special location 0
	std::vector<Step*> steps;
	const int targetGoal = world->GetTargetGoal();
	if (targetGoal != 0) {
		int startingPosition = world->GetStartingPosition();
		// Handle custom targets
		if (targetGoal == 6) {
			// position 1 return
			startingPosition = 6;
		} else if (targetGoal == 7) {
			// position 2 return
			startingPosition = 7;
		}
		steps = locationSteps[startingPosition];
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
