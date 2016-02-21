/*
 * OuterworkStrategies.cpp
 */

#include "OuterworkStrategies.h"
#include "Step.h"


void LowBarStrategy::Init() {
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.75, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.1, 0.0, 0.0, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, true)));
	steps.push_back(std::unique_ptr<Step>(new ForwardCheckRoll(0.60)));
}

// ---------------------------------------------------------------------------//

void RoughTerrainStrategy::Init() {
	cout << "RoughTerrainStrategy::Init\n";
	// Obstacle Strategy
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.75, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.1, 0.0, 0.0, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new ForwardCheckRoll(0.75)));
	cout << "RoughTerrainStrategy::Init with " << steps.size() << " steps\n";
}

// ---------------------------------------------------------------------------//
