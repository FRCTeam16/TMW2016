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
	AlignGoalUsingPID(const float speed_, const bool teleop_mode_=false);
	virtual ~AlignGoalUsingPID();
	bool operator()(World* world) override;
private:
	float startTime = -1;
	const float speed;
	bool teleop_mode;
	const float timeout = 10.0;
	std::unique_ptr<PIDController> pidX;
	std::unique_ptr<VisionPIDAdapter> xAdapter;
	std::unique_ptr<PIDController> pidY;
	std::unique_ptr<VisionPIDAdapter> yAdapter;
	GoalInfo goal;
	CrabInfo lastCrab;
	const int MAX_NO_GOAL_SCANS = 10;
	int noGoalCounter = 0;

	int GOAL_CENTER_OFFSET = -11;	// set with preferences
	const int X_THRESHOLD = 8;

	const int Y_TARGET = 31;
	const int Y_THRESHOLD = 0;

	const float TWIST_THRESHOLD = 0.5;
	float last_twist = -1.0;			// tracks scan to scan twist delta

	int kickCounter = 0;	// track if x position has changed

	bool fired = false;
};

#endif /* SRC_AUTONOMOUS_ALIGNGOALUSINGPID_H_ */
