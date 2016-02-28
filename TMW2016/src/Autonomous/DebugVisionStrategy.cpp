/*
 * DebugVisionStrategy.cpp
 */

#include "DebugVisionStrategy.h"
#include "AlignWithGoal.h"

DebugVisionStrategy::DebugVisionStrategy() {
	// TODO Auto-generated constructor stub

}

DebugVisionStrategy::~DebugVisionStrategy() {
	// TODO Auto-generated destructor stub
}

void DebugVisionStrategy::Init() {
	// timeout, angle, xpseed, yspeed, threshold
	steps.push_back(std::unique_ptr<Step>(new SearchForGoal(2, 0.3)));
	steps.push_back(std::unique_ptr<Step>(new AlignWithGoal(3)));
}
