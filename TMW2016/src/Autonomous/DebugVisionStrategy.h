/*
 * DebugVisionStrategy.h
 */

#ifndef SRC_AUTONOMOUS_DEBUGVISIONSTRATEGY_H_
#define SRC_AUTONOMOUS_DEBUGVISIONSTRATEGY_H_

#include "Strategy.h"

class DebugVisionStrategy : public StepStrategy{
public:
	DebugVisionStrategy();
	virtual ~DebugVisionStrategy();
	void Init(World* world) override;
};

#endif /* SRC_AUTONOMOUS_DEBUGVISIONSTRATEGY_H_ */
