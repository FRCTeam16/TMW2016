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
	const int targetGoal = world->GetTargetGoal();
	const bool useVision = targetGoal < 4;
	const bool waitForArm = false;

	// ALignment tests
	if (false)
	{
		steps.push_back(std::unique_ptr<Step>(new AlignGoalUsingPID(0.3)));
	}

	// Center Strategy
	if (true) {
		steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::ShooterHigh, waitForArm)));
		steps.push_back(std::unique_ptr<Step>(new ControlShooterMotors(true)));

		if (useVision) {
			steps.push_back(std::unique_ptr<Step>(new SearchForGoal(5, 0.3)));
			steps.push_back(std::unique_ptr<Step>(new AlignWithGoalAndShoot(5, 0.2)));
		} else {
			steps.push_back(std::unique_ptr<Step>(new MoveToWallShootingPosition(10, 0.3)));
			steps.push_back(std::unique_ptr<Step>(new SnugToWall(0.3, 0.25)));
			steps.push_back(std::unique_ptr<Step>(new ShootBall(false)));
		}

		// Return Strategy
		if (false) {
			steps.push_back(std::unique_ptr<Step>(new AlignWithGoalAndShoot(5.0, 0.3, true)));
			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, false)));
			steps.push_back(std::unique_ptr<Step>(new Turn(180.0)));
			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
			steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.75, 180.0, 5, false, 0.0)));
			steps.push_back(std::unique_ptr<Step>(new LockWheels()));
		}


//		if (retraverse) {
//			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, false)));
//			steps.push_back(std::unique_ptr<Step>(new ControlBeaterBar(false)));
//			steps.push_back(std::unique_ptr<Step>(new TimedCrab(1.0, 0.0, 0.3, -0.6)));
//			steps.push_back(std::unique_ptr<Step>(new CollideWithWall(0.0, -0.5, -0.0, 7)));
//			steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.5, -2.0, 5, false, 0.0)));
//		}
	}
}
