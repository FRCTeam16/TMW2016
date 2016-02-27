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

class AlignWithGoal : public Step {
public:
	AlignWithGoal(float timeout_, float angle, float yspeed_, float xspeed_, float threshold_);
	virtual ~AlignWithGoal() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float targetAngle;
	const float ySpeed;
	const float xSpeed;
	const float threshold = 0.0;
	std::unique_ptr<VisionPIDAdapter> pidXAdapter;
	std::unique_ptr<PIDController> pidX;
	bool detectedGoal = false;
	float startTime = -1;

};

#endif /* SRC_AUTONOMOUS_ALIGNWITHGOAL_H_ */
