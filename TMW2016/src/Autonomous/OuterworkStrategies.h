/*
 * OuterworkStrategies.h
 */

#ifndef SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_
#define SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_

#include "Strategy.h"
#include <map>

//class LowBarStrategy : public Strategy {
//public:
//	LowBarStrategy(double armDelay_);
//	void Init() override;
//	bool Run(World *world) override;
//private:
//	std::unique_ptr<Step> forwardStep;
//	std::unique_ptr<Step> armStep;
//	double startTime = -1;
//	const double armDelay;
//};

class LowBarStrategy : public StepStrategy {
public:
	void Init() override;
};

class PortcullisStrategy : public StepStrategy {};
class ChevalDeFriseStrategy : public StepStrategy {};
class MoatStrategy : public StepStrategy {};
class RampartsStrategy : public StepStrategy {};
class DrawbridgeStrategy : public StepStrategy {};
class SallyPortStrategy : public StepStrategy {};
class RockWallStrategy : public StepStrategy {};


class RoughTerrainStrategy : public StepStrategy {
public:
	void Init() override;
private:
	bool inRetry = false;
};

// --------------------------------------------------------------------------//

#endif /* SRC_AUTONOMOUS_OUTERWORKSTRATEGIES_H_ */
