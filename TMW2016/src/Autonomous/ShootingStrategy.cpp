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
	bool useVision =  targetGoal < 4;
	bool waitForArm = true;

	// Location Step based on starting position

	//----------------------------------------------------------------------//
	// Noop
	locationSteps[0] = {
			new LockWheels()
	};

	//----------------------------------------------------------------------//
	// Left Goal
	locationSteps[1] = {};
	constructLeftGoalSteps(locationSteps[1], useVision, waitForArm);


	//----------------------------------------------------------------------//
	// Center Goal
	locationSteps[2] = {};
	constructCenterGoalSteps(locationSteps[2], useVision, waitForArm);

	//----------------------------------------------------------------------//
	// Unused Right Goal
	locationSteps[3] = {};
	constructCenterGoalSteps(locationSteps[3], useVision, waitForArm);

	//----------------------------------------------------------------------//
	// Left blind
	locationSteps[4] = {};
	constructCenterGoalSteps(locationSteps[4], useVision, waitForArm);

	//----------------------------------------------------------------------//
	// Unused Right Blind
	locationSteps[5] = {};
	constructCenterGoalSteps(locationSteps[5], useVision, waitForArm);

	//----------------------------------------------------------------------//
}

void ShootingStrategy::constructLeftGoalSteps(
		std::vector<Step*> &vec, const bool useVision, const bool waitForArm) {
	cout << "ShootingStrategy::constructLeftGoalSteps\n";

	vec.push_back(new SetArmPosition(SetArmPosition::Position::ShooterHigh, false));
	vec.push_back(new MoveAlongMoveToWallShootingLine(0.5, 0.3)); // kick out from left wall
	vec.push_back(new ControlShooterMotors(true));

	if (useVision) {
		vec.push_back(new SearchForGoal(5, 0.1));
		vec.push_back(new AlignWithGoalAndShoot(8, 0.1));
	} else {
		vec.push_back(new SetArmPosition(SetArmPosition::Position::Wallshot, false));
		vec.push_back(new MoveToWallShootingPosition(10, 0.3, 0.9));
		vec.push_back(new SnugToWall(0.3, 0.25));
		vec.push_back(new ShootBall(false));
	}
}

void ShootingStrategy::constructCenterGoalSteps(
		std::vector<Step*> &vec, const bool useVision, const bool waitForArm) {
	cout << "ShootingStrategy::constructCenterGoalSteps\n";
	vec.push_back(new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm));
	vec.push_back(new ControlShooterMotors(true));

	if (useVision) {
		vec.push_back(new SearchForGoal(5, 0.1));
		vec.push_back(new AlignWithGoalAndShoot(8, 0.1));
	} else {
		vec.push_back(new SetArmPosition(SetArmPosition::Position::Wallshot, false));
		vec.push_back(new MoveToWallShootingPosition(10, 0.3, 0.9));
		vec.push_back(new SnugToWall(0.3, 0.25));
		vec.push_back(new ShootBall(false));
	}
}

bool ShootingStrategy::Run(World *world) {
	// Check for target goal = 0, which means stop processing and run special location 0
	std::vector<Step*> steps;
	const int targetGoal = world->GetTargetGoal();
	if (targetGoal != 0) {
		const int startingPosition = world->GetStartingPosition();
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
