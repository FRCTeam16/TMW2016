/*
 * AlignWithGoal.h
 */

#ifndef SRC_AUTONOMOUS_ALIGNWITHGOAL_H_
#define SRC_AUTONOMOUS_ALIGNWITHGOAL_H_

#include "Step.h"

float CalculateDriveAngle(const int pos, const int goal);
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


class AlignWithGoal : public Step {
public:
	AlignWithGoal(float timeout_, float speed_);
	virtual ~AlignWithGoal() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float speed;
	const int xthreshold = 10;
	float startTime = -1;
	int fineTuneCounter = 0;
};

//--------------------------------------------------------------------------//

class AlignWithGoalPID : public Step {
public:
	AlignWithGoalPID(float timeout_, float speed_);
	virtual ~AlignWithGoalPID() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float speed;
	const int xthreshold = 5;
	std::unique_ptr<VisionPIDAdapter> pidXAdapter;
	std::unique_ptr<PIDController> pidX;
	float startTime = -1;
	int fineTuneCounter = 0;

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


#endif /* SRC_AUTONOMOUS_ALIGNWITHGOAL_H_ */
