/*
 * ShootingStrategy.h
 */

#ifndef SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_
#define SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_

#include "Strategy.h"
#include "Step.h"

class ShootingStrategy : public Strategy {
public:
	void Init() override;
	virtual bool Run(World *world) override;
private:
	std::vector<Step*> locationSteps[6];			// TODO: Can we use safer pointer here
	unsigned int currentStep = 0;
};

#endif /* SRC_AUTONOMOUS_SHOOTINGSTRATEGY_H_ */
