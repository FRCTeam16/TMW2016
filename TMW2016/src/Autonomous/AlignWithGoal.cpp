/*
 * AlignWithGoal.cpp
 */

#include "../Robot.h"
#include "AlignWithGoal.h"
#include <math.h>

bool SearchForGoal::operator()(World *world) {
	cout << "SearchForGoal()\n";
	const float currentTime = world->GetClock();
	const bool goalDetected = world->GetVisionData().HasData();
	const float timeDelta = currentTime - startTime;

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if (timeDelta > timeout) {
		std::cerr << "SearchForGoal: timed out, halting\n";
		crab->Stop();
		return false;
	} else if (goalDetected && timeDelta > minTime) {
		cout << "SearchForGoal: found the goal\n";
		return true;
	}

	// Calculate movement
	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();
	const float driveAngle = CalculateDriveAngle(startingPosition, targetGoal);
	cout << "SearchForGoal: driveAngle = " << driveAngle << "\n";
	const float radians = driveAngle * M_PI / 180.0;
	const float x = speed * sin(radians);
	const float y = speed * cos(radians);
	crab->Update(driveAngle, y, x, true);
	return false;
}

float SearchForGoal::CalculateDriveAngle(const int pos, const int goal) {
	//all distances in inches
	float dShoot = 108;  //distance from goal for shot
	float dxGoalFromWall = 141; //distance in x of the goal from the left wall
	float dxGoal = 7.5; // additional distance in x after the start of the goal
	float dyGoal = 13; // additional distance in y after the start of the goal
	float dxDefense = 24 + (pos-1)*53; //distance in x of the final robot position after crossing the defense
	float dyDefense = 156; //distance in y of the final robot position after crossing the defense (not 135)
	float dx = 0;
	float dy = 0;

	const float PI_3 = M_PI / 3;
	if(goal == 1) {
		dxGoal = 7.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal - dShoot*sin(PI_3) - dxDefense;
		dy = dyDefense - (dyGoal+dShoot*cos(PI_3));
	}
	if (goal == 2) {
		dxGoal = 30;
		dyGoal = 26;
		dx = dxGoalFromWall + dxGoal - dxDefense;
		dy = dyDefense - (dyGoal + dShoot);
	}
	if(goal == 3) {
		dxGoal = 52.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal + dShoot*sin(PI_3) - dxDefense;
		dy = dyDefense - (dyGoal+dShoot*cos(PI_3));
	}
	cout << "dx: " << dx << " dy: " << dy << endl;
	return atan2(dx,dy);
}

//--------------------------------------------------------------------------//


AlignWithGoal::AlignWithGoal(float timeout_) :timeout(timeout_), pidXAdapter(new VisionPIDAdapter())
{
	Preferences *prefs = Preferences::GetInstance();
	pidX.reset(new PIDController(
				prefs->GetFloat("VisionXP"),
				prefs->GetFloat("VisionXI"),
				prefs->GetFloat("VisionXD"),
				pidXAdapter.get(),
				pidXAdapter.get(),
				0.2));
	pidX->SetSetpoint(0.0);
	pidX->SetContinuous(true);
	pidX->SetOutputRange(-0.3, 0.3);
	pidX->SetInputRange(-160, 160);
	pidX->Enable();
}

bool AlignWithGoal::operator()(World *world) {
	cout << "AlignWithGoal()\n";
	const float currentTime = world->GetClock();

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if ((currentTime - startTime > timeout)) {
		std::cerr << "AlignWithGoal: timed out, halting\n";
		crab->Stop();
		return false;
	}

	const VisionData &vd = world->GetVisionData();
	const bool detectedGoal = vd.HasData();
	const int xthreshold = 5;

	// Blind searching for goal using default movement
	if (!detectedGoal) {
		cout << "AlignWithGoal: No goal visible, stopping...\n";
		crab->Stop();
		return false;
	} else {
		// We've detected goal
		const float currentX = pidXAdapter->PIDGet();
		pidXAdapter->Update(currentX);
		if (fabs(currentX) < xthreshold) {
			std::cout << "AlignWithGoal: Goal aligned!\n";
			crab->Stop();
			return true;
		} else {
			const float targetAngle = Robot::driveBase->imu->GetYaw();
			const float radians = targetAngle * M_PI / 180.0;
			const float magnitude = pidXAdapter->PIDGet();
			cout << "AlignWithGoal adjusting position [" << currentX << " -> speed: " << magnitude << "...\n";
			const float x = magnitude * sin(radians);
			const float y = magnitude * cos(radians);
			crab->Update(targetAngle, y, x, true);
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

//--------------------------------------------------------------------------//



