/*
 * TraverseObstacleSteps.h
 */

#ifndef SRC_AUTONOMOUS_TRAVERSEOBSTACLESTEPS_H_
#define SRC_AUTONOMOUS_TRAVERSEOBSTACLESTEPS_H_

#include "WPILib.h"
#include "Step.h"

class TraverseObstacleWithGyro : public Step {
public:
	TraverseObstacleWithGyro(float speed_ = 0.75, int negativeCounterTarget=5) : speed(speed_), NEGATIVE_COUNTER_TARGET(negativeCounterTarget ){}
	bool operator()(World *world) override;
private:
	const int MAX_LOOPS = 150;
	int loopCounter = 0;

	const double MAX_TRY_TIME = 2.0;
	const double MAX_RETRY_TIME = 2.0;
	double startTime = -1;

	const float speed;
	bool running = false;
	bool startedObstacle = false;

	const int NEGATIVE_COUNTER_TARGET;	// how many loops to detect negative movement
	int negativeCounter = 0;
	bool hitNegative = false;
	int quietCount = 0;

	bool inRetry = false;
	float retryStartTime = -1;
};

// --------------------------------------------------------------------------//

class TraverseObstacleWithGyroAndSonar : public Step {
public:
	TraverseObstacleWithGyroAndSonar(float speed_ = 0.75, float angle_=0.0, int negativeCounterTarget=5) : speed(speed_), angle(angle_), NEGATIVE_COUNTER_TARGET(negativeCounterTarget) {}
	bool operator()(World *world) override;
private:
	const int MAX_LOOPS = 150;
	int loopCounter = 0;

	const double MAX_TRY_TIME = 2.0;
	const double MAX_RETRY_TIME = 2.0;
	double startTime = -1;

	const float speed;
	const float angle;
	bool running = false;
	bool startedObstacle = false;

	const int NEGATIVE_COUNTER_TARGET;	// how many loops to detect negative movement
	int negativeCounter = 0;
	bool hitNegative = false;
	int quietCount = 0;

	bool inRetry = false;
	float retryStartTime = -1;

	const int DELTA_ULTRASONIC = 10;
	int last_distance = 0;
	bool distance_in_obstacle = false;
	const double ULTRASONIC_HOLD = 0.25;	// threshold for how long after setting ultrasonic value in obstacle before we start looking again
	double ultrasonicStartTime = -1;		// when we detected we were in obstacle and start

};


class TraverseObstacleWithGyroAndSonarLockingValues : public Step {
public:
	TraverseObstacleWithGyroAndSonarLockingValues(float speed_ = 0.75, float angle_ = 0.0, int negativeCounterTarget=5, bool forward_ = true, float beaterBarSpeed_ = -1.0) :
		speed(speed_), angle(angle_), NEGATIVE_COUNTER_TARGET(negativeCounterTarget), forward(forward_), beaterBarSpeed(beaterBarSpeed_) {}
	bool operator()(World *world) override;
private:
	const int MAX_LOOPS = 150;
	int loopCounter = 0;

	const double MAX_TRY_TIME = 2.0;
	const double MAX_RETRY_TIME = 2.0;
	double startTime = -1;
	const double TIMEOUT_THRESHOLD = 3.0;

	const float speed;
	bool running = false;
	bool startedObstacle = false;

	const float angle;

	const int NEGATIVE_COUNTER_TARGET;	// how many loops to detect negative movement
	int negativeCounter = 0;
	bool hitNegative = false;
	int quietCount = 0;

	bool inRetry = false;
	float retryStartTime = -1;

	int last_distance = 0;
	bool distance_in_obstacle = false;

	const bool forward;
	const float beaterBarSpeed;

};

#endif /* SRC_AUTONOMOUS_TRAVERSEOBSTACLESTEPS_H_ */
