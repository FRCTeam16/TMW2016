/*
 * TeleopStrategy.cpp
 */

#include "TeleopStrategy.h"
#include "AlignGoalUsingPID.h"

TeleopStrategy::TeleopStrategy() : world(new World()){
	cout << "TeleopStrategy::TeleopStrategy\n";
	started = true;	// hack
	Reset();
}

TeleopStrategy::~TeleopStrategy() {

}

void TeleopStrategy::Reset() {
	if (started) {
		cout << "TeleopStrategy::Reset\n";
		steps.clear();
		steps.push_back(std::unique_ptr<Step>(new AlignGoalUsingPID(0.3)));
		started = false;
		currentStep = 0;	// reset current step executing
	}
};

bool TeleopStrategy::RunPeriodic(VisionData visionData) {
	cout << "TeleopStrategy::RunPeriodic\n";
	world->Update(visionData);
	if (!started) { started = true; }
	return StepStrategy::Run(world.get());
	return false;
}

