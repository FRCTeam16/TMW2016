/*
 * AlignGoalUsingPID.h
 */

#ifndef SRC_AUTONOMOUS_ALIGNGOALUSINGPID_H_
#define SRC_AUTONOMOUS_ALIGNGOALUSINGPID_H_

#include "Step.h"
#include "../Vision/VisionPIDAdapter.h"
#include "World.h"

class AlignGoalUsingPID : public Step {
public:
	AlignGoalUsingPID(const float speed_);
	virtual ~AlignGoalUsingPID();
	bool operator()(World* world) override;
private:
	float startTime = -1;
	const float speed;
	const float timeout = 10.0;
	std::unique_ptr<PIDController> pidX;
	std::unique_ptr<VisionPIDAdapter> xAdapter;
	std::unique_ptr<VisionPIDAdapter> yAdapter;
	GoalInfo goal;
	const int MAX_NO_GOAL_SCANS = 10;
	int noGoalCounter = 0;

	const int GOAL_CENTER_OFFSET = -15.0;
	const int X_THRESHOLD = 8;

	bool fired = false;
};

#endif /* SRC_AUTONOMOUS_ALIGNGOALUSINGPID_H_ */
