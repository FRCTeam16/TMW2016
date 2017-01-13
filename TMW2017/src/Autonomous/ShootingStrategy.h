/*
 * ShootingStrategy.h
 */

#ifndef SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_
#define SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_

#include "Strategy.h"
#include "Step.h"

class ShootingStrategy : public StepStrategy {
public:
	void Init(World* world) override;
	virtual bool Run(World *world) override;
private:
	std::vector<Step*> locationSteps[8];			// TODO: Can we use safer pointer here
	unsigned int currentStep = 0;
	void constructLeftGoalSteps(std::vector<Step*> &vec, const bool useVision, const bool waitForArm);
	void constructCenterGoalSteps(std::vector<Step*> &vec, const bool useVision, const bool waitForArm);
};

#endif /* SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_ */
