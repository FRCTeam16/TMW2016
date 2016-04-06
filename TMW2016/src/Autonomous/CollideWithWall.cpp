/*
 * AlignWithWall.cpp
 */

#include "../Robot.h"
#include "CollideWithWall.h"

CollideWithWall::CollideWithWall(float frontAngleDegrees_, float xSpeed_, float ySpeed_, float jerkThreshold) :
	yawDegrees(frontAngleDegrees_), xSpeed(xSpeed_), ySpeed(ySpeed_), JERK_THRESHOLD(jerkThreshold)
{
	collisionDetector.reset(new CollisionDetector(Robot::driveBase->imu, JERK_THRESHOLD));
}

CollideWithWall::~CollideWithWall() {
}

bool CollideWithWall::operator ()(World* world) {
	if (startTime < 0) {
		startTime = world->GetClock();
		Robot::driveBase->DriveControlTwist->SetSetpoint(yawDegrees);
	}

	// First rotate to align front
	if (!frontInPosition) {
		const float currentYaw = Robot::driveBase->imu->GetYaw();
		if (fabs(currentYaw - yawDegrees) < YAW_THRESHOLD) {
			frontInPosition = true;
		} else {
			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), 0.0, 0.0, true);
		}
	} else {
		// We are pointed in the correct dimension, move towards wall until collision
		if (collideStartTime < 0) {
			collideStartTime = world->GetClock();
		}
		collisionDetected = collisionDetector->Detect();
		const bool timedOut = (world->GetClock() - collideStartTime) > DEFAULT_TIMEOUT;
		cout << "CollideWithWall-> collision? " << collisionDetected << " timeout: " << timedOut << '\n';
		if (!collisionDetected && !timedOut) {
			crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), ySpeed, xSpeed, true);
		} else {
			return true;
		}
	}
	return false;
}
