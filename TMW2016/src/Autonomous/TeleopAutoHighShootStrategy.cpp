/*
 * TeleopStrategy.cpp
 */

#include "TeleopAutoHighShootStrategy.h"

#include "AlignGoalUsingPID.h"

TeleopAutoHighShootStrategy::TeleopAutoHighShootStrategy() {
	cout << "TeleopStrategy::TeleopStrategy\n";
	started = true;	// hack
	Reset();
}

TeleopAutoHighShootStrategy::~TeleopAutoHighShootStrategy() {

}

void TeleopAutoHighShootStrategy::Reset() {
	if (started) {
		cout << "TeleopStrategy::Reset\n";
		steps.clear();
		steps.push_back(std::unique_ptr<Step>(new AlignGoalUsingPID(0.3, true)));
		started = false;
		currentStep = 0;	// reset current step executing
	}
};

bool TeleopAutoHighShootStrategy::RunPeriodic(World *world) {
//	cout << "TeleopStrategy::RunPeriodic\n";
	if (!started) { started = true; }
	return StepStrategy::Run(world);
	return false;
}

