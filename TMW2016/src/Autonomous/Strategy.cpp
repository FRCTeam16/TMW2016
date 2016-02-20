/*
 * Strategy.cpp
 */

#include "Strategy.h"
#include "ShootingStrategy.h"


// --------------------------------------------------------------------------//


bool StepStrategy::Run(World *world) {
	cout << "StepStrategy::Invoke\n";
	// All work is finished in this strategy
	if (currentStep >= steps.size()) {
		return true;
	}

	Step* step = steps[currentStep].get();
	bool stepComplete = step->operator ()(world);
	Drive(step->GetCrabInfo());

	if (stepComplete) {
		currentStep++;
		cout << "Advancing to Step: " << currentStep << '\n';
	}
	return false;
}

void StepStrategy::Drive(const CrabInfo *crab) {
	driveBase->Crab(crab->twist, crab->yspeed, crab->xspeed, crab->gyro);
}

// --------------------------------------------------------------------------//


bool NoOpStrategy::Run(World *world) {
	cout << "NoOpStrategy exiting\n";
	return true;
}

// --------------------------------------------------------------------------//

OuterworkAndShootStrategy::OuterworkAndShootStrategy(Strategy *outerworkStrategy_) {
	outerworkStrategy.reset(outerworkStrategy_);
	shootingStrategy.reset(new ShootingStrategy());
}

bool OuterworkAndShootStrategy::Run(World *world) {
	cout << "OuterworkAndShootStrategy base\n";
	if (!outerworkComplete) {
		if (firstOuterwork) {
			firstOuterwork = false;
			outerworkStrategy->Init();
		}
		outerworkComplete = outerworkStrategy->Run(world);
		return false;	// always return false to trigger shooting strategy run
	} else {
		if (firstShooting) {
			firstShooting = false;
			shootingStrategy->Init();
		}
		return shootingStrategy->Run(world);
	}
}


// --------------------------------------------------------------------------//




