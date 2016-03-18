/*
 * TeleopStrategy.h
 */

#ifndef SRC_AUTONOMOUS_TELEOPSTRATEGY_H_
#define SRC_AUTONOMOUS_TELEOPSTRATEGY_H_

#include "Strategy.h"
#include "World.h"


class TeleopStrategy : public StepStrategy {
public:
	TeleopStrategy();
	virtual ~TeleopStrategy();
	bool RunPeriodic(VisionData visionData);
	void Reset();
private:
	std::unique_ptr<World> world;			// faux world state
	bool started = false;
};

#endif /* SRC_AUTONOMOUS_TELEOPSTRATEGY_H_ */
