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



//--------------------------------------------------------------------------//


AlignWithGoal::AlignWithGoal(float timeout_, float speed_) :timeout(timeout_), speed(speed_)
{
}

bool AlignWithGoal::operator()(World *world) {
	cout << "AlignWithGoal()\n";
	const float currentTime = world->GetClock();
	const VisionData vd = world->GetVisionData();
	const bool detectedGoal = vd.HasData();

	cout << "Current Time: " << currentTime << '\n';
	cout << "Start Time: " << startTime << '\n';
	cout << "Vision X    : " << vd.xposition << '\n';
	cout << "Fine Tune   : " << fineTuneCounter << '\n';

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if ((currentTime - startTime) > timeout) {
		std::cerr << "AlignWithGoal: timed out, halting\n";
		crab->lock = true;
		return false;
	}

	// Verify goal is visible
	if (!detectedGoal) {
		cout << "AlignWithGoal: No goal visible, stopping...\n";
		crab->lock = true;
		return false;
	}

	// We've detected goal
	float P = 1.0;
	const float currentX = vd.xposition;
	if (fabs(currentX) < xthreshold) {
		P = 0.5;
		std::cout << "AlignWithGoal: Goal aligned!\n";
		if (++fineTuneCounter > 10) {
			return true;
		}
	} else {
		fineTuneCounter = 0;
	}

	// Calculate movement
	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();
	const float driveAngleRadians = CalculateDriveAngle(startingPosition, targetGoal);
	cout << "SearchForGoal: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
	float magnitude = P * speed;
	const float x = magnitude * sin(driveAngleRadians);
	const float y = magnitude * cos(driveAngleRadians);
	cout << "Align VX: " << currentX << " magnitude: " << magnitude << '\n';
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);

	return false;
}

//--------------------------------------------------------------------------//


AlignWithGoalPID::AlignWithGoalPID(float timeout_, float speed_) :timeout(timeout_), speed(speed_), pidXAdapter(new VisionPIDAdapter())
{
	// Problem reading Preference values
//	Preferences *prefs = Preferences::GetInstance();
//	pidX.reset(new PIDController(
//				prefs->GetFloat("VisionXP"),
//				prefs->GetFloat("VisionXI"),
//				prefs->GetFloat("VisionXD"),
//				pidXAdapter.get(),
//				pidXAdapter.get(),
//				0.2));
	pidX.reset(new PIDController(0.05, 0, 0, pidXAdapter.get(), pidXAdapter.get(), 0.2));
	pidX->SetSetpoint(0.0);
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
}

bool AlignWithGoalPID::operator()(World *world) {
	cout << "AlignWithGoalPID(P=" << pidX->GetP() << " I=" << pidX->GetI() << " D= " << pidX->GetD() << ")\n";
	const float currentTime = world->GetClock();

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
	} else if ((currentTime - startTime) > timeout) {
		std::cerr << "AlignWithGoal: timed out, halting\n";
		crab->lock = true;
		return false;
	}

	const VisionData vd = world->GetVisionData();
	const bool detectedGoal = vd.HasData();

	// Verify goal is visible
	if (!detectedGoal) {
		cout << "AlignWithGoal: No goal visible, stopping...\n";
		crab->Stop();
		return true;
	}

	// We've detected goal
	const float currentX = vd.xposition;
	pidXAdapter->Update(currentX);

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
		// Need to align with goal
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
			const float magnitude = pidXAdapter->GetOutputValue();
			cout << "AlignWithGoal adjusting position [" << currentX << " -> speed: " << magnitude << "...\n";
			const float x = magnitude * sin(radians);
			const float y = magnitude * cos(radians);

			Robot::driveBase->DriveControlTwist->SetSetpoint(targetAngle);
			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
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


bool MoveToWallShootingPosition::operator ()(World* world) {
	cout << "MoveToWallShootingPosition()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}
	const float deltaTime = currentTime - startTime;

	// Watch for global safety timeout
	if (deltaTime > timeout) {
		std::cerr << "MoveToWallShootingPosition: timed out, halting\n";
		crab->Stop();
		return true;
	}

	// Check for collision
	if (!collisionDetected && deltaTime > INITIAL_MOVE_TIME) {
		collisionDetected = DetectCollision();
	}

	// Movement
	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();
	float yawSetpoint = 90.0;
	if (targetGoal == 5) { yawSetpoint = -90.0; }
	Robot::driveBase->DriveControlTwist->SetSetpoint(yawSetpoint);
	float x = 0.0;
	float y = 0.0;

	if (collisionDetected) {
		if (++quietTimer > QUIETING_PERIOD) {
			return true;
		} else {
			cout << "Quieting -> " << quietTimer << " / " << QUIETING_PERIOD << "\n";
		}
	} else {
		const float driveAngleRadians = CalculateDriveAngle(startingPosition, targetGoal);
		cout << "MoveToWallShootingPosition: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
		x = speed * sin(driveAngleRadians);
		y = speed * cos(driveAngleRadians);
	}
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	return false;
}

bool MoveToWallShootingPosition::DetectCollision() {
	// @see http://www.pdocs.kauailabs.com/navx-mxp/examples/collision-detection/
	double current_accel_x = Robot::driveBase->imu->GetWorldLinearAccelX();
	double current_jerk_x = current_accel_x - last_accel_x;
	last_accel_x = current_accel_x;

	double current_accel_y = Robot::driveBase->imu->GetWorldLinearAccelY();
	double current_jerk_y = current_accel_y - last_accel_y;
	last_accel_y = current_accel_y;

	cout << "MoveToWallShootingPosition jerk: x->" << current_jerk_x << " y-> " << current_jerk_y << "\n";

	return ((fabs(current_jerk_x) > COLLISION_THRESHOLD_DELTA_G) ||
			(fabs(current_jerk_y) > COLLISION_THRESHOLD_DELTA_G));
}

//--------------------------------------------------------------------------//


float CalculateDriveAngle(const int pos, const int goal) {
	//all distances in inches
	float dShoot = 108;  //distance from goal for shot
	float dxGoalFromWall = 141; //distance in x of the goal from the left wall
	float dxGoal = 7.5; // additional distance in x after the start of the goal
	float dyGoal = 13; // additional distance in y after the start of the goal
	float dxDefense = 24 + (pos-1)*53; //distance in x of the final robot position after crossing the defense
	float dyDefense = 140; //distance in y of the final robot position after crossing the defense (not 135)
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
	if(goal == 4) {
		dxGoal = 7.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal - dShoot - dxDefense;
		dy = dyDefense - dyGoal;
	}
	if(goal == 5) {
		dxGoal = 52.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal + dShoot - dxDefense;
		dy = dyDefense - dyGoal;
	}
	cout << "dx: " << dx << " dy: " << dy << " atan2(dx,dy) = " << atan2(dx,dy) << endl;
	return atan2(dx,dy);
}

void DebugDisplayCalculateDriveAngle() {
	for (int i=0;i<100;i++) { cout  << '\n'; }
	for (int pos=1;pos<=5;pos++) {
		for (int t=1;t<=5;t++) {
			cout << "P: " << pos
					<< " T: " << t
					<< " Angle: "
					<< CalculateDriveAngle(pos,t) * 180 / M_PI
					<< '\n';
		}
	}
}

