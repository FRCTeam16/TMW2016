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
	steps.push_back(std::unique_ptr<Step>(new AlignWithGoal(2, 0.0, 0.3, 0.30, 3.0)));
}
