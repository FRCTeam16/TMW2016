/*
 * AlignWithGoal.cpp
 */

#include "../Robot.h"
#include "AlignWithGoal.h"
#include "utils.h"
#include <math.h>


//--------------------------------------------------------------------------//


bool AlignWithGoalAndShoot::operator()(World *world) {
	cout << "AlignWithGoalAndShoot()\n";
	const float currentTime = world->GetClock();
	const VisionData vd = world->GetVisionData();
	const bool detectedGoal = vd.HasData();
	const int targetGoal = world->GetTargetGoal();
	const bool in_twist = InTwist();
	const bool arm_in_position = Robot::arm->DartInPosition();

	goal = detectedGoal ? vd.GetGoal(targetGoal) : goal;

	cout << "Current Time: " << currentTime << '\n';
	cout << "Start Time  : " << startTime << '\n';
	cout << "Target Goal : " << targetGoal << '\n';
	cout << "goal X      : " << goal.xposition << '\n';
	cout << "ReturnStart?: " << returnToStart << '\n';
	cout << "Fine Tune   : " << fineTuneCounter << '\n';
	cout << "Fired       : " << fired << '\n';
	cout << "Kicker      : " << kickCounter << '\n';
	cout << "In Twist    : " << in_twist << '\n';
	cout << "Arm in Pos  : " << arm_in_position << '\n';

	// Startup & Timeout Checks
	if (startTime < 0) {
		startTime = currentTime;
		vision_center_offset = Preferences::GetInstance()->GetInt("VisionCenterOffset", -11);
		cout << "Set Offset to: " << vision_center_offset << '\n';
	} else if ((currentTime - startTime) > timeout) {
		std::cerr << "AlignWithGoal: timed out, halting\n";
		crab->lock = true;
		crab->beater_bar = 0.0;
		return false;
	}

	// Verify goal is visible
	if (detectedGoal) {
		if (lastGoalX != 999) {
			const int diff = abs(vd.GetGoal(targetGoal).xposition - lastGoalX);
			if (diff < 2) {
				kickCounter++;
			} else {
				kickCounter = 0;
			}
		}
		lastGoalX = vd.GetGoal(targetGoal).xposition;
	}

	const float currentX = goal.xposition;
	int OFFSET = vision_center_offset;
	if (returnToStart) {
		const int returnTarget = world->GetReturnPosition();
		switch(returnTarget) {
		case 1:
			cout << "Invalid return position\n";
			break;
		case 2:
			OFFSET = Preferences::GetInstance()->GetInt("AutoReturnOffset2", -11);
			break;
		case 3:
			OFFSET = Preferences::GetInstance()->GetInt("AutoReturnOffset3", -11);
			break;
		case 4:
			OFFSET = Preferences::GetInstance()->GetInt("AutoReturnOffset4", -11);
			break;
		case 5:
			OFFSET = Preferences::GetInstance()->GetInt("AutoReturnOffset5", -11);
			break;
		}
	}
	const int SLOW_THRESHOLD = 20;
	const int FIRE_THRESHOLD = 8;

	const int MIN_SLOW = OFFSET - SLOW_THRESHOLD;
	const int MAX_SLOW = OFFSET + SLOW_THRESHOLD;
	const int MIN_FIRE = OFFSET - FIRE_THRESHOLD;
	const int MAX_FIRE = OFFSET + FIRE_THRESHOLD;

	P = 1.0;
	if (currentX > MIN_SLOW && currentX < MAX_SLOW) {
		P = 0.28;
		if (currentX > MIN_FIRE && currentX < MAX_FIRE) {
			std::cout << "AlignWithGoal: Goal aligned!\n";

			if (!fired && !in_twist && arm_in_position) {
				cout << "***********************-=====> FIRING\n";
				if (!returnToStart) {
					Robot::arm->Fire();
				}
				fired = true;
			}
		}
	}

	if (fired) {
		if (returnToStart) {
			crab->lock = true;
			return true;
		}
		if (fineTuneCounter++ > 5) {
			return true;
		}
		// coast
	} else {
		// Calculate movement
		const int startingPosition = !returnToStart ? world->GetStartingPosition() : world->GetReturnPosition();
		float driveAngleRadians = utils::CalculateDriveAngle(startingPosition, targetGoal, world->GetStartingDefense());
		if (returnToStart) {
			if (driveAngleRadians > M_PI) {
				driveAngleRadians -= M_PI;
			} else {
				driveAngleRadians += M_PI;
			}
		}
		cout << "SearchForGoal: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
		float magnitude = P * speed;
		if (kickCounter > 0) {
			magnitude += 0.005 * kickCounter;
		}
		magnitude = max(magnitude, MIN_MAGNITUDE_FLOOR);
		const float x = magnitude * sin(driveAngleRadians);
		const float y = magnitude * cos(driveAngleRadians);
		cout << "Align X: " << currentX << " magnitude: " << magnitude << '\n';
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	}
	cout << '\n';
	return false;
}

