/*
 * Strategy.cpp
 */

#include "Strategy.h"
#include "ShootingStrategy.h"
#include "../Robot.h"

#include <iomanip>

// --------------------------------------------------------------------------//

bool StepStrategy::Run(World *world) {
	// All work is finished in this strategy
	if (currentStep >= steps.size()) {
		RunPeriodicManagers(STOP.get());
		return true;
	}
	Step* step = steps[currentStep].get();
	bool stepComplete = step->operator ()(world);
	RunPeriodicManagers(step->GetCrabInfo());
	if (stepComplete) {
		currentStep++;
	}
	return false;
}


void StepStrategy::RunPeriodicManagers(const CrabInfo *crab) {
	// Must fire every invocation
	cout << "RunPeriodicManager -> ";
	cout 	<< "\ttwist: " << setw(5) << crab->twist
			<< "  yspeed: " << setw(5) << crab->yspeed
			<< "  xspeed: " << setw(5) << crab->xspeed
			<< "\n";

	// If we are in lock mode, do not send information
	if (!crab->lock) {
		Robot::driveBase->Crab(crab->twist, crab->yspeed, crab->xspeed, crab->gyro);
		Robot::driveBase->tankLeft->Set(crab->yspeed);
		Robot::driveBase->tankRight->Set(crab->yspeed);
	} else {
		Robot::driveBase->Lock();
	}
	Robot::arm->DartManager();
	Robot::arm->FireManager();
	Robot::arm->ShooterManager();
	Robot::arm->BeaterBar(crab->beater_bar);

}

// --------------------------------------------------------------------------//


bool NoOpStrategy::Run(World *world) {
	cout << "NoOpStrategy::Run\n";
	return true;
}

// --------------------------------------------------------------------------//

OuterworkAndShootStrategy::OuterworkAndShootStrategy(Strategy *outerworkStrategy_) :
	outerworkStrategy { outerworkStrategy_ },
	shootingStrategy { new ShootingStrategy() } {}

bool OuterworkAndShootStrategy::Run(World *world) {
	cout << "OuterworkAndShootStrategy base\n";
	if (!outerworkStrategyComplete) {
		if (outerworkStrategyStarted) {
			outerworkStrategyStarted = false;
			outerworkStrategy->Init(world);
		}
		outerworkStrategyComplete = outerworkStrategy->Run(world);
		return false;	// always return false to trigger shooting strategy run
	} else {
		if (firstTimeShooting) {
			firstTimeShooting = false;
			shootingStrategy->Init(world);
		}
		bool runningShooterComplete = shootingStrategy->Run(world);
		if (runningShooterComplete) {
			RunPeriodicManagers(STOP.get());
		}
		return runningShooterComplete;
	}
}


// --------------------------------------------------------------------------//




