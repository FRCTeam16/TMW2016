/*
 * AlignWithGoal.cpp
 */

#include "AlignWithGoal.h"

AlignWithGoal::AlignWithGoal(float timeout_, float angle_, float yspeed_, float xspeed_, float threshold_) :
	timeout(timeout_), targetAngle(angle_), ySpeed(yspeed_), xSpeed(xspeed_), threshold(threshold_),
	pidXAdapter(new VisionPIDAdapter()),
	pidX(new PIDController(1.0, 0.0, 0.0, pidXAdapter.get(), pidXAdapter.get(), 0.2))
{
	pidX->SetSetpoint(0.0);
	pidX->SetContinuous(true);
	pidX->SetOutputRange(-1.0, 1.0);
	pidX->SetInputRange(-160, 160);
	pidX->Enable();
}

bool AlignWithGoal::operator()(World *world) {
	const float currentTime = world->GetClock();

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if ((currentTime - startTime > timeout)) {
		std::cerr << "AlignWithGoal timed out, halting\n";
		crab->Update(0.0, 0.0, 0.0, true);
		return false;
	}

	const VisionData &vd = world->GetVisionData();
	detectedGoal = vd.HasData();

	// Blind searching for goal using default movement
	if (!detectedGoal) {
		cout << "Searching for goal...\n";
		crab->Update(targetAngle, ySpeed, xSpeed, true);
		return false;
	} else {
		// We've detected goal
		const float currentX = pidXAdapter->PIDGet();
		pidXAdapter->Update(currentX);
		if (fabs(currentX) < threshold) {
			std::cout << "Goal aligned!\n";
			crab->Stop();
			return true;
		} else {
			cout << "Tracking X with PID\n";
			crab->Update(targetAngle, ySpeed, pidXAdapter->PIDGet(), true);
		}
	}
	return false;
}

//--------------------------------------------------------------------------//

void VisionPIDAdapter::Update(const float value) {
	inputValue = value;
}

double VisionPIDAdapter::PIDGet() {
	return inputValue;
}

void VisionPIDAdapter::PIDWrite(float output) {
	adjustedValue = output;
}



