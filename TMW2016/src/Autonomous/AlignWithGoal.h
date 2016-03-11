/*
 * AlignWithGoal.h
 */

#ifndef SRC_AUTONOMOUS_ALIGNWITHGOAL_H_
#define SRC_AUTONOMOUS_ALIGNWITHGOAL_H_

#include "Step.h"

float CalculateDriveAngle(const int pos, const int goal, outerworks defense);
void DebugDisplayCalculateDriveAngle();


class VisionPIDAdapter : public PIDSource, public PIDOutput {
public:
	VisionPIDAdapter() {}
	virtual ~VisionPIDAdapter() {};
	void Update(const float value);
	double PIDGet() override;					// used as PID sensor source
	void PIDWrite(float output) override;		// used as PID sensor output
	double GetOutputValue();
private:
	double inputValue = 0;
	double outputValue = 0;
};

//--------------------------------------------------------------------------//

class SearchForGoal : public Step {
public:
	SearchForGoal(float timeout_, float speed_) :
		timeout(timeout_), speed(speed_) {}
	virtual ~SearchForGoal() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float speed;
	float startTime = -1;
};

//--------------------------------------------------------------------------//


//class AlignWithGoal : public Step {
//public:
//	AlignWithGoal(float timeout_, float speed_): timeout(timeout_), speed(speed_) {}
//	virtual ~AlignWithGoal() {}
//	bool operator()(World *world) override;
//private:
//	const float timeout;
//	const float speed;
//	const int xthreshold = 10;
//	float startTime = -1;
//	int fineTuneCounter = 0;
//};

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
};

//--------------------------------------------------------------------------//



//class AlignWithGoalPID : public Step {
//public:
//	AlignWithGoalPID(float timeout_, float speed_);
//	virtual ~AlignWithGoalPID() {}
//	bool operator()(World *world) override;
//private:
//	const float timeout;
//	const float speed;
//	const int xthreshold = 5;
//	std::unique_ptr<VisionPIDAdapter> pidXAdapter;
//	std::unique_ptr<PIDController> pidX;
//	float startTime = -1;
//	int fineTuneCounter = 0;
//
//};

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
	MoveToWallShootingPosition(float timeout_, float speed_) : timeout(timeout_), speed(speed_) {};
	virtual ~MoveToWallShootingPosition() {};
	bool operator()(World *world) override;
private:
	bool DetectCollision();
	const float timeout;
	const float speed;
	float startTime = -1;
	const int INITIAL_MOVE_TIME = 2.0;		// initial time to move before checking for collision

	// collision detection
	const double COLLISION_THRESHOLD_DELTA_G = 1.0f;
	const int QUIETING_PERIOD = 10;
	bool collisionDetected = false;
	int quietTimer = 0;
	double last_accel_x = 0.0;
	double last_accel_y = 0.0;
};

//--------------------------------------------------------------------------//
class SnugToWall : public Step {
public:
	SnugToWall(float speed_, float driveTime_=0.25, float driveWallTime_=1.0, int iterations_=1) :
		speed(speed_), driveTime(driveTime_), driveWallTime(driveWallTime_), iterations(iterations_) {}
	bool operator()(World *world) override;
private:
	const float speed;
	const float driveTime;
	const float driveWallTime;
	const int iterations;
	int currentIteration = 0;
	float startTime = -1;
	bool collisionDetected;
	bool moveTowardsWall = false;		// true = positive y
	bool doingPulse = false;

	// TODO: Duplicated code
	bool DetectCollision();
	const double COLLISION_THRESHOLD_DELTA_G = 1.0f;
	double last_accel_x = 0.0;
	double last_accel_y = 0.0;
};


#endif /* SRC_AUTONOMOUS_ALIGNWITHGOAL_H_ */
