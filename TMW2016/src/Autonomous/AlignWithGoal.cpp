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

	goal = detectedGoal ? vd.GetGoal(targetGoal) : goal;

	cout << "Current Time: " << currentTime << '\n';
	cout << "Start Time  : " << startTime << '\n';
	cout << "Target Goal : " << targetGoal << '\n';
	cout << "goal X      : " << goal.xposition << '\n';
	cout << "Fine Tune   : " << fineTuneCounter << '\n';
	cout << "Fired       : " << fired << '\n';

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
		if (missingGoalCounter++ > 10) {
			cout << "AlignWithGoal: No goal visible, stopping...\n";
			crab->lock = true;
			return false;
		} else {
			// waiting for missing goal counter
		}
	} else {
		missingGoalCounter = 0;
	}

	const float currentX = goal.xposition;
	const int OFFSET = 0;
	const int SLOW_THRESHOLD = 20;
	const int FIRE_THRESHOLD = 10;

	const int MIN_SLOW = OFFSET - SLOW_THRESHOLD;
	const int MAX_SLOW = OFFSET + SLOW_THRESHOLD;
	const int MIN_FIRE = OFFSET - FIRE_THRESHOLD;
	const int MAX_FIRE = OFFSET + FIRE_THRESHOLD;


	// We've detected goal
	float P = 1.0;

	if (currentX > MIN_SLOW && currentX < MAX_SLOW) {
		P = 0.28;
		if (currentX > MIN_FIRE && currentX < MAX_FIRE) {
			std::cout << "AlignWithGoal: Goal aligned!\n";
			if (!fired) {
				cout << "***********************-=====> FIRING\n";
				Robot::arm->Fire();
				fired = true;
			}
			return false;
		} else {
			// keep moving slowly toward target
		}
	}

	if (fired) {
		if (fineTuneCounter++ > 5) {
			return true;
		}
		// coast
	} else {
		// Calculate movement
		const int startingPosition = world->GetStartingPosition();
		const float driveAngleRadians = utils::CalculateDriveAngle(startingPosition, targetGoal, world->GetStartingDefense());
		cout << "SearchForGoal: Pos: " << startingPosition << " Goal: " << targetGoal << " Calculated driveAngle = " << driveAngleRadians << "\n";
		float magnitude = P * speed;
		const float x = magnitude * sin(driveAngleRadians);
		const float y = magnitude * cos(driveAngleRadians);
		cout << "Align X: " << currentX << " magnitude: " << magnitude << '\n';
		crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);
	}
	cout << '\n';
	return false;
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

