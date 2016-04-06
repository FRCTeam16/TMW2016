/*
 * Delay.h
 */

#ifndef SRC_AUTONOMOUS_DELAY_H_
#define SRC_AUTONOMOUS_DELAY_H_

#include "Step.h"
#include <string>

class Delay : public Step {
public:
	Delay();
	Delay(float delayTime_);
	virtual ~Delay();
	bool operator()(World* world) override;
private:
	float delayTime = -1;
	double startTime = -1;
};

#endif /* SRC_AUTONOMOUS_DELAY_H_ */
