/*
 * AlignWithGoal.cpp
 */

#include "../Robot.h"
#include "AlignWithGoal.h"
#include <math.h>

bool SearchForGoal::operator()(World *world) {
	cout << "SearchForGoal()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
			startTime = currentTime;
	}
	const bool goalDetected = world->GetVisionData().HasData();
	const float timeDelta = currentTime - startTime;
	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();

	float minTimes[5][3] = {
			{2,3,5},
			{2,2,5},
			{2,0,2},
			{2,0,2},
			{5,3,2}
	};
	const float minTime = minTimes[startingPosition-1][targetGoal-1];
	cout << "MinTime: " << minTime << '\n';

	// Startup & Timeout Checks

	if (timeDelta > timeout) {
		std::cerr << "SearchForGoal: timed out, halting\n";
		crab->Stop();
		return false;
	} else if (goalDetected && timeDelta > minTime) {
		cout << "SearchForGoal: found the goal\n";
		return true;
	}

	// Calculate movement
	const float driveAngleRadians = CalculateDriveAngle(startingPosition, targetGoal);
	cout << "SearchForGoal: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
	const float x = speed * sin(driveAngleRadians);
	const float y = speed * cos(driveAngleRadians);

	float yawSetpoint;
	switch (targetGoal) {
	case 1:
		yawSetpoint = 60.0;
		break;
	case 2:
		yawSetpoint = 0.0;
		break;
	case 3:
		yawSetpoint = 0.0;
		break;
	}
	Robot::driveBase->DriveControlTwist->SetSetpoint(yawSetpoint);
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
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
	cout << "dx: " << dx << " dy: " << dy << " atan2(dx,dy) = " << atan2(dx,dy) << endl;
	return atan2(dx,dy);
}

void DebugDisplayCalculateDriveAngle() {
	for (int i=0;i<100;i++) { cout  << '\n'; }
	for (int pos=1;pos<=5;pos++) {
		for (int t=1;t<=3;t++) {
			cout << "P: " << pos
					<< " T: " << t
					<< " Angle: "
					<< SearchForGoal::CalculateDriveAngle(pos,t) * 180 / M_PI
					<< '\n';
		}
	}
}

//--------------------------------------------------------------------------//


AlignWithGoal::AlignWithGoal(float timeout_, float speed_) :timeout(timeout_), speed(speed_), pidXAdapter(new VisionPIDAdapter())
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
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
}

bool AlignWithGoal::operator()(World *world) {
	cout << "AlignWithGoal()\n";
	const float currentTime = world->GetClock();

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if ((currentTime - startTime) > timeout) {
		std::cerr << "AlignWithGoal: timed out, halting\n";
		crab->Stop();
		return true;
	}

	const VisionData vd = world->GetVisionData();
	const bool detectedGoal = vd.HasData();
	const int xthreshold = 5;

	Preferences *prefs = Preferences::GetInstance();
//	const float P = prefs->GetFloat("VisionXP");
	cout << "Read P from prefs as: " << prefs->GetFloat("VisionXP") << "\n";
	const float P = -0.05;

	// Verify goal is visible
	if (!detectedGoal) {
		cout << "AlignWithGoal: No goal visible, stopping...\n";
		crab->Stop();
		return true;
	} else {
		// We've detected goal
		const float currentX = vd.xposition;
		if (fabs(currentX) < xthreshold) {
			std::cout << "AlignWithGoal: Goal aligned!\n";
			if (++fineTuneCounter > 10) {
				cout << "QUIETED READY TO SHOOT\n";
				crab->Stop();
				return true;
			} else {
				cout << "Waiting for lock on to goal: " << fineTuneCounter << "\n";
			}
		} else {
			fineTuneCounter = 0;
			const float targetAngle = Robot::driveBase->imu->GetYaw();
			const float radians = targetAngle * M_PI / 180.0;
			cout << "AlignWithGoal P: " << pidX->GetP() << " I: " << pidX->GetI() << " D: " << pidX->GetD() << "\n";
			const float magnitude = max(P * currentX, 1.0f);
			cout << "AlignWithGoal adjusting position [" << currentX << " -> speed: " << magnitude << "...\n";
			const float x = magnitude * sin(radians);
			const float y = magnitude * cos(radians);

			Robot::driveBase->DriveControlTwist->SetSetpoint(targetAngle);
			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);

//		pidXAdapter->Update(currentX);
//		if (fabs(currentX) < xthreshold) {
//			std::cout << "AlignWithGoal: Goal aligned!\n";
//			// 1. Lock, 2. Rverse? 3. PID
//			// Add a fine tune loop
//			//
//			if (++fineTuneCounter > 10) {
//				cout << "QUIETED READY TO SHOOT\n";
//				crab->Stop();
//				return true;
//			} else {
//				cout << "Waiting for lock on to goal: " << fineTuneCounter << "\n";
//			}
//		} else {
//			fineTuneCounter = 0;
//			const float targetAngle = Robot::driveBase->imu->GetYaw();
//			const float radians = targetAngle * M_PI / 180.0;
//			const float magnitude = pidXAdapter->GetOutputValue();
//			cout << "AlignWithGoal adjusting position [" << currentX << " -> speed: " << magnitude << "...\n";
//			const float x = magnitude * sin(radians);
//			const float y = magnitude * cos(radians);
//
//			Robot::driveBase->DriveControlTwist->SetSetpoint(targetAngle);
//			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
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
	outputValue = output;
}

double VisionPIDAdapter::GetOutputValue() {
	return outputValue;
}

//--------------------------------------------------------------------------//



