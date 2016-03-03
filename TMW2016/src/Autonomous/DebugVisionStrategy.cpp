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
	steps.push_back(std::unique_ptr<Step>(new SearchForGoal(7, 0.3)));
//	steps.push_back(std::unique_ptr<Step>(new AlignWithGoal(7, 0.3)));
	steps.push_back(std::unique_ptr<Step>(new AlignWithGoalPID(7, 0.3)));
}
