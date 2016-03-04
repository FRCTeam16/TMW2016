/*
 * Strategy.h
 */

#ifndef SRC_AUTONOMOUS_STRATEGY_H_
#define SRC_AUTONOMOUS_STRATEGY_H_

#include "../Subsystems/DriveBase.h"
#include "World.h"
#include "Step.h"
#include <vector>

class Strategy {
public:
	Strategy() {}
	virtual ~Strategy() {};
	virtual void Init(World* world) {};
	virtual bool Run(World* world) = 0;
};

class NoOpStrategy : public Strategy {
private:
	virtual bool Run(World *world) override;
};

// --------------------------------------------------------------------------//

/**
 * Strategy implementation which relies on iterating a vector of Steps to perform
 */
class StepStrategy : public Strategy {
public:
	StepStrategy() {}
	virtual ~StepStrategy() {}
	virtual bool Run(World *world) override;
protected:
	const std::unique_ptr<CrabInfo> STOP { new CrabInfo() };
	void RunPeriodicManagers(const CrabInfo *crab);
	std::vector<std::unique_ptr<Step>> steps;
	unsigned int currentStep = 0;
};

// --------------------------------------------------------------------------//

class OuterworkAndShootStrategy : public StepStrategy {
public:
	OuterworkAndShootStrategy(Strategy *outerworkStrategy_);
private:
	virtual bool Run(World *world) override;
	std::unique_ptr<Strategy> outerworkStrategy;
	std::unique_ptr<Strategy> shootingStrategy;
	bool outerworkStrategyComplete = false;
	bool outerworkStrategyStarted = true;
	bool firstTimeShooting = true;
};

// --------------------------------------------------------------------------//

#endif /* SRC_AUTONOMOUS_STRATEGY_H_ */