bool AlignWithGoalAndShoot::InTwist() {
	float current_twist = fabs(Robot::driveBase->imu->GetYaw());
	float delta_twist = last_twist < 0 ? current_twist : fabs(current_twist - last_twist);
	if (delta_twist > 180.0) {
		delta_twist = 360.0 - current_twist;
	}
	last_twist = current_twist;
	return delta_twist > TWIST_THRESHOLD;
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
		collisionDetected = collisionDetector->Detect();
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
		const float driveAngleRadians = utils::CalculateDriveAngle(startingPosition, targetGoal, world->GetStartingDefense());
		cout << "MoveToWallShootingPosition: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
		x = speed * sin(driveAngleRadians);
		y = speed * cos(driveAngleRadians);
	}
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	return false;
}

//--------------------------------------------------------------------------//

// TODO: fix duplicated code with movetowall shooting position
bool MoveAlongMoveToWallShootingLine::operator ()(World* world) {
	cout << "MoveAlongMoveToWallShootingLine()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
	}

	const float deltaTime = currentTime - startTime;
	if (deltaTime > TIMEOUT_TIME) {
		cout << "*** TIMEOUT ***\n";
		crab->lock = true;
		return false;
	}

	if (deltaTime > travelTime) {
		return true;
	}

	const int startingPosition = world->GetStartingPosition();
	const int targetGoal = world->GetTargetGoal();
	float yawSetpoint = 90.0;
	if (targetGoal == 5) { yawSetpoint = -90.0; }
	Robot::driveBase->DriveControlTwist->SetSetpoint(yawSetpoint);
	float x = 0.0;
	float y = 0.0;

	const float driveAngleRadians = utils::CalculateDriveAngle(startingPosition, targetGoal, world->GetStartingDefense());
	cout << "MoveToWallShootingPosition: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
	x = speed * sin(driveAngleRadians);
	y = speed * cos(driveAngleRadians);
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	return false;

}

//--------------------------------------------------------------------------//

bool SnugToWall::operator ()(World* world) {
	cout << "SnugToWall()\n";
	const float currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
		doingPulse = true;
	}

	const bool collisionDetected = collisionDetector->Detect();

	bool timeDone;
	if (moveTowardsWall) {
		if (currentIteration >= iterations) {
			// last iteration go further another 0.25
			timeDone = (currentTime - 0.5 - startTime) > driveWallTime;
		} else {
			timeDone = (currentTime - startTime) > driveWallTime;
		}
	} else {
		timeDone = (currentTime - startTime) > driveTime;
	}

	cout << "Current T: " << currentTime << '\n';
	cout << "Start T  : " << startTime << '\n';
	cout << "Iteration: " << currentIteration << '\n';
	cout << "Collision? " << collisionDetected << '\n';
	cout << "MoveWall ? " << moveTowardsWall << '\n';
	cout << "Time Done? " << timeDone << '\n';
	cout << "In Pulse?  " << doingPulse << '\n';

	if (currentIteration > iterations) {
		cout << "Finished Iterations\n";
		crab->Stop();
		return true;
	}

	if (moveTowardsWall) {
		if (timeDone) {
			cout << "\t*** moved toward wall\n";
			doingPulse = true;
			moveTowardsWall = false;
			currentIteration++;
			startTime = currentTime;
		} else {
			crab->Update(0, speed, 0, true);
		}
	} else {
		if (doingPulse) {
			if (timeDone) {
				doingPulse = false;
				startTime = currentTime;
			} else {
				crab->Update(0, -speed, 0, true);
				return false;
			}
		} else {
			// coasting
			if (timeDone) {
				moveTowardsWall = true;
				currentIteration++;
				startTime = currentTime;
			}
			return false;
		}
	}
	return false;
}



//--------------------------------------------------------------------------//




//void DebugDisplayCalculateDriveAngle() {
//	for (int i=0;i<100;i++) { cout  << '\n'; }
//	for (int pos=1;pos<=5;pos++) {
//		for (int t=1;t<=5;t++) {
//			cout << "P: " << pos
//					<< " T: " << t
//					<< " Angle: "
//					<< CalculateDriveAngle(pos,t) * 180 / M_PI
//					<< '\n';
//		}
//	}
//}

