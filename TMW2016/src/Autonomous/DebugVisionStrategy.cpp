/*
 * DebugVisionStrategy.cpp
 */

#include "DebugVisionStrategy.h"
#include "AlignWithGoal.h"

DebugVisionStrategy::DebugVisionStrategy() {

}

DebugVisionStrategy::~DebugVisionStrategy() {
}

void DebugVisionStrategy::Init() {
//	steps.push_back(std::unique_ptr<Step>(new SearchForGoal(7, 0.3)));
//	steps.push_back(std::unique_ptr<Step>(new AlignWithGoal(7, 0.3)));
//	steps.push_back(std::unique_ptr<Step>(new AlignWithGoalPID(7, 0.3)));

	// Position 1
//	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.0, 0.5)));
	steps.push_back(std::unique_ptr<Step>(new MoveToWallShootingPosition(10, 0.3)));
}
