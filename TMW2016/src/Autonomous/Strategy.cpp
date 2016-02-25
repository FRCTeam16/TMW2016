/*
 * Strategy.cpp
 */

#include "Strategy.h"
#include "ShootingStrategy.h"
#include "../Robot.h"

#include <iomanip>

// --------------------------------------------------------------------------//
static CrabInfo * STOP = new CrabInfo();

bool StepStrategy::Run(World *world) {
	cout << "StepStrategy::Invoke\n";
	// All work is finished in this strategy
	if (currentStep >= steps.size()) {
		RunPeriodicManagers(STOP);	// FIXME: memory leak?
		return true;
	}

	Step* step = steps[currentStep].get();
	bool stepComplete = step->operator ()(world);
	RunPeriodicManagers(step->GetCrabInfo());

	if (stepComplete) {
		currentStep++;
		cout << "Advancing to Step: " << currentStep << '\n';
	}
	return false;
}

void StepStrategy::RunPeriodicManagers(const CrabInfo *crab) {
	// Must fire every invocation
	cout << "RunPeriodicManager\n";
	cout 	<< "twist: " << setw(5) << crab->twist
			<< "  yspeed: " << setw(5) << crab->yspeed
			<< "  xspeed: " << setw(5) << crab->xspeed
			<< "\n";
	Robot::driveBase->Crab(crab->twist, crab->yspeed, crab->xspeed, crab->gyro);
	Robot::driveBase->tankLeft->Set(crab->yspeed);
	Robot::driveBase->tankRight->Set(crab->yspeed);
	Robot::arm->DartManager();
	Robot::arm->FireManager();
	Robot::arm->ShooterManager();
}

// --------------------------------------------------------------------------//


bool NoOpStrategy::Run(World *world) {
	cout << "NoOpStrategy::Run\n";
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
		if (firstTimeShooting) {
			firstTimeShooting = false;
			shootingStrategy->Init();
		}
		bool runningShooterComplete = shootingStrategy->Run(world);
		if (runningShooterComplete) {
			RunPeriodicManagers(STOP);	// FIXME: memory leak?
		}
		return runningShooterComplete;
	}
}


// --------------------------------------------------------------------------//




