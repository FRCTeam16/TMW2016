/*
 * SimpleDriveForwardStrategy.h
 */

#ifndef SRC_AUTONOMOUS_SIMPLEDRIVEFORWARDSTRATEGY_H_
#define SRC_AUTONOMOUS_SIMPLEDRIVEFORWARDSTRATEGY_H_

#include "Strategy.h"
#include "World.h"

class SimpleDriveForwardStrategy : public StepStrategy {
public:
	SimpleDriveForwardStrategy() {};
	virtual ~SimpleDriveForwardStrategy();
	void Init();
};

#endif /* SRC_AUTONOMOUS_SIMPLEDRIVEFORWARDSTRATEGY_H_ */
