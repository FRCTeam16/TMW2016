/*
 * Strategy.cpp
 */

#include "Strategy.h"
#include "ShootingStrategy.h"
#include "../Robot.h"
#include "ReturnStrategy.h"

#include <iomanip>

// --------------------------------------------------------------------------//

bool StepStrategy::Run(World *world) {
	cout << "Strategy::Run()\n";
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
		float tankSpeed = 0.0;
		if(fabs(Robot::driveBase->imu->GetYaw())>90) {
			tankSpeed = -crab->yspeed;
		} else {
			tankSpeed = crab->yspeed;
		}
		Robot::driveBase->RunTanks(tankSpeed);
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
	shootingStrategy { new ShootingStrategy() },
	returnStrategy { new ReturnStrategy() }
{
}

bool OuterworkAndShootStrategy::Run(World *world) {
	cout << "OuterworkAndShootStrategy base\n";
	if (!outerworkStrategyComplete) {
		if (outerworkStrategyStarted) {
			outerworkStrategyStarted = false;
			outerworkStrategy->Init(world);
		}
		cout << "\t Running Outerwork\n";
		outerworkStrategyComplete = outerworkStrategy->Run(world);
		return false;	// always return false to trigger shooting strategy run
	} else if (!shootingCompleted) {
		if (firstTimeShooting) {
			firstTimeShooting = false;
			shootingStrategy->Init(world);
		}
		cout << "\t Running Shooting\n";
		shootingCompleted = shootingStrategy->Run(world);
		return false;
	} else {
		cout << "Running Return Strategy\n";
		if (firstTimeReturning) {
			cout << "First Time Returning, running init";
			firstTimeReturning = false;
			returnStrategy->Init(world);
		}
		if (returnComplete) {
			RunPeriodicManagers(STOP.get());
			return true;
		}
		cout << "\t Running Return\n";
		returnComplete = returnStrategy->Run(world);
		return returnComplete;
	}
}


// --------------------------------------------------------------------------//




