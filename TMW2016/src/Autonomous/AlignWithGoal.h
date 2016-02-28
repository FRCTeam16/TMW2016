/*
 * AlignWithGoal.h
 */

#ifndef SRC_AUTONOMOUS_ALIGNWITHGOAL_H_
#define SRC_AUTONOMOUS_ALIGNWITHGOAL_H_

#include "Step.h"

class VisionPIDAdapter : public PIDSource, public PIDOutput {
public:
	VisionPIDAdapter() {}
	virtual ~VisionPIDAdapter() {};
	void Update(const float value);
	double PIDGet() override;
	void PIDWrite(float output) override;
private:
	double inputValue = 0;
	double adjustedValue = 0;
};

class SearchForGoal : public Step {
public:
	SearchForGoal(float timeout_, float speed_, float minTime_=0) :
		timeout(timeout_), speed(speed_), minTime(minTime_) {}
	virtual ~SearchForGoal() {}
	bool operator()(World *world) override;
	float CalculateDriveAngle(const int ops, const int goal);
private:
	const float timeout;
	const float speed;
	const float minTime;	// the minimum amount of time to run
	float startTime = -1;
};

class AlignWithGoal : public Step {
public:
	AlignWithGoal(float timeout_);
	virtual ~AlignWithGoal() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	std::unique_ptr<VisionPIDAdapter> pidXAdapter;
	std::unique_ptr<PIDController> pidX;
	float startTime = -1;

};

#endif /* SRC_AUTONOMOUS_ALIGNWITHGOAL_H_ */
