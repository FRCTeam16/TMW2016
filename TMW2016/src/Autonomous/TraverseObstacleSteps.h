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
	TraverseObstacleWithGyroAndSonar(float speed_ = 0.75) : speed(speed_) {}
	bool operator()(World *world) override;
private:
	double startTime = 01;
	const float speed;
	bool startedObstacle = false;
	int quietCounter = 0;
	int last_distance_one = 0;
	int last_distance_two = 0;

};

// --------------------------------------------------------------------------//

class TraverseObstacleWithGyroAndProximity : public Step {
public:
	TraverseObstacleWithGyroAndProximity(float speed_ = 0.75) : speed(speed_) {};
	bool operator()(World *world) override;
private:
	double startTime = -1;
	const double MAX_TRY_TIME = 5.0;
	const float speed;
	bool startedObstacle = false;
	bool clearedObstacle = false;
	int quietCount = 0;
};



#endif /* SRC_AUTONOMOUS_TRAVERSEOBSTACLESTEPS_H_ */
