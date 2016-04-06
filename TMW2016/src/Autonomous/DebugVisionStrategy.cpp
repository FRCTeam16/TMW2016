/*
 * DebugVisionStrategy.cpp
 */

#include "DebugVisionStrategy.h"
#include "AlignWithGoal.h"
#include "AlignGoalUsingPID.h"
#include "SearchForGoal.h"
#include "CollideWithWall.h"
#include "TraverseObstacleSteps.h"

DebugVisionStrategy::DebugVisionStrategy() {
}

DebugVisionStrategy::~DebugVisionStrategy() {
}

void DebugVisionStrategy::Init(World* world) {


	// different test
	if (false) {
		const bool doShoot = true;
		const bool useVision = world->GetTargetGoal() < 4 || world->GetTargetGoal() == 6;	// FIXME: Target Goal ID Handling

		if (doShoot) {
			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::ShooterHigh, false)));
			steps.push_back(std::unique_ptr<Step>(new ControlShooterMotors(true)));
		}

		if (useVision) {
			steps.push_back(std::unique_ptr<Step>(new SearchForGoal(7, 0.3)));
			steps.push_back(std::unique_ptr<Step>(new AlignWithGoalAndShoot(7, 0.15)));
		} else {
			steps.push_back(std::unique_ptr<Step>(new MoveToWallShootingPosition(10, 0.3)));
		}

		if (doShoot) {
			steps.push_back(std::unique_ptr<Step>(new ShootBall()));
		}
	}

	// ALignment tests
	if (false)
	{
		steps.push_back(std::unique_ptr<Step>(new AlignGoalUsingPID(0.3)));
	}

	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, false)));
	steps.push_back(std::unique_ptr<Step>(new ControlBeaterBar(false)));
	steps.push_back(std::unique_ptr<Step>(new CollideWithWall(0.0, -0.3, 0.0, 0.8)));
	//steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.0, 0.2)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.3, -2.0, 5, false, 0.0)));
}
