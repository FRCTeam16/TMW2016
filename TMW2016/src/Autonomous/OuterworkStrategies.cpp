/*
 * OuterworkStrategies.cpp
 */

#include "OuterworkStrategies.h"
#include "Step.h"

//LowBarStrategy::LowBarStrategy(double armDelay_) : armDelay(armDelay_){
//	forwardStep.reset(new ForwardCheckRoll(0.3));
//	armStep.reset(new SetArmPosition(940));
//}
//
//void LowBarStrategy::Init() {
//}
//
//bool LowBarStrategy::Run(World *world) {
//	cout << "LowBarStrategy::Run\n";
//	if (startTime < 0) {
//			startTime = world->GetClock();
//	}
//
//	bool fwdComplete = forwardStep->operator ()(world);
//	bool armComplete = false;
//	if (world->GetClock() - startTime > armDelay) {
//		armComplete = armStep->operator ()(world);
//	}
//	return fwdComplete && armComplete;
//}

void LowBarStrategy::Init() {
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.75, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.1, 0.0, 0.0, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(940)));
	steps.push_back(std::unique_ptr<Step>(new ForwardCheckRoll(0.60)));
}

// ---------------------------------------------------------------------------//

void RoughTerrainStrategy::Init() {
	cout << "RoughTerrainStrategy::Init\n";
	// Obstacle Strategy
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.25, 0.0, 0.75, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new TimedCrab(0.1, 0.0, 0.0, 0.0)));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(838)));
	steps.push_back(std::unique_ptr<Step>(new ForwardCheckRoll(0.75)));
	cout << "RoughTerrainStrategy::Init with " << steps.size() << " steps\n";
}

// ---------------------------------------------------------------------------//
