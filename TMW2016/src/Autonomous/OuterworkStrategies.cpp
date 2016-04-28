/*
 * OuterworkStrategies.cpp
 */

#include "OuterworkStrategies.h"
#include "Step.h"
#include "AlignWithGoal.h"
#include "TraverseObstacleSteps.h"
#include "Delay.h"


void DoTwoGoalPrevent(std::vector<std::unique_ptr<Step>> &steps) {
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.35, 0.0, 0.30, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, true)));
	steps.push_back(std::unique_ptr<Step>(new ControlBeaterBar(true)));
	steps.push_back(std::unique_ptr<Step>(new Delay(2.0)));
}


void LowBarStrategy::Init(World* world) {
	const bool twoBallPrevent = world->GetTwoBallPrevent();
	steps.push_back(std::unique_ptr<Step>(new Delay()));
	if (!twoBallPrevent) {
		steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, false)));
		steps.push_back(std::unique_ptr<Step>(new TimedCrab(1.5, 0.0, 0.3, 0.0)));
	} else {
		DoTwoGoalPrevent(steps);
	}
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(0.60)));
}

void RoughTerrainStrategy::Init(World* world) {
	cout << "RoughTerrainStrategy::Init\n";
	const bool twoBallPrevent = world->GetTwoBallPrevent();
	steps.push_back(std::unique_ptr<Step>(new Delay()));
	if (!twoBallPrevent) {
		steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	} else {
		DoTwoGoalPrevent(steps);
	}
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonar(0.75)));
}

void MoatStrategy::Init(World* world) {
	cout << "MoatStrategy::Init\n";
	const bool twoBallPrevent = world->GetTwoBallPrevent();
	steps.push_back(std::unique_ptr<Step>(new Delay()));
	if (!twoBallPrevent) {
		steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	} else {
		DoTwoGoalPrevent(steps);
	}
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonar(0.75)));
}

void RampartStrategy::Init(World* world) {
	cout << "MoatStrategy::Init\n";
	const bool twoBallPrevent = world->GetTwoBallPrevent();
	steps.push_back(std::unique_ptr<Step>(new Delay()));
	if (!twoBallPrevent) {
		steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	} else {
		DoTwoGoalPrevent(steps);
	}
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(720, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonar(0.75)));
}


// ---------------------------------------------------------------------------//
