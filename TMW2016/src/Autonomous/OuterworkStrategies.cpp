/*
 * OuterworkStrategies.cpp
 */

#include "OuterworkStrategies.h"
#include "Step.h"
#include "AlignWithGoal.h"


void LowBarStrategy::Init() {
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyro(0.60)));
}

// ---------------------------------------------------------------------------//

void RoughTerrainStrategy::Init() {
	cout << "RoughTerrainStrategy::Init\n";
	// Obstacle Strategy
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyro(0.75)));
	cout << "RoughTerrainStrategy::Init with " << steps.size() << " steps\n";
}

void MoatStrategy::Init() {
	cout << "RoughTerrainStrategy::Init\n";
	// Obstacle Strategy
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyro(0.75, 4)));
	cout << "RoughTerrainStrategy::Init with " << steps.size() << " steps\n";
}




// ---------------------------------------------------------------------------//

void ChevalDeFriseStrategy::Init() {
	cout << "ChevalDeFriseStrategy::Init\n";
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.40, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
	steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyro(0.75)));
}
