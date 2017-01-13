/*
 * TeleopStrategy.h
 */

#ifndef SRC_AUTONOMOUS_TELEOPAUTOHIGHSHOOTSTRATEGY_H_
#define SRC_AUTONOMOUS_TELEOPAUTOHIGHSHOOTSTRATEGY_H_

#include "Strategy.h"
#include "World.h"


class TeleopAutoHighShootStrategy : public StepStrategy {
public:
	TeleopAutoHighShootStrategy();
	virtual ~TeleopAutoHighShootStrategy();
	bool RunPeriodic(World *world);
	void Reset();
private:
	bool started = false;
};

#endif /* SRC_AUTONOMOUS_TELEOPAUTOHIGHSHOOTSTRATEGY_H_ */
