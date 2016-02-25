/*
 * OuterworkStrategies.h
 */

#ifndef SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_
#define SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_

#include "Strategy.h"
#include <map>

class LowBarStrategy : public StepStrategy {
public:
	void Init() override;
};

class RoughTerrainStrategy : public StepStrategy {
public:
	void Init() override;
private:
	bool inRetry = false;
};

class ChevalDeFriseStrategy : public StepStrategy {
public:
	void Init() override;
};


// --------------------------------------------------------------------------//

#endif /* SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_ */
