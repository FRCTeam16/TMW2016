/*
 * DebugVisionStrategy.cpp
 */

#include "DebugVisionStrategy.h"
#include "AlignWithGoal.h"
#include "AlignGoalUsingPID.h"
#include "SearchForGoal.h"

DebugVisionStrategy::DebugVisionStrategy() {

}

DebugVisionStrategy::~DebugVisionStrategy() {
}

void DebugVisionStrategy::Init(World* world) {

	if (false) {
		const bool doShoot = true;
		const bool useVision = world->GetTargetGoal() < 4;

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
	} else {
		steps.push_back(std::unique_ptr<Step>(new AlignGoalUsingPID(0.3)));
	}

//	steps.push_back(std::unique_ptr<Step>(new AlignWithGoal(7, 0.15)));
}
