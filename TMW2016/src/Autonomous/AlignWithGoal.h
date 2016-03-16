/*
 * AlignWithGoal.h
 */

#ifndef SRC_AUTONOMOUS_ALIGNWITHGOAL_H_
#define SRC_AUTONOMOUS_ALIGNWITHGOAL_H_

#include "Step.h"
#include "../Util/CollisionDetector.h"
#include "../Robot.h"



//--------------------------------------------------------------------------//

class AlignWithGoalAndShoot : public Step {
public:
	AlignWithGoalAndShoot(float timeout_, float speed_): timeout(timeout_), speed(speed_) {}
	virtual ~AlignWithGoalAndShoot() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float speed;
	const int xthreshold = 10;
	float startTime = -1;
	bool fired = false;
	int fineTuneCounter = 0;
	int missingGoalCounter = 0;
	GoalInfo goal;
};

//--------------------------------------------------------------------------//


class MoveAlongMoveToWallShootingLine : public Step {
public:
	MoveAlongMoveToWallShootingLine(float travelTime_, float speed_, float timeout_=5) :
		travelTime(travelTime_), speed(speed_), TIMEOUT_TIME(timeout_) {}
	bool operator()(World *world) override;
private:
	const float travelTime;
	const float speed;
	const float TIMEOUT_TIME;
	float startTime = -1;
};


//--------------------------------------------------------------------------//


class MoveToWallShootingPosition : public Step {
public:
	MoveToWallShootingPosition(float timeout_, float speed_) :
		timeout(timeout_), speed(speed_), collisionDetector(new CollisionDetector(Robot::driveBase->imu, 1.0f)) {};
	virtual ~MoveToWallShootingPosition() {};
	bool operator()(World *world) override;
private:
	bool DetectCollision();
	const float timeout;
	const float speed;
	float startTime = -1;
	const int INITIAL_MOVE_TIME = 2.0;		// initial time to move before checking for collision
	unique_ptr<CollisionDetector> collisionDetector;

	// collision detection
	const int QUIETING_PERIOD = 10;
	bool collisionDetected = false;
	int quietTimer = 0;
};

//--------------------------------------------------------------------------//
class SnugToWall : public Step {
public:
	SnugToWall(float speed_, float driveTime_=0.25, float driveWallTime_=1.0, int iterations_=1) :
		speed(speed_), driveTime(driveTime_), driveWallTime(driveWallTime_), iterations(iterations_),
		collisionDetector(new CollisionDetector(Robot::driveBase->imu, 1.0f)) {}
	bool operator()(World *world) override;
private:
	const float speed;
	const float driveTime;
	const float driveWallTime;
	const int iterations;
	int currentIteration = 0;
	float startTime = -1;
	bool collisionDetected = false;
	bool moveTowardsWall = false;		// true = positive y
	bool doingPulse = false;
	unique_ptr<CollisionDetector> collisionDetector;
};


#endif /* SRC_AUTONOMOUS_ALIGNWITHGOAL_H_ */
