/*
 * ReturnStrategy.h
 */

#ifndef SRC_AUTONOMOUS_RETURNSTRATEGY_H_
#define SRC_AUTONOMOUS_RETURNSTRATEGY_H_

#include "Strategy.h"
#include "Step.h"

class ReturnStrategy : public StepStrategy {
public:
	ReturnStrategy();
	virtual ~ReturnStrategy();
	void Init(World* world) override;
private:
};

#endif /* SRC_AUTONOMOUS_RETURNSTRATEGY_H_ */
