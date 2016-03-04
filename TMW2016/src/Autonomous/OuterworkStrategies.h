/*
 * OuterworkStrategies.h
 */

#ifndef SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_
#define SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_

#include "Strategy.h"
#include <map>

class LowBarStrategy : public StepStrategy {
public:
	void Init(World* world) override;
};

class RoughTerrainStrategy : public StepStrategy {
public:
	void Init(World* world) override;
private:
	bool inRetry = false;
};

class MoatStrategy : public StepStrategy {
public:
	void Init(World* world) override;
private:
	bool inRetry = false;
};


// --------------------------------------------------------------------------//

#endif /* SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_ */
